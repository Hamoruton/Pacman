#include "Coffeebreak.h"
#include "DxLib.h"
#include "Scene.h"

//=============================================================
// コンストラクタ
//=============================================================
CCoffeebreak::CCoffeebreak() :m_iPosX(0), m_iPosY(0), m_iAnimCounter(0), m_iTimer(0)
{
	//画像の読み込み
	LoadDivGraph(COFFEEBREAK_PACMAN_IMAGE, COFFEEBREAK_PACMAN_IMAGE_NUM, 3, 4, 16, 16, m_iGHPacman);
	LoadDivGraph(COFFEEBREAK_BLINKY_IMAGE, COFFEEBREAK_ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHBlinky);
	LoadDivGraph(COFFEEBREAK_IZIKE_IMAGE, COFFEEBREAK_ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHIzike);

	//方向
	m_bRightDirection = false;

	//サウンド再生
	PlaySoundMem(g_cScene->GetSoundClass()->GetSECoffeebreak(), DX_PLAYTYPE_BACK);
}

//=============================================================
// デストラクタ
//=============================================================
CCoffeebreak::~CCoffeebreak() 
{
	//画像の破棄
	for (int i = 0; i < COFFEEBREAK_PACMAN_IMAGE_NUM; i++) 
	{
		DeleteGraph(m_iGHPacman[i]);
	}
	for (int i = 0; i < COFFEEBREAK_ENEMY_IMAGE_NUM; i++) 
	{
		DeleteGraph(m_iGHBlinky[i]);
		DeleteGraph(m_iGHIzike[i]);
	}
}

//=============================================================
// 更新
//=============================================================
void CCoffeebreak::Update() 
{
	//動作
	Move();

	//描画
	Draw();
}

//=============================================================
// 動作
//=============================================================
void CCoffeebreak::Move() 
{
	//次のステージ（13秒後）
	NextStage();

	//アニメーション動作
	MoveAnimation();
}

//=============================================================
// アニメーション動作
//=============================================================
void CCoffeebreak::MoveAnimation() 
{
	//反転
	if (m_iPosX < -450)
		m_bRightDirection = true;

	//移動
	if (m_bRightDirection) 
	{
		m_iPosX += 1;
	}
	else
		m_iPosX -= 2;

	//アニメーションカウンタの更新
	m_iAnimCounter++;
}

//=============================================================
// 次のステージ（13秒後）
//=============================================================
void CCoffeebreak::NextStage() 
{
	m_iTimer++;
	if (m_iTimer > 13 * 60) 
	{
		g_cScene->m_eSceneStatus = g_cScene->FROM_COFFEEBREAK_TO_GAME;
	}
}

//=============================================================
// 描画
//=============================================================
void CCoffeebreak::Draw() const 
{
	//アニメーション描画
	DrawAnimation();
}

//=============================================================
// アニメーション描画
//=============================================================
void CCoffeebreak::DrawAnimation() const 
{
	if (m_bRightDirection)
	{
		//パックマン
		DrawRotaGraph3(m_iPosX * 2 + COFFEEBREAK_IMAGE_DRAW_X, m_iPosY + COFFEEBREAK_IMAGE_DRAW_Y - 25, 0, 0,
			4.0, 4.0, 0, m_iGHPacman[(m_iAnimCounter / COFFEEBREAK_IMAGE_ANIM_SPEED) % 3], TRUE);

		//エネミー
		DrawRotaGraph3(m_iPosX + COFFEEBREAK_IMAGE_DRAW_X + 50, m_iPosY + COFFEEBREAK_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHIzike[(m_iAnimCounter / COFFEEBREAK_IMAGE_ANIM_SPEED) % 2], TRUE);
	}
	else
	{
		//パックマン
		DrawRotaGraph3(m_iPosX + COFFEEBREAK_IMAGE_DRAW_X, m_iPosY + COFFEEBREAK_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHPacman[(m_iAnimCounter / COFFEEBREAK_IMAGE_ANIM_SPEED) % 3 + 3], TRUE);

		//エネミー
		DrawRotaGraph3(m_iPosX + COFFEEBREAK_IMAGE_DRAW_X + 30, m_iPosY + COFFEEBREAK_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHBlinky[(m_iAnimCounter / COFFEEBREAK_IMAGE_ANIM_SPEED) % 2], TRUE);
	}

	//カーテン
	DrawBox(480, 150, 640, 480, GetColor(0, 0, 0), TRUE);
	DrawBox(0, 150, 140, 480, GetColor(0, 0, 0), TRUE);
}