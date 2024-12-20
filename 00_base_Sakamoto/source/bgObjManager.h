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

	void SetGame(CMapSystem::GRID& grid);

private:

	// マクロ定義
#define MOUNTAIN_OBJ_NUM	(4)	// 山背景モデルの使用数

	void SetFieldBotton(CMapSystem::GRID& grid);
	void SetScrollDevice(void);
	void SetSlopeDevice(void);
	void SetMountain(void);

	CObjectCharacter* m_pBGCharacter[MOUNTAIN_OBJ_NUM];	// 背景キャラクターのポインタ
	int m_nCount;										// カウント

	static CBgObjManager* m_pBgObj;

};

#endif