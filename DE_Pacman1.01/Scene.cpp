#include "Scene.h"
#include "DxLib.h"

//�O���[�o���ϐ�
CScene* g_cScene;
CFPS g_cFps(10, 0, 60);

//=============================================================
// �R���X�g���N�^
//=============================================================
CScene::CScene() 
{
	//������
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR);
	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SetBackgroundColor(0, 0, 0);

	//�^�C�g���V�[��
	m_eSceneStatus = TITLE;

	//�N���X�̐���
	m_cTitle = new CTitle;
	m_cScore = new CScore;
	m_cSound = new CSound;
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CScene::~CScene() 
{
	//�N���X�̔j��
	delete m_cScore;
	delete m_cSound;

	DxLib_End();
}

//=============================================================
// �X�V
//=============================================================
void CScene::Update() 
{
	while (!ProcessMessage())
	{
		ClearDrawScreen();

		if (!InputKey())
			break;

		switch (m_eSceneStatus) 
		{
		case TITLE:
			m_cTitle->Update();
			break;

		case FROM_TITLE_TO_GAME:
			m_cTitle->Update();
			m_eSceneStatus = GAME_01;

			//�����Ɣj��
			delete m_cTitle;
			m_cGame = new CGame;
			break;

		case GAME_01:
			m_cGame->Update();
			break;

		case FROM_GAME_TO_COFFEEBREAK:
			m_cGame->Update();
			m_eSceneStatus = COFFEEBREAK;

			//�N���X�̐����Ɣj��
			delete m_cGame;
			m_cCoffeebreak = new CCoffeebreak;
			break;

		case COFFEEBREAK:
			m_cCoffeebreak->Update();
			break;

		case FROM_COFFEEBREAK_TO_GAME:
			m_cCoffeebreak->Update();
			m_eSceneStatus = GAME_02;

			//�N���X�̐����Ɣj��
			delete m_cCoffeebreak;
			m_cGame = new CGame;

			//���x���A�b�v�i�X�s�[�h�̏㏸�j
			m_cGame->GetPlayerClass()->SetSpeed(2);
			m_cGame->GetBlinkyClass()->SetSpeed(2);
			m_cGame->GetPinkyClass()->SetSpeed(2);
			m_cGame->GetInkyClass()->SetSpeed(2);
			m_cGame->GetCrydeClass()->SetSpeed(2);
			m_cScore->SetLevel(m_cScore->GetLevel() + 1);
			break;

		case GAME_02:
			m_cGame->Update();
			break;

		case FROM_GAME_TO_TITLE:
			m_cGame->Update();
			m_eSceneStatus = TITLE;

			//�X�R�A�̍X�V/���Z�b�g
			g_cScene->GetScoreClass()->HighScoreUpdate();
			g_cScene->GetScoreClass()->ScoreReset();

			//�N���X�̐����Ɣj��
			delete m_cGame;
			m_cTitle = new CTitle;
			break;
		}
		//FPS�̒���
		g_cFps.Update();
		ScreenFlip();
		g_cFps.Wait();
	}
}

//=============================================================
// ���͏���
//=============================================================
bool CScene::InputKey() 
{
	char cTempKey[256];
	GetHitKeyStateAll(cTempKey);
	for (int i = 0; i < 256; i++)
	{
		if (cTempKey[i] != 0)
		{
			m_iKey[i]++;
		}
		else
		{
			m_iKey[i] = 0;
		}
	}
	if (m_iKey[KEY_INPUT_ESCAPE])
		return false;

	return true;
}