#include "Enemy.h"
#include "DxLib.h"
#include "Scene.h"
#include <time.h>

//=============================================================
// �R���X�g���N�^
//=============================================================
CEnemy::CEnemy(int x, int y)
	:m_iPosX(x), m_iPosY(y),
	m_iAnimCounter(0), m_iIzikeCounter(0), m_iRestartCounter(0), m_iDecelerationCounter(0), m_iPursuitCounter(0), m_iSpeed(1)
{
	//�摜�̓ǂݍ���
	LoadDivGraph(ENEMY_IZIKE_IMAGE, ENEMY_IZIKE_IMAGE_NUM, 2, 4, 16, 16, m_iGHEnemyIzike);

	//���C�Y���
	m_pMaze = m_cMaze.GetMaze();

	//���C�Y�z��ɂ�����G�l�~�[�̈ʒu
	m_iCurrentMazeIndex = m_iPosY / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + m_iPosX / ENEMY_RELATIVE_MAZE_X;
	m_iPreviousMazeIndex = 0;

	//���������
	m_bIzikeState = false;

	//�ҋ@�i�v���C���[�Ƃ̏Փˌ�j
	m_bWait = false;

	//�������[�h
	m_eMoveMode = MODE_INIT;
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CEnemy::~CEnemy() 
{
	//�摜�f�[�^�j��
	for (int i = 0; i < ENEMY_IZIKE_IMAGE_NUM; i++)
	{
		DeleteGraph(m_iGHEnemyIzike[i]);
	}
}

//=============================================================
// �����_���l��DIRECTION�^�ɃZ�b�g
//=============================================================
void CEnemy::SetDirection(int d) 
{
	switch (d) 
	{
	case 0:
		m_eDirection = VECTOR_RIGHT;
		break;

	case 1:
		m_eDirection = VECTOR_LEFT;
		break;

	case 2:
		m_eDirection = VECTOR_UP;
		break;

	case 3:
		m_eDirection = VECTOR_DOWN;
		break;
	}
}

//=============================================================
// ����
//=============================================================
void CEnemy::Move() 
{
	//���݂̃G�l�~�[�̈ʒu
	m_iCurrentMazeIndex = m_iPosY / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + m_iPosX / ENEMY_RELATIVE_MAZE_X;

	//�ړ�
	Movement();

	//��]
	CheckTurn();

	//�v���C���[��H�ׂ�
	EatPlayer();

	//���[�v
	Warp(m_iCurrentMazeIndex);

	//�������J�E���^�X�V
	IzikeCount();

	//�A�j���[�V�����J�E���^
	m_iAnimCounter++;

	//�T�E���h
	SEEnemyMove();
}

//=============================================================
// ����
//=============================================================
void CEnemy::BaseMove() 
{
}

//=============================================================
// ���X�^�[�g���̓����̐���
//=============================================================
void CEnemy::RestartMove()
{
}

//=============================================================
// �ړ�
//=============================================================
void CEnemy::Movement() 
{
	//�ړ�����
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		if (!IsWall())
		{
			//���[�v�g���l���t�ߌ���
			if (IsDeceleration()) 
			{
				m_iDecelerationCounter++;
				if (m_iDecelerationCounter > ENEMY_DECELERATION_SPEED)
				{
					m_iPosX += m_iSpeed;
					m_iDecelerationCounter = 0;
				}
			}
			else
				m_iPosX += m_iSpeed;
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				m_iPosX--;
			}
			m_iDirection = rand() % 4;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_LEFT:		//��
		if (!IsWall())
		{
			//���[�v�g���l���t�ߌ���
			if (IsDeceleration())
			{
				m_iDecelerationCounter++;
				if (m_iDecelerationCounter > ENEMY_DECELERATION_SPEED)
				{
					m_iPosX -= m_iSpeed;
					m_iDecelerationCounter = 0;
				}
			}
			else
				m_iPosX -= m_iSpeed;
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				m_iPosX++;
			}
			m_iDirection = rand() % 4;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_UP:			//��
		if (!IsWall())
		{
			//���[�v�g���l���t�ߌ���
			if (IsDeceleration())
			{
				m_iDecelerationCounter++;
				if (m_iDecelerationCounter > ENEMY_DECELERATION_SPEED)
				{
					m_iPosY -= m_iSpeed;
					m_iDecelerationCounter = 0;
				}
			}
			else
				m_iPosY -= m_iSpeed;
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				m_iPosY++;
			}
			m_iDirection = rand() % 4;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_DOWN:		//��
		if (!IsWall())
		{
			//���[�v�g���l���t�ߌ���
			if (IsDeceleration()) 
			{
				m_iDecelerationCounter++;
				if (m_iDecelerationCounter > ENEMY_DECELERATION_SPEED)
				{
					m_iPosY += m_iSpeed;
					m_iDecelerationCounter = 0;
				}
			}
			else
			{
				m_iPosY += m_iSpeed;
			}
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				m_iPosY--;
			}
			m_iDirection = rand() % 4;
			SetDirection(m_iDirection);
		}
		break;
	}
}

