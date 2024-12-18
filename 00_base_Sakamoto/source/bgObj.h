//============================================
//
//	背景オブジェクト [bjObj.h]
//	Author:sakai minato
//
//============================================
#pragma once

#include "MapSystem.h"

//==========================================
//  名前空間定義
//==========================================
namespace BgObj
{
	void Init(void);
	void Update(void);

	void SetGame(CMapSystem::GRID& grid);
	void SetFieldBotton(CMapSystem::GRID& grid);
	void SetScrollDevice(void);
	void SetSlopeDevice(void);
	void SetMountain(void);
}