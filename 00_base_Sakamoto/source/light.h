//============================================
//
//	ライトの処理 [light.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//マクロ定義
#define MAX_LIGHT (3)	//使用するライトの数

//カメラクラス
class CLight
{
public:
	CLight();
	~CLight();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	D3DXVECTOR3 GetLight() { return m_vecAve; }

private:
	D3DLIGHT9 m_aLight[MAX_LIGHT];

	D3DXVECTOR3 m_vecAve;
};

#endif