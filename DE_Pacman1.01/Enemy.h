#pragma once
#ifndef __ENEMY_H_INCLUDED__
#define __ENEMY_H_INCLUDED__

#include "Maze.h"
#include "Player.h"

//エネミー情報
#define ENEMY_BLINKY_IMAGE			"Assets/Sprites/blinky.png"
#define ENEMY_PINKY_IMAGE			"Assets/Sprites/pinky.png"
#define ENEMY_INKY_IMAGE			"Assets/Sprites/inky.png"
#define ENEMY_CRYDE_IMAGE			"Assets/Sprites/cryde.png"
#define ENEMY_IMAGE_NUM				8
#define ENEMY_ANIM_SPEED			5
#define ENEMY_IZIKE_IMAGE			"Assets/Sprites/izike.png"
#define ENEMY_IZIKE_IMAGE_NUM		8

//減速スピード
#define ENEMY_DECELERATION_SPEED	1

//エネミー画像の調整座標
#define ENEMY_DRAW_X	130
#define ENEMY_DRAW_Y	 20

//エネミー座標をメイズ（20*22）に調整するための変数
#define ENEMY_RELATIVE_MAZE_X  20
#define ENEMY_RELATIVE_MAZE_Y  20

//エネミー画像のサイズ
#define ENEMY_IMAGE_EXPANSION_X   1.5
#define ENEMY_IMAGE_EXPANSION_Y   1.5

//いじけタイマー
#define ENEMY_IZIKE_TIME			420
#define ENEMY_IZIKE_LIMIT_TIME		ENEMY_IZIKE_TIME - 180	

//=============================================================
// エネミークラス
//=============================================================
class CEnemy
{
public:
	//移動状態
	enum MOVEMODE 
	{
		MODE_NONE,
		MODE_INIT,
		MODE_PURSUIT,
		MODE_RESTART,
		MODE_MAX,
	};
	MOVEMODE m_eMoveMode = MODE_NONE;

	//移動方向
	enum DIRECTION
	{
		VECTOR_ZERO,
		VECTOR_RIGHT,
		VECTOR_LEFT,
		VECTOR_UP,
		VECTOR_DOWN,
		VECTOR_MAX,
	};
	DIRECTION m_eDirection = VECTOR_ZERO;
	//ランダム値
	int m_iDirection;

private:
	//メイズ情報
	CMaze m_cMaze;
	int* m_pMaze;

	//現在の位置
	int m_iCurrentMazeIndex;
	//前の位置
	int m_iPreviousMazeIndex;

	//座標
	int m_iPosX;
	int m_iPosY;

	//スピード
	int m_iSpeed;

	//待機（プレイヤーとの衝突後）
	bool m_bWait;

	//グラフィックハンドル
	int m_iGHEnemyIzike[ENEMY_IZIKE_IMAGE_NUM];

	//エネミーがいじけ状態かどうか
	bool m_bIzikeState;
	//いじけカウンタ
	int m_iIzikeCounter;

	//リスタートカウンタ
	int m_iRestartCounter;

	//減速カウンタ
	int m_iDecelerationCounter;

	//追跡カウンタ
	int m_iPursuitCounter;

	//アニメーションカウンタ
	int m_iAnimCounter;

public:
	//コンストラクタ.デストラクタ
	CEnemy(int x = 0, int y = 0);
	~CEnemy();

	//セッター
	void SetPosX(int x) { m_iPosX = x; }
	void SetPosY(int y) { m_iPosY = y; }
	void SetSpeed(int speed) { m_iSpeed = speed; }
	void SetDirection(int d);
	void SetZeroDirection() { m_eDirection = VECTOR_ZERO; }
	void SetAnimCounter(int count) { m_iAnimCounter = count; }
	void SetIzikeState(bool state) { m_bIzikeState = state; }
	void SetIzikeCounter(int count) { m_iIzikeCounter = count; }
	void SetRestartCounter(int count) { m_iRestartCounter = count; }
	void SetCurrentMazeIndex(int index) { m_iCurrentMazeIndex = index; }
	void SetWait(bool wait) { m_bWait = wait; }

	//ゲッター
	int GetPosX() const { return m_iPosX; }
	int GetPosY() const { return m_iPosY; }
	int GetSpeed() const { return m_iSpeed; }
	int* GetMaze() const { return m_pMaze; }
	int GetAnimCounter() const { return m_iAnimCounter; }
	int GetIzikeCounter() const { return m_iIzikeCounter; }
	bool GetIzikeState() const { return m_bIzikeState; }
	int GetRestartCounter() const { return m_iRestartCounter; }
	int GetCurrentMazeIndex() const { return m_iCurrentMazeIndex; }
	bool GetWait() const { return m_bWait; }