//=============================================================
// �ǂƂ̓����蔻��
//=============================================================
bool CEnemy::IsWall()
{
	//���W�̒���
	if (!IsCorrection())
	{
		return false;
	}

	//�ړ���̃G�l�~�[�[���W�z��̈ʒu���擾
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		m_iPreviousMazeIndex = (m_iPosY / ENEMY_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / ENEMY_RELATIVE_MAZE_X) + 1;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_LEFT:		//��
		m_iPreviousMazeIndex = (m_iPosY / ENEMY_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / ENEMY_RELATIVE_MAZE_X) - 1;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_UP:			//��
		m_iPreviousMazeIndex = (m_iPosY / ENEMY_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / ENEMY_RELATIVE_MAZE_X) - MAZE_WIDTH;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_DOWN:		//��
		m_iPreviousMazeIndex = (m_iPosY / ENEMY_RELATIVE_MAZE_Y) * MAZE_WIDTH + (m_iPosX / ENEMY_RELATIVE_MAZE_X) + MAZE_WIDTH;
		if (*(m_pMaze + m_iPreviousMazeIndex) == m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;
	}

	return false;
}

//=============================================================
// ���W�̒���
//=============================================================
bool CEnemy::IsCorrection() 
{
	if ((m_iPosX%ENEMY_RELATIVE_MAZE_X == 0) && (m_iPosY%ENEMY_RELATIVE_MAZE_Y == 0))
	{
		return true;
	}

	return false;
}

//=============================================================
// ���[�v
//=============================================================
void CEnemy::Warp(int index)
{
	//���W�̒���
	if (!IsCorrection())
	{
		return;
	}

	if (index == 200)
	{
		SetPosX(360);
	}
	else if (index == 218)
	{
		SetPosX(20);
	}
}

//=============================================================
// �i�s�������猩�ė��[���ړ��ł��邩�ǂ���
//=============================================================
bool CEnemy::IsBothEndsTurn(int index) 
{
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E����Ɖ����`�F�b�N
		if (*(m_pMaze + index - MAZE_WIDTH) != m_cMaze.TILE_WALL || *(m_pMaze + index + MAZE_WIDTH) != m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_LEFT:		//������Ɖ����`�F�b�N
		if (*(m_pMaze + index - MAZE_WIDTH) != m_cMaze.TILE_WALL || *(m_pMaze + index + MAZE_WIDTH) != m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_UP:			//�と�E�ƍ����`�F�b�N
		if (*(m_pMaze + index + 1) != m_cMaze.TILE_WALL || *(m_pMaze + index - 1) != m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;

	case VECTOR_DOWN:		//�と�E�ƍ����`�F�b�N
		if (*(m_pMaze + index + 1) != m_cMaze.TILE_WALL || *(m_pMaze + index - 1) != m_cMaze.TILE_WALL)
		{
			return true;
		}
		break;
	}

	return false;
}

//=============================================================
// ���݂̈ʒu�����]�ł�������������_���Ō��߂�
//=============================================================
void CEnemy::CheckTurn() 
{
	//���W�̒���
	if (!IsCorrection())
	{
		return;
	}

	if (IsBothEndsTurn(m_iCurrentMazeIndex))
	{
		//�ǐ�
		Pursuit();
	}
	else 
	{	
		m_iPursuitCounter = 0;
	}
}

//=============================================================
// �������J�E���^�̍X�V
//=============================================================
void CEnemy::IzikeCount() 
{
	if (m_bIzikeState)
	{
		//�T�E���h�Đ�
		if (!CheckSoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove()))
		{
			PlaySoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove(), DX_PLAYTYPE_LOOP);
		}

		//�J�E���^�̍X�V
		m_iIzikeCounter++;

		if (m_iIzikeCounter > ENEMY_IZIKE_TIME)
		{
			//�T�E���h��~
			if (CheckSoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove()))
			{
				StopSoundMem(g_cScene->GetSoundClass()->GetSEIzikeMove());
			}

			m_bIzikeState = false;
			m_iIzikeCounter = 0;

			///�G�l�~�[��H�ׂ����̃X�R�A���Z�̕ύX�J�E���^�̏�����
			g_cScene->GetGameClass()->GetPlayerClass()->SetIzikeScoreCounter(0);
		}
	}
}

//=============================================================
// ��������Ԏ��v���C���[�ƏՓˌ�5�b�Ԓ����őҋ@
//=============================================================
void CEnemy::Restart() 
{
}

