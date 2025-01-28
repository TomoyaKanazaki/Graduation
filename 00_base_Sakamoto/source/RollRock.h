//============================================
//
//	転がる岩の処理 [RollRock.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _ROLLROCK_H_
#define _ROLLROCK_H_

#include "main.h"
#include "objectX.h"
#include "MapSystem.h"

//前方宣言
class CObjGauge2D;
class CMoveState;

//転がる岩クラス
class CRollRock : public CObjectX
{
public:

	CRollRock(int nPriority = 3);
	CRollRock(int nPriority, CMapSystem::GRID gridCenter);
	~CRollRock();

	static CRollRock* Create(CMapSystem::GRID gridCenter);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Reset(void);		// リセット

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }

	// マップ番号の設定
	void SetGrid(const CMapSystem::GRID& pos) override { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) override { return m_Grid; }

	void ChangeMoveState(CMoveState* pMoveState) override;   // 移動状態変更

	// 静的メンバ関数
	static CListManager<CRollRock>* GetList(void); // リスト取得

private:
	void Move(D3DXVECTOR3& pos, D3DXVECTOR3& rot);
	void CollisionOut(D3DXVECTOR3& pos);							//ステージ外との当たり判定
	void CollisionPlayer(void);

	CMapSystem::GRID m_Grid;	//グリット番号
	CMapSystem::GRID m_OldGrid;	//グリット番号
	CMapSystem::GRID m_SaveGrid;//グリット番号(保存用)

	int m_nIdxXModel;				//Xモデルの番号
	D3DXVECTOR3 m_CollisionPos;		//当たり判定用の座標
	bool m_bCollision;				//当たり判定用の座標
	int m_nIdxTexture;				//テクスチャの番号
	int m_nStateCount;				//状態管理用変数
	float m_Scaling;				//大きさ
	float m_fColorA;				//不透明度

	CEffekseer* m_pEffect; // エフェクト
	CMoveState* m_pMoveState;		// 移動状態
	CShadow* m_pShadow;				// 影

	// 静的メンバ変数
	static CListManager<CRollRock>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CRollRock>::AIterator m_iterator; // イテレーター
};
#endif