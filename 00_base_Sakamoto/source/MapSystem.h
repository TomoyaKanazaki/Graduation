//========================================================================================
//
// マップシステム[MapSystem.h]
// Author: 坂本　翔唯
//
//========================================================================================
#ifndef _MAPSYSTEM_H_
#define _MAPSYSTEM_H_

#include "main.h"
#include "manager.h"
#include "AStar.h"

class CObject2D;
class CMapMove;

//マクロ定義
#define NUM_WIGHT (23)
#define NUM_HEIGHT (15)

//フェードのクラス宣言
class CMapSystem
{
public:

	// マップのオブジェクトの種類
	//enum MAPTYPE
	//{
	//	MAPTYPE_NONE = 0,		// 何でもない(床)
	//	MAPTYPE_WALL,			// 壁
	//	MAPTYPE_CROSS,			// 十字架
	//	MAPTYPE_DEVILHOLLRANGE,	// デビルホールの生成範囲
	//	MAPTYPE_DEVILHOLL,
	//	MAPTYPE_MAX
	//};

	// 特殊な座標系を管理する構造体
	struct GRID
	{
		// コンストラクタ
		GRID() : x(0), z(0) {};
		GRID(int X, int Z) { x = X; z = Z; };

		// デストラクタ
		~GRID() {};

		// 計算関数
		D3DXVECTOR3 ToWorld();
		AStar::Vec2i ToAStar();
		GRID ToGrid(const D3DXVECTOR3& pos);

		int x;
		int z;

		// オペレーター
		GRID& operator = (const AStar::Vec2i& vec); // A*ライブラリとの互換用
		bool operator == (const GRID& grid); // グリッド同士の比較
		bool operator != (const GRID& grid); // グリッド同士の比較
	};

	CMapSystem();
	~CMapSystem();

	static CMapSystem* GetInstance();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	// グリット位置の取得
	D3DXVECTOR3 GetStartGritPos(float Wight, float Height);
	D3DXVECTOR3 GetGritPos(const GRID& grid);
	int CalcGridX(const float posX);
	int CalcGridZ(const float posZ);
	GRID CalcGrid(const D3DXVECTOR3& pos);
	GRID GetCenter() { return m_mapCenter; };

	void SetGritBool(int nWight, int nHeight, bool Set);
	void SetGritBool(const GRID& grid, bool Set);
	bool GetGritBool(int nWight, int nHeight);
	bool GetGritBool(const GRID& grid);

	void SetRailGritBool(const GRID& grid, bool Set);
	bool GetRailGritBool(const GRID& grid);

	CMapMove* GetMove(void) { return m_pMapMove; }
	void SetMapPos(D3DXVECTOR3 pos) { m_MapPos = pos; }
	D3DXVECTOR3 GetMapPos(void) { return m_MapPos; }
	D3DXVECTOR3 GetInitPos(void) { return m_InitPos; }
	D3DXVECTOR3 GetPlayerPos(unsigned int PlayNumber);			// プレイヤーの位置取得
	GRID GetPlayerGrid(unsigned int PlayNumber);			// プレイヤーの位置取得

	int GetWightMax(void) { return m_WightMax; }
	int GetHeightMax(void) { return m_HeightMax; }

	void SetMapSize(D3DXVECTOR3 size) { m_MapSize = size; }
	D3DXVECTOR3 GetMapSize(void) { return m_MapSize; }
	GRID GetMapGrid(void) { return m_MapGrid; }

	static void Load(const std::string pFilename); // マップ情報の読み込み
	static void MapDelete(); // マップの削除
	static float GetGritSize();

private:

	CMapMove* m_pMapMove;

	static CMapSystem* m_pMapSystem;
	static std::vector<std::tuple<>> m_nData;	// 複数の値を保持
	static std::vector<GRID> m_PosPlayer;		// プレイヤーの位置を保持

	// TODO : Load関数で動的確保する（ｻﾄﾈｼｵﾝ）
	static bool m_bMapGrit[NUM_WIGHT][NUM_HEIGHT];	//グリットにブロックが存在していたらtrue
	static bool m_bMapRailGrit[NUM_WIGHT][NUM_HEIGHT];	//グリットにレールが存在していたらtrue

	GRID m_mapCenter;
	D3DXVECTOR3 m_InitPos;
	D3DXVECTOR3 m_MapPos;
	int m_WightMax;
	int m_HeightMax;
	float m_fGritSize;
	D3DXVECTOR3 m_MapSize;		//マップの境界線の大きさ
	GRID m_MapGrid;		//マップの境界線の大きさ

	D3DXMATRIX* m_mtxStage;

	//MAPTYPE m_MapType;			// マップオブジェクトの種類

};

#endif
