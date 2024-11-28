//============================================
//
//	プレイヤーのレベルエフェクト処理 [LevelModelEffect.h]
//	Author:sakamoto kai
//
//============================================

#ifndef _LEVEL_MODEL_EFFECT_H_
#define _LEVEL_MODEL_EFFECT_H_

#include "objectcharacter.h"

// プレイヤーのレベルエフェクト処理クラス
class CLevelModelEffect : public CObjectCharacter
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

	static CLevelModelEffect* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetColor(D3DCOLORVALUE Color) { m_Color = Color; }
	void SetDel(float fDel) { m_fDel = fDel; }
	void SetAllPose(int nType, int nKey, float nCounter);
	void SetPose(int nType, int nKey, float nCounter, int nModelNumber);

	// 静的メンバ関数
	static CListManager<CLevelModelEffect>* GetList(void); // リスト取得

private:
	ACTION_TYPE m_Action;		//モーション
	D3DCOLORVALUE m_Color;		//色
	float m_fDel;				//色の減衰

	// 静的メンバ変数
	static CListManager<CLevelModelEffect>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CLevelModelEffect>::AIterator m_iterator; // イテレーター
};
#endif