//============================================
//
//	レールマネージャー [RailManager.h]
//	Author:Satone Shion
//
//============================================
#ifndef _RAILMANAGER_H_
#define _RAILMANAGER_H_

#include "main.h"
#include "MapSystem.h"

//前方宣言
class CObjGauge2D;

//レールマネージャークラス
class CRailManager
{
public:

	CRailManager();
	~CRailManager();

	HRESULT Init(CMapSystem::GRID grid);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

	// 静的メンバ変数
	static CListManager<CRailManager>* m_pList;			// オブジェクトリスト
	static std::vector<CMapSystem::GRID> m_GridPos;		// レールの位置を保持

};
#endif