#pragma once
#ifndef __TITLECONFIG_H_INCLUDED__
#define __TITLECONFIG_H_INCLUDED__

//�t�H���g
#define TITLECONFIG_FONT_CHIP		"�A�^���t�H���g���S��"
#define TITLECONFIG_FONT_NUM		3

//=============================================================
// �^�C�g���ݒ�N���X
//=============================================================
class CTitleConfig
{
private:
	//�I�����
	enum SELECTSTATUS
	{
		SELECT_SOUND,
		SELECT_HIGHSCORE,
		SELECT_DECISION,
	};
	SELECTSTATUS m_eSelectStatus;

	//�t�H���g�n���h��
	int m_iFontHandle[TITLECONFIG_FONT_NUM];

	//�T�E���h�̐ݒ�
	bool m_bSoundConfig;

	//�n�C�X�R�A���Z�b�g
	bool m_bHighScoreReset;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CTitleConfig();
	~CTitleConfig();

	//����
	void Input();

	//����
	void Move();
	void SoundSetting();		//�T�E���h�̐ݒ�

	//�`��
	void Draw() const;
	void Text() const;			//�e�L�X�g
	void Frame() const;			//�g
};

#endif