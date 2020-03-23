#pragma once
#ifndef __ENEMY_H_INCLUDED__
#define __ENEMY_H_INCLUDED__

#include "Maze.h"
#include "Player.h"

//�G�l�~�[���
#define ENEMY_BLINKY_IMAGE			"Assets/Sprites/blinky.png"
#define ENEMY_PINKY_IMAGE			"Assets/Sprites/pinky.png"
#define ENEMY_INKY_IMAGE			"Assets/Sprites/inky.png"
#define ENEMY_CRYDE_IMAGE			"Assets/Sprites/cryde.png"
#define ENEMY_IMAGE_NUM				8
#define ENEMY_ANIM_SPEED			5
#define ENEMY_IZIKE_IMAGE			"Assets/Sprites/izike.png"
#define ENEMY_IZIKE_IMAGE_NUM		8

//�����X�s�[�h
#define ENEMY_DECELERATION_SPEED	1

//�G�l�~�[�摜�̒������W
#define ENEMY_DRAW_X	130
#define ENEMY_DRAW_Y	 20

//�G�l�~�[���W�����C�Y�i20*22�j�ɒ������邽�߂̕ϐ�
#define ENEMY_RELATIVE_MAZE_X  20
#define ENEMY_RELATIVE_MAZE_Y  20

//�G�l�~�[�摜�̃T�C�Y
#define ENEMY_IMAGE_EXPANSION_X   1.5
#define ENEMY_IMAGE_EXPANSION_Y   1.5

//�������^�C�}�[
#define ENEMY_IZIKE_TIME			420
#define ENEMY_IZIKE_LIMIT_TIME		ENEMY_IZIKE_TIME - 180	

//=============================================================
// �G�l�~�[�N���X
//=============================================================
class CEnemy
{
public:
	//�ړ����
	enum MOVEMODE 
	{
		MODE_NONE,
		MODE_INIT,
		MODE_PURSUIT,
		MODE_RESTART,
		MODE_MAX,
	};
	MOVEMODE m_eMoveMode = MODE_NONE;

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
	//�����_���l
	int m_iDirection;

private:
	//���C�Y���
	CMaze m_cMaze;
	int* m_pMaze;

	//���݂̈ʒu
	int m_iCurrentMazeIndex;
	//�O�̈ʒu
	int m_iPreviousMazeIndex;

	//���W
	int m_iPosX;
	int m_iPosY;

	//�X�s�[�h
	int m_iSpeed;

	//�ҋ@�i�v���C���[�Ƃ̏Փˌ�j
	bool m_bWait;

	//�O���t�B�b�N�n���h��
	int m_iGHEnemyIzike[ENEMY_IZIKE_IMAGE_NUM];

	//�G�l�~�[����������Ԃ��ǂ���
	bool m_bIzikeState;
	//�������J�E���^
	int m_iIzikeCounter;

	//���X�^�[�g�J�E���^
	int m_iRestartCounter;

	//�����J�E���^
	int m_iDecelerationCounter;

	//�ǐՃJ�E���^
	int m_iPursuitCounter;

	//�A�j���[�V�����J�E���^
	int m_iAnimCounter;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CEnemy(int x = 0, int y = 0);
	~CEnemy();

	//�Z�b�^�[
	void SetPosX(int x) { m_iPosX = x; }
	void SetPosY(int y) { m_iPosY = y; }
	void SetSpeed(int speed) { m_iSpeed = speed; }
	void SetDirection(int d);
	void SetZeroDirection() { m_eDirection = VECTOR_ZERO; }
	void SetAnimCounter(int count) { m_iAnimCounter = count; }
	void SetIzikeState(bool state) { m_bIzikeState = state; }
	void SetIzikeCounter(int count) { m_iIzikeCounter = count; }
	void SetRestartCounter(int count) { m_iRestartCounter = count; }
	void SetCurrentMazeIndex(int index) { m_iCurrentMazeIndex = index; }
	void SetWait(bool wait) { m_bWait = wait; }

	//�Q�b�^�[
	int GetPosX() const { return m_iPosX; }
	int GetPosY() const { return m_iPosY; }
	int GetSpeed() const { return m_iSpeed; }
	int* GetMaze() const { return m_pMaze; }
	int GetAnimCounter() const { return m_iAnimCounter; }
	int GetIzikeCounter() const { return m_iIzikeCounter; }
	bool GetIzikeState() const { return m_bIzikeState; }
	int GetRestartCounter() const { return m_iRestartCounter; }
	int GetCurrentMazeIndex() const { return m_iCurrentMazeIndex; }
	bool GetWait() const { return m_bWait; }

