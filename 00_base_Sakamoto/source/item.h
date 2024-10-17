//============================================
//
//	アイテムの処理 [item.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"
#include "objectX.h"

//オブジェクトアイテムクラス
class CItem : public CObjectX
{
public:

	CItem(int nPriority = 3);
	~CItem();

	//サンプルの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_ACTION,		//行動
		STATE_MAX,
	};
	
	enum TYPE
	{// アイテムの種類
		TYPE_NONE = 0,
		TYPE_CROSS,		// 十字架
		TYPE_BIBLE,		// 聖書
		TYPE_BOWABOWA,	// ぼわぼわ
		TYPE_MAX
	};

	static CItem* Create(TYPE eType, const D3DXVECTOR3& pos);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	virtual bool CollisionPlayer();

	// テクスチャ番号取得
	int GetIdx(void) { return m_nIdxTexture; }

	// Xモデル番号取得
	int GetIdxXModel(void) { return -1; }

	//位置設定・取得
	void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	//位置設定・取得
	void SetPosold(const D3DXVECTOR3& posold) { m_posOld = posold; }
	D3DXVECTOR3 GetPosold(void) { return m_posOld; }

	//移動量設定・取得
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	//向き設定・取得
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	//大きさ設定・取得
	void SetScaling(const float& Scaling) { m_Scaling = Scaling; }
	float GetScaling(void) { return m_Scaling; }

	// 状態取得
	STATE GetState() { return m_eState; }

	// 状態管理の取得
	int GetStateCounter() { return m_nStateCount; }

private:
	STATE m_eState;		// 状態
	TYPE m_eType;		// 種類

	int m_nIdxXModel;		//Xモデルの番号
	int m_nIdxTexture;		//テクスチャの番号
	int m_nStateCount;		//状態管理用変数

	float m_fColorA;		//不透明度
	float m_Scaling;		//大きさ

	bool m_UseItem;			//アイテムが使用可能かどうか
	bool m_bCollision;		//当たり判定用の座標

	D3DXVECTOR3 m_CollisionPos;	//当たり判定用の座標
	D3DXVECTOR3 m_pos;			//位置	
	D3DXVECTOR3 m_posOld;		//過去の位置	
	D3DXVECTOR3 m_move;			//移動量	
	D3DXVECTOR3 m_rot;			//向き	
};

#endif