#pragma once
#ifndef __GAME_H_INCLUDED__
#define __GAME_H_INCLUDED__

#include "Player.h"
#include "Maze.h"
#include "Dot.h"
#include "Enemy.h"
#include "Fruit.h"
#include "Pause.h"

//�v���C���[���X�^�[�g���̈ꎞ��~����
#define GAME_RESTART_TIME	300

//=============================================================
// �Q�[���N���X
//=============================================================
class CGame
{
public:
	//�Q�[�����
	enum GAMESTATUS
	{
		STATUS_GAMEREADY_1 = 0,
		STATUS_GAMEREADY_2 = 1,
		STATUS_GAME = 10,
		STATUS_GAMECLEAR = 100,
		STATUS_GAMEOVER = 150,
		STATUS_PAUSE = 300,
	};
	GAMESTATUS m_eGameStatus;

private:
	//�v���C���[�̃N���X
	CPlayer* m_cPlayer;
	//���C�Y�N���X
	CMaze* m_cMaze;
	//�h�b�g�N���X
	CDot* m_cDot;
	//�G�l�~�[�N���X
	CBlinky* m_cBlinky;
	CPinky* m_cPinky;
	CInky* m_cInky;
	CCryde* m_cCryde;
	//�t���[�c�N���X
	CFruit* m_cFruit;
	//�|�[�Y�N���X
	CPause* m_cPause;

	//���X�^�[�g
	bool m_bRestart = false;
	int  m_iRestartCounter;

	//�Q�[�������^�C�}�[
	int m_iGameReadyTimer;
	//�Q�[���^�C�}�[
	int m_iGameTimer;
	//�Q�[���N���A�^�C�}�[
	int m_iClearTimer;
	//�Q�[���I�[�o�[�^�C�}�[
	int m_iGameOverTimer;
	bool m_bSEGameOver;

	//�t�H���g�n���h��
	int m_iFontHandle;

public:
	//�R���X�g���N�^�A�f�X�g���N�^
	CGame();
	~CGame();

	//�Q�b�^�[
	CMaze* GetMazeClass() const { return m_cMaze; }
	CPlayer* GetPlayerClass() const { return m_cPlayer; }
	CDot* GetDotClass() const { return m_cDot; }
	CBlinky* GetBlinkyClass() const { return m_cBlinky; }
	CPinky* GetPinkyClass() const { return m_cPinky; }
	CInky* GetInkyClass() const { return m_cInky; }
	CCryde* GetCrydeClass() const { return m_cCryde; }
	CFruit* GetFruitClass() const { return m_cFruit; }

	//�Z�b�^�[
	void SetRestart(bool restart) { m_bRestart = restart; }

	//�X�V
	void Update();

private:
	//����
	void Input();

	//����
	void Move();
	void EnemyMove(CEnemy* enemy);		//�G�l�~�[����
	void GameTime();					//�Q�[������
	bool IsGameReady();					//�Q�[������
	bool IsGameClear();					//�Q�[���N���A���ǂ���
	void GameClear();					//�Q�[���N���A
	bool IsGameOver();					//�Q�[���I�[�o�[���ǂ���
	void GameOver();					//�Q�[���I�[�o�[

	//�`��
	void Draw() const;
	void GameReadyText1() const;
	void GameReadyText2() const;
	void GameOverText() const;
	void GameClearText() const;
};
#endif



