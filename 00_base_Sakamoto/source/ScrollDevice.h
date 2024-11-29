//============================================
//
//	マップ移動装置の処理 [ScrollDevice.h]
//	Author:sakai minato
//
//============================================
#ifndef _SCROLL_DEVICE_H_
#define _SCROLL_DEVICE_H_

#include "objectcharacter.h"

// マップ移動装置クラス
class CScrollDevice : public CObjectCharacter
{
public:

	CScrollDevice(int nPriority = 3);
	~CScrollDevice();

	// 状態
	enum STATE
	{
		STATE_NORMAL = 0,	// 通常
		STATE_ROTATE,		// 回転
		STATE_MAX,
	};

	// モーション
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,	// 待機状態
		ACTION_MAX,			// 最大
	};

	// モーション
	enum SETUP_TYPE
	{
		SETUP_TYPE_FOUNDATION = 0,	// 土台
		SETUP_TYPE_MAWASIGURMA,		// 回し車
		SETUP_TYPE_ROLLRE,			// ローラー
		SETUP_TYPE_HEAD,			// 先っぽ
		SETUP_TYPE_MAX,				// 最大
	};

	static CScrollDevice* Create(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(STATE state) { m_State = state; }

	// 静的メンバ関数
	static CListManager<CScrollDevice>* GetList(void); // リスト取得

private:
	HRESULT InitModel(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	void StateManager(D3DXVECTOR3& rotMy);
	void Rotate(D3DXVECTOR3& rotMy,int nNldxModel, D3DXVECTOR3 rotate);

	STATE m_State;					//状態
	int m_nStateCount;				//状態管理用変数

	// 静的メンバ変数
	static CListManager<CScrollDevice>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CScrollDevice>::AIterator m_iterator; // イテレーター

};
#endif