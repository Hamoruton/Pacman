#include "Enemy.h"
#include "DxLib.h"
#include "Scene.h"
#include <time.h>

//=============================================================
// コンストラクタ
//=============================================================
CEnemy::CEnemy(int x, int y)
	:m_iPosX(x), m_iPosY(y),
	m_iAnimCounter(0), m_iIzikeCounter(0), m_iRestartCounter(0), m_iDecelerationCounter(0), m_iPursuitCounter(0), m_iSpeed(1)
{
	//画像の読み込み
	LoadDivGraph(ENEMY_IZIKE_IMAGE, ENEMY_IZIKE_IMAGE_NUM, 2, 4, 16, 16, m_iGHEnemyIzike);

	//メイズ情報
	m_pMaze = m_cMaze.GetMaze();

	//メイズ配列におけるエネミーの位置
	m_iCurrentMazeIndex = m_iPosY / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + m_iPosX / ENEMY_RELATIVE_MAZE_X;
	m_iPreviousMazeIndex = 0;

	//いじけ状態
	m_bIzikeState = false;

	//待機（プレイヤーとの衝突後）
	m_bWait = false;

	//初動モード
	m_eMoveMode = MODE_INIT;
}

//=============================================================
// デストラクタ
//=============================================================
CEnemy::~CEnemy() 
{
	//画像データ破棄
	for (int i = 0; i < ENEMY_IZIKE_IMAGE_NUM; i++)
	{
		DeleteGraph(m_iGHEnemyIzike[i]);
	}
}

//=============================================================
// ランダム値をDIRECTION型にセット
//=============================================================
void CEnemy::SetDirection(int d) 
{
	switch (d) 
	{
	case 0:
		m_eDirection = VECTOR_RIGHT;
		break;

	case 1:
		m_eDirection = VECTOR_LEFT;
		break;

	case 2:
		m_eDirection = VECTOR_UP;
		break;

	case 3:
		m_eDirection = VECTOR_DOWN;
		break;
	}
}

//=============================================================
// 動作
//=============================================================
void CEnemy::Move() 
{
	//現在のエネミーの位置
	m_iCurrentMazeIndex = m_iPosY / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + m_iPosX / ENEMY_RELATIVE_MAZE_X;

	//移動
	Movement();

	//回転
	CheckTurn();

	//プレイヤーを食べる
	EatPlayer();

	//ワープ
	Warp(m_iCurrentMazeIndex);

	//いじけカウンタ更新
	IzikeCount();

	//アニメーションカウンタ
	m_iAnimCounter++;

	//サウンド
	SEEnemyMove();
}

//=============================================================
// 初動
//=============================================================
void CEnemy::BaseMove() 
{
}

//=============================================================
// リスタート時の動きの制限
//=============================================================
void CEnemy::RestartMove()
{
}

//=============================================================
// 移動
//=============================================================
void CEnemy::Movement() 
{
	//移動方向
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		if (!IsWall())
		{
			//ループトンネル付近減速
			if (IsDeceleration()) 
			{
				m_iDecelerationCounter++;
				if (m_iDecelerationCounter > ENEMY_DECELERATION_SPEED)
				{
					m_iPosX += m_iSpeed;
					m_iDecelerationCounter = 0;
				}
			}
			else
				m_iPosX += m_iSpeed;
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				m_iPosX--;
			}
			m_iDirection = rand() % 4;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_LEFT:		//左
		if (!IsWall())
		{
			//ループトンネル付近減速
			if (IsDeceleration())
			{
				m_iDecelerationCounter++;
				if (m_iDecelerationCounter > ENEMY_DECELERATION_SPEED)
				{
					m_iPosX -= m_iSpeed;
					m_iDecelerationCounter = 0;
				}
			}
			else
				m_iPosX -= m_iSpeed;
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				m_iPosX++;
			}
			m_iDirection = rand() % 4;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_UP:			//上
		if (!IsWall())
		{
			//ループトンネル付近減速
			if (IsDeceleration())
			{
				m_iDecelerationCounter++;
				if (m_iDecelerationCounter > ENEMY_DECELERATION_SPEED)
				{
					m_iPosY -= m_iSpeed;
					m_iDecelerationCounter = 0;
				}
			}
			else
				m_iPosY -= m_iSpeed;
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				m_iPosY++;
			}
			m_iDirection = rand() % 4;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_DOWN:		//下
		if (!IsWall())
		{
			//ループトンネル付近減速
			if (IsDeceleration()) 
			{
				m_iDecelerationCounter++;
				if (m_iDecelerationCounter > ENEMY_DECELERATION_SPEED)
				{
					m_iPosY += m_iSpeed;
					m_iDecelerationCounter = 0;
				}
			}
			else
			{
				m_iPosY += m_iSpeed;
			}
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				m_iPosY--;
			}
			m_iDirection = rand() % 4;
			SetDirection(m_iDirection);
		}
		break;
	}
}

