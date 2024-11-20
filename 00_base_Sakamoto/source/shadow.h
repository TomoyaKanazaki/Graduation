//============================================
//
//	�e���� [shadow.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "object3D.h"

//�I�u�W�F�N�g�V���h�E�N���X
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