//=============================================================
// �v���C���[��H�׃X�^�[�g�n�_�֖߂�
//=============================================================
void CEnemy::EatPlayer() 
{
	if (!m_bIzikeState) 
	{
		if (IsPlayer())
		{
			//�G�l�~�[�����ʒu
			g_cScene->GetGameClass()->GetBlinkyClass()->SetPosX(9 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetBlinkyClass()->SetPosY(8 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetPinkyClass()->SetPosX(9 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetPinkyClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetInkyClass()->SetPosX(8 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetInkyClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);
			g_cScene->GetGameClass()->GetCrydeClass()->SetPosX(10 * ENEMY_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetCrydeClass()->SetPosY(10 * ENEMY_RELATIVE_MAZE_Y);

			//�v���C���[�����ʒu
			g_cScene->GetGameClass()->GetPlayerClass()->SetPosX(9 * PLAYER_RELATIVE_MAZE_X);
			g_cScene->GetGameClass()->GetPlayerClass()->SetPosY(16 * PLAYER_RELATIVE_MAZE_Y);

			//���X�^�[�g���[�h
			g_cScene->GetGameClass()->GetBlinkyClass()->m_eMoveMode = MODE_PURSUIT;
			g_cScene->GetGameClass()->GetPinkyClass()->m_eMoveMode = MODE_PURSUIT;
			g_cScene->GetGameClass()->GetInkyClass()->m_eMoveMode = MODE_PURSUIT;
			g_cScene->GetGameClass()->GetCrydeClass()->m_eMoveMode = MODE_PURSUIT;

			//�G�l�~�[�̕������Z�b�g
			g_cScene->GetGameClass()->GetBlinkyClass()->SetDirection(0);
			g_cScene->GetGameClass()->GetPinkyClass()->SetDirection(2);
			g_cScene->GetGameClass()->GetInkyClass()->SetDirection(0);
			g_cScene->GetGameClass()->GetCrydeClass()->SetDirection(1);

			//�v���C���[�̕������Z�b�g
			g_cScene->GetGameClass()->GetPlayerClass()->SetZeroDirection();

			//�v���C���[�̎c�@
			int current_remain = g_cScene->GetScoreClass()->GetRemainCounter();
			g_cScene->GetScoreClass()->SetRemainCounter(current_remain - 1);

			//�Q�[������
			g_cScene->GetGameClass()->m_eGameStatus = g_cScene->GetGameClass()->STATUS_GAMEREADY_2;

			//�T�E���h��~
			StopSoundMem(g_cScene->GetSoundClass()->GetSEDotEat());
			StopSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove());
		}
	}
}

//=============================================================
// �v���C���[�Ƃ̓����蔻��
//=============================================================
bool CEnemy::IsPlayer() 
{
	if (g_cScene->GetGameClass()->GetPlayerClass()->GetCurrentMazeIndex() == m_iCurrentMazeIndex) 
	{
		return true;
	}

	return false;
}

//=============================================================
// ���[�v�g���l���t�ߌ���
//=============================================================
bool CEnemy::IsDeceleration() 
{
	if ((200 <= m_iCurrentMazeIndex) && (m_iCurrentMazeIndex <= 203) ||
		(215 <= m_iCurrentMazeIndex) && (m_iCurrentMazeIndex <= 217))
	{
		return true;
	}

	return false;
}

//=============================================================
// �v���C���[�ǐ�
//=============================================================
void CEnemy::Pursuit() 
{
	int playerX = g_cScene->GetGameClass()->GetPlayerClass()->GetPosX() / PLAYER_IMAGE_EXPANSION_X;
	int playerY = g_cScene->GetGameClass()->GetPlayerClass()->GetPosY() / PLAYER_IMAGE_EXPANSION_Y;

	if ((m_iPosX / ENEMY_IMAGE_EXPANSION_X < playerX) && (m_iPosY / ENEMY_IMAGE_EXPANSION_Y < playerY))			//�E����
	{
		int r = rand() % 2;
		m_iDirection = (r == 0) ? 0 : 3;
		SetDirection(m_iDirection);
		m_iPursuitCounter++;
		if (m_iPursuitCounter > 2) 
		{
			SetDirection(1);		//��
			m_iPursuitCounter++;
			if (m_iPursuitCounter > 3) 
			{
				SetDirection(2);	//��
				m_iPursuitCounter = 0;
			}
		}
	}
	else if ((m_iPosX / ENEMY_IMAGE_EXPANSION_X < playerX) && (m_iPosY / ENEMY_IMAGE_EXPANSION_Y > playerY))	//�E����
	{
		int r = rand() % 2;
		m_iDirection = (r == 0) ? 0 : 2;
		SetDirection(m_iDirection);	
		m_iPursuitCounter++;
		if (m_iPursuitCounter > 2)
		{
			SetDirection(1);		//��
			m_iPursuitCounter++;
			if (m_iPursuitCounter > 3)
			{
				SetDirection(3);	//��
				m_iPursuitCounter = 0;
			}
		}
	}
	else if ((m_iPosX / ENEMY_IMAGE_EXPANSION_X > playerX) && (m_iPosY / ENEMY_IMAGE_EXPANSION_Y < playerY))	//������
	{
		int r = rand() % 2;
		m_iDirection = (r == 0) ? 1 : 3;
		SetDirection(m_iDirection);
		m_iPursuitCounter++;
		if (m_iPursuitCounter > 2)
		{
			SetDirection(0);		//�E
			m_iPursuitCounter++;
			if (m_iPursuitCounter > 3)
			{
				SetDirection(2);	//��
				m_iPursuitCounter = 0;
			}
		}
	}
	else if ((m_iPosX / ENEMY_IMAGE_EXPANSION_X > playerX) && (m_iPosY / ENEMY_IMAGE_EXPANSION_Y > playerY))	//������
	{
		int r = rand() % 2;
		m_iDirection = (r == 0) ? 1 : 2;
		SetDirection(m_iDirection);
		m_iPursuitCounter++;
		if (m_iPursuitCounter > 2)
		{
			SetDirection(0);		//�E
			m_iPursuitCounter++;
			if (m_iPursuitCounter > 3)
			{
				SetDirection(3);	//��
				m_iPursuitCounter = 0;
			}
		}
	}
	else
	{
		m_iDirection = rand() % 4;
		SetDirection(m_iDirection);
	}
}

//=============================================================
// �G�l�~�[�ړ��T�E���h
//=============================================================
void CEnemy::SEEnemyMove() 
{
	if (g_cScene->GetGameClass()->m_eGameStatus == g_cScene->GetGameClass()->STATUS_GAME) 
	{
		if (!m_bIzikeState)
		{
			//�T�E���h�Đ�
			if (!CheckSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove()))
			{
				PlaySoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove(), DX_PLAYTYPE_LOOP);
			}
		}
		else
		{
			//�T�E���h��~
			if (CheckSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove()))
			{
				StopSoundMem(g_cScene->GetSoundClass()->GetSEEnemyMove());
			}
		}
	}
}

