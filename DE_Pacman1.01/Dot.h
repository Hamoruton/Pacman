#pragma once
#ifndef __DOT_H_INCLUDED__
#define __DOT_H_INCLUDED__

//�h�b�g���
#define DOT_IMAGE		 "Assets/Sprites/pacdot.png"
#define POWER_DOT_IMAGE  "Assets/Sprites/powerpacdot.png"

//�`��̒���
#define DOT_DRAW_X					140
#define DOT_DRAW_Y					 30
#define DOT_INTERVAL				 20
#define DOT_IMAGE_EXPANSION			1.5
#define POWER_DOT_IMAGE_EXPANSION     1

#define FLASH_SPEED   20

//=============================================================
// �h�b�g�i�����j�N���X
//=============================================================
class CDot
{
private:
	//�O���t�B�b�N�n���h��
	int m_iGHDot;
	int m_iGHPowerDot;

	//�_��
	int  m_iBlendCount;
	bool m_bBlendLimitMax = false;

	//�h�b�g�J�E���^
	int m_iDotCounter;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CDot();
	~CDot();

	//�Q�b�^�[
	int GetDotCounter() const { return m_iDotCounter; }

	//����
	void Move();
	void DotCount();	//�h�b�g�̐�
	void Flash();		//�_��

	//�`��
	void Draw() const;
	void PowerDotFlash(int x, int y) const;
};

#endif