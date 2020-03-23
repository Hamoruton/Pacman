#pragma once
#ifndef __COFFEEBREAK_H_INCLUDED__
#define __COFFEEBREAK_H_INCLUDED__

//画像情報
#define COFFEEBREAK_PACMAN_IMAGE		"Assets/Sprites/pacman.png"
#define COFFEEBREAK_PACMAN_IMAGE_NUM	12
#define COFFEEBREAK_BLINKY_IMAGE		"Assets/Sprites/blinky.png"
#define COFFEEBREAK_IZIKE_IMAGE			"Assets/Sprites/izike.png"
#define COFFEEBREAK_ENEMY_IMAGE_NUM		 8

//画像の調整
#define COFFEEBREAK_IMAGE_DRAW_X	450
#define COFFEEBREAK_IMAGE_DRAW_Y	180

//アニメーションスピード
#define COFFEEBREAK_IMAGE_ANIM_SPEED  5

//=============================================================
// コーヒーブレイククラス
//=============================================================
class CCoffeebreak
{
private:
	//グラフィックハンドル
	int m_iGHPacman[COFFEEBREAK_PACMAN_IMAGE_NUM];
	int m_iGHBlinky[COFFEEBREAK_ENEMY_IMAGE_NUM];
	int m_iGHIzike[COFFEEBREAK_ENEMY_IMAGE_NUM];

	//座標
	int m_iPosX;
	int m_iPosY;

	//方向
	bool m_bRightDirection;

	//タイマー（13秒後次のステージ遷移）
	int m_iTimer;

	//アニメーションカウンタ
	int m_iAnimCounter;

public:
	//コンストラクタ.デストラクタ
	CCoffeebreak();
	~CCoffeebreak();

	//更新
	void Update();

	//動作
	void Move();
	void MoveAnimation();			//アニメーション動作
	void NextStage();				//次のステージ

	//描画
	void Draw() const;
	void DrawAnimation() const;		//アニメーション描画
};

#endif