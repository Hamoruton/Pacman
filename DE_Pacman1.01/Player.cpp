#include "Player.h"
#include "DxLib.h"
#include "Scene.h"

//=============================================================
// �R���X�g���N�^
//=============================================================
CPlayer::CPlayer(int x, int y)
	:m_iPosX(x*PLAYER_RELATIVE_MAZE_X), m_iPosY(y*PLAYER_RELATIVE_MAZE_Y), m_iAnimCounter(0), m_iIzikeScoreCounter(0), m_iSpeed(1)
{
	//�摜�̓ǂݍ���
	LoadDivGraph(PLAYER_IMAGE, 12, 3, 4, 16, 16, m_iGHPlayer);

	//���C�Y���擾
	m_pMaze = m_cMaze.GetMaze();

	//���C�Y�z��ɂ�����v���C���[�̈ʒu
	m_iCurrentMazeIndex = m_iPosY / PLAYER_RELATIVE_MAZE_Y * MAZE_WIDTH + m_iPosX / PLAYER_RELATIVE_MAZE_X;
	m_iPreviousMazeIndex = 0;

	//�G�l�~�[�Ƃ̏Փ�
	m_bBlinkyCollision = false;
	m_bPinkyCollision = false;
	m_bInkyCollision = false;
	m_bCrydeCollision = false;
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CPlayer::~CPlayer() 
{
	//�摜�f�[�^�̔j��
	for (int i = 0; i < PLAYER_IMAGE_NUM; i++) 
	{
		DeleteGraph(m_iGHPlayer[i]);
	}
}

//=============================================================
// ����
//=============================================================
void CPlayer::Input()
{
	if (g_cScene->m_iKey[KEY_INPUT_RIGHT] == 1)
	{
		//�T�E���h
		SEDotEat();

		//����
		m_eInputDirection = VECTOR_RIGHT;
	}
	else if (g_cScene->m_iKey[KEY_INPUT_LEFT] == 1)
	{
		//�T�E���h
		SEDotEat();

		//����
		m_eInputDirection = VECTOR_LEFT;
	}
	else if (g_cScene->m_iKey[KEY_INPUT_UP] == 1)
	{
		//�T�E���h
		SEDotEat();
		
		//����
		m_eInputDirection = VECTOR_UP;
	}
	else if (g_cScene->m_iKey[KEY_INPUT_DOWN] == 1)
	{
		//�T�E���h
		SEDotEat();

		//����
		m_eInputDirection = VECTOR_DOWN;
	}
	else if (g_cScene->m_iKey[KEY_INPUT_P] == 1) 
	{
		g_cScene->GetGameClass()->m_eGameStatus = g_cScene->GetGameClass()->STATUS_PAUSE;

		//�T�E���h���~�߂�
		StopSound();
	}
}

//=============================================================
// ����
//=============================================================
void CPlayer::Move()
{
	//���݂̃v���C���[�ʒu
	m_iCurrentMazeIndex = m_iPosY / PLAYER_RELATIVE_MAZE_Y * MAZE_WIDTH + m_iPosX / PLAYER_RELATIVE_MAZE_X;

	//�ړ�
	Movement();

	//��]
	CheckTurn();

	//���[�v
	Warp(m_iCurrentMazeIndex);

	//�h�b�g�̃N���A
	ClearDot(m_iCurrentMazeIndex);

	//�G�l�~�[��H�ׂ�
	EatEnemy();

	//�t���[�c��H�ׂ�
	EatFruit();

	//�A�j���[�V�����J�E���^
	m_iAnimCounter++;
}

//=============================================================
// �ړ�
//=============================================================
void CPlayer::Movement() 
{
	//�v���C���[�̈ړ�
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		if (!IsWall())
		{
			m_iPosX += m_iSpeed;
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				m_iPosX++;
			}
		}
		break;

	case VECTOR_LEFT:		//��
		if (!IsWall())
		{
			m_iPosX -= m_iSpeed;
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				m_iPosX++;
			}
		}
		break;

	case VECTOR_UP:			//��
		if (!IsWall())
		{
			m_iPosY -= m_iSpeed;
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				m_iPosY++;
			}
		}
		break;

	case VECTOR_DOWN:		//��
		if (!IsWall())
		{
			m_iPosY += m_iSpeed;
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				m_iPosY--;
			}
		}
		break;

	default:
		break;
	}
}

//=============================================================
// �ǂƂ̓����蔻��
//=============================================================
bool CPlayer::IsWall() 
{
	//���W�̒���
	if (!IsCorrection()) 
	{
		return false;
	}

	//�ړ���̃v���C���[���W�z��̈ʒu���擾
	switch (m_eDirection) 
	{
	case VECTOR_RIGHT:		//�E
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) + 1;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_LEFT:		//��
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) - 1;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_UP:		    //��
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) - MAZE_WIDTH;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_DOWN:		//��
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) + MAZE_WIDTH;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;
	}

	return false;
}

