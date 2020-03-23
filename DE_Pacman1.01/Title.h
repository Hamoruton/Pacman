#pragma once
#ifndef __TITLE_H_INCLUDED_
#define __TITLE_H_INCLUDED_

#include "TitleMenu.h"
#include "TitleConfig.h"

//=============================================================
// �^�C�g���N���X
//=============================================================
class CTitle
{
public:
	enum TITLESTATUS
	{
		STATUS_MENU = 10,
		STATUS_CONFIG = 50,
	};
	TITLESTATUS m_eTitleStatus;

private:
	//�^�C�g�����j���[�N���X
	CTitleMenu* m_cTitleMenu;
	//�^�C�g���ݒ�N���X
	CTitleConfig* m_cTitleConfig;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CTitle();
	~CTitle();

	//�Q�b�^�[
	CTitleMenu* GetTitleMenuClass() const { return m_cTitleMenu; }
	CTitleConfig* GetTitleConfigClass() const { return m_cTitleConfig; }

	//�X�V
	void Update();

	//����
	void Input();

	//����
	void Move();

	//�`��
	void Draw() const;
};

#endif



