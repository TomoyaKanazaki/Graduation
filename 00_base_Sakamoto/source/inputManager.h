//============================================
//
//	���͊Ǘ� [inputManager.h]
//	Author: ���� �쏟
//
//============================================
#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include "main.h"

//�I�u�W�F�N�g�v���C���[�N���X
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