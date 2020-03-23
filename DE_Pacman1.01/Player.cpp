#include "Player.h"
#include "DxLib.h"
#include "Scene.h"

//=============================================================
// コンストラクタ
//=============================================================
CPlayer::CPlayer(int x, int y)
	:m_iPosX(x*PLAYER_RELATIVE_MAZE_X), m_iPosY(y*PLAYER_RELATIVE_MAZE_Y), m_iAnimCounter(0), m_iIzikeScoreCounter(0), m_iSpeed(1)
{
	//画像の読み込み
	LoadDivGraph(PLAYER_IMAGE, 12, 3, 4, 16, 16, m_iGHPlayer);

	//メイズ情報取得
	m_pMaze = m_cMaze.GetMaze();

	//メイズ配列におけるプレイヤーの位置
	m_iCurrentMazeIndex = m_iPosY / PLAYER_RELATIVE_MAZE_Y * MAZE_WIDTH + m_iPosX / PLAYER_RELATIVE_MAZE_X;
	m_iPreviousMazeIndex = 0;

	//エネミーとの衝突
	m_bBlinkyCollision = false;
	m_bPinkyCollision = false;
	m_bInkyCollision = false;
	m_bCrydeCollision = false;
}

//=============================================================
// デストラクタ
//=============================================================
CPlayer::~CPlayer() 
{
	//画像データの破棄
	for (int i = 0; i < PLAYER_IMAGE_NUM; i++) 
	{
		DeleteGraph(m_iGHPlayer[i]);
	}
}

//=============================================================
// 入力
//=============================================================
void CPlayer::Input()
{
	if (g_cScene->m_iKey[KEY_INPUT_RIGHT] == 1)
	{
		//サウンド
		SEDotEat();

		//方向
		m_eInputDirection = VECTOR_RIGHT;
	}
	else if (g_cScene->m_iKey[KEY_INPUT_LEFT] == 1)
	{
		//サウンド
		SEDotEat();

		//方向
		m_eInputDirection = VECTOR_LEFT;
	}
	else if (g_cScene->m_iKey[KEY_INPUT_UP] == 1)
	{
		//サウンド
		SEDotEat();
		
		//方向
		m_eInputDirection = VECTOR_UP;
	}
	else if (g_cScene->m_iKey[KEY_INPUT_DOWN] == 1)
	{
		//サウンド
		SEDotEat();

		//方向
		m_eInputDirection = VECTOR_DOWN;
	}
	else if (g_cScene->m_iKey[KEY_INPUT_P] == 1) 
	{
		g_cScene->GetGameClass()->m_eGameStatus = g_cScene->GetGameClass()->STATUS_PAUSE;

		//サウンドを止める
		StopSound();
	}
}

//=============================================================
// 動作
//=============================================================
void CPlayer::Move()
{
	//現在のプレイヤー位置
	m_iCurrentMazeIndex = m_iPosY / PLAYER_RELATIVE_MAZE_Y * MAZE_WIDTH + m_iPosX / PLAYER_RELATIVE_MAZE_X;

	//移動
	Movement();

	//回転
	CheckTurn();

	//ワープ
	Warp(m_iCurrentMazeIndex);

	//ドットのクリア
	ClearDot(m_iCurrentMazeIndex);

	//エネミーを食べる
	EatEnemy();

	//フルーツを食べる
	EatFruit();

	//アニメーションカウンタ
	m_iAnimCounter++;
}

//=============================================================
// 移動
//=============================================================
void CPlayer::Movement() 
{
	//プレイヤーの移動
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		if (!IsWall())
		{
			m_iPosX += m_iSpeed;
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				m_iPosX++;
			}
		}
		break;

	case VECTOR_LEFT:		//左
		if (!IsWall())
		{
			m_iPosX -= m_iSpeed;
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				m_iPosX++;
			}
		}
		break;

	case VECTOR_UP:			//上
		if (!IsWall())
		{
			m_iPosY -= m_iSpeed;
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				m_iPosY++;
			}
		}
		break;

	case VECTOR_DOWN:		//下
		if (!IsWall())
		{
			m_iPosY += m_iSpeed;
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				m_iPosY--;
			}
		}
		break;

	default:
		break;
	}
}

//=============================================================
// 壁との当たり判定
//=============================================================
bool CPlayer::IsWall() 
{
	//座標の調整
	if (!IsCorrection()) 
	{
		return false;
	}

	//移動先のプレイヤー座標配列の位置を取得
	switch (m_eDirection) 
	{
	case VECTOR_RIGHT:		//右
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) + 1;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_LEFT:		//左
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) - 1;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_UP:		    //上
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) - MAZE_WIDTH;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_DOWN:		//下
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) + MAZE_WIDTH;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;
	}

	return false;
}