//=============================================================
// �`��
//=============================================================
void CEnemy::Draw() const 
{
}

//=============================================================
// �ʏ���
//=============================================================
void CEnemy::NormalState() const 
{
}

//=============================================================
// ���������
//=============================================================
void CEnemy::IzikeState() const 
{
	if (m_iIzikeCounter < ENEMY_IZIKE_LIMIT_TIME)
	{
		DrawRotaGraph3(m_iPosX + ENEMY_DRAW_X, m_iPosY + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHEnemyIzike[(m_iAnimCounter / ENEMY_ANIM_SPEED) % 2], TRUE);
	}
	else
	{
		DrawRotaGraph3(m_iPosX + ENEMY_DRAW_X, m_iPosY + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHEnemyIzike[(m_iAnimCounter / ENEMY_ANIM_SPEED) % 4], TRUE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�A�J�x�C�N���X
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// �R���X�g���N�^
//=============================================================
CBlinky::CBlinky(int x, int y)
	:CEnemy(x*ENEMY_RELATIVE_MAZE_X, y*ENEMY_RELATIVE_MAZE_Y), m_iBaseCounter(0)
{
	//�摜�̓ǂݍ���
	LoadDivGraph(ENEMY_BLINKY_IMAGE, ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHBlinky);

	//�ړ�����
	srand((unsigned int)time(NULL));
	m_iDirection = 1;
	SetDirection(m_iDirection);
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CBlinky::~CBlinky() 
{
	//�摜�f�[�^�j��
	for (int i = 0; i < ENEMY_IMAGE_NUM; i++) 
	{
		DeleteGraph(m_iGHBlinky[i]);
	}
}

//=============================================================
// ����
//=============================================================
void CBlinky::BaseMove() 
{
	//���݂̃G�l�~�[�̈ʒu
	SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

	//�ړ�
	BaseMovement();

	BaseTurn();

	//�v���C���[��H�ׂ�
	EatPlayer();

	//�������J�E���^�X�V
	IzikeCount();

	//�A�j���[�V�����J�E���^
	SetAnimCounter(GetAnimCounter() + 1);

	//�T�E���h
	SEEnemyMove();
}

//=============================================================
// �����̌��܂����ړ��i�E��j
//=============================================================
void CBlinky::BaseMovement() 
{
	int x = GetPosX();
	int y = GetPosY();

	//�ړ�����
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}

			//�ǐՃ��[�h
			m_eMoveMode = MODE_PURSUIT;
			m_iBaseCounter++;
		}
		break;

	case VECTOR_LEFT:		//��
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_UP:			//��
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_DOWN:		//��
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}
			m_iBaseCounter++;
		}
		break;
	}
}

