//============================================
//
//	マップ移動装置の処理 [ScrollDevice.h]
//	Author:sakai minato
//
//============================================
#ifndef _SCROLL_DEVICE_H_
#define _SCROLL_DEVICE_H_

#include "objectcharacter.h"

#include "scrollarrow.h"

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

	// 世界からの設置場所
	enum LOCATE_WORLD_TYPE
	{
		LOCATE_WORLD_TYPE_TOP = 0,	// 上
		LOCATE_WORLD_TYPE_BOTTOM,	// 下
		LOCATE_WORLD_TYPE_LEFT,		// 左
		LOCATE_WORLD_TYPE_RIGHT,	// 右
		LOCATE_WORLD_TYPE_MAX
	};

	// モーション
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,	// 待機状態
		ACTION_MAX,			// 最大
	};

	// パーツ構成
	enum SETUP_TYPE
	{
		SETUP_TYPE_FOUNDATION = 0,	// 土台
		SETUP_TYPE_MAWASIGURMA,		// 回し車
		SETUP_TYPE_ROLLRE,			// ローラー
		SETUP_TYPE_HEAD,			// 先っぽ
		SETUP_TYPE_MAX,				// 最大
	};

	static CScrollDevice* Create(const std::string pModelNameSlopeDevice, const std::string pModelNameEnemy);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(STATE state);
	void SetStateArrow(CScrollArrow::Arrow stateArrow);

	void SetLocateWorldType(LOCATE_WORLD_TYPE LocateWorldType) { m_LocateWorldType = LocateWorldType; }


	// 静的メンバ関数
	static CListManager<CScrollDevice>* GetList(void); // リスト取得

private:
	HRESULT InitModel(const std::string pModelNameScrollDevice, const std::string pModelNameEnemy);

	void StateManager(D3DXVECTOR3& rotMy);
	void Rotate(D3DXVECTOR3& rotMy,int nNldxModel);

	void SetRotMedaman(CScrollArrow::Arrow stateArrow = CScrollArrow::Arrow(0));

	STATE m_State;					//状態
	int m_nStateCount;				//状態管理用変数

	LOCATE_WORLD_TYPE m_LocateWorldType;	// 世界からの設置位置

	D3DXVECTOR3 m_rotMove;					// 向きの移動量

	CObjectCharacter* m_pObjectCharacter;	// オブジェクトキャラクターのポインタ

	// 静的メンバ変数
	static CListManager<CScrollDevice>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CScrollDevice>::AIterator m_iterator; // イテレーター

};
#endif