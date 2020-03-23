#include "TitleMenu.h"
#include "DxLib.h"
#include "Scene.h"

//=============================================================
// �R���X�g���N�^
//=============================================================
CTitleMenu::CTitleMenu()
	: m_iPosX(0), m_iPosY(0), m_iAnimCounter(0)
{
	//�t�H���g�̓ǂݍ���
	m_iFontHandle[0] = CreateFontToHandle(TITLEMENU_FONT_CHIP, 10, 3);
	m_iFontHandle[1] = CreateFontToHandle(TITLEMENU_FONT_CHIP, 50, 3);

	//�摜�̓ǂݍ���
	LoadDivGraph(TITLE_PACMAN_IMAGE, 12, 3, 4, 16, 16, m_iGHPacman);
	LoadDivGraph(TITLE_BLINKY_IMAGE, TITLE_ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHBlinky);
	LoadDivGraph(TITLE_PINKY_IMAGE, TITLE_ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHPinky);
	LoadDivGraph(TITLE_INKY_IMAGE, TITLE_ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHInky);
	LoadDivGraph(TITLE_CRYDE_IMAGE, TITLE_ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHCryde);
	LoadDivGraph(TITLE_IZIKE_IMAGE, TITLE_ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHIzike);

	//�I�����
	m_eSelectStatus = SELECT_PLAY;

	//����
	m_bRightDirection = true;
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CTitleMenu::~CTitleMenu() 
{
	//�t�H���g�̔j��
	for (int i = 0; i < TITLEMENU_FONT_NUM; i++)
	{
		DeleteFontToHandle(m_iFontHandle[i]);
	}

	//�摜�̔j��
	for (int i = 0; i < TITLE_PACMAN_IMAGE_NUM; i++)
	{
		DeleteGraph(m_iGHPacman[i]);
	}
	for (int i = 0; i < TITLE_ENEMY_IMAGE_NUM; i++)
	{
		DeleteGraph(m_iGHBlinky[i]);
		DeleteGraph(m_iGHPinky[i]);
		DeleteGraph(m_iGHInky[i]);
		DeleteGraph(m_iGHCryde[i]);
	}
}

//=============================================================
// ����
//=============================================================
void CTitleMenu::Input() 
{
	//�I�����
	if (g_cScene->m_iKey[KEY_INPUT_UP] == 1)
	{
		//�T�E���h
		PlaySoundMem(g_cScene->GetSoundClass()->GetSECursor(), DX_PLAYTYPE_BACK);

		if (m_eSelectStatus == SELECT_EXIT)
		{
			m_eSelectStatus = SELECT_CONFIG;
		}
		else if (m_eSelectStatus == SELECT_CONFIG)
		{
			m_eSelectStatus = SELECT_PLAY;
		}
	}
	else if (g_cScene->m_iKey[KEY_INPUT_DOWN] == 1)
	{
		//�T�E���h
		PlaySoundMem(g_cScene->GetSoundClass()->GetSECursor(), DX_PLAYTYPE_BACK);

		if (m_eSelectStatus == SELECT_PLAY)
		{
			m_eSelectStatus = SELECT_CONFIG;
		}
		else if (m_eSelectStatus == SELECT_CONFIG)
		{
			m_eSelectStatus = SELECT_EXIT;
		}
	}
	else if (g_cScene->m_iKey[KEY_INPUT_RETURN] == 1)
	{
		if (m_eSelectStatus == SELECT_PLAY)
		{
			//�Q�[���V�[���ڍs
			g_cScene->m_eSceneStatus = g_cScene->FROM_TITLE_TO_GAME;
		}
		else if (m_eSelectStatus == SELECT_CONFIG)
		{
			//�ݒ��ʂ�
			g_cScene->GetTitleClass()->m_eTitleStatus = g_cScene->GetTitleClass()->STATUS_CONFIG;
		}
		else if (m_eSelectStatus == SELECT_EXIT)
		{
			DxLib_End();
		}
	}
}

//=============================================================
// ����
//=============================================================
void CTitleMenu::Move()
{
	//�A�j���[�V��������
	MoveAnimation();
}

//=============================================================
// �A�j���[�V��������
//=============================================================
void CTitleMenu::MoveAnimation()
{
	//���]
	if (m_iPosX > 640)
		m_bRightDirection = false;
	else if (m_iPosX < -150)
		m_bRightDirection = true;

	//�ړ�
	if (m_bRightDirection)
		m_iPosX++;
	else
		m_iPosX--;

	//�A�j���[�V�����J�E���^�̍X�V
	m_iAnimCounter++;
}

//=============================================================
// �`��
//=============================================================
void CTitleMenu::Draw() const
{
	//�e�L�X�g
	Text();

	//�A�j���[�V�����`��
	DrawAnimation();
}

//=============================================================
// �e�L�X�g
//=============================================================
void CTitleMenu::Text() const
{
	DrawStringToHandle(150, 80, "PA�@ MAN", GetColor(255, 255, 255), m_iFontHandle[1]);
	DrawStringToHandle(250, 80, "C", GetColor(255, 255, 0), m_iFontHandle[1]);

	//�I���e�L�X�g
	SelectText();
}

//=============================================================
// �I���e�L�X�g
//=============================================================
void CTitleMenu::SelectText() const
{
	//�I�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
	switch (m_eSelectStatus)
	{
	case SELECT_PLAY:
		DrawBox(250, 260, 370, 290, GetColor(255, 255, 255), TRUE);
		break;

	case SELECT_CONFIG:
		DrawBox(250, 300, 370, 330, GetColor(255, 255, 255), TRUE);
		break;

	case SELECT_EXIT:
		DrawBox(250, 340, 370, 370, GetColor(255, 255, 255), TRUE);
		break;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	DrawStringToHandle(290, 270, "PLAY", GetColor(255, 255, 255), m_iFontHandle[0]);
	DrawBox(250, 260, 370, 290, GetColor(255, 255, 255), FALSE);

	DrawStringToHandle(280, 310, "CONFIG", GetColor(255, 255, 255), m_iFontHandle[0]);
	DrawBox(250, 300, 370, 330, GetColor(255, 255, 255), FALSE);

	DrawStringToHandle(290, 350, "EXIT", GetColor(255, 255, 255), m_iFontHandle[0]);
	DrawBox(250, 340, 370, 370, GetColor(255, 255, 255), FALSE);
}

//=============================================================
// �A�j���[�V�����`��
//=============================================================
void CTitleMenu::DrawAnimation() const
{
	if (m_bRightDirection)
	{
		//�p�b�N�}��
		DrawRotaGraph3(m_iPosX + TITLE_IMAGE_DRAW_X, m_iPosY + TITLE_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHPacman[(m_iAnimCounter / TITLE_IMAGE_ANIM_SPEED) % 3], TRUE);

		//�G�l�~�[
		DrawRotaGraph3(m_iPosX + TITLE_IMAGE_DRAW_X - 50, m_iPosY + TITLE_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHBlinky[(m_iAnimCounter / TITLE_IMAGE_ANIM_SPEED) % 2], TRUE);
		DrawRotaGraph3(m_iPosX + TITLE_IMAGE_DRAW_X - 80, m_iPosY + TITLE_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHPinky[(m_iAnimCounter / TITLE_IMAGE_ANIM_SPEED) % 2], TRUE);
		DrawRotaGraph3(m_iPosX + TITLE_IMAGE_DRAW_X - 110, m_iPosY + TITLE_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHInky[(m_iAnimCounter / TITLE_IMAGE_ANIM_SPEED) % 2], TRUE);
		DrawRotaGraph3(m_iPosX + TITLE_IMAGE_DRAW_X - 140, m_iPosY + TITLE_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHCryde[(m_iAnimCounter / TITLE_IMAGE_ANIM_SPEED) % 2], TRUE);
	}
	else
	{
		//�p�b�N�}��
		DrawRotaGraph3(m_iPosX + TITLE_IMAGE_DRAW_X, m_iPosY + TITLE_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHPacman[(m_iAnimCounter / TITLE_IMAGE_ANIM_SPEED) % 3 + 3], TRUE);

		//�G�l�~�[
		DrawRotaGraph3(m_iPosX + TITLE_IMAGE_DRAW_X - 50, m_iPosY + TITLE_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHIzike[(m_iAnimCounter / TITLE_IMAGE_ANIM_SPEED) % 2], TRUE);
		DrawRotaGraph3(m_iPosX + TITLE_IMAGE_DRAW_X - 80, m_iPosY + TITLE_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHIzike[(m_iAnimCounter / TITLE_IMAGE_ANIM_SPEED) % 2], TRUE);
		DrawRotaGraph3(m_iPosX + TITLE_IMAGE_DRAW_X - 110, m_iPosY + TITLE_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHIzike[(m_iAnimCounter / TITLE_IMAGE_ANIM_SPEED) % 2], TRUE);
		DrawRotaGraph3(m_iPosX + TITLE_IMAGE_DRAW_X - 140, m_iPosY + TITLE_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHIzike[(m_iAnimCounter / TITLE_IMAGE_ANIM_SPEED) % 2], TRUE);
	}


	//�J�[�e��
	DrawBox(480, 150, 640, 480, GetColor(0, 0, 0), TRUE);
	DrawBox(0, 150, 140, 480, GetColor(0, 0, 0), TRUE);

	//�g
	DrawBox(140, 165, 480, 215, GetColor(255, 255, 255), FALSE);
}