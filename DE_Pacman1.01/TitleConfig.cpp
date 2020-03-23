#include "TitleConfig.h"
#include "DxLib.h"
#include "Scene.h"

//=============================================================
// コンストラクタ
//=============================================================
CTitleConfig::CTitleConfig() 
{
	//フォントの読み込み
	m_iFontHandle[0] = CreateFontToHandle(TITLECONFIG_FONT_CHIP, 15, 3);
	m_iFontHandle[1] = CreateFontToHandle(TITLECONFIG_FONT_CHIP, 50, 3);
	m_iFontHandle[2] = CreateFontToHandle(TITLECONFIG_FONT_CHIP, 10, 3);

	//選択状態
	m_eSelectStatus = SELECT_SOUND;

	//サウンド設定
	m_bSoundConfig = true;

	//ハイスコアリセット
	m_bHighScoreReset = false;
}

//=============================================================
// デストラクタ
//=============================================================
CTitleConfig::~CTitleConfig() 
{
	//フォントの破棄
	for (int i = 0; i < TITLECONFIG_FONT_NUM; i++)
	{
		DeleteFontToHandle(m_iFontHandle[i]);
	}
}

//=============================================================
// 入力
//=============================================================
void CTitleConfig::Input() 
{
	if (g_cScene->m_iKey[KEY_INPUT_RIGHT] == 1) 
	{
		if (m_eSelectStatus == SELECT_SOUND) 
		{
			//反転
			m_bSoundConfig = !m_bSoundConfig;
		}
	}
	else if (g_cScene->m_iKey[KEY_INPUT_LEFT] == 1) 
	{
		if (m_eSelectStatus == SELECT_SOUND) 
		{
			//反転
			m_bSoundConfig = !m_bSoundConfig;
		}
	}
	else if (g_cScene->m_iKey[KEY_INPUT_UP] == 1) 
	{
		//サウンド
		PlaySoundMem(g_cScene->GetSoundClass()->GetSECursor(), DX_PLAYTYPE_BACK);

		if (m_eSelectStatus == SELECT_DECISION) 
		{
			m_eSelectStatus = SELECT_HIGHSCORE;
		}
		else if (m_eSelectStatus == SELECT_HIGHSCORE)
		{
			m_eSelectStatus = SELECT_SOUND;
		}
	}
	else if (g_cScene->m_iKey[KEY_INPUT_DOWN] == 1)
	{
		//サウンド
		PlaySoundMem(g_cScene->GetSoundClass()->GetSECursor(), DX_PLAYTYPE_BACK);

		if (m_eSelectStatus == SELECT_SOUND)
		{
			m_eSelectStatus = SELECT_HIGHSCORE;
		}
		else if (m_eSelectStatus == SELECT_HIGHSCORE)
		{
			m_eSelectStatus = SELECT_DECISION;
		}
	}
	else if (g_cScene->m_iKey[KEY_INPUT_RETURN] == 1)
	{
		if (m_eSelectStatus == SELECT_HIGHSCORE) 
		{
			//反転
			m_bHighScoreReset = !m_bHighScoreReset;
		}

		if (m_eSelectStatus == SELECT_DECISION) 
		{
			//スコアリセット
			if (m_bHighScoreReset) 
			{
				g_cScene->GetScoreClass()->HighScoreReset();
				m_bHighScoreReset = false;
			}

			//タイトルメニュー画面へ
			g_cScene->GetTitleClass()->m_eTitleStatus = g_cScene->GetTitleClass()->STATUS_MENU;
			m_eSelectStatus = SELECT_SOUND;
		}
	}
}

//=============================================================
// 動作
//=============================================================
void CTitleConfig::Move() 
{
	//サウンドの設定
	SoundSetting();
}