//=============================================================
// 壁との当たり判定
//=============================================================
bool CEnemy::IsWall()
{
	//座標の調整
	if (!IsCorrection())
	{
		return false;
	}

	//移動先のエネミーー座標配列の位置を取得
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		m_iPreviousMazeIndex = (m_iPosY / ENEMY_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / ENEMY_RELATIVE_MAZE_X) + 1;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_LEFT:		//左
		m_iPreviousMazeIndex = (m_iPosY / ENEMY_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / ENEMY_RELATIVE_MAZE_X) - 1;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_UP:			//上
		m_iPreviousMazeIndex = (m_iPosY / ENEMY_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / ENEMY_RELATIVE_MAZE_X) - MAZE_WIDTH;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_DOWN:		//下
		m_iPreviousMazeIndex = (m_iPosY / ENEMY_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / ENEMY_RELATIVE_MAZE_X) + MAZE_WIDTH;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;
	}

	return false;
}

//=============================================================
// 座標の調整
//=============================================================
bool CEnemy::IsCorrection() 
{
	if ((m_iPosX%ENEMY_RELATIVE_MAZE_X == 0) && (m_iPosY%ENEMY_RELATIVE_MAZE_Y == 0))
	{
		return true;
	}

	return false;
}

//=============================================================
// ワープ
//=============================================================
void CEnemy::Warp(int index)
{
	//座標の調整
	if (!IsCorrection())
	{
		return;
	}

	if (index == 200)
	{
		SetPosX(360);
	}
	else if (index == 218)
	{
		SetPosX(20);
	}
}

//=============================================================
// 進行方向から見て両端が移動できるかどうか
//=============================================================
bool CEnemy::IsBothEndsTurn(int index) 
{
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右→上と下をチェック
		if (*(m_pMaze + index - MAZE_WIDTH) != m_cMaze.TILE_WALL || *(m_pMaze + index + MAZE_WIDTH) != m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_LEFT:		//左→上と下をチェック
		if (*(m_pMaze + index - MAZE_WIDTH) != m_cMaze.TILE_WALL || *(m_pMaze + index + MAZE_WIDTH) != m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_UP:			//上→右と左をチェック
		if (*(m_pMaze + index + 1) != m_cMaze.TILE_WALL || *(m_pMaze + index - 1) != m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_DOWN:		//上→右と左をチェック
		if (*(m_pMaze + index + 1) != m_cMaze.TILE_WALL || *(m_pMaze + index - 1) != m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;
	}

	return false;
}

//=============================================================
// 現在の位置から回転できる方向をランダムで決める
//=============================================================
void CEnemy::CheckTurn() 
{
	//座標の調整
	if (!IsCorrection())
	{
		return;
	}

	if (IsBothEndsTurn(m_iCurrentMazeIndex))
	{
		//追跡
		Pursuit();
	}
	else 
	{	
		m_iPursuitCounter = 0;
	}
}

//=============================================================
// いじけカウンタの更新
//=============================================================
void CEnemy::IzikeCount() 
{
	if (m_bIzikeState)
	{
		//サウンド再生
		if (!CheckSoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove()))
		{
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove(), DX_PLAYTYPE_LOOP);
		}

		//カウンタの更新
		m_iIzikeCounter++;

		if (m_iIzikeCounter > ENEMY_IZIKE_TIME)
		{
			//サウンド停止
			if (CheckSoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove()))
			{
				StopSoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove());
			}

			m_bIzikeState = false;
			m_iIzikeCounter = 0;

			///エネミーを食べた時のスコア加算の変更カウンタの初期化
			g_cScene->GetGameClass()->GetPlayerClass()->SetIzikeScoreCounter(0);
		}
	}
}

