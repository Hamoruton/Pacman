#include "Game.h"
#include "DxLib.h"
#include "Scene.h"

//=============================================================
// �R���X�g���N�^
//=============================================================
CGame::CGame()
	:m_iRestartCounter(0), m_iGameTimer(0), m_iGameReadyTimer(0), m_iClearTimer(0), m_iGameOverTimer(0)
{
	//�Q�[�����
	m_eGameStatus = STATUS_GAMEREADY_1;

	//�N���X�̐���
	m_cMaze = new CMaze;
	m_cDot = new CDot;
	m_cPlayer = new CPlayer(9, 16);
	m_cBlinky = new CBlinky(9, 8);
	m_cPinky = new CPinky(9, 10);
	m_cInky = new CInky(8, 10);
	m_cCryde = new CCryde(10, 10);
	m_cFruit = new CFruit;
	m_cPause = new CPause;

	//�t�H���g�̓ǂݍ���
	m_iFontHandle = CreateFontToHandle(GAME_FONT_CHIP, 12, 3);

	//�T�E���h
	PlaySoundMem(g_cScene->GetSoundClass()->GetSEGameStart(), DX_PLAYTYPE_BACK);
	m_bSEGameOver = false;
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CGame::~CGame()
{
	//�N���X�̔j��
	delete m_cBlinky;
	delete m_cPinky;
	delete m_cInky;
	delete m_cCryde;
	delete m_cPlayer;
	delete m_cDot;
	delete m_cFruit;
	delete m_cMaze;
	delete m_cPause;

	//�t�H���g�̔j��
	DeleteFontToHandle(m_iFontHandle);
}

//=============================================================
// �X�V
//=============================================================
void CGame::Update()
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
void CGame::Input() 
{
	switch (m_eGameStatus) 
	{
	case STATUS_GAME:
		//�v���C���[
		m_cPlayer->Input();
		break;

	case STATUS_PAUSE:
		//�|�[�Y
		m_cPause->Input();
		break;
	}
}

//=============================================================
// ����
//=============================================================
void CGame::Move() 
{
	switch (m_eGameStatus) 
	{
	//�Q�[������
	case STATUS_GAMEREADY_1:
		if (!IsGameReady()) 
		{
			m_eGameStatus = STATUS_GAMEREADY_2;
		}
		break;

	case STATUS_GAMEREADY_2:
		if (!IsGameReady())
		{
			m_eGameStatus = STATUS_GAME;
		}
		break;

	//�Q�[��
	case STATUS_GAME:
		//�Q�[���^�C��
		GameTime();

		//�v���C���[
		m_cPlayer->Move();

		//�G�l�~�[
		EnemyMove(m_cBlinky);
		EnemyMove(m_cPinky);
		EnemyMove(m_cInky);
		EnemyMove(m_cCryde);

		//�t���[�c
		m_cFruit->Move();

		//�Q�[���N���A���ǂ���
		if (IsGameClear()) 
		{
			m_eGameStatus = STATUS_GAMECLEAR;
		}

		//�Q�[���I�[�o�[���ǂ���
		if (IsGameOver())
		{
			m_eGameStatus = STATUS_GAMEOVER;
		}

		break;

	//�Q�[���N���A
	case STATUS_GAMECLEAR:
		GameClear();
		break;

	//�Q�[���I�[�o�[
	case STATUS_GAMEOVER:
		GameOver();
		break;

	//�|�[�Y
	case STATUS_PAUSE:
		m_cPause->Move();
		break;
	}

	//�h�b�g
	m_cDot->Move();

	//���C�Y
	m_cMaze->Move();
}

//=============================================================
// �G�l�~�[����
//=============================================================
void CGame::EnemyMove(CEnemy* enemy) 
{
	switch (enemy->m_eMoveMode) 
	{
	case enemy->MODE_INIT:
		enemy->BaseMove();
		break;

	case enemy->MODE_PURSUIT:
		enemy->Move();
		break;

	case enemy->MODE_RESTART:
		enemy->RestartMove();
		break;
	}
}

//=============================================================
// �Q�[���^�C��
//=============================================================
void CGame::GameTime() 
{
	m_iGameTimer++;
	if (m_iGameTimer > 1 * 60) 
	{
		//�s���L�[�ړ��J�n
		m_cPinky->SetStart(true);
	}

	if (m_iGameTimer > 8 * 60) 
	{
		//�C���L�[�ړ��J�n
		m_cInky->SetStart(true);
	}

	if (m_iGameTimer > 12 * 60) 
	{
		//�O�Y�^�ړ��J�n
		m_cCryde->SetStart(true);
	}
}

//=============================================================
// �Q�[������
//=============================================================
bool CGame::IsGameReady() 
{
	m_iGameReadyTimer++;
	if (m_iGameReadyTimer > 2 * 60) 
	{
		m_iGameReadyTimer = 0;
		return false;
	}
	return true;
}

//=============================================================
// �Q�[���N���A���ǂ���
//=============================================================
bool CGame::IsGameClear() 
{
	//�h�b�g�̐��擾
	int current_dot_count = m_cDot->GetDotCounter();
	if (current_dot_count <= 0) 
	{
		return true;
	}

	return false;
}

//=============================================================
// �Q�[���N���A
//=============================================================
void CGame::GameClear() 
{
	//�T�E���h��~
	if (CheckSoundMem(g_cScene->GetSoundClass()->GetSEDotEat())) 
	{
		StopSoundMem(g_cScene->GetSoundClass()->GetSEDotEat());
	}

	if (CheckSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove()))
	{
		StopSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove());
	}
	if (CheckSoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove()))
	{
		StopSoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove());
	}

	m_iClearTimer++;
	if (m_iClearTimer > 5 * 60) 
	{
		if (g_cScene->m_eSceneStatus == g_cScene->GAME_01) 
		{
			//�R�[�q�[�u���C�N�J��
			g_cScene->m_eSceneStatus = g_cScene->FROM_GAME_TO_COFFEEBREAK;
		}
		else if (g_cScene->m_eSceneStatus == g_cScene->GAME_02) 
		{
			//�^�C�g���J��
			g_cScene->m_eSceneStatus = g_cScene->FROM_GAME_TO_TITLE;
		}
	}
}

