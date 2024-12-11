//============================================
//
//	傾き装置の処理 [SlopeDevice.h]
//	Author:sakai minato
//
//============================================

#ifndef _SLOPE_DEVICE_H_
#define _SLOPE_DEVICE_H_

#include "objectcharacter.h"

#include "scrollarrow.h"

// 傾き装置クラス
class CSlopeDevice : public CObjectCharacter
{
public:

	CSlopeDevice(int nPriority = 3);
	~CSlopeDevice();

	// 世界からの設置場所
	enum LOCATE_WORLD_TYPE
	{
		LOCATE_WORLD_TYPE_TOP_LEFT = 0,		// 左上
		LOCATE_WORLD_TYPE_TOP_RIGHT,		// 右上
		LOCATE_WORLD_TYPE_BOTTOM_LEFT,		// 左下
		LOCATE_WORLD_TYPE_BOTTOM_RIGHT,		// 右下
		LOCATE_WORLD_TYPE_MAX
	};

	// 状態
	enum STATE
	{
		STATE_NORMAL = 0,	// 通常
		STATE_ASCENT,		// 上昇
		STATE_DESCENT,		// 下降
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
		SETUP_TYPE_EDGE,			// 外縁
		SETUP_TYPE_ELEVATING_PART,	// 昇降部位
		SETUP_TYPE_JACK,			// ジャッキ本体
		SETUP_TYPE_INJECTION_PART,	// 注入部位
		SETUP_TYPE_LIVER,			// レバー
		SETUP_TYPE_MAX,				// 最大
	};

	static CSlopeDevice* Create(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(STATE state) { m_State = state; }
	void SetStateArrow(CScrollArrow::Arrow stateArrow);
	void SetStateArrowBack(CScrollArrow::Arrow stateArrow);
	
	void SetLocateWorldType(LOCATE_WORLD_TYPE LocateWorldType) { m_LocateWorldType = LocateWorldType ;}

	// 静的メンバ関数
	static CListManager<CSlopeDevice>* GetList(void); // リスト取得

private:
	HRESULT InitModel(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	void StateManager(void);

	void Ascent(int nNldxModel);
	void Descent(int nNldxModel);

	D3DXVECTOR3 m_posTarget;				// 目的位置
	D3DXVECTOR3 m_posTargetDef;				// デフォルトの目的位置
	D3DXVECTOR3 m_move;						// 移動量

	STATE m_State;							// 状態
	int m_nStateCount;						// 状態管理用変数
	LOCATE_WORLD_TYPE m_LocateWorldType;	// 世界からの設置位置

	CObjectCharacter* m_pObjectCharacter;	// オブジェクトキャラクターのポインタ

	// 静的メンバ変数
	static CListManager<CSlopeDevice>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CSlopeDevice>::AIterator m_iterator; // イテレーター

};
#endif