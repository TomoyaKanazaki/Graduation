//========================================
//
// ��Q��(��)�̏���[obstacleRiver.h]
// Author�F��������
//
//========================================
#ifndef _OBSTACLE_RIVER_H_
#define _OBSTACLE_RIVER_H_

#include "obstacle.h"

//========================================
// ��Q��(��)�N���X
//========================================
class CObstacleRiver : public CObstacle
{
public:

	CObstacleRiver();		// �R���X�g���N�^
	~CObstacleRiver();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(const EType type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:


};

#endif
