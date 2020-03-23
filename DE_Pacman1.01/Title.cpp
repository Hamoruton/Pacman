#include "Title.h"
#include "DxLib.h"
#include "Scene.h"

//=============================================================
// �R���X�g���N�^
//=============================================================
CTitle::CTitle()
{
	//�N���X�̐���
	m_cTitleMenu = new CTitleMenu;
	m_cTitleConfig = new CTitleConfig;

	//�^�C�g�����
	m_eTitleStatus = STATUS_MENU;
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CTitle::~CTitle() 
{
	//�N���X�̔j��
	delete m_cTitleMenu;
	delete m_cTitleConfig;
}

//=============================================================
// �X�V
//=============================================================
void CTitle::Update() 
{
	//����
	Input();

	//����
	Move();

	//�`��
	Draw();
}

//=============================================================
// ����
//=============================================================
void CTitle::Input() 
{
	switch (m_eTitleStatus) 
	{
	case STATUS_MENU:
		m_cTitleMenu->Input();
		break;

	case STATUS_CONFIG:
		m_cTitleConfig->Input();
		break;
	}
}

//=============================================================
// ����
//=============================================================
void CTitle::Move() 
{
	switch (m_eTitleStatus) 
	{
	case STATUS_MENU:
		m_cTitleMenu->Move();
		break;

	case STATUS_CONFIG:
		m_cTitleConfig->Move();
		break;
	}
}

//=============================================================
// �`��
//=============================================================
void CTitle::Draw() const 
{
	switch (m_eTitleStatus)
	{
	case STATUS_MENU:
		m_cTitleMenu->Draw();
		break;

	case STATUS_CONFIG:
		m_cTitleConfig->Draw();
		break;
	}
}