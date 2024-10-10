//============================================
//
//	ボワボワの処理 [SampleObjX.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _BOWABOWA_H_
#define _BOWABOWA_H_

#include "main.h"
#include "objectX.h"

//前方宣言
class CObjGauge2D;

//オブジェクトプレイヤークラス
class CBowabowa : public CObjectX
{
public:

	CBowabowa(int nPriority = 3);
	~CBowabowa();

	//サンプルの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_ACTION,		//行動
		STATE_MAX,
	};

	static CBowabowa* Create(char* pModelName);

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

private:
	void StateManager(void);		//状態管理

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
};
#endif