//=============================================================
// いじけ状態時プレイヤーと衝突後5秒間中央で待機
//=============================================================
void CEnemy::Restart() 
{
}

//=============================================================
// プレイヤーを食べスタート地点へ戻す
//=============================================================
void CEnemy::EatPlayer() 
{
	if (!m_bIzikeState) 
	{
		if (IsPlayer())
		{
			//エネミー初期位置
			g_cScene->GetGameClass()->GetBlinkyClass()->SetPosX(9 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetBlinkyClass()->SetPosY(8 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetPinkyClass()->SetPosX(9 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetPinkyClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetInkyClass()->SetPosX(8 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetInkyClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetCrydeClass()->SetPosX(10 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetCrydeClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);

			//プレイヤー初期位置
			g_cScene->GetGameClass()->GetPlayerClass()->SetPosX(9 * PLAYER_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetPlayerClass()->SetPosY(16 * PLAYER_RELATIVE_MAZE_Y);

			//リスタートモード
			g_cScene->GetGameClass()->GetBlinkyClass()->m_eMoveMode = MODE_PURSUIT;
			g_cScene->GetGameClass()->GetPinkyClass()->m_eMoveMode = MODE_PURSUIT;
			g_cScene->GetGameClass()->GetInkyClass()->m_eMoveMode = MODE_PURSUIT;
			g_cScene->GetGameClass()->GetCrydeClass()->m_eMoveMode = MODE_PURSUIT;

			//エネミーの方向リセット
			g_cScene->GetGameClass()->GetBlinkyClass()->SetDirection(0);
			g_cScene->GetGameClass()->GetPinkyClass()->SetDirection(2);
			g_cScene->GetGameClass()->GetInkyClass()->SetDirection(0);
			g_cScene->GetGameClass()->GetCrydeClass()->SetDirection(1);

			//プレイヤーの方向リセット
			g_cScene->GetGameClass()->GetPlayerClass()->SetZeroDirection();

			//プレイヤーの残機
			int current_remain = g_cScene->GetScoreClass()->GetRemainCounter();
			g_cScene->GetScoreClass()->SetRemainCounter(current_remain - 1);

			//ゲーム準備
			g_cScene->GetGameClass()->m_eGameStatus = g_cScene->GetGameClass()->STATUS_GAMEREADY_2;

			//サウンド停止
			StopSoundMem(g_cScene->GetSoundClass()->GetSEDotEat());
			StopSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove());
		}
	}
}

//=============================================================
// プレイヤーとの当たり判定
//=============================================================
bool CEnemy::IsPlayer() 
{
	if (g_cScene->GetGameClass()->GetPlayerClass()->GetCurrentMazeIndex() == m_iCurrentMazeIndex) 
	{
		return true;
	}

	return false;
}

//=============================================================
// ループトンネル付近減速
//=============================================================
bool CEnemy::IsDeceleration() 
{
	if ((200 <= m_iCurrentMazeIndex) && (m_iCurrentMazeIndex <= 203) ||
		(215 <= m_iCurrentMazeIndex) && (m_iCurrentMazeIndex <= 217))
	{
		return true;
	}

	return false;
}

//=============================================================
// プレイヤー追跡
//=============================================================
void CEnemy::Pursuit() 
{
	int playerX = g_cScene->GetGameClass()->GetPlayerClass()->GetPosX() / PLAYER_IMAGE_EXPANSION_X;
	int playerY = g_cScene->GetGameClass()->GetPlayerClass()->GetPosY() / PLAYER_IMAGE_EXPANSION_Y;

	if ((m_iPosX / ENEMY_IMAGE_EXPANSION_X < playerX) && (m_iPosY / ENEMY_IMAGE_EXPANSION_Y < playerY))			//右か下
	{
		int r = rand() % 2;
		m_iDirection = (r == 0) ? 0 : 3;
		SetDirection(m_iDirection);
		m_iPursuitCounter++;
		if (m_iPursuitCounter > 2) 
		{
			SetDirection(1);		//左
			m_iPursuitCounter++;
			if (m_iPursuitCounter > 3) 
			{
				SetDirection(2);	//上
				m_iPursuitCounter = 0;
			}
		}
	}
	else if ((m_iPosX / ENEMY_IMAGE_EXPANSION_X < playerX) && (m_iPosY / ENEMY_IMAGE_EXPANSION_Y > playerY))	//右か上
	{
		int r = rand() % 2;
		m_iDirection = (r == 0) ? 0 : 2;
		SetDirection(m_iDirection);	
		m_iPursuitCounter++;
		if (m_iPursuitCounter > 2)
		{
			SetDirection(1);		//左
			m_iPursuitCounter++;
			if (m_iPursuitCounter > 3)
			{
				SetDirection(3);	//下
				m_iPursuitCounter = 0;
			}
		}
	}
	else if ((m_iPosX / ENEMY_IMAGE_EXPANSION_X > playerX) && (m_iPosY / ENEMY_IMAGE_EXPANSION_Y < playerY))	//左か下
	{
		int r = rand() % 2;
		m_iDirection = (r == 0) ? 1 : 3;
		SetDirection(m_iDirection);
		m_iPursuitCounter++;
		if (m_iPursuitCounter > 2)
		{
			SetDirection(0);		//右
			m_iPursuitCounter++;
			if (m_iPursuitCounter > 3)
			{
				SetDirection(2);	//上
				m_iPursuitCounter = 0;
			}
		}
	}
	else if ((m_iPosX / ENEMY_IMAGE_EXPANSION_X > playerX) && (m_iPosY / ENEMY_IMAGE_EXPANSION_Y > playerY))	//左か上
	{
		int r = rand() % 2;
		m_iDirection = (r == 0) ? 1 : 2;
		SetDirection(m_iDirection);
		m_iPursuitCounter++;
		if (m_iPursuitCounter > 2)
		{
			SetDirection(0);		//右
			m_iPursuitCounter++;
			if (m_iPursuitCounter > 3)
			{
				SetDirection(3);	//下
				m_iPursuitCounter = 0;
			}
		}
	}
	else
	{
		m_iDirection = rand() % 4;
		SetDirection(m_iDirection);
	}
}

//=============================================================
// エネミー移動サウンド
//=============================================================
void CEnemy::SEEnemyMove() 
{
	if (g_cScene->GetGameClass()->m_eGameStatus == g_cScene->GetGameClass()->STATUS_GAME) 
	{
		if (!m_bIzikeState)
		{
			//サウンド再生
			if (!CheckSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove()))
			{
				PlaySoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove(), DX_PLAYTYPE_LOOP);
			}
		}
		else
		{
			//サウンド停止
			if (CheckSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove()))
			{
				StopSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove());
			}
		}
	}
}

