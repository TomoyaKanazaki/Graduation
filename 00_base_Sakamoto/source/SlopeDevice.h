//============================================
//
//	傾き装置の処理 [SlopeDevice.h]
//	Author:sakai minato
//
//============================================

#ifndef _SLOPE_DEVICE_H_
#define _SLOPE_DEVICE_H_

#include "objectcharacter.h"

// 傾き装置クラス
class CSlopeDevice : public CObjectCharacter
{
public:

	CSlopeDevice(int nPriority = 3);
	~CSlopeDevice();

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

	static CSlopeDevice* Create(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(STATE state) { m_State = state; }

	// 静的メンバ関数
	static CListManager<CSlopeDevice>* GetList(void); // リスト取得

private:
	HRESULT InitModel(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	void StateManager(D3DXVECTOR3& rotMy);

	STATE m_State;					//状態
	int m_nStateCount;				//状態管理用変数

	// 静的メンバ変数
	static CListManager<CSlopeDevice>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CSlopeDevice>::AIterator m_iterator; // イテレーター

};
#endif