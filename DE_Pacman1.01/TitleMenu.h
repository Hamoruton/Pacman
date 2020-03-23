#pragma once
#ifndef __TITLEMENU_H_INCLUDED__
#define __TITLEMENU_H_INCLUDED__

//フォント
#define TITLEMENU_FONT_CHIP		"アタリフォント完全版"
#define TITLEMENU_FONT_NUM		2

// 画像情報
#define TITLE_PACMAN_IMAGE			"Assets/Sprites/pacman.png"
#define TITLE_PACMAN_IMAGE_NUM		12
#define TITLE_BLINKY_IMAGE			"Assets/Sprites/blinky.png"
#define TITLE_PINKY_IMAGE			"Assets/Sprites/pinky.png"
#define TITLE_INKY_IMAGE			"Assets/Sprites/inky.png"
#define TITLE_CRYDE_IMAGE			"Assets/Sprites/cryde.png"
#define TITLE_IZIKE_IMAGE			"Assets/Sprites/izike.png"
#define TITLE_ENEMY_IMAGE_NUM		 8

//画像の調整
#define TITLE_IMAGE_DRAW_X  150
#define TITLE_IMAGE_DRAW_Y  180

//アニメーションスピード
#define TITLE_IMAGE_ANIM_SPEED  5

//=============================================================
// タイトルメニュークラス
//=============================================================
class CTitleMenu 
{
private:
	//選択状態
	enum SELECTSTATUS
	{
		SELECT_PLAY,
		SELECT_CONFIG,
		SELECT_EXIT,
	};
	SELECTSTATUS m_eSelectStatus;

	//フォントハンドル
	int m_iFontHandle[TITLEMENU_FONT_NUM];

	//グラフィックハンドル
	int m_iGHPacman[TITLE_PACMAN_IMAGE_NUM];
	int m_iGHBlinky[TITLE_ENEMY_IMAGE_NUM];
	int m_iGHPinky[TITLE_ENEMY_IMAGE_NUM];
	int m_iGHInky[TITLE_ENEMY_IMAGE_NUM];
	int m_iGHCryde[TITLE_ENEMY_IMAGE_NUM];
	int m_iGHIzike[TITLE_ENEMY_IMAGE_NUM];

	//座標
	int m_iPosX;
	int m_iPosY;

	//方向
	bool m_bRightDirection;

	//アニメーションカウンタ
	int m_iAnimCounter;

public:
	//コンストラクタ.デストラクタ
	CTitleMenu();
	~CTitleMenu();

	//入力
	void Input();

	//動作
	void Move();
	void MoveAnimation();	//アニメーション動作

	//描画
	void Draw() const;
	void Text() const;				//テキスト
	void DrawAnimation() const;		//アニメーション描画
	void SelectText() const;		//選択テキスト
};

#endif