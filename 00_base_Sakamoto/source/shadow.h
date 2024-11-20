//============================================
//
//	影処理 [shadow.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "object3D.h"

//オブジェクトシャドウクラス
class CShadow : public CObject3D
{
public:

	CShadow(int nPriority = 3);
	~CShadow();

	static CShadow* Create(const D3DXVECTOR3& pos, float fWidth, float fHeight);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:

};

#endif