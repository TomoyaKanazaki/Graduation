//============================================
//
//	弓兵の矢 [BowmanArrow.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _BOWMANARROW_H_
#define _BOWMANARROW_H_

#include "main.h"
#include "objectX.h"
#include "useful.h"

//前方宣言
class CObjGauge2D;

//オブジェクトプレイヤークラス
class CBowmanArrow : public CObjectX
{
public:

	CBowmanArrow(int nPriority = 3);
	~CBowmanArrow();

	//サンプルの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_ACTION,		//行動
		STATE_MAX,
	};

	static CBowmanArrow* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);
	void CollisionPlayer(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

private:
	void CollisionMapObject(D3DXVECTOR3 Mypos, D3DXVECTOR3 MyposOld, useful::COLLISION XYZ);	//壁との当たり判定
	void StateManager(void);	//状態管理

	int m_nIdxXModel;				//Xモデルの番号
	D3DXVECTOR3 m_CollisionPos;		//当たり判定用の座標
	bool m_bCollision;				//当たり判定用の座標
	int m_nIdxTexture;				//テクスチャの番号
	STATE m_State;					//状態
	int m_nStateCount;				//状態管理用変数
	float m_Scaling;				//大きさ
	D3DXVECTOR3 m_pos;				//位置	
	D3DXVECTOR3 m_posOld;			//過去の位置	
	D3DXVECTOR3 m_move;				//移動量	
	D3DXVECTOR3 m_rot;				//向き	
	float m_fColorA;				//不透明度
	float m_fLife;					//寿命
	bool m_bStop;					//ストップ
};
#endif