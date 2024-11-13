//========================================================================================
//
//�@�֗��֐�[useful.h]
//	Author:sakamoto kai
//
//========================================================================================
#ifndef _USEFUL_H_
#define _USEFUL_H_

#include "main.h"

// �������J���}�N��
#define SAFE_UNINIT(p)		if ((p) != nullptr) { (p)->Uninit();		(p) = nullptr; }	// Uninit�֐��̔j���}�N��
#define SAFE_FREE(p)		if ((p) != nullptr) { free((p));			(p) = nullptr; }	// free�֐��̔j���}�N��
#define SAFE_RELEASE(p)		if ((p) != nullptr) { (p)->Release();		(p) = nullptr; }	// Release�֐��̔j���}�N��
#define SAFE_DEL_OBJECT(p)	if ((p) != nullptr) { DeleteObject((p));	(p) = nullptr; }	// DeleteObject�֐��̔j���}�N��
#define SAFE_REF_RELEASE(p)	if ((p) != nullptr) { (p)->Release((p)); }						// �Q�ƃ|�C���^�t��Release�֐��̔j���}�N��
#define SAFE_DELETE(p)		if ((p) != nullptr) { delete(p);			(p) = nullptr; }	// �z����g�p���Ȃ�delete���g�p����j���}�N��
#define SAFE_DEL_ARRAY(p)	if ((p) != nullptr) { delete[](p);			(p) = nullptr; }	// �z����g�p����delete[]���g�p����j���}�N��

// �g�p�p�x�̍����֐��}�N��
#define NUM_ARRAY(a)	(sizeof((a)) / sizeof((a)[0]))	// �z��̗v�f���v�Z

// ���O���
namespace useful
{
	//�����蔻��̎g�p��
	enum COLLISION
	{
		COLLISION_XY = 0,
		COLLISION_YZ,
		COLLISION_ZX,
		COLLISION_X,
		COLLISION_Y,
		COLLISION_Z,
		COLLISION_MAX,
	};

	bool MoveIF(float Move1, float Move2);	// �ړ��ʂ��r���ړ��ʂP���ړ��ʂQ�������Ƃ�true��Ԃ�
	int RangeNumber(int Max, int Min, int Number);	// ������ݒ肵���͈͓��ɔ[�߂鏈��
	bool CollisionRectangle2D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 Size1, D3DXVECTOR3 Size2, useful::COLLISION Shaft);
	bool PointSquareXZ(D3DXVECTOR3 Point, D3DXVECTOR3 pos, D3DXVECTOR3 Size);		//�_���l�p�̒��ɑ��݂��邩�ǂ���
	bool PushSquareXZ(D3DXVECTOR3 PosMy, D3DXVECTOR3 SizeMy, D3DXVECTOR3 MoveMy, D3DXVECTOR3 PosPair, D3DXVECTOR3 PosOldPair, D3DXVECTOR3 SizePair, D3DXVECTOR3* pObjMove, useful::COLLISION XYZ);		//��`�̉����o������
	bool CameraFront(D3DXVECTOR3 pos);
	bool CollisionLine(D3DXVECTOR3 Mypos, D3DXVECTOR3 PairPos, float Angle);
	bool SlashCollision(D3DXVECTOR3* EnemyPos, int Max, float SlashRot);
	void CollisionWindow(D3DXVECTOR3* pos);
	bool CollisionWindow(D3DXVECTOR3 pos);
	bool CollisionCircle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float nRadiusOut);	//�~�̓����蔻��
	bool CollisionRectangle(D3DXVECTOR3 posPair, D3DXVECTOR3 sizePair, D3DXVECTOR3 posMy, D3DXVECTOR3 sizeMy);
	bool CollisionBlock(D3DXVECTOR3 PosPair, D3DXVECTOR3 PosOldPair, D3DXVECTOR3 MovePair, D3DXVECTOR3 SizePair, D3DXVECTOR3* pPosMy, D3DXVECTOR3 pPosOldMy, D3DXVECTOR3* pMoveMy, D3DXVECTOR3* pObjMoveMy, D3DXVECTOR3 SizeMy, bool* bJump, useful::COLLISION XYZ);
	void NormalizeAngle(D3DXVECTOR3* rot);
	void NormalizeRot(float* rot);
	void Vec3Project(D3DXVECTOR3* pOut, D3DXVECTOR3 v);	// 2D���W�ɕϊ�
	D3DXVECTOR3 Point2Rot(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);		//�P�ʃx�N�g�����擾
	D3DXVECTOR3 CrossIntersection(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, float SeatchDistance);		//�O�ς̔���̌�_�Ƃ̋��������߂�
	bool DecisionAbsolute(float fDataMy, float fDec);
	bool DecisionAbsolute(D3DXVECTOR3 dataMy, float fDec);
	float PosDirection(D3DXVECTOR3 MyPos, D3DXVECTOR3 PairPos, float ChageRot);		//�����̈ʒu�̕�������������
	float DegreesToRadians(float degrees);			//�x���烉�W�A���ϊ�
}

#endif