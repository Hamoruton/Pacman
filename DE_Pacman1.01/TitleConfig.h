#pragma once
#ifndef __TITLECONFIG_H_INCLUDED__
#define __TITLECONFIG_H_INCLUDED__

//フォント
#define TITLECONFIG_FONT_CHIP		"アタリフォント完全版"
#define TITLECONFIG_FONT_NUM		3

//=============================================================
// タイトル設定クラス
//=============================================================
class CTitleConfig
{
private:
	//選択状態
	enum SELECTSTATUS
	{
		SELECT_SOUND,
		SELECT_HIGHSCORE,
		SELECT_DECISION,
	};
	SELECTSTATUS m_eSelectStatus;

	//フォントハンドル
	int m_iFontHandle[TITLECONFIG_FONT_NUM];

	//サウンドの設定
	bool m_bSoundConfig;

	//ハイスコアリセット
	bool m_bHighScoreReset;

public:
	//コンストラクタ.デストラクタ
	CTitleConfig();
	~CTitleConfig();

	//入力
	void Input();

	//動作
	void Move();
	void SoundSetting();		//サウンドの設定

	//描画
	void Draw() const;
	void Text() const;			//テキスト
	void Frame() const;			//枠
};

#endif