//============================================
//
//	マップモデル [MapModel.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MAPMODEL_H_
#define _MAPMODEL_H_

#include "main.h"
#include "objectX.h"

//前方宣言
class CObjGauge2D;
class CCubeEffect;

//==========================
// 定数定義
//==========================
namespace mapmodel_const
{
	const int SCREENPOS_NUM = 8;	//スクリーン座標用座標数
}

//==============================
//オブジェクトプレイヤークラス
//==============================
class CMapModel : public CObjectX
{
public:

	CMapModel(int nPriority = 3);
	~CMapModel();

	//サンプルの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_ACTION,		//行動
		STATE_MAX,
	};

	static CMapModel* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);
	void SetCollision(bool Set) { m_bCollision = Set; }
	bool GetCollision(void) { return m_bCollision; }
	void CreateBlock(void);		//デバッグ用ブロックの生成
	void UpdateBlock(void);		//デバッグ用ブロックの更新
	void SetEditIdx(int Idx) { m_EditIndex = Idx; }
	int GetEditIdx(void) { return m_EditIndex; }
	D3DXVECTOR3* GetScreenPos(void) { return &m_ScreenPos[0]; }
	void SetCollisionSize(D3DXVECTOR3 size) { collisionSize = size; }
	D3DXVECTOR3 GetCollisionSize() { return collisionSize; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove() { return m_move; }
	void SetSpin(D3DXVECTOR3 spin) { m_Spin = spin; }
	D3DXVECTOR3 GetSpin() { return m_Spin; }
	void SetDebugBlock(CCubeEffect* pCubeEffect) { m_pDebugBlock = pCubeEffect; }
	CCubeEffect*GetDebugBlock() { return m_pDebugBlock; }
	void SetBreakable(bool set) { m_bBreakable = set; }
	bool GetBreakable() { return m_bBreakable; }
	void SetDebris(bool set) { m_bDebris = set; }
	bool GetDebris() { return m_bDebris; }
	void SetBreakEffect(bool set) { m_bBreakEffect = set; }
	bool GetBreakEffect() { return m_bBreakEffect; }

	virtual void Break(D3DXVECTOR3 *pos);	//攻撃によって壊されたとき
	void ScreenCollision();

private:
	void StateManager(void);	//状態管理

	D3DXVECTOR3 m_CollisionPos;		//当たり判定用の座標
	bool m_bCollision;				//当たり判定があるかどうか
	STATE m_State;					//状態
	int m_nStateCount;				//状態管理用変数
	float m_Scaling;				//大きさ
	D3DXVECTOR3 m_posOld;			//過去の位置	
	D3DXVECTOR3 m_move;				//移動量	
	D3DXVECTOR3 m_Spin;				//回転量	
	float m_fColorA;				//不透明度
	int m_EditIndex;				//エディット時のインデックス
	D3DXVECTOR3 m_ScreenPos[mapmodel_const::SCREENPOS_NUM];	//スクリーン座標
	D3DXVECTOR3 collisionSize;					//矩形の当たり判定サイズ
	bool m_bBreakable;		//壊れる存在か
	bool m_bDebris;			//破片が出るかどうか
	bool m_bBreakEffect;	//破壊エフェクトの状態かどうか
	int m_bBreakBound;		//破壊エフェクト状態でバウンドした回数

	CCubeEffect* m_pDebugBlock;		//デバッグ用ブロック
};
#endif