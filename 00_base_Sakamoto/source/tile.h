//============================================
//
//	床モデルの処理 [tile.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TILE_H_
#define _TILE_H_

#include "main.h"
#include "objectX.h"
#include "MapSystem.h"

//床クラス
class CTile : public CObjectX
{
public:

	CTile(int nPriority = 3);
	~CTile();

	static CTile* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	/*int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }*/
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	// マップ番号の設定
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	// 静的メンバ関数
	static CListManager<CTile>* GetList(void); // リスト取得

private:

	CMapSystem::GRID m_Grid;	//グリット番号

	int m_nIdxXModel;				//Xモデルの番号
	//D3DXVECTOR3 m_CollisionPos;		//当たり判定用の座標
	//bool m_bCollision;				//当たり判定用の座標
	int m_nIdxTexture;				//テクスチャの番号
	D3DXVECTOR3 m_pos;				//位置	
	D3DXVECTOR3 m_posOld;			//過去の位置	
	D3DXVECTOR3 m_move;				//移動量

	//bool m_bSet[4];					//上下左右の穴が埋まっているかどうか
	//CObjectX *m_pHoleKey[4];			//上下左右の穴を埋めるポリゴン

	// 静的メンバ変数
	static CListManager<CTile>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CTile>::AIterator m_iterator; // イテレーター

};
#endif