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
#include "player.h"
#include "camera.h"

//==========================================
// �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 EGG_MOVE = D3DXVECTOR3(10.0f, 10.0f, 10.0f);	 //�ړ��ʂ̌������x(��)
	const int INVINCIBLE_TIME = 120;			//���G����
	const float PLAYER_SPEED = 5.0f;		//�v���C���[�̈ړ����x
	const float ENEMY_SPEED = 3.0f;			//�G�̈ړ����x

}

//**********************************************************************
// �ړ���Ԃ̃C���^�[�t�F�[�X
//**********************************************************************
//==========================================
// �R���X�g���N�^
//==========================================
CMoveState::CMoveState()
{
	
}

//**********************************************************************
// ����ł�����
//**********************************************************************
//==========================================
// �R���X�g���N�^
//==========================================
CStateControl::CStateControl()
{
	m_bInput = false;				//���͂��s�������ǂ���
	m_RotState = ROTSTATE_NONE;		// �ړ������̏��
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
		break;

	default:
		break;
	}
}

//==========================================
// �v���C���[�̏�ԍX�V����
//==========================================
void CStateControl::UpdateMovePlayer(CObjectCharacter* pCharacter, D3DXVECTOR3 NormarizeMove)
{
	CPlayer::STATE state = pCharacter->GetState();		// �v���C���[�̏��
	D3DXVECTOR3 EggMove = pCharacter->GetEggMove();	// �v���C���[�̗��ړ���

	if (m_bInput && state != CPlayer::STATE_ATTACK)
	{
		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.z *= PLAYER_SPEED;

		//�ړ��ʂ���
		pCharacter->SetMove(NormarizeMove);

		if (state == CPlayer::STATE_EGG)
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
		}
		//�ړ���Ԃɂ���
		pCharacter->SetState(CObjectCharacter::STATE_WALK);
	}
}

//==========================================
// �G�̏�ԍX�V����
//==========================================
void CStateControl::UpdateMoveEnemy(CObjectCharacter* pCharacter, D3DXVECTOR3 NormarizeMove)
{
	D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

	NormarizeMove.x *= ENEMY_SPEED;
	NormarizeMove.z *= ENEMY_SPEED;

	//�ړ��ʂ���
	pCharacter->SetMove(NormarizeMove);

	//�ړ���Ԃɂ���
	pCharacter->SetState(CObjectCharacter::STATE_WALK);
}

//====================================================================
//�ړ����̓L�[�{�[�h(�L�����N�^�[)
//====================================================================
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

//**********************************************************************
// �����_�����s���
//**********************************************************************
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

//**********************************************************************
// �ǐՏ��
//**********************************************************************
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

//**********************************************************************
// ��~���
//**********************************************************************
//==========================================
// ��~���瑀��ɐ؂�ւ�
//==========================================
void CStateStop::ControlStop(CObjectCharacter* pCharacter)
{
	// �����Ԃɂ���
	pCharacter->ChangeMoveState(new CStateControl);

	////
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
// �ړ���Ԃ̏��擾
//==========================================
CMoveState* CMoveState::GetMoveState(CObjectCharacter* pCharacter)
{
	return pCharacter->GetMoveState();		// ���݂̃L�����N�^�[��Ԃ̏���Ԃ�
}
