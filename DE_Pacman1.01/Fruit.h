#pragma once
#ifndef __FRUIT_H_INCLUDED__
#define __FRUIT_H_INCLUDED__

#include "Maze.h"

//�摜���
#define FRUIT_IMAGE			"Assets/Sprites/fruits.png"
#define FRUIT_IMAGE_NUM		 2

//�摜�̒���
#define FRUIT_DRAW_X	130
#define FRUIT_DRAW_Y	 20

//�T�C�Y
#define FRUIT_IMAGE_EXPANSION_X		1.5
#define FRUIT_IMAGE_EXPANSION_Y		1.5

//�t���[�c���W�����C�Y�i20*22�j�ɒ������邽�߂̕ϐ�
#define FRUIT_RELATIVE_MAZE_X		20
#define FRUIT_RELATIVE_MAZE_Y		20

#define FRUIT_OUTBREAK_SPEED  10*60

//=============================================================
// �t���[�c�N���X
//=============================================================
class CFruit
{
public:
	//�t���[�c�̏��
	enum FRUITSTATUS
	{
		CHERRY,
		STRAWBERRY,
	};
	FRUITSTATUS m_eFruitStatus;

private:
	//�O���t�B�b�N�n���h��
	int m_iGHFruit[FRUIT_IMAGE_NUM];

	//���W
	int m_iPosX;
	int m_iPosY;

	//���݂̈ʒu
	int m_iCurrentMazeIndex;

	//�������ڂ����������ǂ���
	bool m_bOutbreakCherry;

	//�����������������ǂ���
	bool m_bOutbreakStrawberry;

	//�����J�E���^
	int m_iOutbreakCounter;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CFruit();
	~CFruit();

	//�Z�b�^�[
	void SetOutbreakCherry(bool flag) { m_bOutbreakCherry = flag; }
	void SetOutbreakStrawberry(bool flag) { m_bOutbreakStrawberry = flag; }
	void SetOutbreakCounter(int count) { m_iOutbreakCounter = count; }

	//�Q�b�^�[
	int GetCurrentMazeIndex() const { return m_iCurrentMazeIndex; }
	bool GetOutbreakCherry() const { return m_bOutbreakCherry; }
	bool GetOutbreakStrawberry() const { return m_bOutbreakStrawberry; }

	//����
	void Move();

	//�`��
	void Draw() const;
};

#endif