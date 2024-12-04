#pragma once
//============================================
//
//  足跡クラス(footprint.h)
//  Author : Tomoya Kanazaki
//
//============================================
#include "object3D.h"
#include "MapSystem.h"

//==========================================
//  クラス定義
//==========================================
class CFootPrint : public CObject3D
{
public:

	CFootPrint(int nPriority = 7);
	~CFootPrint();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// 静的メンバ関数
	static CFootPrint* Create(CMapSystem::GRID& grid, const D3DXVECTOR3& rot);
	static CListManager<CFootPrint>* GetList(void);	// リスト取得

private:

	// メンバ関数
	void SetPos(const D3DXVECTOR3& pos) override;

	// 静的メンバ変数
	static CListManager<CFootPrint>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CFootPrint>::AIterator m_iterator; // イテレーター

};
