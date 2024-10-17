//============================================
//
//	聖書の処理 [bible.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _BIBLE_H_
#define _BIBLE_H_

#include "main.h"
#include "item.h"

//前方宣言
class CObjGauge2D;

//オブジェクトプレイヤークラス
class CBible : public CItem
{
public:

	CBible(int nPriority = 3);
	~CBible();

	static CBible* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

private:
	int m_nIdxXModel;				//Xモデルの番号
	int m_nIdxTexture;				//テクスチャの番号
	int m_nStateCount;				//状態管理用変数

	float m_Scaling;				//大きさ
	float m_fColorA;				//不透明度

	bool m_bCollision;				//当たり判定用の座標

	D3DXVECTOR3 m_CollisionPos;		//当たり判定用の座標
	D3DXVECTOR3 m_pos;				//位置	
	D3DXVECTOR3 m_posOld;			//過去の位置	
	D3DXVECTOR3 m_move;				//移動量	
	D3DXVECTOR3 m_rot;				//向き	
};

#endif