//=============================================================
// �h�b�g���N���A
//=============================================================
void CPlayer::ClearDot(int index) 
{
	if (*(m_pMaze + index) == m_cMaze.TILE_DOT)
	{
		*(m_pMaze + index) = m_cMaze.TILE_NONE;

		//�X�R�A���Z : 10
		AddScore(10);
	}
	else if (*(m_pMaze + index) == m_cMaze.TILE_POWER_DOT)
	{
		*(m_pMaze + index) = m_cMaze.TILE_NONE;

		//�X�R�A���Z : 50
		AddScore(50);

		//�G�l�~�[���������
		g_cScene->GetGameClass()->GetBlinkyClass()->SetIzikeState(true);
		g_cScene->GetGameClass()->GetBlinkyClass()->SetIzikeCounter(0);
		g_cScene->GetGameClass()->GetPinkyClass()->SetIzikeState(true);
		g_cScene->GetGameClass()->GetPinkyClass()->SetIzikeCounter(0);
		g_cScene->GetGameClass()->GetInkyClass()->SetIzikeState(true);
		g_cScene->GetGameClass()->GetInkyClass()->SetIzikeCounter(0);
		g_cScene->GetGameClass()->GetCrydeClass()->SetIzikeState(true);
		g_cScene->GetGameClass()->GetCrydeClass()->SetIzikeCounter(0);
	}
}

//=============================================================
// ���͐�̈ړ�������ێ�
//=============================================================
bool CPlayer::IsTurn()
{
	//���W�̒���
	if (!IsCorrection())
	{
		return false;
	}

	switch (m_eInputDirection)
	{
	case VECTOR_RIGHT:
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) + 1;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return false;
		}
		break;

	case VECTOR_LEFT:
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) - 1;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return false;
		}
		break;

	case VECTOR_UP:
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) - MAZE_WIDTH;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return false;
		}
		break;

	case VECTOR_DOWN:
		m_iPreviousMazeIndex = (m_iPosY / PLAYER_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / PLAYER_RELATIVE_MAZE_X) + MAZE_WIDTH;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return false;
		}
		break;
	}

	return true;
}

//=============================================================
// ���͐�̈ړ�������ێ����ǂłȂ���Ή�]
//=============================================================
void CPlayer::CheckTurn() 
{
	switch (m_eInputDirection) 
	{
	case VECTOR_RIGHT:
		if (IsTurn())
		{
			m_eDirection = VECTOR_RIGHT;
		}
		break;

	case VECTOR_LEFT:
		if (IsTurn())
		{
			m_eDirection = VECTOR_LEFT;
		}
		break;

	case VECTOR_UP:
		if (IsTurn())
		{
			m_eDirection = VECTOR_UP;
		}
		break;

	case VECTOR_DOWN:
		if (IsTurn())
		{
			m_eDirection = VECTOR_DOWN;
		}
		break;
	}
}

//=============================================================
// ���W�̕␳
//=============================================================
bool CPlayer::IsCorrection() 
{
	if ((m_iPosX%PLAYER_RELATIVE_MAZE_X == 0) && (m_iPosY%PLAYER_RELATIVE_MAZE_Y == 0))
	{
		return true;
	}

	return false;
}

//=============================================================
// ���[�v
//=============================================================
void CPlayer::Warp(int index) 
{
	//���W�̒���
	if (!IsCorrection())
	{
		return;
	}

	if (index == 200) 
	{
		m_iPosX = 360;
	}
	else if (index == 218) 
	{
		m_iPosX = 20;
	}
}