//=============================================================
// ドットをクリア
//=============================================================
void CPlayer::ClearDot(int index) 
{
	if (*(m_pMaze + index) == m_cMaze.TILE_DOT)
	{
		*(m_pMaze + index) = m_cMaze.TILE_NONE;

		//スコア加算 : 10
		AddScore(10);
	}
	else if (*(m_pMaze + index) == m_cMaze.TILE_POWER_DOT)
	{
		*(m_pMaze + index) = m_cMaze.TILE_NONE;

		//スコア加算 : 50
		AddScore(50);

		//エネミーいじけ状態
		g_cScene->GetGameClass()->GetBlinkyClass()->SetIzikeState(true);
		g_cScene->GetGameClass()->GetBlinkyClass()->SetIzikeCounter(0);
		g_cScene->GetGameClass()->GetPinkyClass()->SetIzikeState(true);
		g_cScene->GetGameClass()->GetPinkyClass()->SetIzikeCounter(0);
		g_cScene->GetGameClass()->GetInkyClass()->SetIzikeState(true);
		g_cScene->GetGameClass()->GetInkyClass()->SetIzikeCounter(0);
		g_cScene->GetGameClass()->GetCrydeClass()->SetIzikeState(true);
		g_cScene->GetGameClass()->GetCrydeClass()->SetIzikeCounter(0);
	}
}

//=============================================================
// 入力先の移動方向を保持
//=============================================================
bool CPlayer::IsTurn()
{
	//座標の調整
	if (!IsCorrection())
	{
		return false;
	}

	switch (m_eInputDirection)
	{
	case VECTOR_RIGHT:
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) + 1;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return false;
		}
		break;

	case VECTOR_LEFT:
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) - 1;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return false;
		}
		break;

	case VECTOR_UP:
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) - MAZE_WIDTH;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return false;
		}
		break;

	case VECTOR_DOWN:
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) + MAZE_WIDTH;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return false;
		}
		break;
	}

	return true;
}

//=============================================================
// 入力先の移動方向を保持し壁でなければ回転
//=============================================================
void CPlayer::CheckTurn() 
{
	switch (m_eInputDirection) 
	{
	case VECTOR_RIGHT:
		if (IsTurn())
		{
			m_eDirection = VECTOR_RIGHT;
		}
		break;

	case VECTOR_LEFT:
		if (IsTurn())
		{
			m_eDirection = VECTOR_LEFT;
		}
		break;

	case VECTOR_UP:
		if (IsTurn())
		{
			m_eDirection = VECTOR_UP;
		}
		break;

	case VECTOR_DOWN:
		if (IsTurn())
		{
			m_eDirection = VECTOR_DOWN;
		}
		break;
	}
}

//=============================================================
// 座標の補正
//=============================================================
bool CPlayer::IsCorrection() 
{
	if ((m_iPosX%PLAYER_RELATIVE_MAZE_X == 0) && (m_iPosY%PLAYER_RELATIVE_MAZE_Y == 0))
	{
		return true;
	}

	return false;
}

//=============================================================
// ワープ
//=============================================================
void CPlayer::Warp(int index) 
{
	//座標の調整
	if (!IsCorrection())
	{
		return;
	}

	if (index == 200) 
	{
		m_iPosX = 360;
	}
	else if (index == 218) 
	{
		m_iPosX = 20;
	}
}

