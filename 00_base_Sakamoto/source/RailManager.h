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
#include "Rail.h"

//前方宣言
class CObjGauge2D;

//レールマネージャークラス
class CRailManager
{
public:

	CRailManager();
	~CRailManager();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(CMapSystem::GRID& grid);		// レールの設定

private:
	// メンバ関数
	void SetRot(CMapSystem::GRID& grid, CRail::POSTYPE& PosType0, CRail::POSTYPE& PosType1);		// モデルの向きの設定

	// メンバ関数
	bool m_bRail[CRail::POSTYPE_MAX];					// レールの置ける位置の判定


	// 静的メンバ変数
	static CListManager<CRailManager>* m_pList;			// オブジェクトリスト
	static std::vector<CMapSystem::GRID> m_GridPos;		// レールの位置を保持

};
#endif