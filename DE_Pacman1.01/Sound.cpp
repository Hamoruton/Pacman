#include "Sound.h"
#include "DxLib.h"

//=============================================================
// �R���X�g���N�^
//=============================================================
CSound::CSound() 
{
	//�T�E���h�̓ǂݍ���
	m_iSECursor = LoadSoundMem(SOUND_CURSOR);
	m_iSEGameStart = LoadSoundMem(SOUND_GAME_START);
	m_iSEDotEat = LoadSoundMem(SOUND_DOT_EAT);
	m_iSEEnemyMove = LoadSoundMem(SOUND_ENEMY_MOVE);
	m_iSEIzikeMove = LoadSoundMem(SOUND_IZIKE_MOVE);
	m_iSEEnemyEat = LoadSoundMem(SOUND_ENEMY_EAT);
	m_iSEPlayerEat = LoadSoundMem(SOUND_PLAYER_EAT);
	m_iSECoffeebreak = LoadSoundMem(SOUND_COFFEEBREAK);
	m_iSEFruitEat = LoadSoundMem(SOUND_FRUIT_EAT);
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CSound::~CSound() 
{
	//�T�E���h�̔j��
	DeleteSoundMem(m_iSECursor);
	DeleteSoundMem(m_iSEGameStart);
	DeleteSoundMem(m_iSEDotEat);
	DeleteSoundMem(m_iSEEnemyMove);
	DeleteSoundMem(m_iSEIzikeMove);
	DeleteSoundMem(m_iSEEnemyEat);
	DeleteSoundMem(m_iSEPlayerEat);
	DeleteSoundMem(m_iSECoffeebreak);
	DeleteSoundMem(m_iSEFruitEat);
}