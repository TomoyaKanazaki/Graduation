//============================================
//
//	デビルホールの処理 [DevilHole.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _DEVILHOLE_H_
#define _DEVILHOLE_H_

#include "main.h"
#include "objectX.h"
#include "MapSystem.h"

//前方宣言
class CObjGauge2D;

//オブジェクトプレイヤークラス
class CDevilHole : public CObjectX
{
public:

	CDevilHole(int nPriority = 3);
	CDevilHole(int nPriority, CMapSystem::GRID gridCenter);
	~CDevilHole();

	//サンプルの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_ACTION,		//行動
		STATE_MAX,
	};

	static CDevilHole* Create(CMapSystem::GRID gridCenter);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }

	// マップ番号の設定
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	// 静的メンバ関数
	static CListManager<CDevilHole>* GetList(void); // リスト取得

private:
	void StateManager(void);	//状態管理
	void CollisionOpen(D3DXVECTOR3& pos);	//解除判定
	void ClearJudge(void);		//クリア判定処理

	CMapSystem::GRID m_Grid;	//グリット番号

	int m_nIdxXModel;				//Xモデルの番号
	D3DXVECTOR3 m_CollisionPos;		//当たり判定用の座標
	bool m_bCollision;				//当たり判定用の座標
	int m_nIdxTexture;				//テクスチャの番号
	STATE m_State;					//状態
	int m_nStateCount;				//状態管理用変数
	float m_Scaling;				//大きさ
	float m_fColorA;				//不透明度

	bool m_bSet[4];					//上下左右の穴が埋まっているかどうか
	CObjectX *m_pHoleKey[4];			//上下左右の穴を埋めるポリゴン

	// 静的メンバ変数
	static CListManager<CDevilHole>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CDevilHole>::AIterator m_iterator; // イテレーター

};
#endif