//=============================================================
// �����̌��߂�ꂽ��]
//=============================================================
void CBlinky::BaseTurn() 
{
	//���W�̒���
	if (!IsCorrection())
	{
		return;
	}

	//�Œ�ʒu�ŉ�]
	if ((GetCurrentMazeIndex() == 186) || (GetCurrentMazeIndex() == 204)|| (GetCurrentMazeIndex() == 84))
	{
		m_iBaseCounter++;
	}

	switch (m_iBaseCounter) 
	{
	case 1:
		SetDirection(3);	//��
		break;

	case 2:
		SetDirection(1);	//��
		break;

	case 3:
		SetDirection(2);	//��
		break;

	case 4:
		SetDirection(0);	//�E
		break;
	}
}

//=============================================================
// ���X�^�[�g���̓����̐���
//=============================================================
void CBlinky::RestartMove() 
{
	//��莞�ԑҋ@
	if (GetWait())
	{
		Restart();
	}
	else
	{
		//���݂̃G�l�~�[�̈ʒu
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//�ړ�
		RestartMovement();

		//�v���C���[��H�ׂ�
		EatPlayer();

		//�������J�E���^�X�V
		IzikeCount();
	}
	//�A�j���[�V�����J�E���^
	SetAnimCounter(GetAnimCounter() + 1);

	//�T�E���h
	SEEnemyMove();
}

//=============================================================
// ���X�^�[�g���̈ړ�
//=============================================================
void CBlinky::RestartMovement() 
{
	int x = GetPosX();
	int y = GetPosY();

	//�ړ�����
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			//���ɐ���
			SetDirection(3);
		}
		break;

	case VECTOR_LEFT:		//��
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			//���ɐ���
			SetDirection(3);
		}
		break;

	case VECTOR_UP:			//��
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}

			//�E�����ɐ���
			m_iDirection = rand() % 2;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_DOWN:		//��
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());

		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}

			//�ǐՃ��[�h
			m_eMoveMode = MODE_PURSUIT;
		}
		break;
	}
}

//=============================================================
// ��������Ԏ��v���C���[�ƏՓˌ�5�b�Ԓ����őҋ@
//=============================================================
void CBlinky::Restart()
{
	SetRestartCounter(GetRestartCounter() + 1);
	if (GetRestartCounter() > 300)
	{
		SetWait(false);
		SetRestartCounter(0);
		SetDirection(2);
	}
}

//=============================================================
// �`��
//=============================================================
void CBlinky::Draw() const 
{
	if (GetIzikeState())
	{
		IzikeState();
	}
	else
	{
		NormalState();
	}
}

//=============================================================
// �ʏ���
//=============================================================
void CBlinky::NormalState() const
{
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHBlinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;

	case VECTOR_LEFT:		//��
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHBlinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 2], TRUE);
		break;

	case VECTOR_UP:			//��
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHBlinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 4], TRUE);
		break;

	case VECTOR_DOWN:		//��
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHBlinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 6], TRUE);
		break;

	default:
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHBlinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�s���L�[�N���X
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// �R���X�g���N�^
//=============================================================
CPinky::CPinky(int x, int y)
	:CEnemy(x*ENEMY_RELATIVE_MAZE_X, y*ENEMY_RELATIVE_MAZE_Y), m_iBaseCounter(0)
{
	//�摜�̓ǂݍ���
	LoadDivGraph(ENEMY_PINKY_IMAGE, ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHPinky);

	//�ړ�����
	srand((unsigned int)time(NULL));
	m_iDirection = 2;
	SetDirection(m_iDirection);

	//�X�^�[�g����
	m_bStart = false;
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CPinky::~CPinky()
{
	//�摜�f�[�^�j��
	for (int i = 0; i < ENEMY_IMAGE_NUM; i++)
	{
		DeleteGraph(m_iGHPinky[i]);
	}
}

//=============================================================
// ����
//=============================================================
void CPinky::BaseMove()
{
	if (m_bStart) 
	{
		//���݂̃G�l�~�[�̈ʒu
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//�ړ�
		BaseMovement();

		BaseTurn();

		//�v���C���[��H�ׂ�
		EatPlayer();
	}
	//�������J�E���^�X�V
	IzikeCount();

	//�A�j���[�V�����J�E���^
	SetAnimCounter(GetAnimCounter() + 1);

	//�T�E���h
	SEEnemyMove();
}

//=============================================================
// �����̌��܂����ړ��i�E��j
//=============================================================
void CPinky::BaseMovement()
{
	int x = GetPosX();
	int y = GetPosY();

	//�ړ�����
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_LEFT:		//��
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_UP:			//��
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_DOWN:		//��
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}
			m_iBaseCounter++;
		}
		break;
	}
}

