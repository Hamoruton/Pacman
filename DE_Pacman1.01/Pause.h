#pragma once
#ifndef __PAUSE_H_INCLUDED__
#define __PAUSE_H_INCLUDED__

//フォント
#define PAUSE_FONT_CHIP		"アタリフォント完全版"
#define PAUSE_FONT_NUM		2

//画像情報
#define PAUSE_BIGPACMAN_IMAGE	"Assets/Sprites/big_pacman.png"

//=============================================================
// ポーズクラス
//=============================================================
class CPause
{
private:
	//フォントハンドル
	int m_iFontHandle[PAUSE_FONT_NUM];

	//グラフィックハンドル
	int m_iGHBigPacman;

	//選択状態
	bool m_bReturnTitle;

public:
	//コンストラクタ.デストラクタ
	CPause();
	~CPause();

	//入力
	void Input();

	//動作
	void Move();

	//描画
	void Draw() const;
	void Text() const;
	void Frame() const;
};

#endif



