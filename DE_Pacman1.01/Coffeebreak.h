#pragma once
#ifndef __COFFEEBREAK_H_INCLUDED__
#define __COFFEEBREAK_H_INCLUDED__

//�摜���
#define COFFEEBREAK_PACMAN_IMAGE		"Assets/Sprites/pacman.png"
#define COFFEEBREAK_PACMAN_IMAGE_NUM	12
#define COFFEEBREAK_BLINKY_IMAGE		"Assets/Sprites/blinky.png"
#define COFFEEBREAK_IZIKE_IMAGE			"Assets/Sprites/izike.png"
#define COFFEEBREAK_ENEMY_IMAGE_NUM		 8

//�摜�̒���
#define COFFEEBREAK_IMAGE_DRAW_X	450
#define COFFEEBREAK_IMAGE_DRAW_Y	180

//�A�j���[�V�����X�s�[�h
#define COFFEEBREAK_IMAGE_ANIM_SPEED  5

//=============================================================
// �R�[�q�[�u���C�N�N���X
//=============================================================
class CCoffeebreak
{
private:
	//�O���t�B�b�N�n���h��
	int m_iGHPacman[COFFEEBREAK_PACMAN_IMAGE_NUM];
	int m_iGHBlinky[COFFEEBREAK_ENEMY_IMAGE_NUM];
	int m_iGHIzike[COFFEEBREAK_ENEMY_IMAGE_NUM];

	//���W
	int m_iPosX;
	int m_iPosY;

	//����
	bool m_bRightDirection;

	//�^�C�}�[�i13�b�㎟�̃X�e�[�W�J�ځj
	int m_iTimer;

	//�A�j���[�V�����J�E���^
	int m_iAnimCounter;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CCoffeebreak();
	~CCoffeebreak();

	//�X�V
	void Update();

	//����
	void Move();
	void MoveAnimation();			//�A�j���[�V��������
	void NextStage();				//���̃X�e�[�W

	//�`��
	void Draw() const;
	void DrawAnimation() const;		//�A�j���[�V�����`��
};

#endif