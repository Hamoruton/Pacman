#pragma once
#ifndef __SOUND_H_INCLUDED__
#define __SOUND_H_INCLUDED__

#define SOUND_CURSOR		"Assets/Sounds/01 クレジットサウンド.m4a"
#define SOUND_GAME_START	"Assets/Sounds/02 スタートミュージック.m4a"
#define SOUND_DOT_EAT		"Assets/Sounds/03 パックマン_クッキーを食べる.m4a"
#define SOUND_ENEMY_MOVE	"Assets/Sounds/05 ゴースト_平常移動.m4a"
#define SOUND_FRUIT_EAT		"Assets/Sounds/10 パックマン_フルーツを食べる.m4a"
#define SOUND_IZIKE_MOVE	"Assets/Sounds/11 ゴースト_いじけ.m4a"
#define SOUND_ENEMY_EAT		"Assets/Sounds/12 パックマン_ゴーストを食べる.m4a"
#define SOUND_PLAYER_EAT	"Assets/Sounds/14 ミス.m4a"
#define SOUND_COFFEEBREAK	"Assets/Sounds/15 コーヒーブレイクミュージック.m4a"

//=============================================================
// サウンドクラス
//=============================================================
class CSound
{
private:
	//サウンドハンドル
	int m_iSECursor;
	int m_iSEGameStart;
	int m_iSEDotEat;
	int m_iSEEnemyMove;
	int m_iSEIzikeMove;
	int m_iSEEnemyEat;
	int m_iSEPlayerEat;
	int m_iSECoffeebreak;
	int m_iSEFruitEat;

public:
	//コンストラクタ.デストラクタ
	CSound();
	~CSound();

	//ゲッター
	int GetSECursor() const { return m_iSECursor; }
	int GetSEGameStart() const { return m_iSEGameStart; }
	int GetSEDotEat() const { return m_iSEDotEat; }
	int GetSEEnemyMove() const{return m_iSEEnemyMove;}
	int GetSEIzikeMove() const { return m_iSEIzikeMove; }
	int GetSEEnemyEat() const { return m_iSEEnemyEat; }
	int GetSEPlayerEat() const { return m_iSEPlayerEat; }
	int GetSECoffeebreak() const { return m_iSECoffeebreak; }
	int GetSEFruitEat() const { return m_iSEFruitEat; }
};
#endif