//=============================================================
// 描画
//=============================================================
void CEnemy::Draw() const 
{
}

//=============================================================
// 通常状態
//=============================================================
void CEnemy::NormalState() const 
{
}

//=============================================================
// いじけ状態
//=============================================================
void CEnemy::IzikeState() const 
{
	if (m_iIzikeCounter < ENEMY_IZIKE_LIMIT_TIME)
	{
		DrawRotaGraph3(m_iPosX + ENEMY_DRAW_X, m_iPosY + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHEnemyIzike[(m_iAnimCounter / ENEMY_ANIM_SPEED) % 2], TRUE);
	}
	else
	{
		DrawRotaGraph3(m_iPosX + ENEMY_DRAW_X, m_iPosY + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHEnemyIzike[(m_iAnimCounter / ENEMY_ANIM_SPEED) % 4], TRUE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	アカベイクラス
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// コンストラクタ
//=============================================================
CBlinky::CBlinky(int x, int y)
	:CEnemy(x*ENEMY_RELATIVE_MAZE_X, y*ENEMY_RELATIVE_MAZE_Y), m_iBaseCounter(0)
{
	//画像の読み込み
	LoadDivGraph(ENEMY_BLINKY_IMAGE, ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHBlinky);

	//移動方向
	srand((unsigned int)time(NULL));
	m_iDirection = 1;
	SetDirection(m_iDirection);
}

//=============================================================
// デストラクタ
//=============================================================
CBlinky::~CBlinky() 
{
	//画像データ破棄
	for (int i = 0; i < ENEMY_IMAGE_NUM; i++) 
	{
		DeleteGraph(m_iGHBlinky[i]);
	}
}

//=============================================================
// 初動
//=============================================================
void CBlinky::BaseMove() 
{
	//現在のエネミーの位置
	SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

	//移動
	BaseMovement();

	BaseTurn();

	//プレイヤーを食べる
	EatPlayer();

	//いじけカウンタ更新
	IzikeCount();

	//アニメーションカウンタ
	SetAnimCounter(GetAnimCounter() + 1);

	//サウンド
	SEEnemyMove();
}

//=============================================================
// 初動の決まった移動（右上）
//=============================================================
void CBlinky::BaseMovement() 
{
	int x = GetPosX();
	int y = GetPosY();

	//移動方向
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}

			//追跡モード
			m_eMoveMode = MODE_PURSUIT;
			m_iBaseCounter++;
		}
		break;

	case VECTOR_LEFT:		//左
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_UP:			//上
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_DOWN:		//下
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}
			m_iBaseCounter++;
		}
		break;
	}
}

