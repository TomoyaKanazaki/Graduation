//========================================
//
// ��Q��(�_�炩����)�̏���[obstacleSoftRock.h]
// Author�F��������
//
//========================================
#ifndef _OBSTACLE_SOFTROCK_H_
#define _OBSTACLE_SOFTROCK_H_

#include "obstacle.h"

//========================================
// ��Q��(�_�炩����)�N���X
//========================================
class CObstacleSoftRock : public CObstacle
{
public:

	CObstacleSoftRock();		// �R���X�g���N�^
	~CObstacleSoftRock();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(const EType type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:


};

#endif
