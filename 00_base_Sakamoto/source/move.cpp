//============================================
//
//	�ړ���Ԃ̊Ǘ� [move.cpp]
//	Author:Satone Shion
//
//============================================
#include "move.h"
#include "objectcharacter.h"
#include "manager.h"
#include "sound.h"
#include "debugproc.h"
#include "player.h"
#include "enemy.h"
#include "devil.h"
#include "MapMove.h"
#include "DevilHole.h"
#include "item.h"

#include "camera.h"
#include "MapSystem.h"
#include "useful.h"

//==========================================
// �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 EGG_MOVE = D3DXVECTOR3(10.0f, 10.0f, 10.0f);	 // �ړ��ʂ̌������x(��)
	const int INVINCIBLE_TIME = 120;		// ���G����
	const float PLAYER_SPEED = 5.0f;		// �v���C���[�̈ړ����x
	const float ENEMY_SPEED = 3.0f;			// �G�̈ړ����x
	const float FRIEDEGG_SPEED = 3.0f;		// �ڋʏĂ��̈ړ����x

	const float COORDDINATE_RATE[] =		// �o�H�T�����s���Ԋu
	{
		5.0f,
		3.0f,
		1.0f
	};

	const float TARGET_DIFF = 10.0f;		// ���e�͈�
	const float MOVE_ASTAR = 150.0f;		// �ǐՎ��̈ړ����x

	const float GRIT_OK = 45.0f;			// �ړ��\�ȃO���b�g�͈͓̔�

}

//==========================================
//  �ÓI�x������
//==========================================
static_assert(NUM_ARRAY(COORDDINATE_RATE) == CEnemy::ENEMY_MAX, "ERROR : Type Count Missmatch");

//**********************************************************************************************************
// �ړ���Ԃ̃C���^�[�t�F�[�X
//**********************************************************************************************************
//==========================================
// �R���X�g���N�^
//==========================================
CMoveState::CMoveState()
{
	m_State = STATE_NONE;

	// �i�s�����
	m_Progress.bOKR = false;	//�E
	m_Progress.bOKL = false;	//��
	m_Progress.bOKU = false;	//��
	m_Progress.bOKD = false;	//�� 

	// �O��̐i�s�����
	m_ProgressOld.bOKR = false;	//�E
	m_ProgressOld.bOKL = false;	//��
	m_ProgressOld.bOKU = false;	//��
	m_ProgressOld.bOKD = false;	//�� 

}

//==========================================
// �ړ���Ԃ̏��擾
//==========================================
CMoveState* CMoveState::GetMoveState(CObjectCharacter* pCharacter)
{
	return pCharacter->GetMoveState();		// ���݂̃L�����N�^�[��Ԃ̏���Ԃ�
}

//==========================================
// �f�o�b�O�\���p
//==========================================
void CMoveState::Debug(void)
{
	DebugProc::Print(DebugProc::POINT_LEFT, "�ړ���� : ");

	auto str = magic_enum::enum_name(m_State);

	DebugProc::Print(DebugProc::POINT_LEFT, str.data());
	DebugProc::Print(DebugProc::POINT_LEFT, "\n");

}

//====================================================================
// �ړ���������(�L�����N�^�[)
//====================================================================
void CMoveState::Rot(CObjectCharacter* pCharacter, D3DXVECTOR3& rot)
{
	//�L�[�{�[�h�̎擾
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera(0)->GetRot();
	D3DXVECTOR3 move = pCharacter->GetMove();

	//�ړ������Ɍ��������킹�鏈��
	/*float fRotMove, fRotDest;
	fRotMove = rot.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;*/

	rot.y = atan2f(-move.x, -move.z);

	useful::NormalizeAngle(&rot);
}

//====================================================================
// �ړ���������(�A�C�e��)
//====================================================================
void CMoveState::Rot(CItem* pItem, D3DXVECTOR3& rot)
{
	//�L�[�{�[�h�̎擾
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera(0)->GetRot();
	D3DXVECTOR3 move = pItem->GetMove();

	rot.y = atan2f(-move.x, -move.z);

	useful::NormalizeAngle(&rot);
}

//====================================================================
// �ʒu�X�V����(�L�����N�^�[)
//====================================================================
void CMoveState::UpdatePos(CObjectCharacter* pCharacter, D3DXVECTOR3& pos)
{
	// �ϐ��錾
	CMapMove* pMapMove = CMapSystem::GetInstance()->GetMove();
	D3DXVECTOR3 move = pCharacter->GetMove();		// �ړ���
	D3DXVECTOR3 objMove = pCharacter->GetObjMove();

	float fSpeed = 1.0f;	// �X���[�p default1.0f�ŏ�����

	//�d��
	move.y -= 0.5f;
	pCharacter->SetMove(move);		// �ړ���

	//Y���̈ʒu�X�V
	pos.y += move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	pos.y += objMove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	//CollisionWall(pos,posOldMy,sizeMy,useful::COLLISION_Y);
	//CollisionDevilHole(useful::COLLISION_Y);

	//X���̈ʒu�X�V
	pos.x += move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeX(move.x);
	pos.x += objMove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeX(move.x);

	// �ǂƂ̓����蔻��
	//CollisionWall(pos, posOldMy, sizeMy, useful::COLLISION_X);
	//CollisionDevilHole(useful::COLLISION_X);

	//Z���̈ʒu�X�V
	pos.z += move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeZ(move.z);
	pos.z += objMove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeZ(move.z);

	// �ǂƂ̓����蔻��
	//CollisionWall(pos, posOldMy, sizeMy, useful::COLLISION_Z);
	//CollisionDevilHole(useful::COLLISION_Z);

}

//====================================================================
// �ʒu�X�V����(�A�C�e��)
//====================================================================
void CMoveState::UpdatePos(CItem* pItem, D3DXVECTOR3& pos)
{
	// �ϐ��錾
	CMapMove* pMapMove = CMapSystem::GetInstance()->GetMove();
	D3DXVECTOR3 move = pItem->GetMove();		// �ړ���

	float fSpeed = 1.0f;	// �X���[�p default1.0f�ŏ�����

	//�d��
	move.y -= 0.5f;
	pItem->SetMove(move);		// �ړ���

	//�ʒu�X�V
	pos.x += move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeX(move.x);
	pos.y += move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	pos.z += move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeZ(move.z);
}