//=============================================================
// 初動の決められた回転
//=============================================================
void CBlinky::BaseTurn() 
{
	//座標の調整
	if (!IsCorrection())
	{
		return;
	}

	//固定位置で回転
	if ((GetCurrentMazeIndex() == 186) || (GetCurrentMazeIndex() == 204)|| (GetCurrentMazeIndex() == 84))
	{
		m_iBaseCounter++;
	}

	switch (m_iBaseCounter) 
	{
	case 1:
		SetDirection(3);	//下
		break;

	case 2:
		SetDirection(1);	//左
		break;

	case 3:
		SetDirection(2);	//上
		break;

	case 4:
		SetDirection(0);	//右
		break;
	}
}

//=============================================================
// リスタート時の動きの制限
//=============================================================
void CBlinky::RestartMove() 
{
	//一定時間待機
	if (GetWait())
	{
		Restart();
	}
	else
	{
		//現在のエネミーの位置
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//移動
		RestartMovement();

		//プレイヤーを食べる
		EatPlayer();

		//いじけカウンタ更新
		IzikeCount();
	}
	//アニメーションカウンタ
	SetAnimCounter(GetAnimCounter() + 1);

	//サウンド
	SEEnemyMove();
}

//=============================================================
// リスタート時の移動
//=============================================================
void CBlinky::RestartMovement() 
{
	int x = GetPosX();
	int y = GetPosY();

	//移動方向
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			//下に制限
			SetDirection(3);
		}
		break;

	case VECTOR_LEFT:		//左
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			//下に制限
			SetDirection(3);
		}
		break;

	case VECTOR_UP:			//上
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}

			//右か左に制限
			m_iDirection = rand() % 2;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_DOWN:		//下
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());

		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}

			//追跡モード
			m_eMoveMode = MODE_PURSUIT;
		}
		break;
	}
}

//=============================================================
// いじけ状態時プレイヤーと衝突後5秒間中央で待機
//=============================================================
void CBlinky::Restart()
{
	SetRestartCounter(GetRestartCounter() + 1);
	if (GetRestartCounter() > 300)
	{
		SetWait(false);
		SetRestartCounter(0);
		SetDirection(2);
	}
}

//=============================================================
// 描画
//=============================================================
void CBlinky::Draw() const 
{
	if (GetIzikeState())
	{
		IzikeState();
	}
	else
	{
		NormalState();
	}
}

//=============================================================
// 通常状態
//=============================================================
void CBlinky::NormalState() const
{
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHBlinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;

	case VECTOR_LEFT:		//左
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHBlinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 2], TRUE);
		break;

	case VECTOR_UP:			//上
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHBlinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 4], TRUE);
		break;

	case VECTOR_DOWN:		//下
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHBlinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 6], TRUE);
		break;

	default:
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHBlinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ピンキークラス
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// コンストラクタ
//=============================================================
CPinky::CPinky(int x, int y)
	:CEnemy(x*ENEMY_RELATIVE_MAZE_X, y*ENEMY_RELATIVE_MAZE_Y), m_iBaseCounter(0)
{
	//画像の読み込み
	LoadDivGraph(ENEMY_PINKY_IMAGE, ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHPinky);

	//移動方向
	srand((unsigned int)time(NULL));
	m_iDirection = 2;
	SetDirection(m_iDirection);

	//スタート判定
	m_bStart = false;
}

