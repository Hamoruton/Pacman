#pragma once
#ifndef __PLAYER_INCLUDED_H__
#define __PLAYER_INCLUDED_H__

#include "Maze.h"
#include "Enemy.h"

//プレイヤー情報
#define PLAYER_IMAGE	   "Assets/Sprites/pacman.png"
#define PLAYER_IMAGE_NUM   12
#define PLAYER_ANIM_SPEED   4

//画像の調整
#define PLAYER_DRAW_X	   130
#define PLAYER_DRAW_Y       20

//プレイヤー座標をメイズ（20*22）に調整するための変数
#define PLAYER_RELATIVE_MAZE_X  20
#define PLAYER_RELATIVE_MAZE_Y  20

//プレイヤー画像のサイズ
#define PLAYER_IMAGE_EXPANSION_X   1.5
#define PLAYER_IMAGE_EXPANSION_Y   1.5

//=============================================================
// プレイヤークラス
//=============================================================
class CPlayer
{
private:
	//メイズ情報
	CMaze m_cMaze;
	int* m_pMaze;

	//現在の位置
	int m_iCurrentMazeIndex;
	//前の位置
	int m_iPreviousMazeIndex;

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

	//入力先の方向
	DIRECTION m_eInputDirection = VECTOR_ZERO;

	//座標
	int m_iPosX;
	int m_iPosY;

	//スピード
	int m_iSpeed;

	//エネミーと衝突したかどうか
	bool m_bBlinkyCollision;
	bool m_bPinkyCollision;
	bool m_bInkyCollision;
	bool m_bCrydeCollision;

	//エネミーを食べた時のスコア加算の変更カウンタ
	int m_iIzikeScoreCounter;

	//グラフィックハンドル
	int m_iGHPlayer[PLAYER_IMAGE_NUM];

	//アニメーションカウンタ
	int m_iAnimCounter;

public:
	//コンストラクタ.デストラクタ
	CPlayer(int x = 0, int y = 0);
	~CPlayer();

	//セッター
	void SetPosX(int x) { m_iPosX = x; }
	void SetPosY(int y) { m_iPosY = y; }
	void SetSpeed(int speed) { m_iSpeed = speed; }
	void SetZeroDirection() { m_eDirection = VECTOR_ZERO; }
	void SetBlinkyCollision(bool collision) { m_bBlinkyCollision = collision; }
	void SetPinkyCollision(bool collision) { m_bPinkyCollision = collision; }
	void SetInkyCollision(bool collision) { m_bInkyCollision = collision; }
	void SetCrydeCollision(bool collision) { m_bCrydeCollision = collision; }
	void SetIzikeScoreCounter(int count) { m_iIzikeScoreCounter = count; }

	//ゲッター
	int GetPosX() const { return m_iPosX; }
	int GetPosY() const { return m_iPosY; }
	int* GetMaze() const { return m_pMaze; }
	int GetCurrentMazeIndex() const { return m_iCurrentMazeIndex; }
	bool GetBlinkyCollision() const { return m_bBlinkyCollision; }
	bool GetPinkyCollision() const { return m_bPinkyCollision; }
	bool GetInkyCollision() const { return m_bInkyCollision; }
	bool GetCrydeCollision() const { return m_bCrydeCollision; }
	int GetIzikeScoreCounter() const { return m_iIzikeScoreCounter; }

	//入力
	void Input();

	//動作
	void Move();
	void Movement();			//移動
	bool IsWall();				//壁との当たり判定
	void ClearDot(int index);	//ドットをクリア
	bool IsTurn();				//入力先の移動方向を保持
	void CheckTurn();			//入力先の移動方向を保持し壁でなければ回転
	bool IsCorrection();		//座標の補正
	void Warp(int index);		//ワープ  200⇔218
	void EatEnemy();			//エネミーを食べスタート地点へ戻す
	void IsEnemy();				//エネミーとの当たり判定
	void AddScore(int add);		//スコア加算
	void AddScoreOnEnemy();		//エネミーにおけるスコア加算
	void SEDotEat();			//ドットイートサウンド
	void StopSound();			//サウンドをすべて止める
	void EatFruit();			//フルーツを食べてスコアアップ

	//描画
	void Draw() const;
};

#endif



