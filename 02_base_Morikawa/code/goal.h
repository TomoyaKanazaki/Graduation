//========================================
//
// ��Q���̏���[obstacle.h]
// Author�F��������
//
//========================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "model.h"

//========================================
// �S�[���N���X
//========================================
class CGoal : public CModel
{
public:

	CGoal();		// �R���X�g���N�^
	~CGoal();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CGoal* Create();

private:

};

#endif