//=============================================================
// デストラクタ
//=============================================================
CPinky::~CPinky()
{
	//画像データ破棄
	for (int i = 0; i < ENEMY_IMAGE_NUM; i++)
	{
		DeleteGraph(m_iGHPinky[i]);
	}
}

//=============================================================
// 初動
//=============================================================
void CPinky::BaseMove()
{
	if (m_bStart) 
	{
		//現在のエネミーの位置
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//移動
		BaseMovement();

		BaseTurn();

		//プレイヤーを食べる
		EatPlayer();
	}
	//いじけカウンタ更新
	IzikeCount();

	//アニメーションカウンタ
	SetAnimCounter(GetAnimCounter() + 1);

	//サウンド
	SEEnemyMove();
}

//=============================================================
// 初動の決まった移動（右上）
//=============================================================
void CPinky::BaseMovement()
{
	int x = GetPosX();
	int y = GetPosY();

	//移動方向
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_LEFT:		//左
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_UP:			//上
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_DOWN:		//下
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}
			m_iBaseCounter++;
		}
		break;
	}
}

//=============================================================
// 初動の決められた回転
//=============================================================
void CPinky::BaseTurn()
{
	//座標の調整
	if (!IsCorrection())
	{
		return;
	}

	//固定位置で回転
	if ((GetCurrentMazeIndex() == 186) || (GetCurrentMazeIndex() == 204) || (GetCurrentMazeIndex() == 84))
	{
		m_iBaseCounter++;
	}

	switch (m_iBaseCounter)
	{
	case 1:
		SetDirection(1);	//右
		break;

	case 2:
		SetDirection(3);	//下
		break;

	case 3:
		SetDirection(1);	//左
		break;

	case 4:
		SetDirection(2);	//上
		break;

	case 5:
		//追跡モード
		m_eMoveMode = MODE_PURSUIT;
		break;
	}
}

//=============================================================
// リスタート時の動きの制限
//=============================================================
void CPinky::RestartMove()
{
	//一定時間待機
	if (GetWait())
	{
		Restart();
	}
	else
	{
		//現在のエネミーの位置
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//移動
		RestartMovement();

		//プレイヤーを食べる
		EatPlayer();

		//いじけカウンタ更新
		IzikeCount();
	}
	//アニメーションカウンタ
	SetAnimCounter(GetAnimCounter() + 1);

	//サウンド
	SEEnemyMove();
}

//=============================================================
// リスタート時の移動
//=============================================================
void CPinky::RestartMovement()
{
	int x = GetPosX();
	int y = GetPosY();

	//移動方向
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			//下に制限
			SetDirection(3);
		}
		break;

	case VECTOR_LEFT:		//左
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			//下に制限
			SetDirection(3);
		}
		break;

	case VECTOR_UP:			//上
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}

			//右か左に制限
			m_iDirection = rand() % 2;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_DOWN:		//下
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());

		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}

			//追跡モード
			m_eMoveMode = MODE_PURSUIT;
		}
		break;
	}
}

//=============================================================
// いじけ状態時プレイヤーと衝突後5秒間中央で待機
//=============================================================
void CPinky::Restart()
{
	SetRestartCounter(GetRestartCounter() + 1);
	if (GetRestartCounter() > 300)
	{
		SetWait(false);
		SetRestartCounter(0);
		SetDirection(2);
	}
}

//=============================================================
// 描画
//=============================================================
void CPinky::Draw() const
{
	if (GetIzikeState())
	{
		IzikeState();
	}
	else
	{
		NormalState();
	}
}

//=============================================================
// 通常状態
//=============================================================
void CPinky::NormalState() const
{
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHPinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;

	case VECTOR_LEFT:		//左
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHPinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 2], TRUE);
		break;

	case VECTOR_UP:			//上
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHPinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 4], TRUE);
		break;

	case VECTOR_DOWN:		//下
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHPinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 6], TRUE);
		break;

	default:
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHPinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	アオスケクラス
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// コンストラクタ
//=============================================================
CInky::CInky(int x, int y)
	:CEnemy(x*ENEMY_RELATIVE_MAZE_X, y*ENEMY_RELATIVE_MAZE_Y), m_iBaseCounter(0)
{
	//画像の読み込み
	LoadDivGraph(ENEMY_INKY_IMAGE, ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHInky);

	//移動方向
	srand((unsigned int)time(NULL));
	m_iDirection = 0;
	SetDirection(m_iDirection);

	//スタート判定
	m_bStart = false;
}