//=============================================================
// エネミーを食べスタート地点へ戻す
//=============================================================
void CPlayer::EatEnemy() 
{
	//当たり判定
	IsEnemy();

	//いじけ状態の場合
	if (m_bBlinkyCollision) 
	{
		if (g_cScene->GetGameClass()->GetBlinkyClass()->GetIzikeState())
		{
			//スコア加算
			AddScoreOnEnemy();

			//サウンド
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEEnemyEat(), DX_PLAYTYPE_BACK);

			//一時停止
			WaitTimer(1000);

			//リセット
			g_cScene->GetGameClass()->GetBlinkyClass()->SetPosX(9 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetBlinkyClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetBlinkyClass()->SetIzikeState(false);
			g_cScene->GetGameClass()->GetBlinkyClass()->SetZeroDirection();
			g_cScene->GetGameClass()->GetBlinkyClass()->m_eMoveMode = g_cScene->GetGameClass()->GetBlinkyClass()->MODE_RESTART;
		}
	}

	if (m_bPinkyCollision)
	{
		if (g_cScene->GetGameClass()->GetPinkyClass()->GetIzikeState())
		{
			//スコア加算
			AddScoreOnEnemy();

			//サウンド
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEEnemyEat(), DX_PLAYTYPE_BACK);

			//一時停止
			WaitTimer(1000);

			//リセット
			g_cScene->GetGameClass()->GetPinkyClass()->SetPosX(9 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetPinkyClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetPinkyClass()->SetIzikeState(false);
			g_cScene->GetGameClass()->GetPinkyClass()->SetZeroDirection();
			g_cScene->GetGameClass()->GetPinkyClass()->m_eMoveMode = g_cScene->GetGameClass()->GetPinkyClass()->MODE_RESTART;
		}
	}

	if (m_bInkyCollision)
	{
		if (g_cScene->GetGameClass()->GetInkyClass()->GetIzikeState())
		{
			//スコア加算
			AddScoreOnEnemy();

			//サウンド
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEEnemyEat(), DX_PLAYTYPE_BACK);

			//一時停止
			WaitTimer(1000);

			//リセット
			g_cScene->GetGameClass()->GetInkyClass()->SetPosX(8 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetInkyClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetInkyClass()->SetIzikeState(false);
			g_cScene->GetGameClass()->GetInkyClass()->SetZeroDirection();
			g_cScene->GetGameClass()->GetInkyClass()->m_eMoveMode = g_cScene->GetGameClass()->GetInkyClass()->MODE_RESTART;
		}
	}

	if (m_bCrydeCollision)
	{
		if (g_cScene->GetGameClass()->GetCrydeClass()->GetIzikeState())
		{
			//スコア加算
			AddScoreOnEnemy();

			//サウンド
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEEnemyEat(), DX_PLAYTYPE_BACK);

			//一時停止
			WaitTimer(1000);

			//リセット
			g_cScene->GetGameClass()->GetCrydeClass()->SetPosX(10 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetCrydeClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetCrydeClass()->SetIzikeState(false);
			g_cScene->GetGameClass()->GetCrydeClass()->SetZeroDirection();
			g_cScene->GetGameClass()->GetCrydeClass()->m_eMoveMode = g_cScene->GetGameClass()->GetCrydeClass()->MODE_RESTART;
		}
	}
}

//=============================================================
// エネミーとの当たり判定
//=============================================================
void CPlayer::IsEnemy()
{
	if (m_iCurrentMazeIndex == g_cScene->GetGameClass()->GetBlinkyClass()->GetCurrentMazeIndex()) 
	{
		m_bBlinkyCollision = true;

		//待機
		g_cScene->GetGameClass()->GetBlinkyClass()->SetWait(true);
	}
	else
	{
		m_bBlinkyCollision = false;
	}

	if (m_iCurrentMazeIndex == g_cScene->GetGameClass()->GetPinkyClass()->GetCurrentMazeIndex())
	{
		m_bPinkyCollision = true;

		//待機
		g_cScene->GetGameClass()->GetPinkyClass()->SetWait(true);
	}
	else
	{
		m_bPinkyCollision = false;
	}

	if (m_iCurrentMazeIndex == g_cScene->GetGameClass()->GetInkyClass()->GetCurrentMazeIndex())
	{
		m_bInkyCollision = true;

		//待機
		g_cScene->GetGameClass()->GetInkyClass()->SetWait(true);
	}
	else 
	{
		m_bInkyCollision = false;
	}

	if (m_iCurrentMazeIndex == g_cScene->GetGameClass()->GetCrydeClass()->GetCurrentMazeIndex())
	{
		m_bCrydeCollision = true;

		//待機
		g_cScene->GetGameClass()->GetCrydeClass()->SetWait(true);
	}
	else
	{
		m_bCrydeCollision = false;
	}
}

//=============================================================
// スコア加算
//=============================================================
void CPlayer::AddScore(int add) 
{
	//現在のスコア取得
	int current_score = g_cScene->GetScoreClass()->GetScore();

	//最大値の制限
	if (current_score > 99999) 
	{
		current_score = 99999;
	}

	//スコア加算
	g_cScene->GetScoreClass()->SetScore(current_score + add);
}

//=============================================================
// エネミーにおけるスコア加算
//=============================================================
void CPlayer::AddScoreOnEnemy() 
{
	//スコア加算
	switch (m_iIzikeScoreCounter) 
	{
	case 0:
		AddScore(200);
		m_iIzikeScoreCounter++;
		break;

	case 1:
		AddScore(400);
		m_iIzikeScoreCounter++;
		break;

	case 2:
		AddScore(800);
		m_iIzikeScoreCounter++;
		break;

	case 3:
		AddScore(1600);
		m_iIzikeScoreCounter++;
		break;
	}
}

