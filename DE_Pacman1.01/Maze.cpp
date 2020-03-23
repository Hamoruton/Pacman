#include "Maze.h"
#include "DxLib.h"
#include "Scene.h"
#include <iostream>
#include <string>
#include <fstream>

//=============================================================
// �R���X�g���N�^
//=============================================================
CMaze::CMaze() :m_iFlashTimer(0)
{
	//���C�Y������
	InitMaze();

	//�N���A���̃X�e�[�W�_��
	m_bFlash = false;
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CMaze::~CMaze() 
{
}

//=============================================================
// ���C�Y������
//=============================================================
void CMaze::InitMaze() 
{
	std::ifstream FileName("Stage.txt");
	if (FileName.fail())
	{
		DrawFormatString(0, 0, GetColor(255, 255, 255), "�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B");
	}

	char temp;
	for (int y = 0; y < MAZE_HEIGHT; y++)
	{
		for (int x = 0; x < MAZE_WIDTH; x++)
		{
			if (FileName.get(temp))
			{
				m_iMaze[y][x] = (int)temp;
			}
		}
	}
	m_pMaze = &m_iMaze[0][0];
}

//=============================================================
// ����
//=============================================================
void CMaze::Move() 
{
	//�N���A���̃X�e�[�W�_��
	if (g_cScene->GetGameClass()->m_eGameStatus == g_cScene->GetGameClass()->STATUS_GAMECLEAR) 
	{
		if (m_bFlash)
		{
			m_iFlashTimer++;
			if (m_iFlashTimer > 15)
			{
				//���]
				m_bFlash = false;
				m_iFlashTimer = 0;
			}
		}
		else
		{
			m_iFlashTimer++;
			if (m_iFlashTimer > 15)
			{
				//���]
				m_bFlash = true;
				m_iFlashTimer = 0;
			}
		}
	}
}

//=============================================================
// �`��
//=============================================================
void CMaze::Draw() const 
{
	for (int y = 0; y < MAZE_HEIGHT; y++)
	{
		for (int x = 0; x < MAZE_WIDTH; x++)
		{
			if (m_iMaze[y][x] == TILE_WALL) 
			{
				if (!m_bFlash)
					DrawFormatString(x * MAZE_INTERVAL + MAZE_DRAW_X, y * MAZE_INTERVAL + MAZE_DRAW_Y, GetColor(0, 0, 255), "��");
				else
					DrawFormatString(x * MAZE_INTERVAL + MAZE_DRAW_X, y * MAZE_INTERVAL + MAZE_DRAW_Y, GetColor(255, 255, 255), "��");
			}
		}
	}
}