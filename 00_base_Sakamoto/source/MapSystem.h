//========================================================================================
//
// マップシステム[MapSystem.h]
// Author: 坂本　翔唯
//
//========================================================================================
#ifndef _MAPSYSTEM_H_
#define _MAPSYSTEM_H_

#include "main.h"
#include "manager.h"

class CObject2D;

//フェードのクラス宣言
class CMapSystem
{
public:
	CMapSystem();
	~CMapSystem();

	void Init();
	void Uninit();
	void Update();
	void Draw();

protected:
};

#endif
