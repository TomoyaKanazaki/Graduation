//========================================
//
// ��Q��(��)�̏���[obstacleHole.h]
// Author�F��������
//
//========================================
#ifndef _OBSTACLE_HOLE_H_
#define _OBSTACLE_HOLE_H_

#include "obstacle.h"

//========================================
// ��Q��(��)�N���X
//========================================
class CObstacleHole : public CObstacle
{
public:

	CObstacleHole();		// �R���X�g���N�^
	~CObstacleHole();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(const EType type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:


};

#endif