//=============================================================
// デストラクタ
//=============================================================
CInky::~CInky()
{
	//画像データ破棄
	for (int i = 0; i < ENEMY_IMAGE_NUM; i++)
	{
		DeleteGraph(m_iGHInky[i]);
	}
}

//=============================================================
// 初動
//=============================================================
void CInky::BaseMove()
{
	if (m_bStart)
	{
		//現在のエネミーの位置
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//移動
		BaseMovement();

		BaseTurn();

		//プレイヤーを食べる
		EatPlayer();
	}
	//いじけカウンタ更新
	IzikeCount();

	//アニメーションカウンタ
	SetAnimCounter(GetAnimCounter() + 1);

	//サウンド
	SEEnemyMove();
}

//=============================================================
// 初動の決まった移動（右上）
//=============================================================
void CInky::BaseMovement()
{
	int x = GetPosX();
	int y = GetPosY();

	//移動方向
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_LEFT:		//左
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_UP:			//上
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_DOWN:		//下
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}
			m_iBaseCounter++;
		}
		break;
	}
}

//=============================================================
// 初動の決められた回転
//=============================================================
void CInky::BaseTurn()
{
	//座標の調整
	if (!IsCorrection())
	{
		return;
	}

	//固定位置で回転
	if ((GetCurrentMazeIndex() == 208))
	{
		m_iBaseCounter++;
	}

	switch (m_iBaseCounter)
	{
	case 1:
		SetDirection(2);	//上
		break;

	case 2:
		SetDirection(1);	//左
		break;

	case 3:
		SetDirection(3);	//下
		break;

	case 4:
		SetDirection(1);	//左
		break;

	case 5:
		//追跡モード
		m_eMoveMode = MODE_PURSUIT;
		break;
	}
}

//=============================================================
// リスタート時の動きの制限
//=============================================================
void CInky::RestartMove()
{
	//一定時間待機
	if (GetWait())
	{
		Restart();
	}
	else
	{
		//現在のエネミーの位置
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//移動
		RestartMovement();

		//プレイヤーを食べる
		EatPlayer();

		//いじけカウンタ更新
		IzikeCount();
	}
	//アニメーションカウンタ
	SetAnimCounter(GetAnimCounter() + 1);

	//サウンド
	SEEnemyMove();
}

//=============================================================
// リスタート時の移動
//=============================================================
void CInky::RestartMovement()
{
	int x = GetPosX();
	int y = GetPosY();

	//移動方向
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			//下に制限
			SetDirection(3);
		}
		break;

	case VECTOR_LEFT:		//左
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			//下に制限
			SetDirection(3);
		}
		break;

	case VECTOR_UP:			//上
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}

			//右か左に制限
			m_iDirection = rand() % 2;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_DOWN:		//下
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());

		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}

			//追跡モード
			m_eMoveMode = MODE_PURSUIT;
		}
		break;
	}
}

//=============================================================
// いじけ状態時プレイヤーと衝突後5秒間中央で待機
//=============================================================
void CInky::Restart()
{
	SetRestartCounter(GetRestartCounter() + 1);
	if (GetRestartCounter() > 300)
	{
		SetWait(false);
		SetRestartCounter(0);
		SetDirection(2);
	}
}

//=============================================================
// 描画
//=============================================================
void CInky::Draw() const
{
	if (GetIzikeState())
	{
		IzikeState();
	}
	else
	{
		NormalState();
	}
}

//=============================================================
// 通常状態
//=============================================================
void CInky::NormalState() const
{
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHInky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;

	case VECTOR_LEFT:		//左
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHInky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 2], TRUE);
		break;

	case VECTOR_UP:			//上
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHInky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 4], TRUE);
		break;

	case VECTOR_DOWN:		//下
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHInky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 6], TRUE);
		break;

	default:
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHInky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	グズタクラス
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// コンストラクタ
//=============================================================
CCryde::CCryde(int x, int y)
	:CEnemy(x*ENEMY_RELATIVE_MAZE_X, y*ENEMY_RELATIVE_MAZE_Y), m_iBaseCounter(0)
{
	//画像の読み込み
	LoadDivGraph(ENEMY_CRYDE_IMAGE, ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHCryde);

	//移動方向
	srand((unsigned int)time(NULL));
	m_iDirection = 1;
	SetDirection(m_iDirection);

	//スタート判定
	m_bStart = false;
}