//=============================================================
// �G�l�~�[��H�׃X�^�[�g�n�_�֖߂�
//=============================================================
void CPlayer::EatEnemy() 
{
	//�����蔻��
	IsEnemy();

	//��������Ԃ̏ꍇ
	if (m_bBlinkyCollision) 
	{
		if (g_cScene->GetGameClass()->GetBlinkyClass()->GetIzikeState())
		{
			//�X�R�A���Z
			AddScoreOnEnemy();

			//�T�E���h
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEEnemyEat(), DX_PLAYTYPE_BACK);

			//�ꎞ��~
			WaitTimer(1000);

			//���Z�b�g
			g_cScene->GetGameClass()->GetBlinkyClass()->SetPosX(9 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetBlinkyClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetBlinkyClass()->SetIzikeState(false);
			g_cScene->GetGameClass()->GetBlinkyClass()->SetZeroDirection();
			g_cScene->GetGameClass()->GetBlinkyClass()->m_eMoveMode = g_cScene->GetGameClass()->GetBlinkyClass()->MODE_RESTART;
		}
	}

	if (m_bPinkyCollision)
	{
		if (g_cScene->GetGameClass()->GetPinkyClass()->GetIzikeState())
		{
			//�X�R�A���Z
			AddScoreOnEnemy();

			//�T�E���h
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEEnemyEat(), DX_PLAYTYPE_BACK);

			//�ꎞ��~
			WaitTimer(1000);

			//���Z�b�g
			g_cScene->GetGameClass()->GetPinkyClass()->SetPosX(9 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetPinkyClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetPinkyClass()->SetIzikeState(false);
			g_cScene->GetGameClass()->GetPinkyClass()->SetZeroDirection();
			g_cScene->GetGameClass()->GetPinkyClass()->m_eMoveMode = g_cScene->GetGameClass()->GetPinkyClass()->MODE_RESTART;
		}
	}

	if (m_bInkyCollision)
	{
		if (g_cScene->GetGameClass()->GetInkyClass()->GetIzikeState())
		{
			//�X�R�A���Z
			AddScoreOnEnemy();

			//�T�E���h
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEEnemyEat(), DX_PLAYTYPE_BACK);

			//�ꎞ��~
			WaitTimer(1000);

			//���Z�b�g
			g_cScene->GetGameClass()->GetInkyClass()->SetPosX(8 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetInkyClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetInkyClass()->SetIzikeState(false);
			g_cScene->GetGameClass()->GetInkyClass()->SetZeroDirection();
			g_cScene->GetGameClass()->GetInkyClass()->m_eMoveMode = g_cScene->GetGameClass()->GetInkyClass()->MODE_RESTART;
		}
	}

	if (m_bCrydeCollision)
	{
		if (g_cScene->GetGameClass()->GetCrydeClass()->GetIzikeState())
		{
			//�X�R�A���Z
			AddScoreOnEnemy();

			//�T�E���h
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEEnemyEat(), DX_PLAYTYPE_BACK);

			//�ꎞ��~
			WaitTimer(1000);

			//���Z�b�g
			g_cScene->GetGameClass()->GetCrydeClass()->SetPosX(10 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetCrydeClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetCrydeClass()->SetIzikeState(false);
			g_cScene->GetGameClass()->GetCrydeClass()->SetZeroDirection();
			g_cScene->GetGameClass()->GetCrydeClass()->m_eMoveMode = g_cScene->GetGameClass()->GetCrydeClass()->MODE_RESTART;
		}
	}
}

//=============================================================
// �G�l�~�[�Ƃ̓����蔻��
//=============================================================
void CPlayer::IsEnemy()
{
	if (m_iCurrentMazeIndex == g_cScene->GetGameClass()->GetBlinkyClass()->GetCurrentMazeIndex()) 
	{
		m_bBlinkyCollision = true;

		//�ҋ@
		g_cScene->GetGameClass()->GetBlinkyClass()->SetWait(true);
	}
	else
	{
		m_bBlinkyCollision = false;
	}

	if (m_iCurrentMazeIndex == g_cScene->GetGameClass()->GetPinkyClass()->GetCurrentMazeIndex())
	{
		m_bPinkyCollision = true;

		//�ҋ@
		g_cScene->GetGameClass()->GetPinkyClass()->SetWait(true);
	}
	else
	{
		m_bPinkyCollision = false;
	}

	if (m_iCurrentMazeIndex == g_cScene->GetGameClass()->GetInkyClass()->GetCurrentMazeIndex())
	{
		m_bInkyCollision = true;

		//�ҋ@
		g_cScene->GetGameClass()->GetInkyClass()->SetWait(true);
	}
	else 
	{
		m_bInkyCollision = false;
	}

	if (m_iCurrentMazeIndex == g_cScene->GetGameClass()->GetCrydeClass()->GetCurrentMazeIndex())
	{
		m_bCrydeCollision = true;

		//�ҋ@
		g_cScene->GetGameClass()->GetCrydeClass()->SetWait(true);
	}
	else
	{
		m_bCrydeCollision = false;
	}
}

//=============================================================
// �X�R�A���Z
//=============================================================
void CPlayer::AddScore(int add) 
{
	//���݂̃X�R�A�擾
	int current_score = g_cScene->GetScoreClass()->GetScore();

	//�ő�l�̐���
	if (current_score > 99999) 
	{
		current_score = 99999;
	}

	//�X�R�A���Z
	g_cScene->GetScoreClass()->SetScore(current_score + add);
}

//=============================================================
// �G�l�~�[�ɂ�����X�R�A���Z
//=============================================================
void CPlayer::AddScoreOnEnemy() 
{
	//�X�R�A���Z
	switch (m_iIzikeScoreCounter) 
	{
	case 0:
		AddScore(200);
		m_iIzikeScoreCounter++;
		break;

	case 1:
		AddScore(400);
		m_iIzikeScoreCounter++;
		break;

	case 2:
		AddScore(800);
		m_iIzikeScoreCounter++;
		break;

	case 3:
		AddScore(1600);
		m_iIzikeScoreCounter++;
		break;
	}
}

