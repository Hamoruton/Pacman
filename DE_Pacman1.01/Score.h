#pragma once
#ifndef __SCORE_H_INCLUDED__
#define __SCORE_H_INCLUDED__

#define GAME_FONT_CHIP  "�A�^���t�H���g���S��"

//�摜���
#define SCORE_REMAIN_IMAGE		"Assets/Sprites/pacman.png"
#define SCORE_REMAIN_IMAGE_NUM	 12
#define SCORE_REMAIN_INTERVAL    25

#define SCORE_FRUIT_IMAGE		"Assets/Sprites/fruits.png"
#define SCORE_FRUIT_IMAGE_NUM	 2

//=============================================================
// �X�R�A�N���X
//=============================================================
class CScore
{
private:
	//�O���t�B�b�N�n���h��
	int m_iGHRemain[SCORE_REMAIN_IMAGE_NUM];
	int m_iGHFruit[SCORE_FRUIT_IMAGE_NUM];

	//�t�H���g�n���h��
	int m_iFontHandle;

	//�X�R�A
	int m_iHighScore;
	int m_iScore;
	int m_iLevel;

	//�v���C���[�c�@
	int m_iRemainCounter;

public:
	//�R���X�g���N�^.�f�X�g���N�^
	CScore();
	~CScore();

	//�Z�b�^�[
	void SetScore(int score) { m_iScore = score; }
	void SetLevel(int level) { m_iLevel = level; }
	void SetRemainCounter(int count) { m_iRemainCounter = count; }
	void SetHighScore(int score) { m_iHighScore = score; }

	//�Q�b�^�[
	int GetScore() const { return m_iScore; }
	int GetLevel() const { return m_iLevel; }
	int GetRemainCounter() const { return m_iRemainCounter; }
	int FileGetHighScore();
	int GetHighScore() const { return m_iHighScore; }

	//����
	void ScoreReset();			//�X�R�A���Z�b�g
	void HighScoreReset();		//�n�C�X�R�A���Z�b�g
	void HighScoreUpdate();		//�n�C�X�R�A�X�V

	//�`��
	void Draw() const;
	void Text() const;						//�e�L�X�g
	void RemainPlayer() const;				//�v���C���[�̎c�@
	void OutbreakFruit() const;				//�����t���[�c�̎��
};

#endif