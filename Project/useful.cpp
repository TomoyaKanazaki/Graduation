//========================================================================================
//
//�@�֗��֐�[useful.cpp]
//	Author:sakamoto kai
//
//========================================================================================
#include "useful.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "debugproc.h"

//====================================================================
//
//====================================================================
bool useful::CollisionRectangle2D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 Size1, D3DXVECTOR3 Size2, useful::COLLISION Shaft)
{
	bool Hit = false;

	switch (Shaft)
	{
	case useful::COLLISION_XY:
		if (pos1.x + Size1.x > pos2.x - Size2.x &&
			pos1.x - Size1.x < pos2.x + Size2.x &&
			pos1.y + Size1.y > pos2.y - Size2.y &&
			pos1.y - Size1.y < pos2.y + Size2.y)
		{
			Hit = true;
		}
		break;

	case useful::COLLISION_YZ:
		if (pos1.y + Size1.y < pos2.y - Size2.y &&
			pos1.y - Size1.y > pos2.y + Size2.y &&
			pos1.z + Size1.z < pos2.z - Size2.z &&
			pos1.z - Size1.z > pos2.z + Size2.z)
		{
			Hit = true;
		}
		break;

	case useful::COLLISION_ZX:
		if (pos1.z + Size1.z < pos2.z - Size2.z &&
			pos1.z - Size1.z > pos2.z + Size2.z &&
			pos1.x + Size1.x < pos2.x - Size2.x &&
			pos1.x - Size1.x > pos2.x + Size2.x)
		{
			Hit = true;
		}
		break;
	}

	return Hit;
}

//====================================================================
//�J�����̑O���ɑ��݂��Ă��邩�ǂ���
//====================================================================
bool useful::CameraFront(D3DXVECTOR3 pos)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	D3DXVECTOR3 CamPos = pCamera->GetPosV();
	float CamRot = pCamera->GetRot().y;

	D3DXVECTOR3 vecToPos;
	D3DXVECTOR3 vecLine;
	float A;

	vecToPos.x = (CamPos.x) - (pos.x);
	vecToPos.z = (CamPos.z) - (pos.z);

	vecLine.x = sinf(CamRot + D3DX_PI * 0.5f);
	vecLine.y = 0.0f;
	vecLine.z = cosf(CamRot + D3DX_PI * 0.5f);

	A = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);

	if (A > 0)
	{
		return true;
	}
	return false;
}

//====================================================================
// �����P�̈ʒu��������R�̊p�x�̐������������Ɉ����Q�̈ʒu�����̉E�ɂ��邩�ǂ���
//====================================================================
bool useful::CollisionLine(D3DXVECTOR3 Mypos, D3DXVECTOR3 PairPos, float Angle)
{
	D3DXVECTOR3 vecToPos;
	D3DXVECTOR3 vecLine;
	float A;

	vecToPos.x = (Mypos.x) - (PairPos.x);
	vecToPos.z = (Mypos.z) - (PairPos.z);

	vecLine.x = sinf(Angle + D3DX_PI * 0.5f);
	vecLine.y = 0.0f;
	vecLine.z = cosf(Angle + D3DX_PI * 0.5f);

	A = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);

	if (A > 0)
	{
		return true;
	}
	return false;
}

//====================================================================
//�a���̓����蔻��
//====================================================================
bool useful::SlashCollision(D3DXVECTOR3 *EnemyPos, int Max, float SlashRot)
{
	//�v���C���[�ƓG�̈ʒu�֌W�������O��
	int Left = 0;
	int	Right = 0;

	for (int nCnt = 0; nCnt < Max; nCnt++, EnemyPos++)
	{
		D3DXVECTOR3 vecToPos;
		D3DXVECTOR3 vecLine;
		float A;

		vecToPos.x = (EnemyPos->x) - (SCREEN_CENTER.x);
		vecToPos.y = (EnemyPos->y) - (SCREEN_CENTER.y);

		vecLine.x = sinf(SlashRot);
		vecLine.y = cosf(SlashRot);
		vecLine.z = 0.0f;

		A = (vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y);

		if (A < 0)
		{
			Right++;
		}
		else
		{
			Left++;
		}
	}

	//�G�̓_�̊ԂɍU�����̐��������Ă����ꍇ
	if (Left > 0 && Right > 0)
	{
		return true;
	}

	return false;
}