//=============================================================
// デストラクタ
//=============================================================
CCryde::~CCryde()
{
	//画像データ破棄
	for (int i = 0; i < ENEMY_IMAGE_NUM; i++)
	{
		DeleteGraph(m_iGHCryde[i]);
	}
}

//=============================================================
// 初動
//=============================================================
void CCryde::BaseMove()
{
	if (m_bStart)
	{
		//現在のエネミーの位置
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//移動
		BaseMovement();

		BaseTurn();

		//プレイヤーを食べる
		EatPlayer();
	}
	//いじけカウンタ更新
	IzikeCount();

	//アニメーションカウンタ
	SetAnimCounter(GetAnimCounter() + 1);

	//サウンド
	SEEnemyMove();
}

//=============================================================
// 初動の決まった移動（右上）
//=============================================================
void CCryde::BaseMovement()
{
	int x = GetPosX();
	int y = GetPosY();

	//移動方向
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_LEFT:		//左
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_UP:			//上
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_DOWN:		//下
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}
			m_iBaseCounter++;
		}
		break;
	}
}

//=============================================================
// 初動の決められた回転
//=============================================================
void CCryde::BaseTurn()
{
	//座標の調整
	if (!IsCorrection())
	{
		return;
	}

	//固定位置で回転
	if ((GetCurrentMazeIndex() == 209))
	{
		m_iBaseCounter++;
	}

	switch (m_iBaseCounter)
	{
	case 1:
		SetDirection(2);	//上
		g_cScene->GetGameClass()->GetPinkyClass()->SetStart(true);
		break;

	case 2:
		SetDirection(0);	//右
		break;

	case 3:
		SetDirection(3);	//下
		break;

	case 4:
		SetDirection(0);	//右
		break;

	case 5:
		//追跡モード
		m_eMoveMode = MODE_PURSUIT;
		break;
	}
}

//=============================================================
// リスタート時の動きの制限
//=============================================================
void CCryde::RestartMove()
{
	//一定時間待機
	if (GetWait())
	{
		Restart();
	}
	else
	{
		//現在のエネミーの位置
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//移動
		RestartMovement();

		//プレイヤーを食べる
		EatPlayer();

		//いじけカウンタ更新
		IzikeCount();
	}
	//アニメーションカウンタ
	SetAnimCounter(GetAnimCounter() + 1);

	//サウンド
	SEEnemyMove();
}

//=============================================================
// リスタート時の移動
//=============================================================
void CCryde::RestartMovement()
{
	int x = GetPosX();
	int y = GetPosY();

	//移動方向
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			//下に制限
			SetDirection(3);
		}
		break;

	case VECTOR_LEFT:		//左
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			//下に制限
			SetDirection(3);
		}
		break;

	case VECTOR_UP:			//上
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}

			//右か左に制限
			m_iDirection = rand() % 2;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_DOWN:		//下
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());

		}
		else
		{
			//座標の調整
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}

			//追跡モード
			m_eMoveMode = MODE_PURSUIT;
		}
		break;
	}
}

//=============================================================
// いじけ状態時プレイヤーと衝突後5秒間中央で待機
//=============================================================
void CCryde::Restart()
{
	SetRestartCounter(GetRestartCounter() + 1);
	if (GetRestartCounter() > 300)
	{
		SetWait(false);
		SetRestartCounter(0);
		SetDirection(2);
	}
}

//=============================================================
// 描画
//=============================================================
void CCryde::Draw() const
{
	if (GetIzikeState())
	{
		IzikeState();
	}
	else
	{
		NormalState();
	}
}

//=============================================================
// 通常状態
//=============================================================
void CCryde::NormalState() const
{
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//右
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHCryde[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;

	case VECTOR_LEFT:		//左
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHCryde[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 2], TRUE);
		break;

	case VECTOR_UP:			//上
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHCryde[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 4], TRUE);
		break;

	case VECTOR_DOWN:		//下
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHCryde[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 6], TRUE);
		break;

	default:
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHCryde[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;
	}
}