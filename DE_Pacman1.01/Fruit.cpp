#include "Fruit.h"
#include "DxLib.h"

//=============================================================
// �R���X�g���N�^
//=============================================================
CFruit::CFruit()
	:m_iPosX(9 * FRUIT_RELATIVE_MAZE_X), m_iPosY(12 * FRUIT_RELATIVE_MAZE_Y), m_iOutbreakCounter(0)
{
	//�摜�̓ǂݍ���
	LoadDivGraph(FRUIT_IMAGE, FRUIT_IMAGE_NUM, 32, 16, 16, 16, m_iGHFruit);

	//���C�Y�z��ɂ�����t���[�c�̈ʒu
	m_iCurrentMazeIndex = m_iPosY / FRUIT_RELATIVE_MAZE_X * MAZE_WIDTH + m_iPosX / FRUIT_RELATIVE_MAZE_Y;

	//�t���[�c�̏��
	m_eFruitStatus = CHERRY;
	m_bOutbreakCherry = false;
	m_bOutbreakStrawberry = false;
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CFruit::~CFruit() 
{
	//�摜�f�[�^�j��
	for (int i = 0; i < FRUIT_IMAGE_NUM; i++) 
	{
		DeleteGraph(m_iGHFruit[i]);
	}
}

//=============================================================
// ����
//=============================================================
void CFruit::Move() 
{
	m_iOutbreakCounter++;
	if (m_iOutbreakCounter > FRUIT_OUTBREAK_SPEED)
	{
		switch (m_eFruitStatus) 
		{
		case CHERRY:

			//���]
			m_bOutbreakCherry = !m_bOutbreakCherry;

			if (!m_bOutbreakCherry) 
			{
				m_eFruitStatus = STRAWBERRY;
			}
			break;

		case STRAWBERRY:

			//���]
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
// �`��
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