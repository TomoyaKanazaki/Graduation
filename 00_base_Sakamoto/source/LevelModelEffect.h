//============================================
//
//	プレイヤーのレベルエフェクト処理 [LevelModelEffect.h]
//	Author:sakamoto kai
//
//============================================

#ifndef _LEVEL_MODEL_EFFECT_H_
#define _LEVEL_MODEL_EFFECT_H_

#include "character.h"

// プレイヤーのレベルエフェクト処理クラス
class CLevelModelEffect : public CCharacter
{
public:
	CLevelModelEffect(int nPriority = 5);
	~CLevelModelEffect();

	//プレイヤーのモーション
	typedef enum
	{
		ACTION_WAIT = 0,
		ACTION_MOVE,
		ACTION_ATTACK,
		ACTION_JAMP,
		ACTION_MAX,

	}ACTION_TYPE;

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CLevelModelEffect* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetColor(D3DCOLORVALUE Color) { m_Color = Color; }
	void SetDel(float fDel) { m_fDel = fDel; }
	void SetAllPose(int nType, int nKey, float nCounter);
	void SetPose(int nType, int nKey, float nCounter, int nModelNumber);

	// 静的メンバ関数
	static CListManager<CLevelModelEffect>* GetList(void); // リスト取得

private:
	ACTION_TYPE m_Action;		//モーション
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_rot;			//向き
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DCOLORVALUE m_Color;		//色
	float m_fDel;				//色の減衰

	// 静的メンバ変数
	static CListManager<CLevelModelEffect>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CLevelModelEffect>::AIterator m_iterator; // イテレーター
};
#endif