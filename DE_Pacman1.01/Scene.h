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
// シーンクラス
//=============================================================
class CScene
{
public:
	//シーン状態
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
	//タイトルクラス
	CTitle* m_cTitle;
	//ゲームクラス
	CGame* m_cGame;
	//コーヒーブレイククラス
	CCoffeebreak* m_cCoffeebreak;

	//スコアクラス
	CScore* m_cScore;
	//サウンドクラス
	CSound* m_cSound;

public:
	//コンストラクタ.デストラクタ
	CScene();
	~CScene();

	//ゲッター
	CTitle* GetTitleClass() const { return m_cTitle; }
	CGame* GetGameClass() const { return m_cGame; }
	CCoffeebreak* GetCoffeebreakClass() const { return m_cCoffeebreak; }
	CScore* GetScoreClass() const { return m_cScore; }
	CSound* GetSoundClass() const { return m_cSound; }

	//入力
	int m_iKey[256] = { 0 };

	//更新
	void Update();

	//入力処理
	bool InputKey();
};
extern CScene* g_cScene;

#endif
