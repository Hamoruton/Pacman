#include "Dot.h"
#include "DxLib.h"
#include "Scene.h"

//=============================================================
// �R���X�g���N�^
//=============================================================
CDot::CDot()
	:m_iBlendCount(0), m_iDotCounter(156)
{
	//�摜�̓ǂݍ���
	m_iGHDot = LoadGraph(DOT_IMAGE);
	m_iGHPowerDot = LoadGraph(POWER_DOT_IMAGE);
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CDot::~CDot() 
{
	//�摜�f�[�^�̔j��
	DeleteGraph(m_iGHDot);
	DeleteGraph(m_iGHPowerDot);
}

//=============================================================
// ����
//=============================================================
void CDot::Move() 
{
	//�h�b�g�̐�
	DotCount();

	//�_��
	Flash();
}

//=============================================================
// �h�b�g�̐�
//=============================================================
void CDot::DotCount() 
{
	int count = 0;
	m_iDotCounter = 0;
	for (int y = 0; y < MAZE_HEIGHT; y++) 
	{
		for (int x = 0; x < MAZE_WIDTH; x++) 
		{
			if (*(g_cScene->GetGameClass()->GetPlayerClass()->GetMaze() + count) == g_cScene->GetGameClass()->GetMazeClass()->TILE_DOT) 
			{
				m_iDotCounter++;
			}
			else if (*(g_cScene->GetGameClass()->GetPlayerClass()->GetMaze() + count) == g_cScene->GetGameClass()->GetMazeClass()->TILE_POWER_DOT) 
			{
				m_iDotCounter++;
			}
			count++;
		}
	}
}

//=============================================================
// �_��
//=============================================================
void CDot::Flash() 
{
	//�_��
	if (!m_bBlendLimitMax)
	{
		m_iBlendCount += FLASH_SPEED;
		if (m_iBlendCount > 255)
			m_bBlendLimitMax = true;
	}
	else
	{
		m_iBlendCount -= FLASH_SPEED;
		if (m_iBlendCount < 0)
			m_bBlendLimitMax = false;
	}
}

//=============================================================
// �`��
//=============================================================
void CDot::Draw() const
{
	int count = 0;
	for (int y = 0; y < MAZE_HEIGHT; y++) 
	{
		for (int x = 0; x < MAZE_WIDTH; x++) 
		{
			if (*(g_cScene->GetGameClass()->GetPlayerClass()->GetMaze() + count) == g_cScene->GetGameClass()->GetMazeClass()->TILE_DOT)
			{
				DrawRotaGraph(x*DOT_INTERVAL + DOT_DRAW_X, y*DOT_INTERVAL + DOT_DRAW_Y, DOT_IMAGE_EXPANSION, 0, m_iGHDot, TRUE);
			}
			else if (*(g_cScene->GetGameClass()->GetPlayerClass()->GetMaze() + count) == g_cScene->GetGameClass()->GetMazeClass()->TILE_POWER_DOT)
			{
				PowerDotFlash(x, y);
			}
			count++;
		}
	}
}

//=============================================================
// �p���[�h�b�g�i�_�Łj
//=============================================================
void CDot::PowerDotFlash(int x, int y) const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_iBlendCount);
	DrawRotaGraph(x*DOT_INTERVAL + DOT_DRAW_X, y*DOT_INTERVAL + DOT_DRAW_Y, POWER_DOT_IMAGE_EXPANSION, 0, m_iGHPowerDot, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}