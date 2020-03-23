#pragma once
#ifndef __PLAYER_INCLUDED_H__
#define __PLAYER_INCLUDED_H__

#include "Maze.h"
#include "Enemy.h"

//�v���C���[���
#define PLAYER_IMAGE	   "Assets/Sprites/pacman.png"
#define PLAYER_IMAGE_NUM   12
#define PLAYER_ANIM_SPEED   4

//�摜�̒���
#define PLAYER_DRAW_X	   130
#define PLAYER_DRAW_Y       20

//�v���C���[���W�����C�Y�i20*22�j�ɒ������邽�߂̕ϐ�
#define PLAYER_RELATIVE_MAZE_X  20
#define PLAYER_RELATIVE_MAZE_Y  20

//�v���C���[�摜�̃T�C�Y
#define PLAYER_IMAGE_EXPANSION_X   1.5
#define PLAYER_IMAGE_EXPANSION_Y   1.5

//=============================================================
// �v���C���[�N���X
//=============================================================
class CPlayer
{
private:
	//���C�Y���
	CMaze m_cMaze;
	int* m_pMaze;

	//���݂̈ʒu
	int m_iCurrentMazeIndex;
	//�O�̈ʒu
	int m_iPreviousMazeIndex;

	//�ړ�����
	enum DIRECTION 
	{
		VECTOR_ZERO,
		VECTOR_RIGHT,
		VECTOR_LEFT,
		VECTOR_UP,
		VECTOR_DOWN,
		VECTOR_MAX,
	};
	DIRECTION m_eDirection = VECTOR_ZERO;

	//���͐�̕���
	DIRECTION m_eInputDirection = VECTOR_ZERO;

	//���W
	int m_iPosX;
	int m_iPosY;

	//�X�s�[�h
	int m_iSpeed;

	//�G�l�~�[�ƏՓ˂������ǂ���
	bool m_bBlinkyCollision;
	bool m_bPinkyCollision;
	bool m_bInkyCollision;
	bool m_bCrydeCollision;

	//�G�l�~�[��H�ׂ����̃X�R�A���Z�̕ύX�J�E���^
	int m_iIzikeScoreCounter;

	//�O���t�B�b�N�n���h��
	int m_iGHPlayer[PLAYER_IMAGE_NUM];

	//�A�j���[�V�����J�E���^
	int m_iAnimCounter;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CPlayer(int x = 0, int y = 0);
	~CPlayer();

	//�Z�b�^�[
	void SetPosX(int x) { m_iPosX = x; }
	void SetPosY(int y) { m_iPosY = y; }
	void SetSpeed(int speed) { m_iSpeed = speed; }
	void SetZeroDirection() { m_eDirection = VECTOR_ZERO; }
	void SetBlinkyCollision(bool collision) { m_bBlinkyCollision = collision; }
	void SetPinkyCollision(bool collision) { m_bPinkyCollision = collision; }
	void SetInkyCollision(bool collision) { m_bInkyCollision = collision; }
	void SetCrydeCollision(bool collision) { m_bCrydeCollision = collision; }
	void SetIzikeScoreCounter(int count) { m_iIzikeScoreCounter = count; }

	//�Q�b�^�[
	int GetPosX() const { return m_iPosX; }
	int GetPosY() const { return m_iPosY; }
	int* GetMaze() const { return m_pMaze; }
	int GetCurrentMazeIndex() const { return m_iCurrentMazeIndex; }
	bool GetBlinkyCollision() const { return m_bBlinkyCollision; }
	bool GetPinkyCollision() const { return m_bPinkyCollision; }
	bool GetInkyCollision() const { return m_bInkyCollision; }
	bool GetCrydeCollision() const { return m_bCrydeCollision; }
	int GetIzikeScoreCounter() const { return m_iIzikeScoreCounter; }

	//����
	void Input();

	//����
	void Move();
	void Movement();			//�ړ�
	bool IsWall();				//�ǂƂ̓����蔻��
	void ClearDot(int index);	//�h�b�g���N���A
	bool IsTurn();				//���͐�̈ړ�������ێ�
	void CheckTurn();			//���͐�̈ړ�������ێ����ǂłȂ���Ή�]
	bool IsCorrection();		//���W�̕␳
	void Warp(int index);		//���[�v  200��218
	void EatEnemy();			//�G�l�~�[��H�׃X�^�[�g�n�_�֖߂�
	void IsEnemy();				//�G�l�~�[�Ƃ̓����蔻��
	void AddScore(int add);		//�X�R�A���Z
	void AddScoreOnEnemy();		//�G�l�~�[�ɂ�����X�R�A���Z
	void SEDotEat();			//�h�b�g�C�[�g�T�E���h
	void StopSound();			//�T�E���h�����ׂĎ~�߂�
	void EatFruit();			//�t���[�c��H�ׂăX�R�A�A�b�v

	//�`��
	void Draw() const;
};

#endif