	//����
	void Move();
	virtual void BaseMove();			//����
	virtual void RestartMove();			//���X�^�[�g���̓����̐���
	void Movement();					//�ړ�
	bool IsWall();						//�ǂƂ̓����蔻��
	bool IsCorrection();				//���W�̒���
	void Warp(int index);				//���[�v  200��218
	bool IsBothEndsTurn(int index);		//�i�s�������猩�ė��[���ړ��ł��邩�ǂ���
	void CheckTurn();					//���݂̈ʒu�����]�ł�������������_���Ō��߂�
	void IzikeCount();					//�������J�E���^�̍X�V
	virtual void Restart();				//��������Ԏ��v���C���[�ƏՓˌ�5�b�Ԓ����őҋ@
	void EatPlayer();					//�v���C���[��H�׃X�^�[�g�n�_�֖߂�
	bool IsPlayer();					//�v���C���[�Ƃ̓����蔻��
	bool IsDeceleration();				//���[�v�g���l���t�ߌ���
	void Pursuit();						//�v���C���[�ǐ�
	void SEEnemyMove();					//�G�l�~�[�ړ��T�E���h

	//�`��
	virtual void Draw() const;
	void NormalState() const;		//�ʏ���
	void IzikeState() const;		//���������
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// �A�J�x�C�N���X�i�G�l�~�[�N���X�j
//=============================================================
class CBlinky 
	:public CEnemy
{
private:
	//�O���t�B�b�N�n���h��
	int m_iGHBlinky[ENEMY_IMAGE_NUM];

	//�x�[�X�J�E���^
	int m_iBaseCounter;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CBlinky(int x = 0, int y = 0);
	~CBlinky();

	//����
	void BaseMove();				//����
	void BaseMovement();			//�����̌��܂����ړ��i�E��ֈړ��j
	void BaseTurn();				//�����̌��߂�ꂽ��]
	void RestartMove();				//���X�^�[�g���̓����̐���
	void RestartMovement();			//���X�^�[�g���̈ړ�
	void Restart();					//��������Ԏ��v���C���[�ƏՓˌ�5�b�Ԓ����őҋ@

	//�`��
	void Draw() const;
	void NormalState() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// �s���L�[�N���X�i�G�l�~�[�N���X�j
//=============================================================
class CPinky
	:public CEnemy
{
private:
	//�O���t�B�b�N�n���h��
	int m_iGHPinky[ENEMY_IMAGE_NUM];

	//�x�[�X�J�E���^
	int m_iBaseCounter;

	//�X�^�[�g����
	bool m_bStart;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CPinky(int x = 0, int y = 0);
	~CPinky();

	//�Z�b�^�[
	void SetStart(bool start) { m_bStart = start; }

	//�Q�b�^�[
	bool GetStart() const { return m_bStart; }

	//����
	void BaseMove();				//����
	void BaseMovement();			//�����̌��܂����ړ��i�E��ֈړ��j
	void BaseTurn();				//�����̌��߂�ꂽ��]
	void RestartMove();				//���X�^�[�g���̓����̐���
	void RestartMovement();			//���X�^�[�g���̈ړ�
	void Restart();					//��������Ԏ��v���C���[�ƏՓˌ�5�b�Ԓ����őҋ@

	//�`��
	void Draw() const;
	void NormalState() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// �A�I�X�P�N���X�i�G�l�~�[�N���X�j
//=============================================================
class CInky
	:public CEnemy
{
private:
	//�O���t�B�b�N�n���h��
	int m_iGHInky[ENEMY_IMAGE_NUM];

	//�x�[�X�J�E���^
	int m_iBaseCounter;

	//�X�^�[�g����
	bool m_bStart;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CInky(int x = 0, int y = 0);
	~CInky();

	//�Z�b�^�[
	void SetStart(bool start) { m_bStart = start; }

	//�Q�b�^�[
	bool GetStart() const { return m_bStart; }

	//����
	void BaseMove();				//����
	void BaseMovement();			//�����̌��܂����ړ��i�E��ֈړ��j
	void BaseTurn();				//�����̌��߂�ꂽ��]
	void RestartMove();				//���X�^�[�g���̓����̐���
	void RestartMovement();			//���X�^�[�g���̈ړ�
	void Restart();					//��������Ԏ��v���C���[�ƏՓˌ�5�b�Ԓ����őҋ@

	//�`��
	void Draw() const;
	void NormalState() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// �O�Y�^�N���X�i�G�l�~�[�N���X�j
//=============================================================
class CCryde
	:public CEnemy
{
private:
	//�O���t�B�b�N�n���h��
	int m_iGHCryde[ENEMY_IMAGE_NUM];

	//�x�[�X�J�E���^
	int m_iBaseCounter;

	//�X�^�[�g����
	bool m_bStart;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CCryde(int x = 0, int y = 0);
	~CCryde();

	//�Z�b�^�[
	void SetStart(bool start) { m_bStart = start; }

	//�Q�b�^�[
	bool GetStart() const { return m_bStart; }

	//����
	void BaseMove();				//����
	void BaseMovement();			//�����̌��܂����ړ��i�E��ֈړ��j
	void BaseTurn();				//�����̌��߂�ꂽ��]
	void RestartMove();				//���X�^�[�g���̓����̐���
	void RestartMovement();			//���X�^�[�g���̈ړ�
	void Restart();					//��������Ԏ��v���C���[�ƏՓˌ�5�b�Ԓ����őҋ@

	//�`��
	void Draw() const;
	void NormalState() const;
};

#endif



