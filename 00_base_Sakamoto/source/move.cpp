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
#include "DevilHole.h"

#include "camera.h"
#include "MapSystem.h"
#include "useful.h"

//==========================================
// �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 EGG_MOVE = D3DXVECTOR3(10.0f, 10.0f, 10.0f);	 //�ړ��ʂ̌������x(��)
	const int INVINCIBLE_TIME = 120;		//���G����
	const float PLAYER_SPEED = 5.0f;		//�v���C���[�̈ړ����x
	const float ENEMY_SPEED = 3.0f;			//�G�̈ړ����x

	const float COORDDINATE_RATE[] =		// �o�H�T�����s���Ԋu
	{
		5.0f,
		3.0f,
		1.0f
	};

	const float TARGET_DIFF = 10.0f;		// ���e�͈�
	const float MOVE_ASTAR = 150.0f;		// �ǐՎ��̈ړ����x

	const float GRIT_OK = 45.0f;			//�ړ��\�ȃO���b�g�͈͓̔�

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
// �ړ���������
//====================================================================
void CMoveState::Rot(CObjectCharacter* pCharacter, D3DXVECTOR3& rot)
{
	//�L�[�{�[�h�̎擾
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
	D3DXVECTOR3 move = pCharacter->GetMove();

	//�ړ������Ɍ��������킹�鏈��
	/*float fRotMove, fRotDest;
	fRotMove = rot.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;*/

	rot.y = atan2f(-move.x, -move.z);

	useful::NormalizeAngle(&rot);
}

//====================================================================
// �ʒu�X�V����
//====================================================================
void CMoveState::UpdatePos(CObjectCharacter* pCharacter, D3DXVECTOR3& pos)
{
	// �ϐ��錾
	CDevil* pDevil = CDevil::GetListTop();
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
	pos.x += move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX(move.x);
	pos.x += objMove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX(move.x);

	// �ǂƂ̓����蔻��
	//CollisionWall(pos, posOldMy, sizeMy, useful::COLLISION_X);
	//CollisionDevilHole(useful::COLLISION_X);

	//Z���̈ʒu�X�V
	pos.z += move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ(move.z);
	pos.z += objMove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ(move.z);

	// �ǂƂ̓����蔻��
	//CollisionWall(pos, posOldMy, sizeMy, useful::COLLISION_Z);
	//CollisionDevilHole(useful::COLLISION_Z);

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
	m_SelectGrid.x = 0;
	m_SelectGrid.z = 0;
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

	// �ړ������̑I��
	MoveSelect(pCharacter);

	// �ړ���������
	Rot(pCharacter, rot);

	// �ʒu�X�V����
	UpdatePos(pCharacter, pos);
}

