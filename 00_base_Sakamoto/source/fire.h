//============================================
//
//	オブジェクトＸモデルのサンプル [SampleObjX.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _FIRE_H_
#define _FIRE_H_

#include "main.h"
#include "objectX.h"
#include "MapSystem.h"

//オブジェクトファイアクラス
class CFire : public CObjectX
{
public:

	CFire(int nPriority = 3);
	~CFire();

	static CFire* Create(char* pModelName, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CollisionEnemy();
	void CollisionWall();

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	// 静的メンバ関数
	static CListManager<CFire>* GetList(void); // リスト取得

private:

	// 静的メンバ変数
	static CListManager<CFire>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CFire>::AIterator m_iterator; // イテレーター
	int m_nLife;					// 体力
	float m_Scaling;				// 大きさ
	float m_fColorA;				// 不透明度
	D3DXVECTOR3 m_move;				// 移動量	
	CMapSystem::GRID m_Grid;		// マップ番号
	CEffekseer* m_pEffect;			// エフェクト

};
#endif