//=============================================================
// ドットイートサウンド
//=============================================================
void CPlayer::SEDotEat() 
{
	if (!CheckSoundMem(g_cScene->GetSoundClass()->GetSEDotEat())) 
	{
		if (g_cScene->GetGameClass()->m_eGameStatus == g_cScene->GetGameClass()->STATUS_GAME)
		{
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEDotEat(), DX_PLAYTYPE_LOOP);
		}
	}
}

//=============================================================
// サウンドをすべて止める
//=============================================================
void CPlayer::StopSound() 
{
	if (CheckSoundMem(g_cScene->GetSoundClass()->GetSEDotEat()))
	{
		StopSoundMem(g_cScene->GetSoundClass()->GetSEDotEat());
	}

	if (CheckSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove()))
	{
		StopSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove());
	}

	if (CheckSoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove()))
	{
		StopSoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove());
	}
}

//=============================================================
// フルーツを食べてスコアアップ
//=============================================================
void CPlayer::EatFruit() 
{
	if (m_iCurrentMazeIndex == g_cScene->GetGameClass()->GetFruitClass()->GetCurrentMazeIndex()) 
	{
		//さくらんぼ
		if (g_cScene->GetGameClass()->GetFruitClass()->GetOutbreakCherry()) 
		{
			//リセット
			g_cScene->GetGameClass()->GetFruitClass()->SetOutbreakCherry(false);
			g_cScene->GetGameClass()->GetFruitClass()->m_eFruitStatus = g_cScene->GetGameClass()->GetFruitClass()->STRAWBERRY;
			g_cScene->GetGameClass()->GetFruitClass()->SetOutbreakCounter(0);

			//スコア
			AddScore(100);

			//サウンド
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEFruitEat(), DX_PLAYTYPE_BACK);

			//一時停止
			WaitTimer(1000);

		}
		//いちご
		else if (g_cScene->GetGameClass()->GetFruitClass()->GetOutbreakStrawberry()) 
		{
			//リセット
			g_cScene->GetGameClass()->GetFruitClass()->SetOutbreakStrawberry(false);
			g_cScene->GetGameClass()->GetFruitClass()->m_eFruitStatus = g_cScene->GetGameClass()->GetFruitClass()->CHERRY;
			g_cScene->GetGameClass()->GetFruitClass()->SetOutbreakCounter(0);

			//スコア
			AddScore(300);

			//サウンド
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEFruitEat(), DX_PLAYTYPE_BACK);

			//一時停止
			WaitTimer(1000);
		}
	}
}

//=============================================================
// 描画
//=============================================================
void CPlayer::Draw() const 
{
	switch (m_eDirection) 
	{
	case VECTOR_RIGHT:
		DrawRotaGraph3(m_iPosX + PLAYER_DRAW_X, m_iPosY + PLAYER_DRAW_Y, 0, 0,
			PLAYER_IMAGE_EXPANSION_X, PLAYER_IMAGE_EXPANSION_Y, 0, m_iGHPlayer[(m_iAnimCounter / PLAYER_ANIM_SPEED) % 3], TRUE);
		break;

	case VECTOR_LEFT:
		DrawRotaGraph3(m_iPosX + PLAYER_DRAW_X, m_iPosY + PLAYER_DRAW_Y, 0, 0,
			PLAYER_IMAGE_EXPANSION_X, PLAYER_IMAGE_EXPANSION_Y, 0, m_iGHPlayer[(m_iAnimCounter / PLAYER_ANIM_SPEED) % 3 + 3], TRUE);
		break;

	case VECTOR_UP:
		DrawRotaGraph3(m_iPosX + PLAYER_DRAW_X, m_iPosY + PLAYER_DRAW_Y, 0, 0,
			PLAYER_IMAGE_EXPANSION_X, PLAYER_IMAGE_EXPANSION_Y, 0, m_iGHPlayer[(m_iAnimCounter / PLAYER_ANIM_SPEED) % 3 + 6], TRUE);
		break;

	case VECTOR_DOWN:
		DrawRotaGraph3(m_iPosX + PLAYER_DRAW_X, m_iPosY + PLAYER_DRAW_Y, 0, 0,
			PLAYER_IMAGE_EXPANSION_X, PLAYER_IMAGE_EXPANSION_Y, 0, m_iGHPlayer[(m_iAnimCounter / PLAYER_ANIM_SPEED) % 3 + 9], TRUE);
		break;

	default:
		DrawRotaGraph3(m_iPosX + PLAYER_DRAW_X, m_iPosY + PLAYER_DRAW_Y, 0, 0,
			PLAYER_IMAGE_EXPANSION_X, PLAYER_IMAGE_EXPANSION_Y, 0, m_iGHPlayer[1], TRUE);
		break;
	}
}
