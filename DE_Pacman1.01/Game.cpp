#include "Game.h"
#include "DxLib.h"
#include "Scene.h"

//=============================================================
// コンストラクタ
//=============================================================
CGame::CGame()
	:m_iRestartCounter(0), m_iGameTimer(0), m_iGameReadyTimer(0), m_iClearTimer(0), m_iGameOverTimer(0)
{
	//ゲーム状態
	m_eGameStatus = STATUS_GAMEREADY_1;

	//クラスの生成
	m_cMaze = new CMaze;
	m_cDot = new CDot;
	m_cPlayer = new CPlayer(9, 16);
	m_cBlinky = new CBlinky(9, 8);
	m_cPinky = new CPinky(9, 10);
	m_cInky = new CInky(8, 10);
	m_cCryde = new CCryde(10, 10);
	m_cFruit = new CFruit;
	m_cPause = new CPause;

	//フォントの読み込み
	m_iFontHandle = CreateFontToHandle(GAME_FONT_CHIP, 12, 3);

	//サウンド
	PlaySoundMem(g_cScene->GetSoundClass()->GetSEGameStart(), DX_PLAYTYPE_BACK);
	m_bSEGameOver = false;
}

//=============================================================
// デストラクタ
//=============================================================
CGame::~CGame()
{
	//クラスの破棄
	delete m_cBlinky;
	delete m_cPinky;
	delete m_cInky;
	delete m_cCryde;
	delete m_cPlayer;
	delete m_cDot;
	delete m_cFruit;
	delete m_cMaze;
	delete m_cPause;

	//フォントの破棄
	DeleteFontToHandle(m_iFontHandle);
}

//=============================================================
// 更新
//=============================================================
void CGame::Update()
{
	//入力
	Input();
	//動作
	Move();
	//描画
	Draw();
}

//=============================================================
// 入力
//=============================================================
void CGame::Input() 
{
	switch (m_eGameStatus) 
	{
	case STATUS_GAME:
		//プレイヤー
		m_cPlayer->Input();
		break;

	case STATUS_PAUSE:
		//ポーズ
		m_cPause->Input();
		break;
	}
}

//=============================================================
// 動作
//=============================================================
void CGame::Move() 
{
	switch (m_eGameStatus) 
	{
	//ゲーム準備
	case STATUS_GAMEREADY_1:
		if (!IsGameReady()) 
		{
			m_eGameStatus = STATUS_GAMEREADY_2;
		}
		break;

	case STATUS_GAMEREADY_2:
		if (!IsGameReady())
		{
			m_eGameStatus = STATUS_GAME;
		}
		break;

	//ゲーム
	case STATUS_GAME:
		//ゲームタイム
		GameTime();

		//プレイヤー
		m_cPlayer->Move();

		//エネミー
		EnemyMove(m_cBlinky);
		EnemyMove(m_cPinky);
		EnemyMove(m_cInky);
		EnemyMove(m_cCryde);

		//フルーツ
		m_cFruit->Move();

		//ゲームクリアかどうか
		if (IsGameClear()) 
		{
			m_eGameStatus = STATUS_GAMECLEAR;
		}

		//ゲームオーバーかどうか
		if (IsGameOver())
		{
			m_eGameStatus = STATUS_GAMEOVER;
		}

		break;

	//ゲームクリア
	case STATUS_GAMECLEAR:
		GameClear();
		break;

	//ゲームオーバー
	case STATUS_GAMEOVER:
		GameOver();
		break;

	//ポーズ
	case STATUS_PAUSE:
		m_cPause->Move();
		break;
	}

	//ドット
	m_cDot->Move();

	//メイズ
	m_cMaze->Move();
}

//=============================================================
// エネミー動作
//=============================================================
void CGame::EnemyMove(CEnemy* enemy) 
{
	switch (enemy->m_eMoveMode) 
	{
	case enemy->MODE_INIT:
		enemy->BaseMove();
		break;

	case enemy->MODE_PURSUIT:
		enemy->Move();
		break;

	case enemy->MODE_RESTART:
		enemy->RestartMove();
		break;
	}
}

//=============================================================
// ゲームタイム
//=============================================================
void CGame::GameTime() 
{
	m_iGameTimer++;
	if (m_iGameTimer > 1 * 60) 
	{
		//ピンキー移動開始
		m_cPinky->SetStart(true);
	}

	if (m_iGameTimer > 8 * 60) 
	{
		//インキー移動開始
		m_cInky->SetStart(true);
	}

	if (m_iGameTimer > 12 * 60) 
	{
		//グズタ移動開始
		m_cCryde->SetStart(true);
	}
}

//=============================================================
// ゲーム準備
//=============================================================
bool CGame::IsGameReady() 
{
	m_iGameReadyTimer++;
	if (m_iGameReadyTimer > 2 * 60) 
	{
		m_iGameReadyTimer = 0;
		return false;
	}
	return true;
}

//=============================================================
// ゲームクリアかどうか
//=============================================================
bool CGame::IsGameClear() 
{
	//ドットの数取得
	int current_dot_count = m_cDot->GetDotCounter();
	if (current_dot_count <= 0) 
	{
		return true;
	}

	return false;
}