//=============================================================
// �����̌��߂�ꂽ��]
//=============================================================
void CPinky::BaseTurn()
{
	//���W�̒���
	if (!IsCorrection())
	{
		return;
	}

	//�Œ�ʒu�ŉ�]
	if ((GetCurrentMazeIndex() == 186) || (GetCurrentMazeIndex() == 204) || (GetCurrentMazeIndex() == 84))
	{
		m_iBaseCounter++;
	}

	switch (m_iBaseCounter)
	{
	case 1:
		SetDirection(1);	//�E
		break;

	case 2:
		SetDirection(3);	//��
		break;

	case 3:
		SetDirection(1);	//��
		break;

	case 4:
		SetDirection(2);	//��
		break;

	case 5:
		//�ǐՃ��[�h
		m_eMoveMode = MODE_PURSUIT;
		break;
	}
}

//=============================================================
// ���X�^�[�g���̓����̐���
//=============================================================
void CPinky::RestartMove()
{
	//��莞�ԑҋ@
	if (GetWait())
	{
		Restart();
	}
	else
	{
		//���݂̃G�l�~�[�̈ʒu
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//�ړ�
		RestartMovement();

		//�v���C���[��H�ׂ�
		EatPlayer();

		//�������J�E���^�X�V
		IzikeCount();
	}
	//�A�j���[�V�����J�E���^
	SetAnimCounter(GetAnimCounter() + 1);

	//�T�E���h
	SEEnemyMove();
}

//=============================================================
// ���X�^�[�g���̈ړ�
//=============================================================
void CPinky::RestartMovement()
{
	int x = GetPosX();
	int y = GetPosY();

	//�ړ�����
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			//���ɐ���
			SetDirection(3);
		}
		break;

	case VECTOR_LEFT:		//��
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			//���ɐ���
			SetDirection(3);
		}
		break;

	case VECTOR_UP:			//��
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}

			//�E�����ɐ���
			m_iDirection = rand() % 2;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_DOWN:		//��
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());

		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}

			//�ǐՃ��[�h
			m_eMoveMode = MODE_PURSUIT;
		}
		break;
	}
}

//=============================================================
// ��������Ԏ��v���C���[�ƏՓˌ�5�b�Ԓ����őҋ@
//=============================================================
void CPinky::Restart()
{
	SetRestartCounter(GetRestartCounter() + 1);
	if (GetRestartCounter() > 300)
	{
		SetWait(false);
		SetRestartCounter(0);
		SetDirection(2);
	}
}

//=============================================================
// �`��
//=============================================================
void CPinky::Draw() const
{
	if (GetIzikeState())
	{
		IzikeState();
	}
	else
	{
		NormalState();
	}
}

//=============================================================
// �ʏ���
//=============================================================
void CPinky::NormalState() const
{
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHPinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;

	case VECTOR_LEFT:		//��
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHPinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 2], TRUE);
		break;

	case VECTOR_UP:			//��
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHPinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 4], TRUE);
		break;

	case VECTOR_DOWN:		//��
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHPinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 6], TRUE);
		break;

	default:
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHPinky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�A�I�X�P�N���X
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// �R���X�g���N�^
//=============================================================
CInky::CInky(int x, int y)
	:CEnemy(x*ENEMY_RELATIVE_MAZE_X, y*ENEMY_RELATIVE_MAZE_Y), m_iBaseCounter(0)
{
	//�摜�̓ǂݍ���
	LoadDivGraph(ENEMY_INKY_IMAGE, ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHInky);

	//�ړ�����
	srand((unsigned int)time(NULL));
	m_iDirection = 0;
	SetDirection(m_iDirection);

	//�X�^�[�g����
	m_bStart = false;
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CInky::~CInky()
{
	//�摜�f�[�^�j��
	for (int i = 0; i < ENEMY_IMAGE_NUM; i++)
	{
		DeleteGraph(m_iGHInky[i]);
	}
}

//=============================================================
// ����
//=============================================================
void CInky::BaseMove()
{
	if (m_bStart)
	{
		//���݂̃G�l�~�[�̈ʒu
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//�ړ�
		BaseMovement();

		BaseTurn();

		//�v���C���[��H�ׂ�
		EatPlayer();
	}
	//�������J�E���^�X�V
	IzikeCount();

	//�A�j���[�V�����J�E���^
	SetAnimCounter(GetAnimCounter() + 1);

	//�T�E���h
	SEEnemyMove();
}

//=============================================================
// �����̌��܂����ړ��i�E��j
//=============================================================
void CInky::BaseMovement()
{
	int x = GetPosX();
	int y = GetPosY();

	//�ړ�����
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_LEFT:		//��
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_UP:			//��
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_DOWN:		//��
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}
			m_iBaseCounter++;
		}
		break;
	}
}

