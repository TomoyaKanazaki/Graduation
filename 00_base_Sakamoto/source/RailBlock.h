//============================================
//
//	レールブロックの処理 [RailBlock.h]
//	Author:sakamoto kai
//  Author:Satone Shion
//
//============================================
#ifndef _RAILBLOCK_H_
#define _RAILBLOCK_H_

#include "main.h"
#include "ObjectX.h"
#include "MapSystem.h"

class CRail;
class CMoveState;		// 移動状態

//オブジェクトメッシュフィールドクラス
class CRailBlock : public CObjectX
{
public:
	CRailBlock(int nPriority = 3);
	CRailBlock(int nPriority, CMapSystem::GRID gridCenter);
	~CRailBlock();

	static CRailBlock* Create(CMapSystem::GRID gridCenter);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CMapSystem::GRID GetGrid(void) { return m_Grid; }	// グリッド取得

	void ChangeMoveState(CMoveState* pMoveState) override;   // 移動状態変更

	// 静的メンバ関数
	static CListManager<CRailBlock>* GetList(void); // リスト取得

private:

	void Move(D3DXVECTOR3 *Pos);
	void CollisionOut(D3DXVECTOR3& pos);							//ステージ外との当たり判定
	void Coodinate();

	CMapSystem::GRID m_Grid;	//グリット番号
	CMapSystem::GRID m_OldGrid;	//グリット番号
	CMoveState* m_pMoveState;	// 移動状態

	// 静的メンバ変数
	static CListManager<CRailBlock>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CRailBlock>::AIterator m_iterator; // イテレーター

};
#endif