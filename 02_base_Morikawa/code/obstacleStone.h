//========================================
//
// ��Q��(��)�̏���[obstacleStone.h]
// Author�F��������
//
//========================================
#ifndef _OBSTACLE_STONE_H_
#define _OBSTACLE_STONE_H_

#include "obstacle.h"

//========================================
// ��Q��(��)�N���X
//========================================
class CObstacleStone : public CObstacle
{
public:

	CObstacleStone();		// �R���X�g���N�^
	~CObstacleStone();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(const EType type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rSize, const CPlayer::PLAYERSTATE state) override;	// �v���C���[�Ƃ̓����蔻��

private:


};

#endif