//=============================================================
// サウンドの設定
//=============================================================
void CTitleConfig::SoundSetting() 
{
	if (m_bSoundConfig) 
	{
		ChangeVolumeSoundMem(255, g_cScene->GetSoundClass()->GetSECursor());
		ChangeVolumeSoundMem(255, g_cScene->GetSoundClass()->GetSECoffeebreak());
		ChangeVolumeSoundMem(255, g_cScene->GetSoundClass()->GetSEDotEat());
		ChangeVolumeSoundMem(255, g_cScene->GetSoundClass()->GetSEEnemyEat());
		ChangeVolumeSoundMem(255, g_cScene->GetSoundClass()->GetSEEnemyMove());
		ChangeVolumeSoundMem(255, g_cScene->GetSoundClass()->GetSEGameStart());
		ChangeVolumeSoundMem(255, g_cScene->GetSoundClass()->GetSEIzikeMove());
		ChangeVolumeSoundMem(255, g_cScene->GetSoundClass()->GetSEPlayerEat());
		ChangeVolumeSoundMem(255, g_cScene->GetSoundClass()->GetSEFruitEat());
	}
	else 
	{
		ChangeVolumeSoundMem(0, g_cScene->GetSoundClass()->GetSECursor());
		ChangeVolumeSoundMem(0, g_cScene->GetSoundClass()->GetSECoffeebreak());
		ChangeVolumeSoundMem(0, g_cScene->GetSoundClass()->GetSEDotEat());
		ChangeVolumeSoundMem(0, g_cScene->GetSoundClass()->GetSEEnemyEat());
		ChangeVolumeSoundMem(0, g_cScene->GetSoundClass()->GetSEEnemyMove());
		ChangeVolumeSoundMem(0, g_cScene->GetSoundClass()->GetSEGameStart());
		ChangeVolumeSoundMem(0, g_cScene->GetSoundClass()->GetSEIzikeMove());
		ChangeVolumeSoundMem(0, g_cScene->GetSoundClass()->GetSEPlayerEat());
		ChangeVolumeSoundMem(0, g_cScene->GetSoundClass()->GetSEFruitEat());
	}
}

//=============================================================
// 描画
//=============================================================
void CTitleConfig::Draw() const 
{
	//枠
	Frame();

	//テキスト
	Text();
}

//=============================================================
// テキスト
//=============================================================
void CTitleConfig::Text() const 
{
	//設定
	DrawStringToHandle(170, 30, "CONFIG", GetColor(255, 255, 255), m_iFontHandle[1]);

	//サウンド
	DrawStringToHandle(50, 130, "SOUND", GetColor(255, 255, 255), m_iFontHandle[0]);
	if (m_bSoundConfig) 
	{
		DrawStringToHandle(480, 130, "- O N  -", GetColor(255, 255, 255), m_iFontHandle[0]);
	}
	else 
	{
		DrawStringToHandle(480, 130, "- OFF  -", GetColor(255, 255, 255), m_iFontHandle[0]);
	}
	
	//ハイスコア
	DrawStringToHandle(50, 230, "HIGHSCORE RESET", GetColor(255, 255, 255), m_iFontHandle[0]);
	if (m_bHighScoreReset) 
	{
		DrawStringToHandle(514, 234, "*", GetColor(255, 255, 255), m_iFontHandle[0]);
	}

	//決定
	DrawStringToHandle(540, 430, "OK", GetColor(255, 255, 255), m_iFontHandle[2]);
}

//=============================================================
// 枠
//=============================================================
void CTitleConfig::Frame() const 
{
	DrawBox(30, 100, 610, 180, GetColor(255, 255, 255), FALSE);
	DrawBox(30, 200, 610, 280, GetColor(255, 255, 255), FALSE);
	DrawBox(500, 220, 540, 260, GetColor(255, 255, 255), FALSE);
	DrawBox(500, 410, 600, 460, GetColor(255, 255, 255), FALSE);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
	switch (m_eSelectStatus) 
	{
	case SELECT_SOUND:
		DrawBox(30, 100, 610, 180, GetColor(255, 255, 255), TRUE);
		break;

	case SELECT_HIGHSCORE:
		DrawBox(30, 200, 610, 280, GetColor(255, 255, 255), TRUE);
		break;

	case SELECT_DECISION:
		DrawBox(500, 410, 600, 460, GetColor(255, 255, 255), TRUE);
		break;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}