//=============================================================
// �����̌��߂�ꂽ��]
//=============================================================
void CInky::BaseTurn()
{
	//���W�̒���
	if (!IsCorrection())
	{
		return;
	}

	//�Œ�ʒu�ŉ�]
	if ((GetCurrentMazeIndex() == 208))
	{
		m_iBaseCounter++;
	}

	switch (m_iBaseCounter)
	{
	case 1:
		SetDirection(2);	//��
		break;

	case 2:
		SetDirection(1);	//��
		break;

	case 3:
		SetDirection(3);	//��
		break;

	case 4:
		SetDirection(1);	//��
		break;

	case 5:
		//�ǐՃ��[�h
		m_eMoveMode = MODE_PURSUIT;
		break;
	}
}

//=============================================================
// ���X�^�[�g���̓����̐���
//=============================================================
void CInky::RestartMove()
{
	//��莞�ԑҋ@
	if (GetWait())
	{
		Restart();
	}
	else
	{
		//���݂̃G�l�~�[�̈ʒu
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//�ړ�
		RestartMovement();

		//�v���C���[��H�ׂ�
		EatPlayer();

		//�������J�E���^�X�V
		IzikeCount();
	}
	//�A�j���[�V�����J�E���^
	SetAnimCounter(GetAnimCounter() + 1);

	//�T�E���h
	SEEnemyMove();
}

//=============================================================
// ���X�^�[�g���̈ړ�
//=============================================================
void CInky::RestartMovement()
{
	int x = GetPosX();
	int y = GetPosY();

	//�ړ�����
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			//���ɐ���
			SetDirection(3);
		}
		break;

	case VECTOR_LEFT:		//��
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			//���ɐ���
			SetDirection(3);
		}
		break;

	case VECTOR_UP:			//��
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}

			//�E�����ɐ���
			m_iDirection = rand() % 2;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_DOWN:		//��
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());

		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}

			//�ǐՃ��[�h
			m_eMoveMode = MODE_PURSUIT;
		}
		break;
	}
}

//=============================================================
// ��������Ԏ��v���C���[�ƏՓˌ�5�b�Ԓ����őҋ@
//=============================================================
void CInky::Restart()
{
	SetRestartCounter(GetRestartCounter() + 1);
	if (GetRestartCounter() > 300)
	{
		SetWait(false);
		SetRestartCounter(0);
		SetDirection(2);
	}
}

//=============================================================
// �`��
//=============================================================
void CInky::Draw() const
{
	if (GetIzikeState())
	{
		IzikeState();
	}
	else
	{
		NormalState();
	}
}

//=============================================================
// �ʏ���
//=============================================================
void CInky::NormalState() const
{
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHInky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;

	case VECTOR_LEFT:		//��
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHInky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 2], TRUE);
		break;

	case VECTOR_UP:			//��
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHInky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 4], TRUE);
		break;

	case VECTOR_DOWN:		//��
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHInky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 6], TRUE);
		break;

	default:
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHInky[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�O�Y�^�N���X
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================
// �R���X�g���N�^
//=============================================================
CCryde::CCryde(int x, int y)
	:CEnemy(x*ENEMY_RELATIVE_MAZE_X, y*ENEMY_RELATIVE_MAZE_Y), m_iBaseCounter(0)
{
	//�摜�̓ǂݍ���
	LoadDivGraph(ENEMY_CRYDE_IMAGE, ENEMY_IMAGE_NUM, 2, 4, 16, 16, m_iGHCryde);

	//�ړ�����
	srand((unsigned int)time(NULL));
	m_iDirection = 1;
	SetDirection(m_iDirection);

	//�X�^�[�g����
	m_bStart = false;
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CCryde::~CCryde()
{
	//�摜�f�[�^�j��
	for (int i = 0; i < ENEMY_IMAGE_NUM; i++)
	{
		DeleteGraph(m_iGHCryde[i]);
	}
}

//=============================================================
// ����
//=============================================================
void CCryde::BaseMove()
{
	if (m_bStart)
	{
		//���݂̃G�l�~�[�̈ʒu
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//�ړ�
		BaseMovement();

		BaseTurn();

		//�v���C���[��H�ׂ�
		EatPlayer();
	}
	//�������J�E���^�X�V
	IzikeCount();

	//�A�j���[�V�����J�E���^
	SetAnimCounter(GetAnimCounter() + 1);

	//�T�E���h
	SEEnemyMove();
}

//=============================================================
// �����̌��܂����ړ��i�E��j
//=============================================================
void CCryde::BaseMovement()
{
	int x = GetPosX();
	int y = GetPosY();

	//�ړ�����
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_LEFT:		//��
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_UP:			//��
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}
			m_iBaseCounter++;
		}
		break;

	case VECTOR_DOWN:		//��
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}
			m_iBaseCounter++;
		}
		break;
	}
}

