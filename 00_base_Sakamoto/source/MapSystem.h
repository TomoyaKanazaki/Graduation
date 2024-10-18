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

class CObject2D;

//マクロ定義
#define NUM_WIGHT (16)
#define NUM_HEIGHT (12)

//フェードのクラス宣言
class CMapSystem
{
public:
	CMapSystem();
	~CMapSystem();

	static CMapSystem* GetInstance();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetGritPos(int nWight, int nHeight) { m_nMapGrit[nWight][nHeight] = true; }

	D3DXVECTOR3 GetMapPos(void) { return m_MapPos; }
	int GetWightMax(void) { return m_WightMax; }
	int GetHeightMax(void) { return m_HeightMax; }
	float GetGritSize(void) { return m_fGritSize; }

protected:

public:
	static CMapSystem* pMapSystem;

	bool m_nMapGrit[NUM_WIGHT][NUM_HEIGHT];
	D3DXVECTOR3 m_MapPos;
	int m_WightMax;
	int m_HeightMax;
	float m_fGritSize;
};

#endif