//====================================================================
//�E�B���h�E��ʂ̓����蔻��
//====================================================================
void useful::CollisionWindow(D3DXVECTOR3* pos)
{
	if (pos->x >= SCREEN_WIDTH)
	{
		pos->x = SCREEN_WIDTH;
	}
	else if (pos->x <= 0.0f)
	{
		pos->x = 0.0f;
	}

	if (pos->y >= SCREEN_HEIGHT)
	{
		pos->y = SCREEN_HEIGHT;
	}
	else if (pos->y <= 0.0f)
	{
		pos->y = 0.0f;
	}
}

//====================================================================
//�E�B���h�E��ʂ̓����蔻��
//====================================================================
bool useful::CollisionWindow(D3DXVECTOR3 pos)
{
	bool bHit = false;

	if (pos.x >= SCREEN_WIDTH)
	{
		bHit = true;
	}
	else if (pos.x <= 0.0f)
	{
		bHit = true;
	}

	if (pos.y >= SCREEN_HEIGHT)
	{
		bHit = true;
	}
	else if (pos.y <= 0.0f)
	{
		bHit = true;
	}

	return bHit;
}

//====================================================================
//�~�̓����蔻��
//====================================================================
bool useful::CollisionCircle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float nRadiusOut)
{
	bool nHit = false;

	if (sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.y - pos2.y) * (pos1.y - pos2.y)) <= nRadiusOut)
	{//�~�̔��肪��������
		if (sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
			+ (pos1.z - pos2.z) * (pos1.z - pos2.z)) <= nRadiusOut)
		{//�~�̔��肪��������
			nHit = true;
		}
	}

	return nHit;
}

//====================================================================
// ��`�̓����蔻��i����j
//====================================================================
bool useful::CollisionRectangle(D3DXVECTOR3 posMy, D3DXVECTOR3 sizeMy, D3DXVECTOR3 posPair, D3DXVECTOR3 sizePair)
{
	if (posMy.x + sizeMy.x >= posPair.x - sizePair.x &&
		posMy.x - sizeMy.x <= posPair.x + sizePair.x &&
		posMy.y + sizeMy.y >= posPair.y - sizePair.y &&
		posMy.y - sizeMy.y <= posPair.y + sizePair.y &&
		posMy.z + sizeMy.z >= posPair.z - sizePair.z &&
		posMy.z - sizeMy.z <= posPair.z + sizePair.z)
	{
		return true;
	}

	return false;
}