//====================================================================
// �ǂ̍��G����(�ړ��I���̏���)
//====================================================================
void CStateRandom::SearchWall(CObjectCharacter* pCharacter, D3DXVECTOR3& pos)
{
	CMapSystem::GRID grid = pCharacter->GetGrid();
	CObjectCharacter::PROGGRESS progress = pCharacter->GetProgress();	// �ړ��̐i�s����

	bool OKR = true;	//�E
	bool OKL = true;	//��
	bool OKU = true;	//��
	bool OKD = true;	//��

	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	int nMapWightMax = pMapSystem->GetWightMax();
	int nMapHeightMax = pMapSystem->GetHeightMax();
	D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

	int nRNumber = grid.x + 1;
	int nLNumber = grid.x - 1;
	int nUNumber = grid.z - 1;
	int nDNumber = grid.z + 1;

	nRNumber = useful::RangeNumber(nMapWightMax, 0, nRNumber);
	nLNumber = useful::RangeNumber(nMapWightMax, 0, nLNumber);
	nUNumber = useful::RangeNumber(nMapHeightMax, 0, nUNumber);
	nDNumber = useful::RangeNumber(nMapHeightMax, 0, nDNumber);

	OKR = !pMapSystem->GetGritBool(nRNumber, grid.z);
	OKL = !pMapSystem->GetGritBool(nLNumber, grid.z);
	OKU = !pMapSystem->GetGritBool(grid.x, nUNumber);
	OKD = !pMapSystem->GetGritBool(grid.x, nDNumber);

	//�����̗����Ă���O���b�g�̒��S�ʒu�����߂�
	D3DXVECTOR3 MyGritPos = grid.ToWorld();
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "�G�̈ʒu %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if ((pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) &&
		pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&
		pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
		pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK)) &&
		(grid.x != m_SelectGrid.x || grid.z != m_SelectGrid.z))
	{// �O���b�g�̒��S�ʒu�ɗ����Ă���Ȃ瑀����󂯕t����

		if (!progress.bOKR && OKR)
		{
			pCharacter->SetState(CObjectCharacter::STATE_WAIT);
		}
		if (!progress.bOKL && OKL)
		{
			pCharacter->SetState(CObjectCharacter::STATE_WAIT);
		}
		if (!progress.bOKU && OKU)
		{
			pCharacter->SetState(CObjectCharacter::STATE_WAIT);
		}
		if (!progress.bOKD && OKD)
		{
			pCharacter->SetState(CObjectCharacter::STATE_WAIT);
		}

		progress.bOKR = OKR;	//�E
		progress.bOKL = OKL;	//��
		progress.bOKU = OKU;	//��
		progress.bOKD = OKD;	//��
	}
	else
	{
		progress.bOKR = false;	//�E
		progress.bOKL = false;	//��
		progress.bOKU = false;	//��
		progress.bOKD = false;	//��
	}
}

//====================================================================
// �ړ������̑I��
//====================================================================
void CStateRandom::MoveSelect(CObjectCharacter* pCharacter)
{
	float OKRot[4];
	int RotNumber = 0;
	CObjectCharacter::PROGGRESS progress = pCharacter->GetProgress();	// �ړ��̐i�s����
	D3DXVECTOR3 move = pCharacter->GetMove();		// �ړ���
	m_RotState = GetRotState();						// �ړ������̏��

	// �i�s�ł���������m�F
	if (progress.bOKL && m_RotState != CMoveState::ROTSTATE_RIGHT)
	{
		OKRot[RotNumber] = D3DX_PI * -0.5f;
		RotNumber++;
	}
	if (progress.bOKR && m_RotState != CMoveState::ROTSTATE_LEFT)
	{
		OKRot[RotNumber] = D3DX_PI * 0.5f;
		RotNumber++;
	}
	if (progress.bOKU && m_RotState != CMoveState::ROTSTATE_DOWN)
	{
		OKRot[RotNumber] = D3DX_PI * 0.0f;
		RotNumber++;
	}
	if (progress.bOKD && m_RotState != CMoveState::ROTSTATE_UP)
	{
		OKRot[RotNumber] = D3DX_PI * 1.0f;
		RotNumber++;
	}

	// �i�s�ł������������
	if (RotNumber != 0)
	{
		int nRand = rand() % RotNumber;

		move.x = sinf(OKRot[nRand]) * 3.0f;
		move.z = cosf(OKRot[nRand]) * 3.0f;

		if (move.x >= 3.0f)
		{
			m_RotState = CMoveState::ROTSTATE_RIGHT;
		}
		else if (move.x <= -3.0f)
		{
			m_RotState = CMoveState::ROTSTATE_LEFT;
		}
		else if (move.z >= 3.0f)
		{
			m_RotState = CMoveState::ROTSTATE_UP;
		}
		else if (move.z <= -3.0f)
		{
			m_RotState = CMoveState::ROTSTATE_DOWN;
		}

		m_SelectGrid = pCharacter->GetGrid();
	}

	// �ړ��ʐݒ�
	pCharacter->SetMove(move);
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
	m_fCoordinateTimer = 0.0f;
	m_nNumCoordinate = 0;
	m_nTargetIndex = 0;

	m_State = STATE_ASTAR;			// ������


}

//==========================================
// ����������
//==========================================
void CStateAStar::Init()
{
	m_fCoordinateTimer = 0.0f;
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
	Init();

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
// �L�����N�^�[�̈ړ�����
//==========================================
void CStateStop::Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{

}