//**********************************************************************************************************
// ����ł�����
//**********************************************************************************************************
//==========================================
// �R���X�g���N�^
//==========================================
CStateControl::CStateControl()
{
	m_bInput = false;				// ���͂��s�������ǂ���
	m_RotState = ROTSTATE_NONE;		// �ړ������̏��

	m_State = STATE_CONTROL;			// ������

	// �i�s�����
	m_Progress.bOKR = false;	//�E
	m_Progress.bOKL = false;	//��
	m_Progress.bOKU = false;	//��
	m_Progress.bOKD = false;	//�� 

	// �O��̐i�s�����
	m_ProgressOld.bOKR = false;	//�E
	m_ProgressOld.bOKL = false;	//��
	m_ProgressOld.bOKU = false;	//��
	m_ProgressOld.bOKD = false;	//�� 

}

//==========================================
// �j��
//==========================================
void CStateControl::Release()
{

}

//==========================================
// ���삩�烉���_�����s�ɐ؂�ւ�
//==========================================
void CStateControl::ControlRandom(CObjectCharacter* pCharacter)
{
	// �����_�����s��Ԃɂ���
	pCharacter->ChangeMoveState(new CStateRandom);
}

//==========================================
// ���삩��ǐՂɐ؂�ւ�
//==========================================
void CStateControl::ControlAStar(CObjectCharacter* pCharacter)
{
	// �ǐՏ�Ԃɂ���
	pCharacter->ChangeMoveState(new CStateAStar);
}

//==========================================
// ���삩���~�ɐ؂�ւ�
//==========================================
void CStateControl::ControlStop(CObjectCharacter* pCharacter)
{
	// ��~��Ԃɂ���
	pCharacter->ChangeMoveState(new CStateStop);
}

//==========================================
// �ړ�����
//==========================================
void CStateControl::Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bInput = false;			// ���͂��ĂȂ���Ԃɂ���

	// ���͏���
	switch (pCharacter->GetType())
	{
	case CObject::TYPE_PLAYER3D:		// �v���C���[
		NormarizeMove = InputKey(pCharacter, pos, rot, NormarizeMove, PLAYER_SPEED);
		UpdateMovePlayer(pCharacter, NormarizeMove);		// �ړ��X�V

		break;

	case CObject::TYPE_ENEMY3D:			// �G
		NormarizeMove = InputKey(pCharacter, pos, rot, NormarizeMove, ENEMY_SPEED);
		UpdateMoveEnemy(pCharacter, NormarizeMove);			// �ړ��X�V

		// �ړ���������
		Rot(pCharacter, rot);

		// �ʒu�X�V����
		UpdatePos(pCharacter, pos);

		break;

	default:
		break;
	}

	
}

//==========================================
// �v���C���[�̈ړ��X�V����
//==========================================
void CStateControl::UpdateMovePlayer(CObjectCharacter* pCharacter, D3DXVECTOR3& NormarizeMove)
{
	CObjectCharacter::STATE state = pCharacter->GetState();
	D3DXVECTOR3 EggMove = pCharacter->GetEggMove();	// �v���C���[�̗��ړ���

	if (m_bInput && state != CObjectCharacter::STATE_ATTACK)
	{
		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.z *= PLAYER_SPEED;

		//�ړ��ʂ���
		pCharacter->SetMove(NormarizeMove);

		if (state == CObjectCharacter::STATE_EGG)
		{
			// ���f�����̎擾
			int nNumModel = pCharacter->GetNumModel();

			for (int nCnt = 0; nCnt < nNumModel; nCnt++)
			{
				// ���f���̎擾
				CModel* pModel = pCharacter->GetModel(nCnt);

				if (pModel != nullptr)
				{
					pModel->SetDisp(true);
				}
			}

			EggMove.y = EGG_MOVE.y;

			switch (m_RotState)
			{
			case CMoveState::ROTSTATE_LEFT:
				EggMove.x = EGG_MOVE.x;
				EggMove.z = 0.0f;
				break;
			case CMoveState::ROTSTATE_RIGHT:
				EggMove.x = -EGG_MOVE.x;
				EggMove.z = 0.0f;
				break;
			case CMoveState::ROTSTATE_UP:
				EggMove.x = 0.0f;
				EggMove.z = -EGG_MOVE.z;
				break;
			case CMoveState::ROTSTATE_DOWN:
				EggMove.x = 0.0f;
				EggMove.z = EGG_MOVE.z;
				break;
			}

			// ���̈ړ��ʐݒ�
			pCharacter->SetEggMove(EggMove);

			//���G��Ԃ̐ݒ�
			pCharacter->SetInvincible(true);
			pCharacter->SetInvincibleCount(INVINCIBLE_TIME);

			pCharacter->PlayerNumberDisp(true);
		}
		//�ړ���Ԃɂ���
		pCharacter->SetState(CObjectCharacter::STATE_WALK);
	}
}

//==========================================
// �G�̈ړ��X�V����
//==========================================
void CStateControl::UpdateMoveEnemy(CObjectCharacter* pCharacter, D3DXVECTOR3& NormarizeMove)
{
	D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

	NormarizeMove.x *= ENEMY_SPEED;
	NormarizeMove.z *= ENEMY_SPEED;

	//�ړ��ʂ���
	pCharacter->SetMove(NormarizeMove);

	//�ړ���Ԃɂ���
	pCharacter->SetState(CObjectCharacter::STATE_WALK);
}