//====================================================================
// �����ʂɑ΂��铖���蔻��
//====================================================================
bool useful::CollisionBlock(
	D3DXVECTOR3 PosPair, D3DXVECTOR3 PosOldPair, D3DXVECTOR3 MovePair, D3DXVECTOR3 SizePair,
	D3DXVECTOR3* pPosMy, D3DXVECTOR3 pPosOldMy, D3DXVECTOR3* pMoveMy, D3DXVECTOR3* pObjMoveMy, D3DXVECTOR3 SizeMy, bool* bJump, useful::COLLISION XYZ)
{
	switch (XYZ)
	{
	case useful::COLLISION_X:

		//�{����|�̖�
		if (PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			(PosOldPair.x + SizePair.x <= pPosMy->x - SizeMy.x ||
				PosPair.x + SizePair.x <= pPosOldMy.x - SizeMy.x ||
				(PosPair.x + SizePair.x >= pPosOldMy.x - SizeMy.x &&
					PosOldPair.x + SizePair.x <= pPosOldMy.x - SizeMy.x &&
					PosOldPair.x + SizePair.x >= pPosMy->x - SizeMy.x)) &&
			PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			PosPair.z - SizePair.z < pPosMy->z + SizeMy.z &&
			PosPair.y + SizePair.y > pPosMy->y &&
			PosPair.y - SizePair.y < pPosMy->y + SizeMy.y
			)
		{
			if (MovePair.x >= 0.0f)
			{
				pPosMy->x = pPosOldMy.x - SizeMy.x + SizeMy.x + MovePair.x;
			}
			else
			{
				pPosMy->x = PosPair.x + SizePair.x + SizeMy.x + MovePair.x;
			}
			pMoveMy->x = 0.0f;
			return true;
		}

		//�|����{�̖�
		if (PosPair.x - SizePair.x < pPosMy->x + SizeMy.x &&
			(PosOldPair.x - SizePair.x >= pPosMy->x + SizeMy.x ||
				PosPair.x - SizePair.x >= pPosOldMy.x + SizeMy.x ||
				(PosPair.x - SizePair.x <= pPosOldMy.x + SizeMy.x &&
					PosOldPair.x - SizePair.x >= pPosOldMy.x + SizeMy.x &&
					PosOldPair.x - SizePair.x <= pPosMy->x + SizeMy.x)) &&
			PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			PosPair.z - SizePair.z < pPosMy->z + SizeMy.z &&
			PosPair.y + SizePair.y >= pPosMy->y &&
			PosPair.y - SizePair.y <= pPosMy->y + SizeMy.y
			)
		{
			if (MovePair.x <= 0.0f)
			{
				pPosMy->x = pPosOldMy.x + SizeMy.x - SizeMy.x + MovePair.x;
			}
			else
			{
				pPosMy->x = PosPair.x - SizePair.x - SizeMy.x + MovePair.x;
			}
			pMoveMy->x = 0.0f;
			return true;
		}

		break;

	case useful::COLLISION_Y:

		//�{����|�̖�
		if (PosPair.y + SizePair.y > pPosMy->y &&
			(PosOldPair.y + SizePair.y <= pPosMy->y ||
				PosPair.y + SizePair.y <= pPosOldMy.y ||
				(PosPair.y + SizePair.y >= pPosOldMy.y &&
					PosOldPair.y + SizePair.y <= pPosOldMy.y &&
					PosOldPair.y + SizePair.y >= pPosMy->y)) &&
			PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			PosPair.z - SizePair.z < pPosMy->z + SizeMy.z &&
			PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			PosPair.x - SizePair.x < pPosMy->x + SizeMy.x
			)
		{
			if (MovePair.y >= 0.0f)
			{
				pPosMy->y = pPosOldMy.y + MovePair.y;
			}
			else
			{
				pPosMy->y = PosPair.y + SizePair.y + MovePair.y;
			}
			pMoveMy->y = 0.0f;
			*bJump = false;
			pObjMoveMy->x = MovePair.x;
			pObjMoveMy->y = MovePair.y;
			pObjMoveMy->z = MovePair.z;
			return true;
		}

		//�|����{�̖�
		if (PosPair.y - SizePair.y < pPosMy->y + SizeMy.y &&
			(PosOldPair.y - SizePair.y >= pPosMy->y + SizeMy.y ||
				PosPair.y - SizePair.y >= pPosOldMy.y + SizeMy.y ||
				(PosPair.y - SizePair.y <= pPosOldMy.y + SizeMy.y &&
					PosOldPair.y - SizePair.y >= pPosOldMy.y + SizeMy.y &&
					PosOldPair.y - SizePair.y <= pPosMy->y + SizeMy.y)) &&
			PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			PosPair.z - SizePair.z < pPosMy->z + SizeMy.z &&
			PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			PosPair.x - SizePair.x < pPosMy->x + SizeMy.x
			)
		{
			pPosMy->y = PosPair.y - SizePair.y - SizeMy.y + MovePair.y;
			pMoveMy->y = MovePair.y;
			return true;
		}

		break;

	case useful::COLLISION_Z:

		//�{����|�̖�
		if (PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			(PosOldPair.z + SizePair.z <= pPosMy->z - SizeMy.z ||
				PosPair.z + SizePair.z <= pPosOldMy.z - SizeMy.z ||
				(PosPair.z + SizePair.z >= pPosOldMy.z - SizeMy.z &&
					PosOldPair.z + SizePair.z <= pPosOldMy.z - SizeMy.z &&
					PosOldPair.z + SizePair.z >= pPosMy->z - SizeMy.z)) &&
			PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			PosPair.x - SizePair.x < pPosMy->x + SizeMy.x &&
			PosPair.y + SizePair.y > pPosMy->y &&
			PosPair.y - SizePair.y < pPosMy->y + SizeMy.y
			)
		{
			if (MovePair.z >= 0.0f)
			{
				pPosMy->z = pPosOldMy.z - SizeMy.z + SizeMy.z + MovePair.z;
			}
			else
			{
				pPosMy->z = PosPair.z + SizePair.z + SizeMy.z + MovePair.z;
			}
			pMoveMy->z = 0.0f;
			return true;
		}

		//�|����{�̖�
		if (PosPair.z - SizePair.z < pPosMy->z + SizeMy.z &&
			(PosOldPair.z - SizePair.z >= pPosMy->z + SizeMy.z ||
				PosPair.z - SizePair.z >= pPosOldMy.z + SizeMy.z ||
				(PosPair.z - SizePair.z <= pPosOldMy.z + SizeMy.z &&
					PosOldPair.z - SizePair.z >= pPosOldMy.z + SizeMy.z &&
					PosOldPair.z - SizePair.z <= pPosMy->z + SizeMy.z)) &&
			PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			PosPair.x - SizePair.x < pPosMy->x + SizeMy.x &&
			PosPair.y + SizePair.y > pPosMy->y &&
			PosPair.y - SizePair.y < pPosMy->y + SizeMy.y
			)
		{
			if (MovePair.z <= 0.0f)
			{
				pPosMy->z = pPosOldMy.z + SizeMy.z - SizeMy.z + MovePair.z;
			}
			else
			{
				pPosMy->z = PosPair.z - SizePair.z - SizeMy.z + MovePair.z;
			}
			pMoveMy->z = 0.0f;
			return true;
		}

		break;
	}

	return false;
}

