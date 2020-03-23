#include "Score.h"
#include "DxLib.h"
#include "Scene.h"
#include <fstream>
#include <iostream>
#include <string>

//=============================================================
// コンストラクタ
//=============================================================
CScore::CScore()
	:m_iHighScore(FileGetHighScore()), m_iScore(0), m_iLevel(1), m_iRemainCounter(2)
{
	//画像の読み込み
	LoadDivGraph(SCORE_REMAIN_IMAGE, 12, 3, 4, 16, 16, m_iGHRemain);
	LoadDivGraph(SCORE_FRUIT_IMAGE, SCORE_FRUIT_IMAGE_NUM, 32, 16, 16, 16, m_iGHFruit);

	//フォントの読み込み
	m_iFontHandle = CreateFontToHandle(GAME_FONT_CHIP, 10, 3);
}

//=============================================================
// デストラクタ
//=============================================================
CScore::~CScore() 
{
	//画像の破棄
	for (int i = 0; i < SCORE_REMAIN_IMAGE_NUM; i++)
	{
		DeleteGraph(m_iGHRemain[i]);
	}
	for (int i = 0; i < SCORE_FRUIT_IMAGE_NUM; i++) 
	{
		DeleteGraph(m_iGHFruit[i]);
	}

	//フォントの破棄
	DeleteFontToHandle(m_iFontHandle);
}

//=============================================================
// スコアリセット
//=============================================================
void CScore::ScoreReset() 
{
	//リセット
	SetScore(0);
	SetLevel(1);
	SetRemainCounter(2);
}

//=============================================================
// ハイスコア取得
//=============================================================
int CScore::FileGetHighScore() 
{
	//ファイルの読み込み
	std::ifstream output_file_name("HighScore.txt");

	if (output_file_name.fail())
	{
		std::cout << "ファイルの読み込みに失敗しました。" << std::endl;
	}

	int  high_score;
	output_file_name >> high_score;

	//最大値チェック
	if (high_score > 99999)
	{
		high_score = 99999;
	}

	return high_score;
}

//=============================================================
// ハイスコアリセット
//=============================================================
void CScore::HighScoreReset() 
{
	//ファイルの書き込み
	std::ofstream input_file_name("HighScore.txt");

	if (input_file_name.fail())
	{
		std::cout << "ファイルの読み込みに失敗しました。" << std::endl;
	}

	//ハイスコアリセット
	int high_score = 0;
	input_file_name << high_score;

	SetHighScore(0);

	//ファイルを閉じる
	input_file_name.close();
}

//=============================================================
// ハイスコア更新
//=============================================================
void CScore::HighScoreUpdate() 
{
	//ファイルの読み込み
	std::ifstream output_file_name("HighScore.txt");

	if (output_file_name.fail())
	{
		std::cout << "ファイルの読み込みに失敗しました。" << std::endl;
	}

	int  high_score;
	output_file_name >> high_score;

	//最大値チェック
	if (GetScore() > 99999)
	{
		SetScore(99999);
	}

	//ハイスコア更新
	if (GetScore() > high_score) 
	{
		high_score = GetScore();
	}

	//ファイルを閉じる
	output_file_name.close();

	//ファイルの書き込み
	std::ofstream input_file_name("HighScore.txt");

	if (input_file_name.fail())
	{
		std::cout << "ファイルの読み込みに失敗しました。" << std::endl;
	}

	input_file_name << high_score;

	SetHighScore(high_score);

	//ファイルを閉じる
	input_file_name.close();
}

//=============================================================
// 描画
//=============================================================
void CScore::Draw() const 
{
	//テキスト
	Text();

	//プレイヤーの残機
	RemainPlayer();

	//発生フルーツの種類
	OutbreakFruit();
}

//=============================================================
// テキスト
//=============================================================
void CScore::Text() const 
{
	//ハイスコア
	DrawStringToHandle(10, 10, "HIGH SCORE", GetColor(255, 255, 255), m_iFontHandle);
	DrawFormatStringToHandle(30, 50, GetColor(255, 255, 255), m_iFontHandle, "%05d", m_iHighScore);

	//スコア
	DrawStringToHandle(10, 110, "SCORE", GetColor(255, 255, 255), m_iFontHandle);
	DrawFormatStringToHandle(30, 150, GetColor(255, 255, 255), m_iFontHandle, "%05d", m_iScore);

	//レベル
	DrawStringToHandle(10, 210, "LEVEL", GetColor(255, 255, 255), m_iFontHandle);
	DrawFormatStringToHandle(30, 250, GetColor(255, 255, 255), m_iFontHandle, "%05d", m_iLevel);

	//ポーズ
	DrawBox(70, 435, 90, 455, GetColor(255, 255, 255), FALSE);
	DrawStringToHandle(75, 440, "P", GetColor(255, 255, 255), m_iFontHandle);
	DrawStringToHandle(10, 440, "PAUSE", GetColor(255, 255, 255), m_iFontHandle);
}

//=============================================================
// プレイヤーの残機
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
// 発生フルーツの種類
//=============================================================
void CScore::OutbreakFruit() const 
{
	DrawRotaGraph3(530 , 380, 0, 0, 1.2, 1.2, 0, m_iGHFruit[0], TRUE);
	DrawRotaGraph3(555, 380, 0, 0, 1.2, 1.2, 0, m_iGHFruit[1], TRUE);
}