#include "Title.h"
#include "DxLib.h"
#include "Scene.h"

//=============================================================
// コンストラクタ
//=============================================================
CTitle::CTitle()
{
	//クラスの生成
	m_cTitleMenu = new CTitleMenu;
	m_cTitleConfig = new CTitleConfig;

	//タイトル状態
	m_eTitleStatus = STATUS_MENU;
}

//=============================================================
// デストラクタ
//=============================================================
CTitle::~CTitle() 
{
	//クラスの破棄
	delete m_cTitleMenu;
	delete m_cTitleConfig;
}

//=============================================================
// 更新
//=============================================================
void CTitle::Update() 
{
	//入力
	Input();

	//動作
	Move();

	//描画
	Draw();
}

//=============================================================
// 入力
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
// 動作
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
// 描画
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