//==========================================
// �ړ����̓L�[�{�[�h(�L�����N�^�[)
//==========================================
D3DXVECTOR3 CStateControl::InputKey(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot, D3DXVECTOR3 Move, float fSpeed)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// �ړ��̐i�s����
	CObjectCharacter::PROGGRESS progress = pCharacter->GetProgress();
	bool bGridCenter = pCharacter->GetGritCenter();		// �O���b�h���W�̒��S�ɂ��邩

	//�L�[�{�[�h�̈ړ�����
	if ((pInputKeyboard->GetPress(DIK_W) && progress.bOKU && bGridCenter) ||
		(pInputKeyboard->GetPress(DIK_W) && m_RotState == ROTSTATE_DOWN))
	{
		Move.z += 1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
		Move.x += 1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

		m_bInput = true;
		m_RotState = ROTSTATE_UP;
	}
	else if (((pInputKeyboard->GetPress(DIK_S) && progress.bOKD && bGridCenter) ||
		(pInputKeyboard->GetPress(DIK_S) && m_RotState == ROTSTATE_UP)) &&
		pInputKeyboard->GetPress(DIK_W) == false)
	{
		Move.z += -1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
		Move.x += -1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

		m_bInput = true;
		m_RotState = ROTSTATE_DOWN;
	}
	else if ((pInputKeyboard->GetPress(DIK_A) && progress.bOKL && bGridCenter) ||
		(pInputKeyboard->GetPress(DIK_A) && m_RotState == ROTSTATE_RIGHT))
	{
		Move.x += -1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
		Move.z -= -1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

		m_bInput = true;
		m_RotState = ROTSTATE_LEFT;
	}
	else if (((pInputKeyboard->GetPress(DIK_D) && progress.bOKR && bGridCenter) ||
		(pInputKeyboard->GetPress(DIK_D) && m_RotState == ROTSTATE_LEFT)) &&
		pInputKeyboard->GetPress(DIK_A) == false)
	{
		Move.x += 1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
		Move.z -= 1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

		m_bInput = true;
		m_RotState = ROTSTATE_RIGHT;
	}

	return Move;
}

//**********************************************************************************************************
// �����_�����s���
//**********************************************************************************************************
//==========================================
// �R���X�g���N�^
//==========================================
CStateRandom::CStateRandom()
{
	m_State = STATE_RANDOM;
	m_RotState = ROTSTATE_NONE;

	// �i�s�����
	m_Progress.bOKR = false;	//�E
	m_Progress.bOKL = false;	//��
	m_Progress.bOKU = false;	//��
	m_Progress.bOKD = false;	//�� 

	// �O��̐i�s�����
	m_ProgressOld.bOKR = false;	//�E
	m_ProgressOld.bOKL = false;	//��
	m_ProgressOld.bOKU = false;	//��
	m_ProgressOld.bOKD = false;	//�� 

	m_bSwitchMove = false;             // �ړ�������ς��邩
}

//==========================================
// �j��
//==========================================
void CStateRandom::Release()
{

}

//==========================================
// �����_�����s���瑀��ɐ؂�ւ�
//==========================================
void CStateRandom::ControlRandom(CObjectCharacter* pCharacter)
{
	// �����Ԃɂ���
	pCharacter->ChangeMoveState(new CStateControl);
}

//==========================================
// �����_�����s����ǐՂɐ؂�ւ�
//==========================================
void CStateRandom::RandomAStar(CObjectCharacter* pCharacter)
{
	// �ǐՏ�Ԃɂ���
	pCharacter->ChangeMoveState(new CStateAStar);
}

//==========================================
// �����_�����s�����~�ɐ؂�ւ�
//==========================================
void CStateRandom::RandomStop(CObjectCharacter* pCharacter)
{
	// ��~��Ԃɂ���
	pCharacter->ChangeMoveState(new CStateStop);
}

//==========================================
// �L�����N�^�[�̈ړ�����
//==========================================
void CStateRandom::Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	// �ǂ̍��G����
	SearchWall(pCharacter, pos);

	// �ړ���������
	Rot(pCharacter, rot);

	// �ʒu�X�V����
	UpdatePos(pCharacter, pos);

	DebugProc::Print(DebugProc::POINT_LEFT, "�ړ����� : ");

	auto str = magic_enum::enum_name(m_RotState);

	DebugProc::Print(DebugProc::POINT_LEFT, str.data());
	DebugProc::Print(DebugProc::POINT_LEFT, "\n");
}

//==========================================
// �A�C�e���̈ړ�����
//==========================================
void CStateRandom::Move(CItem* pItem, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	// �ǂ̍��G����
	SearchWall(pItem, pos);

	// �ړ���������
	Rot(pItem, rot);

	// �ʒu�X�V����
	UpdatePos(pItem, pos);

	DebugProc::Print(DebugProc::POINT_LEFT, "�ړ����� : ");

	auto str = magic_enum::enum_name(m_RotState);

	DebugProc::Print(DebugProc::POINT_LEFT, str.data());
	DebugProc::Print(DebugProc::POINT_LEFT, "\n");
}

