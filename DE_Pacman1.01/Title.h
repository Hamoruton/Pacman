#pragma once
#ifndef __TITLE_H_INCLUDED_
#define __TITLE_H_INCLUDED_

#include "TitleMenu.h"
#include "TitleConfig.h"

//=============================================================
// タイトルクラス
//=============================================================
class CTitle
{
public:
	enum TITLESTATUS
	{
		STATUS_MENU = 10,
		STATUS_CONFIG = 50,
	};
	TITLESTATUS m_eTitleStatus;

private:
	//タイトルメニュークラス
	CTitleMenu* m_cTitleMenu;
	//タイトル設定クラス
	CTitleConfig* m_cTitleConfig;

public:
	//コンストラクタ.デストラクタ
	CTitle();
	~CTitle();

	//ゲッター
	CTitleMenu* GetTitleMenuClass() const { return m_cTitleMenu; }
	CTitleConfig* GetTitleConfigClass() const { return m_cTitleConfig; }

	//更新
	void Update();

	//入力
	void Input();

	//動作
	void Move();

	//描画
	void Draw() const;
};

#endif