//=============================================================
// �����̌��߂�ꂽ��]
//=============================================================
void CCryde::BaseTurn()
{
	//���W�̒���
	if (!IsCorrection())
	{
		return;
	}

	//�Œ�ʒu�ŉ�]
	if ((GetCurrentMazeIndex() == 209))
	{
		m_iBaseCounter++;
	}

	switch (m_iBaseCounter)
	{
	case 1:
		SetDirection(2);	//��
		g_cScene->GetGameClass()->GetPinkyClass()->SetStart(true);
		break;

	case 2:
		SetDirection(0);	//�E
		break;

	case 3:
		SetDirection(3);	//��
		break;

	case 4:
		SetDirection(0);	//�E
		break;

	case 5:
		//�ǐՃ��[�h
		m_eMoveMode = MODE_PURSUIT;
		break;
	}
}

//=============================================================
// ���X�^�[�g���̓����̐���
//=============================================================
void CCryde::RestartMove()
{
	//��莞�ԑҋ@
	if (GetWait())
	{
		Restart();
	}
	else
	{
		//���݂̃G�l�~�[�̈ʒu
		SetCurrentMazeIndex(GetPosY() / ENEMY_RELATIVE_MAZE_Y * MAZE_WIDTH + GetPosX() / ENEMY_RELATIVE_MAZE_X);

		//�ړ�
		RestartMovement();

		//�v���C���[��H�ׂ�
		EatPlayer();

		//�������J�E���^�X�V
		IzikeCount();
	}
	//�A�j���[�V�����J�E���^
	SetAnimCounter(GetAnimCounter() + 1);

	//�T�E���h
	SEEnemyMove();
}

//=============================================================
// ���X�^�[�g���̈ړ�
//=============================================================
void CCryde::RestartMovement()
{
	int x = GetPosX();
	int y = GetPosY();

	//�ړ�����
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		if (!IsWall())
		{
			SetPosX(x += GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x - 1);
			}
			//���ɐ���
			SetDirection(3);
		}
		break;

	case VECTOR_LEFT:		//��
		if (!IsWall())
		{
			SetPosX(x -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosX(x + 1);
			}
			//���ɐ���
			SetDirection(3);
		}
		break;

	case VECTOR_UP:			//��
		if (!IsWall())
		{
			SetPosY(y -= GetSpeed());
		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y - 1);
			}

			//�E�����ɐ���
			m_iDirection = rand() % 2;
			SetDirection(m_iDirection);
		}
		break;

	case VECTOR_DOWN:		//��
		if (!IsWall())
		{
			SetPosY(y += GetSpeed());

		}
		else
		{
			//���W�̒���
			while (!IsCorrection())
			{
				SetPosY(y + 1);
			}

			//�ǐՃ��[�h
			m_eMoveMode = MODE_PURSUIT;
		}
		break;
	}
}

//=============================================================
// ��������Ԏ��v���C���[�ƏՓˌ�5�b�Ԓ����őҋ@
//=============================================================
void CCryde::Restart()
{
	SetRestartCounter(GetRestartCounter() + 1);
	if (GetRestartCounter() > 300)
	{
		SetWait(false);
		SetRestartCounter(0);
		SetDirection(2);
	}
}

//=============================================================
// �`��
//=============================================================
void CCryde::Draw() const
{
	if (GetIzikeState())
	{
		IzikeState();
	}
	else
	{
		NormalState();
	}
}

//=============================================================
// �ʏ���
//=============================================================
void CCryde::NormalState() const
{
	switch (m_eDirection)
	{
	case VECTOR_RIGHT:		//�E
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHCryde[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;

	case VECTOR_LEFT:		//��
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHCryde[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 2], TRUE);
		break;

	case VECTOR_UP:			//��
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHCryde[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 4], TRUE);
		break;

	case VECTOR_DOWN:		//��
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHCryde[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2 + 6], TRUE);
		break;

	default:
		DrawRotaGraph3(GetPosX() + ENEMY_DRAW_X, GetPosY() + ENEMY_DRAW_Y, 0, 0,
			ENEMY_IMAGE_EXPANSION_X, ENEMY_IMAGE_EXPANSION_Y, 0, m_iGHCryde[(GetAnimCounter() / ENEMY_ANIM_SPEED) % 2], TRUE);
		break;
	}
}