//====================================================================
// �ǂ̍��G����(�L�����N�^�[)
//====================================================================
void CStateRandom::SearchWall(CObjectCharacter* pCharacter, D3DXVECTOR3& pos)
{
	CMapSystem::GRID grid = pCharacter->GetGrid();

	m_ProgressOld = m_Progress;		// ���݂̐i�s�󋵂ɂ���

	CMapSystem* pMapSystem = CMapSystem::GetInstance(); // �}�b�v�V�X�e���̃C���X�^���X���擾
	/* GRID �\���̂ɂ���*/
	CMapSystem::GRID MaxGrid;
	MaxGrid.x = pMapSystem->GetWightMax();	// �}�b�v�̉���
	MaxGrid.z = pMapSystem->GetHeightMax(); // �}�b�v�̗���
	D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos(); // �X�N���[���ł���Ă镝

	/* ���g�̗אڂS�}�X�̃O���b�h */
	int nNumber[ROTSTATE_MAX];	// 4�����̗אڂ���O���b�h
	nNumber[ROTSTATE_LEFT] = grid.x - 1;		// ��
	nNumber[ROTSTATE_RIGHT] = grid.x + 1;		// �E
	nNumber[ROTSTATE_UP] = grid.z - 1;			// ��
	nNumber[ROTSTATE_DOWN] = grid.z + 1;		// ��

	/*
	* useful::RangeNumber(int , int , int);
	* ��O���������A�������͈͓̔��ɑ��݂��Ȃ��ꍇ�Ɋۂߍ��݂��s���֐�
	* ���������傫����Α������̒l��
	* ��������菬������Α������̒l���Ԃ��Ă���
	*/
	nNumber[ROTSTATE_LEFT] = useful::RangeNumber(MaxGrid.x, 0, nNumber[ROTSTATE_LEFT]);
	nNumber[ROTSTATE_RIGHT] = useful::RangeNumber(MaxGrid.x, 0, nNumber[ROTSTATE_RIGHT]);
	nNumber[ROTSTATE_UP] = useful::RangeNumber(MaxGrid.z, 0, nNumber[ROTSTATE_UP]);
	nNumber[ROTSTATE_DOWN] = useful::RangeNumber(MaxGrid.z, 0, nNumber[ROTSTATE_DOWN]);

	// �אڂS�}�X���ړ��\�����f����
	m_Progress.bOKL = !pMapSystem->GetGritBool(nNumber[ROTSTATE_LEFT], grid.z);
	m_Progress.bOKR = !pMapSystem->GetGritBool(nNumber[ROTSTATE_RIGHT], grid.z);
	m_Progress.bOKU = !pMapSystem->GetGritBool(grid.x, nNumber[ROTSTATE_UP]);
	m_Progress.bOKD = !pMapSystem->GetGritBool(grid.x, nNumber[ROTSTATE_DOWN]);

	// �O��ʂ����O���b�h�Ɠ����ꍇ�A�ʂ�Ȃ��悤�ɂ���
	if (nNumber[ROTSTATE_LEFT] == m_GridOld.x)
	{
		m_Progress.bOKL = false;
	}
	else if(nNumber[ROTSTATE_RIGHT] == m_GridOld.x)
	{
		m_Progress.bOKR = false;
	}
	else if (nNumber[ROTSTATE_UP] == m_GridOld.z)
	{
		m_Progress.bOKU = false;
	}
	else if (nNumber[ROTSTATE_DOWN] == m_GridOld.z)
	{
		m_Progress.bOKD = false;
	}

	if (grid != m_GridOld)
	{ // �O��̃O���b�h�ƈʒu���Ⴄ��
		if (m_Progress.bOKD != m_ProgressOld.bOKD || m_Progress.bOKL != m_ProgressOld.bOKL ||
			m_Progress.bOKR != m_ProgressOld.bOKR || m_Progress.bOKU != m_ProgressOld.bOKU)
		{ // �O��Ɛi�߂�������ς��ꍇ

			m_bSwitchMove = true;		// �����I������
		}
	}

	//�����̗����Ă���O���b�g�̒��S�ʒu�����߂�
	D3DXVECTOR3 MyGritPos = grid.ToWorld();
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "�G�̈ʒu %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "�G�̈ʒu : %d, %d\n", grid.x, grid.z);

	// ���g�̍��W�ƃO���b�h�̒��S�����e�͈͂����������ꍇ
	if (
		(pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) && // ���g�̍��W <= ���g�����݂��Ă���O���b�h�̒��S + �O���b�h�T�C�Y * 0.5f - ���e�͈�
		pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&

		pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
		pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK)))
	{// �O���b�g�̒��S�ʒu�ɗ����Ă���Ȃ瑀����󂯕t����

		if (!m_Progress.bOKR && !m_Progress.bOKL && !m_Progress.bOKU && !m_Progress.bOKD)
		{ // �����ꂩ�̐i�s��������ĂȂ��Ƃ�
			// �ҋ@��Ԃɂ���
			pCharacter->SetState(CObjectCharacter::STATE_WAIT);
			//m_RotState = ROTSTATE_WAIT;
		}

		if (m_bSwitchMove == true)
		{ // �����ς�����ꍇ

			// �ړ������̑I��
			MoveSelect(pCharacter);
			m_bSwitchMove = false;
			m_GridOld = grid;	// �O��̈ʒu�X�V

		}

	}
}