//=============================================================
// �h�b�g�C�[�g�T�E���h
//=============================================================
void CPlayer::SEDotEat() 
{
	if (!CheckSoundMem(g_cScene->GetSoundClass()->GetSEDotEat())) 
	{
		if (g_cScene->GetGameClass()->m_eGameStatus == g_cScene->GetGameClass()->STATUS_GAME)
		{
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEDotEat(), DX_PLAYTYPE_LOOP);
		}
	}
}

//=============================================================
// �T�E���h�����ׂĎ~�߂�
//=============================================================
void CPlayer::StopSound() 
{
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
}

//=============================================================
// �t���[�c��H�ׂăX�R�A�A�b�v
//=============================================================
void CPlayer::EatFruit() 
{
	if (m_iCurrentMazeIndex == g_cScene->GetGameClass()->GetFruitClass()->GetCurrentMazeIndex()) 
	{
		//��������
		if (g_cScene->GetGameClass()->GetFruitClass()->GetOutbreakCherry()) 
		{
			//���Z�b�g
			g_cScene->GetGameClass()->GetFruitClass()->SetOutbreakCherry(false);
			g_cScene->GetGameClass()->GetFruitClass()->m_eFruitStatus = g_cScene->GetGameClass()->GetFruitClass()->STRAWBERRY;
			g_cScene->GetGameClass()->GetFruitClass()->SetOutbreakCounter(0);

			//�X�R�A
			AddScore(100);

			//�T�E���h
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEFruitEat(), DX_PLAYTYPE_BACK);

			//�ꎞ��~
			WaitTimer(1000);

		}
		//������
		else if (g_cScene->GetGameClass()->GetFruitClass()->GetOutbreakStrawberry()) 
		{
			//���Z�b�g
			g_cScene->GetGameClass()->GetFruitClass()->SetOutbreakStrawberry(false);
			g_cScene->GetGameClass()->GetFruitClass()->m_eFruitStatus = g_cScene->GetGameClass()->GetFruitClass()->CHERRY;
			g_cScene->GetGameClass()->GetFruitClass()->SetOutbreakCounter(0);

			//�X�R�A
			AddScore(300);

			//�T�E���h
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEFruitEat(), DX_PLAYTYPE_BACK);

			//�ꎞ��~
			WaitTimer(1000);
		}
	}
}

//=============================================================
// �`��
//=============================================================
void CPlayer::Draw() const 
{
	switch (m_eDirection) 
	{
	case VECTOR_RIGHT:
		DrawRotaGraph3(m_iPosX + PLAYER_DRAW_X, m_iPosY + PLAYER_DRAW_Y, 0, 0,
			PLAYER_IMAGE_EXPANSION_X, PLAYER_IMAGE_EXPANSION_Y, 0, m_iGHPlayer[(m_iAnimCounter / PLAYER_ANIM_SPEED) % 3], TRUE);
		break;

	case VECTOR_LEFT:
		DrawRotaGraph3(m_iPosX + PLAYER_DRAW_X, m_iPosY + PLAYER_DRAW_Y, 0, 0,
			PLAYER_IMAGE_EXPANSION_X, PLAYER_IMAGE_EXPANSION_Y, 0, m_iGHPlayer[(m_iAnimCounter / PLAYER_ANIM_SPEED) % 3 + 3], TRUE);
		break;

	case VECTOR_UP:
		DrawRotaGraph3(m_iPosX + PLAYER_DRAW_X, m_iPosY + PLAYER_DRAW_Y, 0, 0,
			PLAYER_IMAGE_EXPANSION_X, PLAYER_IMAGE_EXPANSION_Y, 0, m_iGHPlayer[(m_iAnimCounter / PLAYER_ANIM_SPEED) % 3 + 6], TRUE);
		break;

	case VECTOR_DOWN:
		DrawRotaGraph3(m_iPosX + PLAYER_DRAW_X, m_iPosY + PLAYER_DRAW_Y, 0, 0,
			PLAYER_IMAGE_EXPANSION_X, PLAYER_IMAGE_EXPANSION_Y, 0, m_iGHPlayer[(m_iAnimCounter / PLAYER_ANIM_SPEED) % 3 + 9], TRUE);
		break;

	default:
		DrawRotaGraph3(m_iPosX + PLAYER_DRAW_X, m_iPosY + PLAYER_DRAW_Y, 0, 0,
			PLAYER_IMAGE_EXPANSION_X, PLAYER_IMAGE_EXPANSION_Y, 0, m_iGHPlayer[1], TRUE);
		break;
	}
}
