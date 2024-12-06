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
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 pos) { m_move = pos; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	// マップ番号の設定
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	// 静的メンバ関数
	static CListManager<CRollRock>* GetList(void); // リスト取得

private:
	void Move(void);
	void CollisionWall(useful::COLLISION XYZ);		//壁との当たり判定
	void CollisionOut();							//ステージ外との当たり判定
	void Coodinate();

	CMapSystem::GRID m_Grid;	//グリット番号
	CMapSystem::GRID m_OldGrid;	//グリット番号
	int m_nIdxXModel;				//Xモデルの番号
	D3DXVECTOR3 m_CollisionPos;		//当たり判定用の座標
	bool m_bCollision;				//当たり判定用の座標
	int m_nIdxTexture;				//テクスチャの番号
	int m_nStateCount;				//状態管理用変数
	float m_Scaling;				//大きさ
	D3DXVECTOR3 m_pos;				//位置	
	D3DXVECTOR3 m_posOld;			//過去の位置	
	D3DXVECTOR3 m_move;				//移動量	
	D3DXVECTOR3 m_rot;				//向き	
	float m_fColorA;				//不透明度

	bool m_OKL;					//左への進行が許されるかどうか
	bool m_OKR;					//右への進行が許されるかどうか
	bool m_OKU;					//上への進行が許されるかどうか
	bool m_OKD;					//下への進行が許されるかどうか

	CEffekseer* m_pEffect; // エフェクト

		// 静的メンバ変数
	static CListManager<CRollRock>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CRollRock>::AIterator m_iterator; // イテレーター
};
#endif