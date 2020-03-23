#pragma once
#ifndef __MAZE_H_INCLUDED__
#define __MAZE_H_INCLUDED__

//�z��
#define MAZE_WIDTH  20
#define MAZE_HEIGHT 22

//�`��̒���
#define MAZE_DRAW_X	   130
#define MAZE_DRAW_Y		20
#define MAZE_INTERVAL	20

//=============================================================
// ���C�Y�N���X
//=============================================================
class CMaze
{
public:
	//���C�Y�^�C���̏��
	enum MAZETILE
	{
		TILE_INDENTION = ' ',
		TILE_NONE = '-',
		TILE_WALL = '#',
		TILE_DOT = 'o',
		TILE_POWER_DOT = '0',
		TILE_MAX,
	};
	MAZETILE m_eMazeTile;

private:
	//���C�Y���
	int m_iMaze[MAZE_HEIGHT][MAZE_WIDTH] = { 0 };
	int* m_pMaze;

	//�N���A���̃X�e�[�W�_��
	bool m_bFlash;
	//�_�Ń^�C�}�[
	int m_iFlashTimer;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CMaze();
	~CMaze();

	//�Z�b�^�[
	void SetFlash(bool flash) { m_bFlash = flash; }
	void SetFlashTimer(int time) { m_iFlashTimer = time; }

	//�Q�b�^�[
	int* GetMaze() const { return m_pMaze; }

	//����
	void Move();
	void InitMaze();	//���C�Y������

	//�`��
	void Draw() const;
};

#endif

