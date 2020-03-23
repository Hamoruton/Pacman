#include "Score.h"
#include "DxLib.h"
#include "Scene.h"
#include <fstream>
#include <iostream>
#include <string>

//=============================================================
// �R���X�g���N�^
//=============================================================
CScore::CScore()
	:m_iHighScore(FileGetHighScore()), m_iScore(0), m_iLevel(1), m_iRemainCounter(2)
{
	//�摜�̓ǂݍ���
	LoadDivGraph(SCORE_REMAIN_IMAGE, 12, 3, 4, 16, 16, m_iGHRemain);
	LoadDivGraph(SCORE_FRUIT_IMAGE, SCORE_FRUIT_IMAGE_NUM, 32, 16, 16, 16, m_iGHFruit);

	//�t�H���g�̓ǂݍ���
	m_iFontHandle = CreateFontToHandle(GAME_FONT_CHIP, 10, 3);
}

//=============================================================
// �f�X�g���N�^
//=============================================================
CScore::~CScore() 
{
	//�摜�̔j��
	for (int i = 0; i < SCORE_REMAIN_IMAGE_NUM; i++)
	{
		DeleteGraph(m_iGHRemain[i]);
	}
	for (int i = 0; i < SCORE_FRUIT_IMAGE_NUM; i++) 
	{
		DeleteGraph(m_iGHFruit[i]);
	}

	//�t�H���g�̔j��
	DeleteFontToHandle(m_iFontHandle);
}

//=============================================================
// �X�R�A���Z�b�g
//=============================================================
void CScore::ScoreReset() 
{
	//���Z�b�g
	SetScore(0);
	SetLevel(1);
	SetRemainCounter(2);
}

//=============================================================
// �n�C�X�R�A�擾
//=============================================================
int CScore::FileGetHighScore() 
{
	//�t�@�C���̓ǂݍ���
	std::ifstream output_file_name("HighScore.txt");

	if (output_file_name.fail())
	{
		std::cout << "�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B" << std::endl;
	}

	int  high_score;
	output_file_name >> high_score;

	//�ő�l�`�F�b�N
	if (high_score > 99999)
	{
		high_score = 99999;
	}

	return high_score;
}

//=============================================================
// �n�C�X�R�A���Z�b�g
//=============================================================
void CScore::HighScoreReset() 
{
	//�t�@�C���̏�������
	std::ofstream input_file_name("HighScore.txt");

	if (input_file_name.fail())
	{
		std::cout << "�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B" << std::endl;
	}

	//�n�C�X�R�A���Z�b�g
	int high_score = 0;
	input_file_name << high_score;

	SetHighScore(0);

	//�t�@�C�������
	input_file_name.close();
}

//=============================================================
// �n�C�X�R�A�X�V
//=============================================================
void CScore::HighScoreUpdate() 
{
	//�t�@�C���̓ǂݍ���
	std::ifstream output_file_name("HighScore.txt");

	if (output_file_name.fail())
	{
		std::cout << "�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B" << std::endl;
	}

	int  high_score;
	output_file_name >> high_score;

	//�ő�l�`�F�b�N
	if (GetScore() > 99999)
	{
		SetScore(99999);
	}

	//�n�C�X�R�A�X�V
	if (GetScore() > high_score) 
	{
		high_score = GetScore();
	}

	//�t�@�C�������
	output_file_name.close();

	//�t�@�C���̏�������
	std::ofstream input_file_name("HighScore.txt");

	if (input_file_name.fail())
	{
		std::cout << "�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B" << std::endl;
	}

	input_file_name << high_score;

	SetHighScore(high_score);

	//�t�@�C�������
	input_file_name.close();
}

//=============================================================
// �`��
//=============================================================
void CScore::Draw() const 
{
	//�e�L�X�g
	Text();

	//�v���C���[�̎c�@
	RemainPlayer();

	//�����t���[�c�̎��
	OutbreakFruit();
}

//=============================================================
// �e�L�X�g
//=============================================================
void CScore::Text() const 
{
	//�n�C�X�R�A
	DrawStringToHandle(10, 10, "HIGH SCORE", GetColor(255, 255, 255), m_iFontHandle);
	DrawFormatStringToHandle(30, 50, GetColor(255, 255, 255), m_iFontHandle, "%05d", m_iHighScore);

	//�X�R�A
	DrawStringToHandle(10, 110, "SCORE", GetColor(255, 255, 255), m_iFontHandle);
	DrawFormatStringToHandle(30, 150, GetColor(255, 255, 255), m_iFontHandle, "%05d", m_iScore);

	//���x��
	DrawStringToHandle(10, 210, "LEVEL", GetColor(255, 255, 255), m_iFontHandle);
	DrawFormatStringToHandle(30, 250, GetColor(255, 255, 255), m_iFontHandle, "%05d", m_iLevel);

	//�|�[�Y
	DrawBox(70, 435, 90, 455, GetColor(255, 255, 255), FALSE);
	DrawStringToHandle(75, 440, "P", GetColor(255, 255, 255), m_iFontHandle);
	DrawStringToHandle(10, 440, "PAUSE", GetColor(255, 255, 255), m_iFontHandle);
}

//=============================================================
// �v���C���[�̎c�@
//=============================================================
void CScore::RemainPlayer() const 
{
	int current_remain = m_iRemainCounter;

	for (int i = 0; i < current_remain; i++)
	{
		DrawRotaGraph3(530 + i * SCORE_REMAIN_INTERVAL, 420, 0, 0, 1.2, 1.2, 0, m_iGHRemain[4], TRUE);
	}
}

//=============================================================
// �����t���[�c�̎��
//=============================================================
void CScore::OutbreakFruit() const 
{
	DrawRotaGraph3(530 , 380, 0, 0, 1.2, 1.2, 0, m_iGHFruit[0], TRUE);
	DrawRotaGraph3(555, 380, 0, 0, 1.2, 1.2, 0, m_iGHFruit[1], TRUE);
}