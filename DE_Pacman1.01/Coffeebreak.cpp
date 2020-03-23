#include "Coffeebreak.h"
#include "DxLib.h"
#include "Scene.h"

//=============================================================
// �R���X�g���N�^
//=============================================================
CCoffeebreak::CCoffeebreak() :m_iPosX(0), m_iPosY(0), m_iAnimCounter(0), m_iTimer(0)
{
	//�摜�̓ǂݍ���
	LoadDivGraph(COFFEEBREAK_PACMAN_IMAGE, COFFEEBREAK_PACMAN_IMAGE_NUM, 3, 4, 16, 16, m_iGHPacman);
	LoadDivGraph(COFFEEBREAK_BLINKY_IMAGE, COFFEEBREAK_ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHBlinky);
	LoadDivGraph(COFFEEBREAK_IZIKE_IMAGE, COFFEEBREAK_ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHIzike);

	//����
	m_bRightDirection = false;

	//�T�E���h�Đ�
	PlaySoundMem(g_cScene->GetSoundClass()->GetSECoffeebreak(), DX_PLAYTYPE_BACK);
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CCoffeebreak::~CCoffeebreak() 
{
	//�摜�̔j��
	for (int i = 0; i < COFFEEBREAK_PACMAN_IMAGE_NUM; i++) 
	{
		DeleteGraph(m_iGHPacman[i]);
	}
	for (int i = 0; i < COFFEEBREAK_ENEMY_IMAGE_NUM; i++) 
	{
		DeleteGraph(m_iGHBlinky[i]);
		DeleteGraph(m_iGHIzike[i]);
	}
}

//=============================================================
// �X�V
//=============================================================
void CCoffeebreak::Update() 
{
	//����
	Move();

	//�`��
	Draw();
}

//=============================================================
// ����
//=============================================================
void CCoffeebreak::Move() 
{
	//���̃X�e�[�W�i13�b��j
	NextStage();

	//�A�j���[�V��������
	MoveAnimation();
}

//=============================================================
// �A�j���[�V��������
//=============================================================
void CCoffeebreak::MoveAnimation() 
{
	//���]
	if (m_iPosX < -450)
		m_bRightDirection = true;

	//�ړ�
	if (m_bRightDirection) 
	{
		m_iPosX += 1;
	}
	else
		m_iPosX -= 2;

	//�A�j���[�V�����J�E���^�̍X�V
	m_iAnimCounter++;
}

//=============================================================
// ���̃X�e�[�W�i13�b��j
//=============================================================
void CCoffeebreak::NextStage() 
{
	m_iTimer++;
	if (m_iTimer > 13 * 60) 
	{
		g_cScene->m_eSceneStatus = g_cScene->FROM_COFFEEBREAK_TO_GAME;
	}
}

//=============================================================
// �`��
//=============================================================
void CCoffeebreak::Draw() const 
{
	//�A�j���[�V�����`��
	DrawAnimation();
}

//=============================================================
// �A�j���[�V�����`��
//=============================================================
void CCoffeebreak::DrawAnimation() const 
{
	if (m_bRightDirection)
	{
		//�p�b�N�}��
		DrawRotaGraph3(m_iPosX * 2 + COFFEEBREAK_IMAGE_DRAW_X, m_iPosY + COFFEEBREAK_IMAGE_DRAW_Y - 25, 0, 0,
			4.0, 4.0, 0, m_iGHPacman[(m_iAnimCounter / COFFEEBREAK_IMAGE_ANIM_SPEED) % 3], TRUE);

		//�G�l�~�[
		DrawRotaGraph3(m_iPosX + COFFEEBREAK_IMAGE_DRAW_X + 50, m_iPosY + COFFEEBREAK_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHIzike[(m_iAnimCounter / COFFEEBREAK_IMAGE_ANIM_SPEED) % 2], TRUE);
	}
	else
	{
		//�p�b�N�}��
		DrawRotaGraph3(m_iPosX + COFFEEBREAK_IMAGE_DRAW_X, m_iPosY + COFFEEBREAK_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHPacman[(m_iAnimCounter / COFFEEBREAK_IMAGE_ANIM_SPEED) % 3 + 3], TRUE);

		//�G�l�~�[
		DrawRotaGraph3(m_iPosX + COFFEEBREAK_IMAGE_DRAW_X + 30, m_iPosY + COFFEEBREAK_IMAGE_DRAW_Y, 0, 0,
			1.5, 1.5, 0, m_iGHBlinky[(m_iAnimCounter / COFFEEBREAK_IMAGE_ANIM_SPEED) % 2], TRUE);
	}

	//�J�[�e��
	DrawBox(480, 150, 640, 480, GetColor(0, 0, 0), TRUE);
	DrawBox(0, 150, 140, 480, GetColor(0, 0, 0), TRUE);
}