//====================================================================
// �ǂ̍��G����(�A�C�e��)
//====================================================================
void CStateRandom::SearchWall(CItem* pItem, D3DXVECTOR3& pos)
{
	CMapSystem::GRID grid = pItem->GetGrid();

	m_ProgressOld = m_Progress;		// ���݂̐i�s�󋵂ɂ���

	CMapSystem* pMapSystem = CMapSystem::GetInstance(); // �}�b�v�V�X�e���̃C���X�^���X���擾
	/* GRID �\���̂ɂ���*/
	CMapSystem::GRID MaxGrid;
	MaxGrid.x = pMapSystem->GetWightMax();	// �}�b�v�̉���
	MaxGrid.z = pMapSystem->GetHeightMax(); // �}�b�v�̗���

	/* ���g�̗אڂS�}�X�̃O���b�h */
	int nNumber[ROTSTATE_MAX];	// 4�����̗אڂ���O���b�h
	nNumber[ROTSTATE_LEFT] = grid.x - 1;		// ��
	nNumber[ROTSTATE_RIGHT] = grid.x + 1;		// �E
	nNumber[ROTSTATE_UP] = grid.z - 1;			// ��
	nNumber[ROTSTATE_DOWN] = grid.z + 1;		// ��

	/*
	* useful::RangeNumber(int , int , int);
	* ��O���������A�������͈͓̔��ɑ��݂��Ȃ��ꍇ�Ɋۂߍ��݂��s���֐�
	* ���������傫����Α������̒l��
	* ��������菬������Α������̒l���Ԃ��Ă���
	*/
	nNumber[ROTSTATE_LEFT] = useful::RangeNumber(MaxGrid.x, 0, nNumber[ROTSTATE_LEFT]);
	nNumber[ROTSTATE_RIGHT] = useful::RangeNumber(MaxGrid.x, 0, nNumber[ROTSTATE_RIGHT]);
	nNumber[ROTSTATE_UP] = useful::RangeNumber(MaxGrid.z, 0, nNumber[ROTSTATE_UP]);
	nNumber[ROTSTATE_DOWN] = useful::RangeNumber(MaxGrid.z, 0, nNumber[ROTSTATE_DOWN]);

	// �אڂS�}�X���ړ��\�����f����
	m_Progress.bOKL = !pMapSystem->GetGritBool(nNumber[ROTSTATE_LEFT], grid.z);
	m_Progress.bOKR = !pMapSystem->GetGritBool(nNumber[ROTSTATE_RIGHT], grid.z);
	m_Progress.bOKU = !pMapSystem->GetGritBool(grid.x, nNumber[ROTSTATE_UP]);
	m_Progress.bOKD = !pMapSystem->GetGritBool(grid.x, nNumber[ROTSTATE_DOWN]);

	// �O��ʂ����O���b�h�Ɠ����ꍇ�A�ʂ�Ȃ��悤�ɂ���
	if (nNumber[ROTSTATE_LEFT] == m_GridOld.x)
	{
		m_Progress.bOKL = false;
	}
	else if (nNumber[ROTSTATE_RIGHT] == m_GridOld.x)
	{
		m_Progress.bOKR = false;
	}
	else if (nNumber[ROTSTATE_UP] == m_GridOld.z)
	{
		m_Progress.bOKU = false;
	}
	else if (nNumber[ROTSTATE_DOWN] == m_GridOld.z)
	{
		m_Progress.bOKD = false;
	}

	if (grid != m_GridOld)
	{ // �O��̃O���b�h�ƈʒu���Ⴄ��
		if (m_Progress.bOKD != m_ProgressOld.bOKD || m_Progress.bOKL != m_ProgressOld.bOKL ||
			m_Progress.bOKR != m_ProgressOld.bOKR || m_Progress.bOKU != m_ProgressOld.bOKU)
		{ // �O��Ɛi�߂�������ς��ꍇ

			m_bSwitchMove = true;		// �����I������
		}
	}

	//�����̗����Ă���O���b�g�̒��S�ʒu�����߂�
	D3DXVECTOR3 MyGritPos = grid.ToWorld();
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "�G�̈ʒu %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "�G�̈ʒu : %d, %d\n", grid.x, grid.z);

	// ���g�̍��W�ƃO���b�h�̒��S�����e�͈͂����������ꍇ
	if (
		(pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) && // ���g�̍��W <= ���g�����݂��Ă���O���b�h�̒��S + �O���b�h�T�C�Y * 0.5f - ���e�͈�
			pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&

			pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
			pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK)))
	{// �O���b�g�̒��S�ʒu�ɗ����Ă���Ȃ瑀����󂯕t����

		if (m_bSwitchMove == true)
		{ // �����ς�����ꍇ

			// �ړ������̑I��
			MoveSelect(pItem);
			m_bSwitchMove = false;
			m_GridOld = grid;	// �O��̈ʒu�X�V

		}

	}
}

//====================================================================
// �ړ������̑I��(�L�����N�^�[)
//====================================================================
void CStateRandom::MoveSelect(CObjectCharacter* pCharacter)
{
	std::vector<D3DXVECTOR3> move = {};
	std::vector<ROTSTATE> RotState = {};

	D3DXVECTOR3 moveSave = pCharacter->GetMove();		// �ړ���
	float fAngle = 0.0f;	// ����
	
	// �e�ړ������̈ړ��ʐݒ�
	MoveAngle(moveSave, move, RotState);

	// �i�s�������������
	if (move.size() >= 2)
	{ // �i�s�ł��������2�ȏ゠��ꍇ

		int nRand = rand() % move.size();		// �i�s��������

		m_RotState = RotState[nRand];			// �����̏��

		// �ړ��ʐݒ�
		pCharacter->SetMove(move[nRand]);
	}
	else if(move.size() == 1)
	{
		m_RotState = RotState[0];			// �����̏��

		// �ړ��ʐݒ�
		pCharacter->SetMove(move[0]);
	}
	else if (move.empty())
	{ // ���ȊO�i�߂Ȃ���
		
		// �����̏�Ԑݒ�
		switch (m_RotState)
		{
		case CMoveState::ROTSTATE_LEFT:		// ��
			// �E�Ɉړ�
			fAngle = D3DX_PI * 0.5f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
			RotState.push_back(ROTSTATE_RIGHT);
			break;
		case CMoveState::ROTSTATE_RIGHT:	// �E
			// ���Ɉړ�
			fAngle = D3DX_PI * -0.5f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
			RotState.push_back(ROTSTATE_LEFT);

			break;
		case CMoveState::ROTSTATE_UP:		// ��
			// ���Ɉړ�
			fAngle = D3DX_PI * 1.0f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
			RotState.push_back(ROTSTATE_DOWN);

			break;
		case CMoveState::ROTSTATE_DOWN:		// ��
			// ��Ɉړ�
			fAngle = D3DX_PI * 0.0f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
			RotState.push_back(ROTSTATE_UP);

			break;

		case CMoveState::ROTSTATE_WAIT:		// �ҋ@
			// �~�܂�
			fAngle = D3DX_PI * 0.0f;
			move.push_back(D3DXVECTOR3(0.0f, moveSave.y, 0.0f));

			break;
		default:
			break;
		}

		// �����̏�Ԑݒ�
		m_RotState = RotState[0];

		// �ړ��ʐݒ�
		pCharacter->SetMove(move[0]);
	}

	m_ProgressOld = m_Progress;		// ���݂̐i�s�󋵂ɂ���
}

