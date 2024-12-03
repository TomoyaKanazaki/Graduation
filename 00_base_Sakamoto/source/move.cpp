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

}

//**********************************************************************
// �ړ���Ԃ̃C���^�[�t�F�[�X
//**********************************************************************
//==========================================
// ���쏈��
//==========================================
void CMoveState::Control(CObjectCharacter* pCharacter)
{

}

//==========================================
// ���쏈��(�v���C���[)
//==========================================
void CMoveState::Control(CPlayer* pPlayer, D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis)
{
	CPlayer::STATE state = pPlayer->GetState();		// �v���C���[�̏��
	CPlayer::MOVE_STATE moveState = pPlayer->GetMoveState();	// �v���C���[�̈ړ������̏��
	D3DXVECTOR3 EggMove = pPlayer->GetEggMove();	// �v���C���[�̗��ړ���
	float fSpeed = pPlayer->GetSpeed();				// �v���C���[�̑���
	bool bInput = false;							// �v���C���[�̓��͔���

	// ���͂��ĂȂ���Ԃɂ���
	pPlayer->SetInput(bInput);

	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���͏���
	NormarizeMove = InputKey(pPlayer, posThis, rotThis, NormarizeMove, fSpeed);

	/*NormarizeMove = MoveInputPadStick(posThis, rotThis, NormarizeMove);

	NormarizeMove = MoveInputPadKey(posThis, rotThis, NormarizeMove);*/

	if (bInput && state != CPlayer::STATE_ATTACK)
	{
		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= fSpeed;
		NormarizeMove.z *= fSpeed;

		//�ړ��ʂ���
		pPlayer->GetMove() = NormarizeMove;

		if (state == CPlayer::STATE_EGG)
		{
			// ���f�����̎擾
			int nNumModel = pPlayer->GetNumModel();

			for (int nCnt = 0; nCnt < nNumModel; nCnt++)
			{
				// ���f���̎擾
				CModel* pModel = pPlayer->GetModel(nCnt);

				if (pModel != nullptr)
				{
					pModel->SetDisp(true);
				}
			}

			EggMove.y = EGG_MOVE.y;

			switch (moveState)
			{
			case CPlayer::MOVE_STATE_LEFT:
				EggMove.x = EGG_MOVE.x;
				EggMove.z = 0.0f;
				break;
			case CPlayer::MOVE_STATE_RIGHT:
				EggMove.x = -EGG_MOVE.x;
				EggMove.z = 0.0f;
				break;
			case CPlayer::MOVE_STATE_UP:
				EggMove.x = 0.0f;
				EggMove.z = -EGG_MOVE.z;
				break;
			case CPlayer::MOVE_STATE_DOWN:
				EggMove.x = 0.0f;
				EggMove.z = EGG_MOVE.z;
				break;
			}

			// ���̈ړ��ʐݒ�
			pPlayer->SetEggMove(EggMove);

			//���G��Ԃ̐ݒ�
			pPlayer->SetInvincible(true);
			pPlayer->SetInvincibleCount(INVINCIBLE_TIME);
		}
		//�ړ���Ԃɂ���
		pPlayer->SetState(CPlayer::STATE_WALK);
	}
}

//====================================================================
//�ړ����̓L�[�{�[�h
//====================================================================
D3DXVECTOR3 CMoveState::InputKey(CObjectCharacter* pCharacter, D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis, D3DXVECTOR3 Move, float fSpeed)
{
	////�L�[�{�[�h�̎擾
	//CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	////�L�[�{�[�h�̈ړ�����
	//if ((pInputKeyboard->GetPress(DIK_W) && m_OKU && m_bGritCenter) ||
	//	(pInputKeyboard->GetPress(DIK_W) && m_MoveState == CPlayer::MOVE_STATE_DOWN))
	//{
	//	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
	//	D3DXMATRIX mat = *GetUseMultiMatrix();
	//	D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
	//	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotThis);

	//	Move.z += 1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
	//	Move.x += 1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

	//	m_bInput = true;
	//	m_MoveState = MOVE_STATE_UP;
	//}
	//else if (((pInputKeyboard->GetPress(DIK_S) && m_OKD && m_bGritCenter) ||
	//	(pInputKeyboard->GetPress(DIK_S) && m_MoveState == MOVE_STATE_UP)) &&
	//	pInputKeyboard->GetPress(DIK_W) == false)
	//{
	//	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
	//	D3DXMATRIX mat = *GetUseMultiMatrix();
	//	D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
	//	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotThis);

	//	Move.z += -1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
	//	Move.x += -1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

	//	m_bInput = true;
	//	m_MoveState = MOVE_STATE_DOWN;
	//}
	//else if ((pInputKeyboard->GetPress(DIK_A) && m_OKL && m_bGritCenter) ||
	//	(pInputKeyboard->GetPress(DIK_A) && m_MoveState == MOVE_STATE_RIGHT))
	//{
	//	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
	//	D3DXMATRIX mat = *GetUseMultiMatrix();
	//	D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
	//	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotThis);

	//	Move.x += -1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
	//	Move.z -= -1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

	//	m_bInput = true;
	//	m_MoveState = MOVE_STATE_LEFT;
	//}
	//else if (((pInputKeyboard->GetPress(DIK_D) && m_OKR && m_bGritCenter) ||
	//	(pInputKeyboard->GetPress(DIK_D) && m_MoveState == MOVE_STATE_LEFT)) &&
	//	pInputKeyboard->GetPress(DIK_A) == false)
	//{
	//	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
	//	D3DXMATRIX mat = *GetUseMultiMatrix();
	//	D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
	//	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotThis);

	//	Move.x += 1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
	//	Move.z -= 1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

	//	m_bInput = true;
	//	m_MoveState = MOVE_STATE_RIGHT;
	//}

	return Move;
}

//**********************************************************************
// ����ł�����
//**********************************************************************
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
void CStateControl::Move(CObjectCharacter* pCharacter)
{
	// ����ł��鏈��
	Control(pCharacter);
}

//==========================================
// �ړ�����(�v���C���[)
//==========================================
void CStateControl::Move(CPlayer* pPlayer, D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis)
{
	// ����ł��鏈��
	Control(pPlayer, posThis, rotThis);
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
