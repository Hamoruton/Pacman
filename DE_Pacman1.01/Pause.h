#pragma once
#ifndef __PAUSE_H_INCLUDED__
#define __PAUSE_H_INCLUDED__

//�t�H���g
#define PAUSE_FONT_CHIP		"�A�^���t�H���g���S��"
#define PAUSE_FONT_NUM		2

//�摜���
#define PAUSE_BIGPACMAN_IMAGE	"Assets/Sprites/big_pacman.png"

//=============================================================
// �|�[�Y�N���X
//=============================================================
class CPause
{
private:
	//�t�H���g�n���h��
	int m_iFontHandle[PAUSE_FONT_NUM];

	//�O���t�B�b�N�n���h��
	int m_iGHBigPacman;

	//�I�����
	bool m_bReturnTitle;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CPause();
	~CPause();

	//����
	void Input();

	//����
	void Move();

	//�`��
	void Draw() const;
	void Text() const;
	void Frame() const;
};

#endif