//====================================================================
// �ړ������̑I��(�A�C�e��)
//====================================================================
void CStateRandom::MoveSelect(CItem* pItem)
{
	std::vector<D3DXVECTOR3> move = {};
	std::vector<ROTSTATE> RotState = {};
	D3DXVECTOR3 moveSave = pItem->GetMove();		// �ړ���
	float fAngle = 0.0f;	// ����

	// �e�ړ������̈ړ��ʐݒ�
	MoveAngle(moveSave, move, RotState);

	// �i�s�������������
	if (move.size() >= 2)
	{ // �i�s�ł��������2�ȏ゠��ꍇ

		int nRand = rand() % move.size();		// �i�s��������

		m_RotState = RotState[nRand];			// �����̏��

		// �ړ��ʐݒ�
		pItem->SetMove(move[nRand]);
	}
	else if (move.size() == 1)
	{
		m_RotState = RotState[0];			// �����̏��

		// �ړ��ʐݒ�
		pItem->SetMove(move[0]);
	}
	else if (move.empty())
	{ // ���ȊO�i�߂Ȃ���

		// �����̏�Ԑݒ�
		switch (m_RotState)
		{
		case CMoveState::ROTSTATE_LEFT:		// ��
			// �E�Ɉړ�
			fAngle = D3DX_PI * 0.5f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * FRIEDEGG_SPEED, moveSave.y, cosf(fAngle) * FRIEDEGG_SPEED));
			RotState.push_back(ROTSTATE_RIGHT);
			break;
		case CMoveState::ROTSTATE_RIGHT:	// �E
			// ���Ɉړ�
			fAngle = D3DX_PI * -0.5f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * FRIEDEGG_SPEED, moveSave.y, cosf(fAngle) * FRIEDEGG_SPEED));
			RotState.push_back(ROTSTATE_LEFT);

			break;
		case CMoveState::ROTSTATE_UP:		// ��
			// ���Ɉړ�
			fAngle = D3DX_PI * 1.0f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * FRIEDEGG_SPEED, moveSave.y, cosf(fAngle) * FRIEDEGG_SPEED));
			RotState.push_back(ROTSTATE_DOWN);

			break;
		case CMoveState::ROTSTATE_DOWN:		// ��
			// ��Ɉړ�
			fAngle = D3DX_PI * 0.0f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * FRIEDEGG_SPEED, moveSave.y, cosf(fAngle) * FRIEDEGG_SPEED));
			RotState.push_back(ROTSTATE_UP);

			break;

		case CMoveState::ROTSTATE_WAIT:		// �ҋ@
			// �~�܂�
			fAngle = D3DX_PI * 0.0f;
			move.push_back(D3DXVECTOR3(0.0f, moveSave.y, 0.0f));

			break;
		default:
			break;
		}

		// �����̏�Ԑݒ�
		m_RotState = RotState[0];

		// �ړ��ʐݒ�
		pItem->SetMove(move[0]);
	}

	m_ProgressOld = m_Progress;		// ���݂̐i�s�󋵂ɂ���
}

//====================================================================
// �e�����̈ړ��ʐݒ�
//====================================================================
void CStateRandom::MoveAngle(D3DXVECTOR3& moveSave, std::vector<D3DXVECTOR3>& move, std::vector<ROTSTATE>& rotState)
{
	float fAngle = 0.0f;	// ����

		// �i�s�ł���������m�F
	if (m_Progress.bOKL/* && m_RotState != CMoveState::ROTSTATE_LEFT*/)
	{ // ��

		// �ړ������ݒ�
		fAngle = D3DX_PI * -0.5f;
		move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
		rotState.push_back(ROTSTATE_LEFT);

	}
	if (m_Progress.bOKR/* && m_RotState != CMoveState::ROTSTATE_RIGHT*/)
	{ // �E

		// �ړ������ݒ�
		fAngle = D3DX_PI * 0.5f;
		move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
		rotState.push_back(ROTSTATE_RIGHT);

	}
	if (m_Progress.bOKU/* && m_RotState != CMoveState::ROTSTATE_UP*/)
	{ // ��

		// �ړ������ݒ�
		fAngle = D3DX_PI * 0.0f;
		move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
		rotState.push_back(ROTSTATE_UP);

	}
	if (m_Progress.bOKD/* && m_RotState != CMoveState::ROTSTATE_DOWN*/)
	{ // ��

		// �ړ������ݒ�
		fAngle = D3DX_PI * 1.0f;
		move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
		rotState.push_back(ROTSTATE_DOWN);

	}

}

//**********************************************************************************************************
// �ǐՏ��
//**********************************************************************************************************
//==========================================
// �R���X�g���N�^
//==========================================
CStateAStar::CStateAStar()
{
	m_pPath = nullptr;
	m_EnemyType = CEnemy::ENEMY_NONE;
	m_fCoordinateTimer = COORDDINATE_RATE[m_EnemyType];
	m_nNumCoordinate = 0;
	m_nTargetIndex = 0;

	m_State = STATE_ASTAR;			// ������


}

//==========================================
// ����������
//==========================================
void CStateAStar::Init()
{
	m_fCoordinateTimer = COORDDINATE_RATE[m_EnemyType];
	m_nNumCoordinate = 0;
	m_nTargetIndex = 0;
}

//==========================================
// �j��
//==========================================
void CStateAStar::Release()
{
	// ���������폜
	if (m_pPath != nullptr) { delete[] m_pPath; };
}

//==========================================
// �ǐՂ��瑀��ɐ؂�ւ�
//==========================================
void CStateAStar::ControlAStar(CObjectCharacter* pCharacter)
{
	// �����Ԃɂ���
	pCharacter->ChangeMoveState(new CStateControl);
}

//==========================================
// �ǐՂ��烉���_�����s�ɐ؂�ւ�
//==========================================
void CStateAStar::RandomAStar(CObjectCharacter* pCharacter)
{
	// �����_�����s��Ԃɂ���
	pCharacter->ChangeMoveState(new CStateRandom);
}

//==========================================
// �ǐՂ����~�ɐ؂�ւ�
//==========================================
void CStateAStar::AStarStop(CObjectCharacter* pCharacter)
{
	// ��~��Ԃɂ���
	pCharacter->ChangeMoveState(new CStateStop);
}

//==========================================
// �L�����N�^�[�̈ړ�����
//==========================================
void CStateAStar::Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	// �ŒZ�o�H�T��
	Coordinate(pCharacter);

	// �ŒZ�o�H�����ǂ�
	Route(pCharacter);

	// �ړ���������
	Rot(pCharacter, rot);

	// �ʒu�X�V����
	UpdatePos(pCharacter, pos);
}

