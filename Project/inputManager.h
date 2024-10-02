//============================================
//
//	入力管理 [inputManager.h]
//	Author: 酒井 南勝
//
//============================================
#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include "main.h"

//オブジェクトプレイヤークラス
class CInputManager
{
public:

	CInputManager();
	~CInputManager();

	static CInputManager* Create(void);

	HRESULT Init(void);
	void Uninit(void);

private:

};
#endif