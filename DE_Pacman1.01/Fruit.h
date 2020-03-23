#pragma once
#ifndef __FRUIT_H_INCLUDED__
#define __FRUIT_H_INCLUDED__

#include "Maze.h"

//画像情報
#define FRUIT_IMAGE			"Assets/Sprites/fruits.png"
#define FRUIT_IMAGE_NUM		 2

//画像の調整
#define FRUIT_DRAW_X	130
#define FRUIT_DRAW_Y	 20

//サイズ
#define FRUIT_IMAGE_EXPANSION_X		1.5
#define FRUIT_IMAGE_EXPANSION_Y		1.5

//フルーツ座標をメイズ（20*22）に調整するための変数
#define FRUIT_RELATIVE_MAZE_X		20
#define FRUIT_RELATIVE_MAZE_Y		20

#define FRUIT_OUTBREAK_SPEED  10*60

//=============================================================
// フルーツクラス
//=============================================================
class CFruit
{
public:
	//フルーツの状態
	enum FRUITSTATUS
	{
		CHERRY,
		STRAWBERRY,
	};
	FRUITSTATUS m_eFruitStatus;

private:
	//グラフィックハンドル
	int m_iGHFruit[FRUIT_IMAGE_NUM];

	//座標
	int m_iPosX;
	int m_iPosY;

	//現在の位置
	int m_iCurrentMazeIndex;

	//さくらんぼが発生中かどうか
	bool m_bOutbreakCherry;

	//いちごが発生中かどうか
	bool m_bOutbreakStrawberry;

	//発生カウンタ
	int m_iOutbreakCounter;

public:
	//コンストラクタ.デストラクタ
	CFruit();
	~CFruit();

	//セッター
	void SetOutbreakCherry(bool flag) { m_bOutbreakCherry = flag; }
	void SetOutbreakStrawberry(bool flag) { m_bOutbreakStrawberry = flag; }
	void SetOutbreakCounter(int count) { m_iOutbreakCounter = count; }

	//ゲッター
	int GetCurrentMazeIndex() const { return m_iCurrentMazeIndex; }
	bool GetOutbreakCherry() const { return m_bOutbreakCherry; }
	bool GetOutbreakStrawberry() const { return m_bOutbreakStrawberry; }

	//動作
	void Move();

	//描画
	void Draw() const;
};

#endif