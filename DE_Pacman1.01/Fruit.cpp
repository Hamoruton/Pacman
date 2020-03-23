#include "Fruit.h"
#include "DxLib.h"

//=============================================================
// コンストラクタ
//=============================================================
CFruit::CFruit()
	:m_iPosX(9 * FRUIT_RELATIVE_MAZE_X), m_iPosY(12 * FRUIT_RELATIVE_MAZE_Y), m_iOutbreakCounter(0)
{
	//画像の読み込み
	LoadDivGraph(FRUIT_IMAGE, FRUIT_IMAGE_NUM, 32, 16, 16, 16, m_iGHFruit);

	//メイズ配列におけるフルーツの位置
	m_iCurrentMazeIndex = m_iPosY / FRUIT_RELATIVE_MAZE_X * MAZE_WIDTH + m_iPosX / FRUIT_RELATIVE_MAZE_Y;

	//フルーツの状態
	m_eFruitStatus = CHERRY;
	m_bOutbreakCherry = false;
	m_bOutbreakStrawberry = false;
}

//=============================================================
// デストラクタ
//=============================================================
CFruit::~CFruit() 
{
	//画像データ破棄
	for (int i = 0; i < FRUIT_IMAGE_NUM; i++) 
	{
		DeleteGraph(m_iGHFruit[i]);
	}
}

//=============================================================
// 動作
//=============================================================
void CFruit::Move() 
{
	m_iOutbreakCounter++;
	if (m_iOutbreakCounter > FRUIT_OUTBREAK_SPEED)
	{
		switch (m_eFruitStatus) 
		{
		case CHERRY:

			//反転
			m_bOutbreakCherry = !m_bOutbreakCherry;

			if (!m_bOutbreakCherry) 
			{
				m_eFruitStatus = STRAWBERRY;
			}
			break;

		case STRAWBERRY:

			//反転
			m_bOutbreakStrawberry = !m_bOutbreakStrawberry;

			if (!m_bOutbreakStrawberry)
			{
				m_eFruitStatus = CHERRY;
			}
			break;
		}
		m_iOutbreakCounter = 0;
	}
}

//=============================================================
// 描画
//=============================================================
void CFruit::Draw() const 
{
	switch (m_eFruitStatus) 
	{
	case CHERRY:
		if (m_bOutbreakCherry)
		{
			DrawRotaGraph3(m_iPosX + FRUIT_DRAW_X, m_iPosY + FRUIT_DRAW_Y, 0, 0, FRUIT_IMAGE_EXPANSION_X, FRUIT_IMAGE_EXPANSION_Y, 0, m_iGHFruit[0], TRUE);
		}
		break;

	case STRAWBERRY:
		if (m_bOutbreakStrawberry)
		{
			DrawRotaGraph3(m_iPosX + FRUIT_DRAW_X, m_iPosY + FRUIT_DRAW_Y, 0, 0, FRUIT_IMAGE_EXPANSION_X, FRUIT_IMAGE_EXPANSION_Y, 0, m_iGHFruit[1], TRUE);
		}
		break;
	}
}