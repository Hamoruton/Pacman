#pragma once
#ifndef __GAME_H_INCLUDED__
#define __GAME_H_INCLUDED__

#include "Player.h"
#include "Maze.h"
#include "Dot.h"
#include "Enemy.h"
#include "Fruit.h"
#include "Pause.h"

//プレイヤーリスタート時の一時停止時間
#define GAME_RESTART_TIME	300

//=============================================================
// ゲームクラス
//=============================================================
class CGame
{
public:
	//ゲーム状態
	enum GAMESTATUS
	{
		STATUS_GAMEREADY_1 = 0,
		STATUS_GAMEREADY_2 = 1,
		STATUS_GAME = 10,
		STATUS_GAMECLEAR = 100,
		STATUS_GAMEOVER = 150,
		STATUS_PAUSE = 300,
	};
	GAMESTATUS m_eGameStatus;

private:
	//プレイヤーのクラス
	CPlayer* m_cPlayer;
	//メイズクラス
	CMaze* m_cMaze;
	//ドットクラス
	CDot* m_cDot;
	//エネミークラス
	CBlinky* m_cBlinky;
	CPinky* m_cPinky;
	CInky* m_cInky;
	CCryde* m_cCryde;
	//フルーツクラス
	CFruit* m_cFruit;
	//ポーズクラス
	CPause* m_cPause;

	//リスタート
	bool m_bRestart = false;
	int  m_iRestartCounter;

	//ゲーム準備タイマー
	int m_iGameReadyTimer;
	//ゲームタイマー
	int m_iGameTimer;
	//ゲームクリアタイマー
	int m_iClearTimer;
	//ゲームオーバータイマー
	int m_iGameOverTimer;
	bool m_bSEGameOver;

	//フォントハンドル
	int m_iFontHandle;

public:
	//コンストラクタ、デストラクタ
	CGame();
	~CGame();

	//ゲッター
	CMaze* GetMazeClass() const { return m_cMaze; }
	CPlayer* GetPlayerClass() const { return m_cPlayer; }
	CDot* GetDotClass() const { return m_cDot; }
	CBlinky* GetBlinkyClass() const { return m_cBlinky; }
	CPinky* GetPinkyClass() const { return m_cPinky; }
	CInky* GetInkyClass() const { return m_cInky; }
	CCryde* GetCrydeClass() const { return m_cCryde; }
	CFruit* GetFruitClass() const { return m_cFruit; }

	//セッター
	void SetRestart(bool restart) { m_bRestart = restart; }

	//更新
	void Update();

private:
	//入力
	void Input();

	//動作
	void Move();
	void EnemyMove(CEnemy* enemy);		//エネミー動作
	void GameTime();					//ゲーム時間
	bool IsGameReady();					//ゲーム準備
	bool IsGameClear();					//ゲームクリアかどうか
	void GameClear();					//ゲームクリア
	bool IsGameOver();					//ゲームオーバーかどうか
	void GameOver();					//ゲームオーバー

	//描画
	void Draw() const;
	void GameReadyText1() const;
	void GameReadyText2() const;
	void GameOverText() const;
	void GameClearText() const;
};
#endif



