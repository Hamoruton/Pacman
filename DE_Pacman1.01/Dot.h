#pragma once
#ifndef __DOT_H_INCLUDED__
#define __DOT_H_INCLUDED__

//ドット情報
#define DOT_IMAGE		 "Assets/Sprites/pacdot.png"
#define POWER_DOT_IMAGE  "Assets/Sprites/powerpacdot.png"

//描画の調整
#define DOT_DRAW_X					140
#define DOT_DRAW_Y					 30
#define DOT_INTERVAL				 20
#define DOT_IMAGE_EXPANSION			1.5
#define POWER_DOT_IMAGE_EXPANSION     1

#define FLASH_SPEED   20

//=============================================================
// ドット（えさ）クラス
//=============================================================
class CDot
{
private:
	//グラフィックハンドル
	int m_iGHDot;
	int m_iGHPowerDot;

	//点滅
	int  m_iBlendCount;
	bool m_bBlendLimitMax = false;

	//ドットカウンタ
	int m_iDotCounter;

public:
	//コンストラクタ.デストラクタ
	CDot();
	~CDot();

	//ゲッター
	int GetDotCounter() const { return m_iDotCounter; }

	//動作
	void Move();
	void DotCount();	//ドットの数
	void Flash();		//点滅

	//描画
	void Draw() const;
	void PowerDotFlash(int x, int y) const;
};

#endif