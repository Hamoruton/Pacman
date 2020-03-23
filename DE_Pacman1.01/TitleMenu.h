#pragma once
#ifndef __TITLEMENU_H_INCLUDED__
#define __TITLEMENU_H_INCLUDED__

//�t�H���g
#define TITLEMENU_FONT_CHIP		"�A�^���t�H���g���S��"
#define TITLEMENU_FONT_NUM		2

// �摜���
#define TITLE_PACMAN_IMAGE			"Assets/Sprites/pacman.png"
#define TITLE_PACMAN_IMAGE_NUM		12
#define TITLE_BLINKY_IMAGE			"Assets/Sprites/blinky.png"
#define TITLE_PINKY_IMAGE			"Assets/Sprites/pinky.png"
#define TITLE_INKY_IMAGE			"Assets/Sprites/inky.png"
#define TITLE_CRYDE_IMAGE			"Assets/Sprites/cryde.png"
#define TITLE_IZIKE_IMAGE			"Assets/Sprites/izike.png"
#define TITLE_ENEMY_IMAGE_NUM		 8

//�摜�̒���
#define TITLE_IMAGE_DRAW_X  150
#define TITLE_IMAGE_DRAW_Y  180

//�A�j���[�V�����X�s�[�h
#define TITLE_IMAGE_ANIM_SPEED  5

//=============================================================
// �^�C�g�����j���[�N���X
//=============================================================
class CTitleMenu 
{
private:
	//�I�����
	enum SELECTSTATUS
	{
		SELECT_PLAY,
		SELECT_CONFIG,
		SELECT_EXIT,
	};
	SELECTSTATUS m_eSelectStatus;

	//�t�H���g�n���h��
	int m_iFontHandle[TITLEMENU_FONT_NUM];

	//�O���t�B�b�N�n���h��
	int m_iGHPacman[TITLE_PACMAN_IMAGE_NUM];
	int m_iGHBlinky[TITLE_ENEMY_IMAGE_NUM];
	int m_iGHPinky[TITLE_ENEMY_IMAGE_NUM];
	int m_iGHInky[TITLE_ENEMY_IMAGE_NUM];
	int m_iGHCryde[TITLE_ENEMY_IMAGE_NUM];
	int m_iGHIzike[TITLE_ENEMY_IMAGE_NUM];

	//���W
	int m_iPosX;
	int m_iPosY;

	//����
	bool m_bRightDirection;

	//�A�j���[�V�����J�E���^
	int m_iAnimCounter;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CTitleMenu();
	~CTitleMenu();

	//����
	void Input();

	//����
	void Move();
	void MoveAnimation();	//�A�j���[�V��������

	//�`��
	void Draw() const;
	void Text() const;				//�e�L�X�g
	void DrawAnimation() const;		//�A�j���[�V�����`��
	void SelectText() const;		//�I���e�L�X�g
};

#endif