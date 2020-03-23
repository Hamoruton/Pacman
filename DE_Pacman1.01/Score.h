#pragma once
#ifndef __SCORE_H_INCLUDED__
#define __SCORE_H_INCLUDED__

#define GAME_FONT_CHIP  "アタリフォント完全版"

//画像情報
#define SCORE_REMAIN_IMAGE		"Assets/Sprites/pacman.png"
#define SCORE_REMAIN_IMAGE_NUM	 12
#define SCORE_REMAIN_INTERVAL    25

#define SCORE_FRUIT_IMAGE		"Assets/Sprites/fruits.png"
#define SCORE_FRUIT_IMAGE_NUM	 2

//=============================================================
// スコアクラス
//=============================================================
class CScore
{
private:
	//グラフィックハンドル
	int m_iGHRemain[SCORE_REMAIN_IMAGE_NUM];
	int m_iGHFruit[SCORE_FRUIT_IMAGE_NUM];

	//フォントハンドル
	int m_iFontHandle;

	//スコア
	int m_iHighScore;
	int m_iScore;
	int m_iLevel;

	//プレイヤー残機
	int m_iRemainCounter;

public:
	//コンストラクタ.デストラクタ
	CScore();
	~CScore();

	//セッター
	void SetScore(int score) { m_iScore = score; }
	void SetLevel(int level) { m_iLevel = level; }
	void SetRemainCounter(int count) { m_iRemainCounter = count; }
	void SetHighScore(int score) { m_iHighScore = score; }

	//ゲッター
	int GetScore() const { return m_iScore; }
	int GetLevel() const { return m_iLevel; }
	int GetRemainCounter() const { return m_iRemainCounter; }
	int FileGetHighScore();
	int GetHighScore() const { return m_iHighScore; }

	//動作
	void ScoreReset();			//スコアリセット
	void HighScoreReset();		//ハイスコアリセット
	void HighScoreUpdate();		//ハイスコア更新

	//描画
	void Draw() const;
	void Text() const;						//テキスト
	void RemainPlayer() const;				//プレイヤーの残機
	void OutbreakFruit() const;				//発生フルーツの種類
};

#endif