//====================================================================
// �p�x�̐��K���i����j
//====================================================================
void useful::NormalizeAngle(D3DXVECTOR3* rot)
{
	// �ړI�̌����̕␳
	if (rot->x > D3DX_PI)
	{
		rot->x += -D3DX_PI * 2;
	}
	else if (rot->x < -D3DX_PI)
	{
		rot->x += D3DX_PI * 2;
	}

	// �ړI�̌����̕␳
	if (rot->y > D3DX_PI)
	{
		rot->y += -D3DX_PI * 2;
	}
	else if (rot->y < -D3DX_PI)
	{
		rot->y += D3DX_PI * 2;
	}

	// �ړI�̌����̕␳
	if (rot->z > D3DX_PI)
	{
		rot->z += -D3DX_PI * 2;
	}
	else if (rot->z < -D3DX_PI)
	{
		rot->z += D3DX_PI * 2;
	}
}

//====================================================================
// �p�x�̐��K���i���c�j
//====================================================================
void useful::NormalizeRot(float* rot)
{
	// �ړI�̌����̕␳
	if (*rot > D3DX_PI)
	{
		*rot += -D3DX_PI * 2;
	}
	else if (*rot < -D3DX_PI)
	{
		*rot += D3DX_PI * 2;
	}
}

//====================================================================
// 2D���W�ɕϊ� CieCv2D(�o�͐�A�ϊ����W)
//====================================================================
void useful::Vec3Project(D3DXVECTOR3 *pOut, D3DXVECTOR3 v)
{
	// �ϐ��錾
	D3DXMATRIX mtxProjection;
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxWorld;
	D3DVIEWPORT9 viewport;

	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;

	//�}�g���b�N�X�̎擾
	mtxView = CManager::GetInstance()->GetCamera()->GetViewMatrix();
	mtxProjection = CManager::GetInstance()->GetCamera()->GetProjectionMatrix();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ���W�ϊ�
	D3DXVec3Project(pOut, &v, &viewport, &mtxProjection, &mtxView, NULL);
}