//==========================================
//  �ŒZ�o�H�T��
//==========================================
void CStateAStar::Coordinate(CObjectCharacter* pCharacter)
{
	CMapSystem::GRID grid = pCharacter->GetGrid();

	// �T���^�C�}�[�����Z
	m_fCoordinateTimer += DeltaTime::Get();

	// �T���̃^�C�~���O�łȂ��ꍇ�֐��𔲂���
	if (m_fCoordinateTimer < COORDDINATE_RATE[m_EnemyType]) { return; }

	// �ŒZ�o�H�̎��̖ڕW�����Z�b�g
	m_nTargetIndex = 1;

	// �^�C�}�[�̃��Z�b�g
	m_fCoordinateTimer -= COORDDINATE_RATE[m_EnemyType];

	// �ŒZ�o�H���擾
	AStar::CoordinateList Path = AStar::Generator::GetInstance()->FindPlayer({ grid.x, grid.z });
	m_nNumCoordinate = Path.size();

	// ���������폜
	if (m_pPath != nullptr) { delete[] m_pPath; };

	// �ŒZ�o�H�ɕK�v�ȃO���b�h�������������m��
	m_pPath = new CMapSystem::GRID[m_nNumCoordinate];

	// �m�ۂ����������ɍŒZ�o�H�̃O���b�h���i�[
	for (int i = 0; i < m_nNumCoordinate; ++i)
	{
		m_pPath[i] = Path.at(i);
	}
}

//==========================================
// �ŒZ�o�H�����ǂ�
//==========================================
void CStateAStar::Route(CObjectCharacter* pCharacter)
{
	// ���g�̈ʒu�E�ړ��ʎ擾
	D3DXVECTOR3 pos = pCharacter->GetPos();
	D3DXVECTOR3 move = pCharacter->GetMove();
	D3DXVECTOR3 rot = pCharacter->GetRot();

	// �ŒZ�o�H�������Ƃ�
	if (m_pPath == nullptr)
	{
		return;
	}

	// �ڕW�n�_�̍��W�����߂�
	D3DXVECTOR3 path = m_pPath[m_nTargetIndex].ToWorld();

	// ���Ɍ������O���b�h�ɏd�Ȃ����炻�̎��̖ڕW��ݒ�
	if (fabsf(path.x - pos.x) <= TARGET_DIFF &&
		fabsf(path.z - pos.z) <= TARGET_DIFF) // ���͈͓��ł����
	{
		// �C���f�b�N�X�ԍ������Z
		m_nTargetIndex++;
		path = m_pPath[m_nTargetIndex].ToWorld();
	}

	// ���̖ڕW�����݂��Ȃ�������֐��𔲂���
	if (m_nTargetIndex >= m_nNumCoordinate)
	{
		pCharacter->ChangeMoveState(new CStateRandom);		// �����_����Ԃɂ���
		return;
	}

	// ���̖ڕW�ʒu�Ƃ̊p�x
	float RotDest = atan2f(path.z - pos.z, path.x - pos.x);

	// ���̖ڕW�ʒu�Ɉړ�
	move = path - pos;
	D3DXVec3Normalize(&move, &move);
	move *= DeltaTime::Get() * MOVE_ASTAR;

	// �ʒu�X�V
	pos += move;

	//�ړI�̌���
	float DiffRot = (RotDest - rot.y) * 0.1f;
	rot.y += DiffRot;

	// �ʒu�E�ړ��ʐݒ�
	pCharacter->SetPos(pos);
	pCharacter->SetMove(move);
	pCharacter->SetRot(rot);
}

//**********************************************************************************************************
// ��~���
//**********************************************************************************************************
//==========================================
// �j��
//==========================================
void CStateStop::Release()
{
	
}

//==========================================
// ��~���瑀��ɐ؂�ւ�
//==========================================
void CStateStop::ControlStop(CObjectCharacter* pCharacter)
{
	// �����Ԃɂ���
	pCharacter->ChangeMoveState(new CStateControl);
}

//==========================================
// ��~���烉���_�����s�ɐ؂�ւ�
//==========================================
void CStateStop::RandomStop(CObjectCharacter* pCharacter)
{
	// �����_�����s��Ԃɂ���
	pCharacter->ChangeMoveState(new CStateRandom);
}

//==========================================
// ��~����ǐՂɐ؂�ւ�
//==========================================
void CStateStop::AStarStop(CObjectCharacter* pCharacter)
{
	// �ǐՏ�Ԃɂ���
	pCharacter->ChangeMoveState(new CStateAStar);
}

//==========================================
// �]����ƒ�~�؂�ւ�
//==========================================
void CStateStop::RollStop(CObjectX* pObjectX)
{
	// �]�����Ԃɂ���
	pObjectX->ChangeMoveState(new CStateRoll);
}

//==========================================
// �L�����N�^�[�̈ړ�����
//==========================================
void CStateStop::Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{

}

//**********************************************************************************************************
// �]������
//**********************************************************************************************************
//==========================================
// �R���X�g���N�^
//==========================================
CStateRoll::CStateRoll()
{
	m_State = STATE_ROLL;

	// �i�s�����
	m_Progress.bOKR = false;	//�E
	m_Progress.bOKL = false;	//��
	m_Progress.bOKU = false;	//��
	m_Progress.bOKD = false;	//�� 

	// �O��̐i�s�����
	m_ProgressOld.bOKR = false;	//�E
	m_ProgressOld.bOKL = false;	//��
	m_ProgressOld.bOKU = false;	//��
	m_ProgressOld.bOKD = false;	//�� 
}
//==========================================
// �j��
//==========================================
void CStateRoll::Release()
{

}

//==========================================
// �]����ƒ�~�؂�ւ�
//==========================================
void CStateRoll::RollStop(CObjectX* pObjectX)
{
	// ��~��Ԃɂ���
	pObjectX->ChangeMoveState(new CStateStop);
}

