#pragma once
#ifndef __MAZE_H_INCLUDED__
#define __MAZE_H_INCLUDED__

//配列
#define MAZE_WIDTH  20
#define MAZE_HEIGHT 22

//描画の調整
#define MAZE_DRAW_X	   130
#define MAZE_DRAW_Y		20
#define MAZE_INTERVAL	20

//=============================================================
// メイズクラス
//=============================================================
class CMaze
{
public:
	//メイズタイルの情報
	enum MAZETILE
	{
		TILE_INDENTION = ' ',
		TILE_NONE = '-',
		TILE_WALL = '#',
		TILE_DOT = 'o',
		TILE_POWER_DOT = '0',
		TILE_MAX,
	};
	MAZETILE m_eMazeTile;

private:
	//メイズ情報
	int m_iMaze[MAZE_HEIGHT][MAZE_WIDTH] = { 0 };
	int* m_pMaze;

	//クリア時のステージ点滅
	bool m_bFlash;
	//点滅タイマー
	int m_iFlashTimer;

public:
	//コンストラクタ.デストラクタ
	CMaze();
	~CMaze();

	//セッター
	void SetFlash(bool flash) { m_bFlash = flash; }
	void SetFlashTimer(int time) { m_iFlashTimer = time; }

	//ゲッター
	int* GetMaze() const { return m_pMaze; }

	//動作
	void Move();
	void InitMaze();	//メイズ初期化

	//描画
	void Draw() const;
};

#endif