//=============================================================
// ゲームクリア
//=============================================================
void CGame::GameClear() 
{
	//サウンド停止
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

	m_iClearTimer++;
	if (m_iClearTimer > 5 * 60) 
	{
		if (g_cScene->m_eSceneStatus == g_cScene->GAME_01) 
		{
			//コーヒーブレイク遷移
			g_cScene->m_eSceneStatus = g_cScene->FROM_GAME_TO_COFFEEBREAK;
		}
		else if (g_cScene->m_eSceneStatus == g_cScene->GAME_02) 
		{
			//タイトル遷移
			g_cScene->m_eSceneStatus = g_cScene->FROM_GAME_TO_TITLE;
		}
	}
}

//=============================================================
// ゲームオーバーかどうか
//=============================================================
bool CGame::IsGameOver()
{
	//プレイヤーの残機取得
	int current_remain = g_cScene->GetScoreClass()->GetRemainCounter();
	if (current_remain < 0) 
	{
		return true;
	}

	return false;
}

//=============================================================
// ゲームオーバー
//=============================================================
void CGame::GameOver() 
{
	//サウンド停止
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

	//サウンド再生
	if (!CheckSoundMem(g_cScene->GetSoundClass()->GetSEPlayerEat())) 
	{
		if (!m_bSEGameOver) 
		{
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEPlayerEat(), DX_PLAYTYPE_BACK);
			m_bSEGameOver = true;
		}
	}

	m_iGameOverTimer++;
	if (m_iGameOverTimer > 5 * 60) 
	{
		//タイトル遷移
		g_cScene->m_eSceneStatus = g_cScene->FROM_GAME_TO_TITLE;
	}
}

//=============================================================
// 描画
//=============================================================
void CGame::Draw() const 
{
	switch (m_eGameStatus) 
	{
	//ゲーム準備
	case STATUS_GAMEREADY_1:
		//テキスト
		GameReadyText1();

		//メイズ
		m_cMaze->Draw();

		//ドット
		m_cDot->Draw();

		//スコア
		g_cScene->GetScoreClass()->Draw();
		break;

	case STATUS_GAMEREADY_2:
		//テキスト
		GameReadyText2();

		//メイズ
		m_cMaze->Draw();

		//ドット
		m_cDot->Draw();

		//スコア
		g_cScene->GetScoreClass()->Draw();

		//プレイヤー
		m_cPlayer->Draw();

		//エネミー
		m_cBlinky->Draw();
		m_cPinky->Draw();
		m_cInky->Draw();
		m_cCryde->Draw();
		break;

	//ゲーム
	case STATUS_GAME:
		//メイズ
		m_cMaze->Draw();

		//ドット
		m_cDot->Draw();

		//フルーツ
		m_cFruit->Draw();

		//スコア
		g_cScene->GetScoreClass()->Draw();

		//エネミー
		m_cBlinky->Draw();
		m_cPinky->Draw();
		m_cInky->Draw();
		m_cCryde->Draw();

		//プレイヤー
		m_cPlayer->Draw();
		break;

	//ゲームクリア
	case STATUS_GAMECLEAR:
		//テキスト
		GameClearText();

		//メイズ
		m_cMaze->Draw();

		//ドット
		m_cDot->Draw();

		//スコア
		g_cScene->GetScoreClass()->Draw();
		break;

	//ゲームオーバー
	case STATUS_GAMEOVER:
		//テキスト
		GameOverText();

		//メイズ
		m_cMaze->Draw();

		//ドット
		m_cDot->Draw();

		//スコア
		g_cScene->GetScoreClass()->Draw();
		break;

	//ポーズ
	case STATUS_PAUSE:
		m_cPause->Draw();
		break;
	}
}

//=============================================================
// ゲーム準備テキスト
//=============================================================
void CGame::GameReadyText1() const 
{
	DrawStringFToHandle(280, 185, "PLAYER", GetColor(0, 255, 255), m_iFontHandle);
	DrawStringFToHandle(280, 265, "READY!", GetColor(255, 255, 0), m_iFontHandle);
}

void CGame::GameReadyText2() const 
{
	DrawStringFToHandle(280, 265, "READY!", GetColor(255, 255, 0), m_iFontHandle);
}

//=============================================================
// ゲームクリアテキスト
//=============================================================
void CGame::GameClearText() const
{
	DrawStringFToHandle(280, 185, "PLAYER", GetColor(0, 255, 255), m_iFontHandle);
	DrawStringFToHandle(260, 265, "GAMECLEAR", GetColor(255, 255, 0), m_iFontHandle);
}

//=============================================================
// ゲームオーバーテキスト
//=============================================================
void CGame::GameOverText() const 
{
	DrawStringFToHandle(280, 185, "PLAYER", GetColor(0, 255, 255), m_iFontHandle);
	DrawStringFToHandle(270, 265, "GAMEOVER", GetColor(255, 0, 0), m_iFontHandle);
}