//=============================================================
// �Q�[���I�[�o�[���ǂ���
//=============================================================
bool CGame::IsGameOver()
{
	//�v���C���[�̎c�@�擾
	int current_remain = g_cScene->GetScoreClass()->GetRemainCounter();
	if (current_remain < 0) 
	{
		return true;
	}

	return false;
}

//=============================================================
// �Q�[���I�[�o�[
//=============================================================
void CGame::GameOver() 
{
	//�T�E���h��~
	if (CheckSoundMem(g_cScene->GetSoundClass()->GetSEDotEat()))
	{
		StopSoundMem(g_cScene->GetSoundClass()->GetSEDotEat());
	}

	if (CheckSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove()))
	{
		StopSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove());
	}
	if (CheckSoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove()))
	{
		StopSoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove());
	}

	//�T�E���h�Đ�
	if (!CheckSoundMem(g_cScene->GetSoundClass()->GetSEPlayerEat())) 
	{
		if (!m_bSEGameOver) 
		{
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEPlayerEat(), DX_PLAYTYPE_BACK);
			m_bSEGameOver = true;
		}
	}

	m_iGameOverTimer++;
	if (m_iGameOverTimer > 5 * 60) 
	{
		//�^�C�g���J��
		g_cScene->m_eSceneStatus = g_cScene->FROM_GAME_TO_TITLE;
	}
}

//=============================================================
// �`��
//=============================================================
void CGame::Draw() const 
{
	switch (m_eGameStatus) 
	{
	//�Q�[������
	case STATUS_GAMEREADY_1:
		//�e�L�X�g
		GameReadyText1();

		//���C�Y
		m_cMaze->Draw();

		//�h�b�g
		m_cDot->Draw();

		//�X�R�A
		g_cScene->GetScoreClass()->Draw();
		break;

	case STATUS_GAMEREADY_2:
		//�e�L�X�g
		GameReadyText2();

		//���C�Y
		m_cMaze->Draw();

		//�h�b�g
		m_cDot->Draw();

		//�X�R�A
		g_cScene->GetScoreClass()->Draw();

		//�v���C���[
		m_cPlayer->Draw();

		//�G�l�~�[
		m_cBlinky->Draw();
		m_cPinky->Draw();
		m_cInky->Draw();
		m_cCryde->Draw();
		break;

	//�Q�[��
	case STATUS_GAME:
		//���C�Y
		m_cMaze->Draw();

		//�h�b�g
		m_cDot->Draw();

		//�t���[�c
		m_cFruit->Draw();

		//�X�R�A
		g_cScene->GetScoreClass()->Draw();

		//�G�l�~�[
		m_cBlinky->Draw();
		m_cPinky->Draw();
		m_cInky->Draw();
		m_cCryde->Draw();

		//�v���C���[
		m_cPlayer->Draw();
		break;

	//�Q�[���N���A
	case STATUS_GAMECLEAR:
		//�e�L�X�g
		GameClearText();

		//���C�Y
		m_cMaze->Draw();

		//�h�b�g
		m_cDot->Draw();

		//�X�R�A
		g_cScene->GetScoreClass()->Draw();
		break;

	//�Q�[���I�[�o�[
	case STATUS_GAMEOVER:
		//�e�L�X�g
		GameOverText();

		//���C�Y
		m_cMaze->Draw();

		//�h�b�g
		m_cDot->Draw();

		//�X�R�A
		g_cScene->GetScoreClass()->Draw();
		break;

	//�|�[�Y
	case STATUS_PAUSE:
		m_cPause->Draw();
		break;
	}
}

//=============================================================
// �Q�[�������e�L�X�g
//=============================================================
void CGame::GameReadyText1() const 
{
	DrawStringFToHandle(280, 185, "PLAYER", GetColor(0, 255, 255), m_iFontHandle);
	DrawStringFToHandle(280, 265, "READY!", GetColor(255, 255, 0), m_iFontHandle);
}

void CGame::GameReadyText2() const 
{
	DrawStringFToHandle(280, 265, "READY!", GetColor(255, 255, 0), m_iFontHandle);
}

//=============================================================
// �Q�[���N���A�e�L�X�g
//=============================================================
void CGame::GameClearText() const
{
	DrawStringFToHandle(280, 185, "PLAYER", GetColor(0, 255, 255), m_iFontHandle);
	DrawStringFToHandle(260, 265, "GAMECLEAR", GetColor(255, 255, 0), m_iFontHandle);
}

//=============================================================
// �Q�[���I�[�o�[�e�L�X�g
//=============================================================
void CGame::GameOverText() const 
{
	DrawStringFToHandle(280, 185, "PLAYER", GetColor(0, 255, 255), m_iFontHandle);
	DrawStringFToHandle(270, 265, "GAMEOVER", GetColor(255, 0, 0), m_iFontHandle);
}