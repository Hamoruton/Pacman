#include "Pause.h"
#include "DxLib.h"
#include "Scene.h"

//=============================================================
// コンストラクタ
//=============================================================
CPause::CPause() 
{
	//フォントの読み込み
	m_iFontHandle[0] = CreateFontToHandle(PAUSE_FONT_CHIP, 15, 3);
	m_iFontHandle[1] = CreateFontToHandle(PAUSE_FONT_CHIP, 50, 3);

	//画像の読み込み
	m_iGHBigPacman = LoadGraph(PAUSE_BIGPACMAN_IMAGE);

	//選択状態
	m_bReturnTitle = true;
}

//=============================================================
// デストラクタ
//=============================================================
CPause::~CPause() 
{
	//フォントの破棄
	for (int i = 0; i < PAUSE_FONT_NUM; i++)
	{
		DeleteFontToHandle(m_iFontHandle[i]);
	}

	//画像の破棄
	DeleteGraph(m_iGHBigPacman);
}

//=============================================================
// 入力
//=============================================================
void CPause::Input() 
{
	if (g_cScene->m_iKey[KEY_INPUT_UP] == 1) 
	{
		//サウンド
		PlaySoundMem(g_cScene->GetSoundClass()->GetSECursor(), DX_PLAYTYPE_BACK);

		if (!m_bReturnTitle) 
			m_bReturnTitle = true;

	}
	else if (g_cScene->m_iKey[KEY_INPUT_DOWN] == 1) 
	{
		//サウンド
		PlaySoundMem(g_cScene->GetSoundClass()->GetSECursor(), DX_PLAYTYPE_BACK);

		if (m_bReturnTitle)
			m_bReturnTitle = false;
	}
	else if (g_cScene->m_iKey[KEY_INPUT_RETURN] == 1) 
	{
		if (m_bReturnTitle) 
		{
			g_cScene->m_eSceneStatus = g_cScene->FROM_GAME_TO_TITLE;
		}
		else
		{
			g_cScene->GetGameClass()->m_eGameStatus = g_cScene->GetGameClass()->STATUS_GAME;
			m_bReturnTitle = true;
		}
	}
}

//=============================================================
// 動作
//=============================================================
void CPause::Move() 
{
}

//=============================================================
// 描画
//=============================================================
void CPause::Draw() const
{
	//パックマン
	DrawGraph(400, 200, m_iGHBigPacman, TRUE);

	//枠
	Frame();

	//テキスト
	Text();
}

//=============================================================
// テキスト
//=============================================================
void CPause::Text() const 
{
	DrawStringToHandle(180, 60, "PAUSE", GetColor(255, 255, 255), m_iFontHandle[1]);

	DrawStringToHandle(200, 200, "RETURN TITLE", GetColor(255, 255, 255), m_iFontHandle[0]);
	DrawStringToHandle(200, 300, "RETURN GAME ", GetColor(255, 255, 255), m_iFontHandle[0]);
}

//=============================================================
// 枠
//=============================================================
void CPause::Frame() const 
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
	if (m_bReturnTitle) 
	{
		DrawBox(180, 180, 380, 235, GetColor(125, 125, 125), TRUE);
	}
	else
	{
		DrawBox(180, 280, 380, 335, GetColor(125, 125, 125), TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	DrawBox(180, 180, 380, 235, GetColor(255, 255, 255), FALSE);
	DrawBox(180, 280, 380, 335, GetColor(255, 255, 255), FALSE);
}