//================
// 2�_�Ԃ̒P�ʃx�N�g���擾
//@param (pos0)��ƂȂ�ʒu
//@param (pos1)����̈ʒu
//@return �x�N�g��
//================
D3DXVECTOR3 useful::Point2Rot(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3 vecDiff = pos1 - pos0;
	D3DXVec3Normalize(&vecDiff, &vecDiff);		//�x�N�g���𐳋K������(1.0f �` -1.0f�ɂ���)

	return vecDiff;
}

//====================================================================
//	�O�ς̔���̌�_�Ƃ̋��������߂�
//====================================================================
D3DXVECTOR3 useful::CrossIntersection(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, float SeatchDistance)
{
	D3DXVECTOR3 vecToPos[2];
	D3DXVECTOR3 vecLine;
	D3DXVECTOR3 vecToMove;
	float fA;
	float fB;
	int nA = 0;
	float fAngle = atan2f((pos0.x - pos1.x), (pos0.z - pos1.z));

	vecToPos[0].x = (pos0.x) - (pos2.x);
	vecToPos[0].z = (pos0.z) - (pos2.z);

	vecToPos[1].x = (pos0.x) - (pos3.x);
	vecToPos[1].z = (pos0.z) - (pos3.z);

	vecToMove.x = (pos3.x) - (pos2.x);
	vecToMove.z = (pos3.z) - (pos2.z);

	vecLine.x = sinf(fAngle + D3DX_PI * 0.0f) * 700.0f;
	vecLine.y = 0.0f;
	vecLine.z = cosf(fAngle + D3DX_PI * 0.0f) * 700.0f;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		fA = (vecLine.z * vecToPos[nCnt].x) - (vecLine.x * vecToPos[nCnt].z);

		if (fA > 0)
		{
			nA++;
		}
		else
		{
			nA--;
		}
	}

	if (nA == 0)
	{
		fA = (vecToPos[0].z * vecToMove.x) - (vecToPos[0].x * vecToMove.z);
		fB = (vecLine.z * vecToMove.x) - (vecLine.x * vecToMove.z);

		float fRate = fA / fB;

		//CManager::GetInstance()->GetDebugProc()->Print("%f\n", fRate);

		if (fRate >= 0.0f && fRate <= 1.0f)
		{
			D3DXVECTOR3 CrossPoint = INITVECTOR3;
			CrossPoint.x = pos0.x - vecLine.x * fRate;
			CrossPoint.z = pos0.z - vecLine.z * fRate;

			return D3DXVECTOR3(CrossPoint.x, 0.0f, CrossPoint.z);
		}
	}

	return D3DXVECTOR3(10000.0f, 0.0f, 10000.0f);
}

//====================================================================
// ��Βl����[float]�i����j
//====================================================================
bool useful::DecisionAbsolute(float fDataMy, float fDec)
{
	// ��Βl�Ƃ��Ēl���擾
	fDataMy = std::abs(fDataMy);

	// ��r�Ώۂ��傫��
	if (fDataMy >= fDec)
	{
		return true;
	}

	return false;
}

//====================================================================
// ��Βl����[D3DXVECTOR3]�i����j
//====================================================================
bool useful::DecisionAbsolute(D3DXVECTOR3 dataMy, float fDec)
{
	// ��Βl�Ƃ��Ēl���擾
	dataMy.x = std::abs(dataMy.x);
	dataMy.y = std::abs(dataMy.y);
	dataMy.z = std::abs(dataMy.z);

	// ��r�Ώۂ��傫��
	if (dataMy.x >= fDec ||
		dataMy.y >= fDec ||
		dataMy.z >= fDec)
	{
		return true;
	}

	return false;
}

//====================================================================
// �����̈ʒu�̕�������������
//====================================================================
float useful::PosDirection(D3DXVECTOR3 MyPos, D3DXVECTOR3 PairPos, float ChageRot)
{
	float fRot = 0.0f;

	fRot = atan2f(PairPos.z - MyPos.z, MyPos.x - PairPos.x) + D3DX_PI * ChageRot;

	return fRot;
}

//====================================================================
// �x���烉�W�A���ϊ�
//====================================================================
float useful::DegreesToRadians(float degrees)
{
	return degrees * (D3DX_PI / 180.0f);
}
