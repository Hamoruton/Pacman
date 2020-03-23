#pragma once
#ifndef __SOUND_H_INCLUDED__
#define __SOUND_H_INCLUDED__

#define SOUND_CURSOR		"Assets/Sounds/01 �N���W�b�g�T�E���h.m4a"
#define SOUND_GAME_START	"Assets/Sounds/02 �X�^�[�g�~���[�W�b�N.m4a"
#define SOUND_DOT_EAT		"Assets/Sounds/03 �p�b�N�}��_�N�b�L�[��H�ׂ�.m4a"
#define SOUND_ENEMY_MOVE	"Assets/Sounds/05 �S�[�X�g_����ړ�.m4a"
#define SOUND_FRUIT_EAT		"Assets/Sounds/10 �p�b�N�}��_�t���[�c��H�ׂ�.m4a"
#define SOUND_IZIKE_MOVE	"Assets/Sounds/11 �S�[�X�g_������.m4a"
#define SOUND_ENEMY_EAT		"Assets/Sounds/12 �p�b�N�}��_�S�[�X�g��H�ׂ�.m4a"
#define SOUND_PLAYER_EAT	"Assets/Sounds/14 �~�X.m4a"
#define SOUND_COFFEEBREAK	"Assets/Sounds/15 �R�[�q�[�u���C�N�~���[�W�b�N.m4a"

//=============================================================
// �T�E���h�N���X
//=============================================================
class CSound
{
private:
	//�T�E���h�n���h��
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
	//�R���X�g���N�^.�f�X�g���N�^
	CSound();
	~CSound();

	//�Q�b�^�[
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