	//動作
	void Move();
	virtual void BaseMove();			//初動
	virtual void RestartMove();			//リスタート時の動きの制限
	void Movement();					//移動
	bool IsWall();						//壁との当たり判定
	bool IsCorrection();				//座標の調整
	void Warp(int index);				//ワープ  200⇔218
	bool IsBothEndsTurn(int index);		//進行方向から見て両端が移動できるかどうか
	void CheckTurn();					//現在の位置から回転できる方向をランダムで決める
	void IzikeCount();					//いじけカウンタの更新
	virtual void Restart();				//いじけ状態時プレイヤーと衝突後5秒間中央で待機
	void EatPlayer();					//プレイヤーを食べスタート地点へ戻す
	bool IsPlayer();					//プレイヤーとの当たり判定
	bool IsDeceleration();				//ループトンネル付近減速
	void Pursuit();						//プレイヤー追跡
	void SEEnemyMove();					//エネミー移動サウンド

	//描画
	virtual void Draw() const;
	void NormalState() const;		//通常状態
	void IzikeState() const;		//いじけ状態
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// アカベイクラス（エネミークラス）
//=============================================================
class CBlinky 
	:public CEnemy
{
private:
	//グラフィックハンドル
	int m_iGHBlinky[ENEMY_IMAGE_NUM];

	//ベースカウンタ
	int m_iBaseCounter;

public:
	//コンストラクタ.デストラクタ
	CBlinky(int x = 0, int y = 0);
	~CBlinky();

	//動作
	void BaseMove();				//初動
	void BaseMovement();			//初動の決まった移動（右上へ移動）
	void BaseTurn();				//初動の決められた回転
	void RestartMove();				//リスタート時の動きの制限
	void RestartMovement();			//リスタート時の移動
	void Restart();					//いじけ状態時プレイヤーと衝突後5秒間中央で待機

	//描画
	void Draw() const;
	void NormalState() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// ピンキークラス（エネミークラス）
//=============================================================
class CPinky
	:public CEnemy
{
private:
	//グラフィックハンドル
	int m_iGHPinky[ENEMY_IMAGE_NUM];

	//ベースカウンタ
	int m_iBaseCounter;

	//スタート判定
	bool m_bStart;

public:
	//コンストラクタ.デストラクタ
	CPinky(int x = 0, int y = 0);
	~CPinky();

	//セッター
	void SetStart(bool start) { m_bStart = start; }

	//ゲッター
	bool GetStart() const { return m_bStart; }

	//動作
	void BaseMove();				//初動
	void BaseMovement();			//初動の決まった移動（右上へ移動）
	void BaseTurn();				//初動の決められた回転
	void RestartMove();				//リスタート時の動きの制限
	void RestartMovement();			//リスタート時の移動
	void Restart();					//いじけ状態時プレイヤーと衝突後5秒間中央で待機

	//描画
	void Draw() const;
	void NormalState() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// アオスケクラス（エネミークラス）
//=============================================================
class CInky
	:public CEnemy
{
private:
	//グラフィックハンドル
	int m_iGHInky[ENEMY_IMAGE_NUM];

	//ベースカウンタ
	int m_iBaseCounter;

	//スタート判定
	bool m_bStart;

public:
	//コンストラクタ.デストラクタ
	CInky(int x = 0, int y = 0);
	~CInky();

	//セッター
	void SetStart(bool start) { m_bStart = start; }

	//ゲッター
	bool GetStart() const { return m_bStart; }

	//動作
	void BaseMove();				//初動
	void BaseMovement();			//初動の決まった移動（右上へ移動）
	void BaseTurn();				//初動の決められた回転
	void RestartMove();				//リスタート時の動きの制限
	void RestartMovement();			//リスタート時の移動
	void Restart();					//いじけ状態時プレイヤーと衝突後5秒間中央で待機

	//描画
	void Draw() const;
	void NormalState() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// グズタクラス（エネミークラス）
//=============================================================
class CCryde
	:public CEnemy
{
private:
	//グラフィックハンドル
	int m_iGHCryde[ENEMY_IMAGE_NUM];

	//ベースカウンタ
	int m_iBaseCounter;

	//スタート判定
	bool m_bStart;

public:
	//コンストラクタ.デストラクタ
	CCryde(int x = 0, int y = 0);
	~CCryde();

	//セッター
	void SetStart(bool start) { m_bStart = start; }

	//ゲッター
	bool GetStart() const { return m_bStart; }

	//動作
	void BaseMove();				//初動
	void BaseMovement();			//初動の決まった移動（右上へ移動）
	void BaseTurn();				//初動の決められた回転
	void RestartMove();				//リスタート時の動きの制限
	void RestartMovement();			//リスタート時の移動
	void Restart();					//いじけ状態時プレイヤーと衝突後5秒間中央で待機

	//描画
	void Draw() const;
	void NormalState() const;
};

#endif



