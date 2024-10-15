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

//前方宣言
class CObjGauge2D;

//オブジェクトプレイヤークラス
class CFire : public CObjectX
{
public:

	CFire(int nPriority = 3);
	~CFire();

	//サンプルの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_ACTION,		//行動
		STATE_MAX,
	};

	static CFire* Create(char* pModelName, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);
	void CollisionEnemy();

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

private:
	void StateManager(void);		// 状態管理

	STATE m_State;					// 状態

	int m_nIdxXModel;				// Xモデルの番号
	int m_nIdxTexture;				// テクスチャの番号
	int m_nStateCount;				// 状態管理用変数
	int m_nLife;					// 体力

	float m_Scaling;				// 大きさ
	float m_fColorA;				// 不透明度

	bool m_bCollision;				// 当たり判定用の座標

	D3DXVECTOR3 m_CollisionPos;		// 当たり判定用の座標
	D3DXVECTOR3 m_pos;				// 位置	
	D3DXVECTOR3 m_posOld;			// 過去の位置	
	D3DXVECTOR3 m_move;				// 移動量	
	D3DXVECTOR3 m_rot;				// 向き	
};
#endif