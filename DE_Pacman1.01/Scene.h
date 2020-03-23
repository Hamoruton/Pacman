#pragma once
#ifndef __SCENE_H_INCLUDED__
#define __SCENE_H_INCLUDED__

#include "Title.h"
#include "Game.h"
#include "Coffeebreak.h"

#include "Score.h"
#include "Sound.h"
#include "FPS.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_COLOR   32

//=============================================================
// �V�[���N���X
//=============================================================
class CScene
{
public:
	//�V�[�����
	enum SCENESTATUS
	{
		TITLE = 0,
		FROM_TITLE_TO_GAME = 1,
		GAME_01 = 100,
		GAME_02 = 101,
		FROM_GAME_TO_COFFEEBREAK = 201,
		COFFEEBREAK = 205,
		FROM_COFFEEBREAK_TO_GAME = 210,
		FROM_GAME_TO_TITLE = 300,
	};
	SCENESTATUS m_eSceneStatus;

private:
	//�^�C�g���N���X
	CTitle* m_cTitle;
	//�Q�[���N���X
	CGame* m_cGame;
	//�R�[�q�[�u���C�N�N���X
	CCoffeebreak* m_cCoffeebreak;

	//�X�R�A�N���X
	CScore* m_cScore;
	//�T�E���h�N���X
	CSound* m_cSound;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CScene();
	~CScene();

	//�Q�b�^�[
	CTitle* GetTitleClass() const { return m_cTitle; }
	CGame* GetGameClass() const { return m_cGame; }
	CCoffeebreak* GetCoffeebreakClass() const { return m_cCoffeebreak; }
	CScore* GetScoreClass() const { return m_cScore; }
	CSound* GetSoundClass() const { return m_cSound; }

	//����
	int m_iKey[256] = { 0 };

	//�X�V
	void Update();

	//���͏���
	bool InputKey();
};
extern CScene* g_cScene;

#endif
