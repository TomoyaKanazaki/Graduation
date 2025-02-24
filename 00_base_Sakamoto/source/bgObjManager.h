//============================================
//
//	背景オブジェクト管理 [bjObjManager.h]
//	Author:sakai minato
//
//============================================

#ifndef _BG_OBJ_MANAGER_H_
#define _BG_OBJ_MANAGER_H_

#include "MapSystem.h"

// 前方宣言
class CObjectCharacter;
class CObjectX;
class CObjmeshDome;

// 背景オブジェクト管理クラス
class CBgObjManager
{
public:

	CBgObjManager();
	~CBgObjManager();

	static CBgObjManager* GetInstance();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetDeviceAppear(bool bAppear);

	void SetGame(CMapSystem::GRID& grid);
	CObjmeshDome* GetDomeUp(void) { return m_pMeshDomeUp; }

private:

	// マクロ定義
#define MOUNTAIN_OBJ_NUM	(4)	// 山背景モデルの使用数

	void SetFieldBotton(CMapSystem::GRID& grid);
	void SetScrollDevice(void);
	void SetSlopeDevice(void);
	void SetMountain(void);
	void SetAirShip(void);


	CObjmeshDome* m_pMeshDomeUp;						// メッシュドーム
	CObjectCharacter* m_pBGCharacter[MOUNTAIN_OBJ_NUM];	// 背景キャラクターのポインタ
	CObjectX* m_pAirShip;								// 飛空艇
	int m_nCount;										// カウント

	static CBgObjManager* m_pBgObj;

};

#endif