//==========================================
// �]����̈ړ�����
//==========================================
void CStateRoll::Move(CObjectX* pObjectX, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	D3DXVECTOR3 SlopeRot = CMapSystem::GetInstance()->GetMove()->GetDevilRot();		// �}�b�v�̌X��
	D3DXVECTOR3 move = pObjectX->GetMove();			// �ړ���
	CMapSystem::GRID grid = pObjectX->GetGrid();	// �O���b�h
	CObject::OBJECT_TYPE type = pObjectX->GetType();	// �I�u�W�F�N�g�̎��

	// �X���ɂ��ړ��ʐݒ�
	move.x = -SlopeRot.z * 10.0f;
	move.z = SlopeRot.x * 10.0f;

	//�����̗����Ă���O���b�g�̒��S�ʒu�����߂�
	D3DXVECTOR3 MyGritPos = pObjectX->GetGrid().ToWorld();
	float MapGritSize = CMapSystem::GetInstance()->GetGritSize();

	// �ړ�����
	if (useful::CollisionCircle(MyGritPos, D3DXVECTOR3(pos.x, MyGritPos.y, pos.z), 5.0f) == true)
	{// �u���b�N�̒��S�ɂ��鎞�ɏ㉺�����E�̂ǂ��炩�ɂȂ�܂łɈړ�����(�X���ĂȂ���)

		pos.x += move.x;
		pos.z += move.z;
	}
	else
	{// �u���b�N�̒��S�ɂȂ��Ƃ�(�X������)

		//�㉺�ړ�
		if (MyGritPos.x - pos.x >= -5.0f && MyGritPos.x - pos.x <= 5.0f)
		{
			pos.z += move.z;
		}
		else
		{
			pos.z = MyGritPos.z;
		}

		//���E�ړ�
		if (MyGritPos.z - pos.z >= -5.0f && MyGritPos.z - pos.z <= 5.0f)
		{
			pos.x += move.x;
		}
		else
		{
			pos.x = MyGritPos.x;
		}
	}

	// ��~���Ă��邩������Ƃ�
	if (pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - (GRIT_OK * m_Progress.bOKR)) &&	//��
		pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - (GRIT_OK * m_Progress.bOKL)) &&	//�E
		pos.z <= MyGritPos.z + ((MapGritSize * 0.5f)) &&	//��
		pos.z >= MyGritPos.z - ((MapGritSize * 0.5f)))	//��
	{// �O���b�g�̒��S�ʒu�ɗ����Ă��邩

		int nRGridX = grid.x + 1;
		int nLGridX = grid.x - 1;

		nRGridX = useful::RangeNumber(CMapSystem::GetInstance()->GetWightMax(), 0, nRGridX);
		nLGridX = useful::RangeNumber(CMapSystem::GetInstance()->GetWightMax(), 0, nLGridX);

		// ���E�̈ړ�����
		if (type == CObject::TYPE_ROLLROCK)
		{ // ��
			SetJudg(nRGridX, grid.z, m_Progress.bOKR);
			SetJudg(nLGridX, grid.z, m_Progress.bOKL);
		}
		else if (type == CObject::TYPE_RAILBLOCK)
		{ // ���[���u���b�N
			RailCheck(CMapSystem::GRID(nRGridX, grid.z), m_Progress.bOKR);
			RailCheck(CMapSystem::GRID(nLGridX, grid.z), m_Progress.bOKL);
		}
	}

	if (pos.x <= MyGritPos.x + ((MapGritSize * 0.5f)) &&	//��
		pos.x >= MyGritPos.x - ((MapGritSize * 0.5f)) &&	//�E
		pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - (GRIT_OK * m_Progress.bOKD)) &&	//��
		pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - (GRIT_OK * m_Progress.bOKU)))	//��
	{// �O���b�g�̒��S�ʒu�ɗ����Ă��邩

		int nUGridZ = grid.z - 1;
		int nDGridZ = grid.z + 1;

		nUGridZ = useful::RangeNumber(CMapSystem::GetInstance()->GetHeightMax(), 0, nUGridZ);
		nDGridZ = useful::RangeNumber(CMapSystem::GetInstance()->GetHeightMax(), 0, nDGridZ);

		// �㉺�̈ړ�����
		if (type == CObject::TYPE_ROLLROCK)
		{
			SetJudg(grid.x, nUGridZ, m_Progress.bOKU);
			SetJudg(grid.x, nDGridZ, m_Progress.bOKD);
		}
		else if (type == CObject::TYPE_RAILBLOCK)
		{ // ���[���u���b�N
			RailCheck(CMapSystem::GRID(grid.x, nUGridZ), m_Progress.bOKU);
			RailCheck(CMapSystem::GRID(grid.x, nDGridZ), m_Progress.bOKD);
		}
	}

	// ��~����
	Stop(pos, MyGritPos, move);

	pObjectX->SetMove(move);	// �ړ��ʐݒ�
}

//==========================================
// ���[�������邩�m�F
//==========================================
void CStateRoll::RailCheck(CMapSystem::GRID& grid, bool& bProgress)
{
	// ���[���u���b�N�����邩��������
	if (CMapSystem::GetInstance()->GetRailGritBool(grid) == true)
	{
		bProgress = true;		// �]���邱�Ƃ��o����
	}
	else
	{
		bProgress = false;		// �]���邱�Ƃ��o���Ȃ�
	}
}

//==========================================
// �]����邩���f���鏈��
//==========================================
void CStateRoll::SetJudg(int& nGridPosX, int& nGridPosZ, bool& bProgress)
{
	// �]�����ꏊ�����邩��������
	if (CMapSystem::GetInstance()->GetGritBool(nGridPosX, nGridPosZ) == true)
	{
		bProgress = false;		// �]���邱�Ƃ��o���Ȃ�
	}
	else
	{
		bProgress = true;		// �]���邱�Ƃ��o����
	}
}

//==========================================
// �]����Ȃ��Ƃ��̈ʒu
//==========================================
void CStateRoll::Stop(D3DXVECTOR3& pos, D3DXVECTOR3& GridPos, D3DXVECTOR3& move)
{
	// ��~���Ă���ꍇ�A���̈ʒu�ɌŒ肷��
	if (!m_Progress.bOKR && move.x > 0.0f)
	{
		if (pos.x > GridPos.x)
		{
			pos.x = GridPos.x;
			move.x = 0.0f;
		}
	}
	if (!m_Progress.bOKL && move.x < 0.0f)
	{
		if (pos.x < GridPos.x)
		{
			pos.x = GridPos.x;
			move.x = 0.0f;
		}
	}
	if (!m_Progress.bOKU && move.z > 0.0f)
	{
		if (pos.z > GridPos.z)
		{
			pos.z = GridPos.z;
			move.z = 0.0f;
		}
	}
	if (!m_Progress.bOKD && move.z < 0.0f)
	{
		if (pos.z < GridPos.z)
		{
			pos.z = GridPos.z;
			move.z = 0.0f;
		}
	}
}
