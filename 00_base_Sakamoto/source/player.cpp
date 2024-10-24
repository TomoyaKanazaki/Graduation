//============================================
//
//	�v���C���[�̏��� [player.cpp]
//	Author:sakamoto kai
//
//============================================
#include "player.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "debugproc.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"
#include "enemy.h"
#include "objGauge2D.h"
#include "CubeBlock.h"
#include "CubeColl.h"
#include "slowManager.h"
#include "Number.h"
#include "MapModel.h"
#include "effect.h"
#include "sound.h"
#include "LifeUi.h"
#include "cross.h"
#include "bowabowa.h"
#include "fire.h"
#include "DevilHole.h"
#include "devil.h"
#include "MapSystem.h"

//===========================================
// �萔��`
//===========================================
namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(45.0f, 40.0f, 45.0f);		//���̓����蔻��
	const float PLAYER_SPEED = 5.0f;		//�v���C���[�̈ړ����x
	const int LIFE_MAX = 2;	//�������C�t��

	const D3DXVECTOR3 LIFE_POS = D3DXVECTOR3(50.0f, 650.0f, 0.0f);
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CPlayer>* CPlayer::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{
	SetSize(COLLISION_SIZE);
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_bJump = false;
	m_nActionCount = 0;
	m_Action = ACTION_BWAIT;
	m_AtkAction = ACTION_BWAIT;
	m_State = STATE_EGG;
	m_nStateCount = 0;
	m_AtkPos = INITVECTOR3;
	m_CollisionRot = 0.0f;
	m_pMotion = nullptr;
	m_OKL = true;
	m_OKR = true;
	m_OKU = true;
	m_OKD = true;
	m_bInput = false;
	m_UseItem = false;
	m_pLifeUi = nullptr;
	m_nLife = LIFE_MAX;
	m_eItemType = TYPE_NONE;
	m_MoveState = MOVE_STATE_WAIT;
	m_nMapWight = 0;
	m_nMapHeight = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CPlayer::~CPlayer()
{

}

//====================================================================
//��������
//====================================================================
CPlayer* CPlayer::Create()
{
	CPlayer* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//�v���C���[�̐���
		pPlayer = new CPlayer();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pPlayer->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pPlayer;
}

//====================================================================
//����������
//====================================================================
HRESULT CPlayer::Init(void)
{
	if (CScene::GetMode() == CScene::MODE_GAME ||
		CScene::GetMode() == CScene::MODE_TUTORIAL)
	{
		MyObjCreate();
	}

	//��ސݒ�
	SetType(CObject::TYPE_PLAYER3D);

	//���f���̐���
	LoadLevelData("data\\TXT\\motion_tamagon.txt");

	// �v���C���[�̎w��p�[�c�폜
	SetPartsDisp(3, false);
	SetPartsDisp(0, false);

	//���[�V�����̐���
	if (m_pMotion == nullptr)
	{
		//���[�V�����̐���
		m_pMotion = new CMotion;
	}

	//����������
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData("data\\TXT\\motion_tamagon.txt");

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		break;

	case CScene::MODE_RESULT:
		break;
	}

	m_pLifeUi = CLifeUi::Create();

	// ���l
	m_pLifeUi->GetNumber()->SetPos(D3DXVECTOR3(LIFE_POS.x + 200.0f, LIFE_POS.y, LIFE_POS.z));

	// �̗�
	m_pLifeUi->SetPos(LIFE_POS);
	m_pLifeUi->GetNumber()->SetNumber(m_nLife);

	// �X���[�̐���
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_PLAYER, CSlowManager::TAG_PLAYER);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CPlayer>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�������ێ�����I�u�W�F�N�g�̐���
//====================================================================
void CPlayer::MyObjCreate(void)
{
	//�I�u�W�F�N�g����
}

//====================================================================
//�I������
//====================================================================
void CPlayer::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Uninit();
		delete m_apModel[nCntModel];
		m_apModel[nCntModel] = nullptr;
	}

	//���[�V�����̏I������
	if (m_pMotion != nullptr)
	{
		//���[�V�����̔j��
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CPlayer::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
		GameUpdate();
		break;

	case CScene::MODE_TUTORIAL:
		TutorialUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//�^�C�g���ł̍X�V����
//====================================================================
void CPlayer::TitleUpdate(void)
{
	//���[�V�����̍X�V
	m_pMotion->Update();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CPlayer::GameUpdate(void)
{
	// �ߋ��̈ʒu�ɑ��
	m_posOld = m_pos;

	if (m_State != STATE_DEATH)
	{
		//�ǂ����邩���f
		SearchWall();

		// �ړ�����
		Move();

		// �����ړ�����
		Rot();

		if (m_UseItem)
		{
			Attack();
		}

		// �J�����X�V����
		CameraPosUpdate();

		// �ʒu�X�V����
		PosUpdate();

		if (m_State != STATE_EGG)
		{
			//��ʊO����
			CollisionStageOut();
		}

		// �G�̔���
		CollisionEnemy();
	}

	// �v���C���[���}�b�v�̂ǂ̃}�X�ɑ��݂��Ă��邩�ݒ肷��
	MapSystemNumber();

	//��Ԃ̊Ǘ�
	StateManager();

	if (m_pMotion != nullptr)
	{
		//���[�V�����̍X�V
		m_pMotion->Update();
	}

	//���[�V�����̊Ǘ�
	ActionState();

	//�f�o�b�O�L�[�̏����Ɛݒ�
	DebugKey();

	//�f�o�b�O�\��
	DebugProc::Print(DebugProc::POINT_LEFT,"[����]�ʒu %f : %f : %f\n", m_pos.x, m_pos.y, m_pos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[����]���� %f : %f : %f\n", m_rot.x, m_rot.y, m_rot.z);
	DebugProc::Print(DebugProc::POINT_LEFT,"[����]�� %d : �c %d\n", m_nMapWight, m_nMapHeight);
}

//====================================================================
//�`���[�g���A���ł̍X�V����
//====================================================================
void CPlayer::TutorialUpdate(void)
{

}

//====================================================================
//�`�揈��
//====================================================================
void CPlayer::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���f���̕`��(�S�p�[�c)
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}
}

//====================================================================
//�ړ�����
//====================================================================
void CPlayer::Move(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_bInput = false;

	//�L�[�{�[�h�̈ړ�����
	if ((pInputKeyboard->GetPress(DIK_W) && m_OKU) ||
		(pInputKeyboard->GetPress(DIK_W) && m_MoveState == MOVE_STATE_DOWN))
	{
		NormarizeMove.z += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		NormarizeMove.x += 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_UP;
	}
	if ((pInputKeyboard->GetPress(DIK_S) && m_OKD) ||
		(pInputKeyboard->GetPress(DIK_S) && m_MoveState == MOVE_STATE_UP))
	{
		NormarizeMove.z += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		NormarizeMove.x += -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_DOWN;
	}
	if ((pInputKeyboard->GetPress(DIK_A) && m_OKL) ||
		(pInputKeyboard->GetPress(DIK_A) && m_MoveState == MOVE_STATE_RIGHT))
	{
		NormarizeMove.x += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		NormarizeMove.z -= -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_LEFT;
	}
	if ((pInputKeyboard->GetPress(DIK_D) && m_OKR) ||
		(pInputKeyboard->GetPress(DIK_D) && m_MoveState == MOVE_STATE_LEFT))
	{
		NormarizeMove.x += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		NormarizeMove.z -= 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_RIGHT;
	}

	if (pInputKeyboard->GetPress(DIK_W) == false && pInputKeyboard->GetPress(DIK_A) == false && pInputKeyboard->GetPress(DIK_S) == false && pInputKeyboard->GetPress(DIK_D) == false)
	{
		//���X�e�B�b�N�ɂ��O��ړ�	
		m_move.z += pInputJoypad->Get_Stick_Left(0).y * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		m_move.x += pInputJoypad->Get_Stick_Left(0).y * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		//���X�e�B�b�N�ɂ�鍶�E�ړ�
		m_move.x += pInputJoypad->Get_Stick_Left(0).x * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		m_move.z -= pInputJoypad->Get_Stick_Left(0).x * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;
	}

	if (m_bInput)
	{
		float JunpPawer = NormarizeMove.y;
		NormarizeMove.y = 0.0f;

		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.y = JunpPawer;
		NormarizeMove.z *= PLAYER_SPEED;

		//�ړ��ʂ���
		m_move = NormarizeMove;

		//�ړ���Ԃɂ���
		m_State = STATE_WALK;
	}
}

//====================================================================
//�ړ���������
//====================================================================
void CPlayer::Rot(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	//�ړ������Ɍ��������킹�鏈��
	float fRotMove, fRotDest;
	fRotMove = m_rot.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;

	if (pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{
		m_rot.y = atan2f(m_move.z, -m_move.x);
	}

	useful::NormalizeAngle(&m_rot);
}

//====================================================================
//�U������
//====================================================================
void CPlayer::Attack(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	{
		CFire::Create("data\\model\\BlockTest.x", m_pos, m_move);
	}
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CPlayer::ActionState(void)
{
	//�ړ����[�V����
	if (m_State == STATE_DEATH)
	{
		if (m_Action != ACTION_SDEATH)
		{
			m_Action = ACTION_SDEATH;
			m_pMotion->Set(ACTION_SDEATH, 5);
		}
	}

	//�ړ����[�V����
	if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
	{
		if (m_Action != ACTION_BMOVE)
		{
			m_Action = ACTION_BMOVE;
			m_pMotion->Set(ACTION_BMOVE, 5);
		}
	}
	//�����[�V����
	else if (m_State == STATE_EGG)
	{
		if (m_Action != ACTION_SWAIT)
		{
			m_Action = ACTION_SWAIT;
			m_pMotion->Set(ACTION_SWAIT, 5);
		}
	}
	//�j���[�g�������[�V����
	else
	{
		if (m_Action != ACTION_BWAIT)
		{
			m_Action = ACTION_BWAIT;
			m_pMotion->Set(ACTION_BWAIT, 5);
		}
	}
}

//====================================================================
//��ԊǗ�
//====================================================================
void CPlayer::StateManager(void)
{
	switch (m_State)
	{
	case STATE_WAIT:
		//	�X���[��default��
		CSlowManager::SetValueDefault();
		break;

	case STATE_WALK:
		break;

	case STATE_DEATH:

		if (m_nStateCount == 0)
		{
			m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_State = STATE_EGG;
		}

		break;

	case STATE_EGG:
		//if (m_nStateCount == 0)
		//{
		//	m_State = STATE_WAIT;
		//}
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
// �ǂƂ̓����蔻��
//====================================================================
void CPlayer::CollisionWall(useful::COLLISION XYZ)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == TYPE_CUBEBLOCK)
			{//��ނ��u���b�N�̎�

				CCubeBlock* pBlock = (CCubeBlock*)pObj;	// �u���b�N���̎擾

				D3DXVECTOR3 pos = pBlock->GetPos();
				D3DXVECTOR3 posOld = pBlock->GetPosOld();
				D3DXVECTOR3 Move = pBlock->GetMove();
				D3DXVECTOR3 Size = pBlock->GetSize();

				// ��`�̓����蔻��
				if (useful::CollisionBlock(pos, posOld, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
				{
					//�ҋ@��Ԃɂ���
					m_State = STATE_WAIT;
					m_MoveState = MOVE_STATE_WAIT;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// �ǂƂ̓����蔻��
//====================================================================
void CPlayer::SearchWall(void)
{
	bool OKR = true;	//�E
	bool OKL = true;	//��
	bool OKU = true;	//��
	bool OKD = true;	//��

	CMapSystem *pMapSystem = CMapSystem::GetInstance();
	int nMapWightMax = pMapSystem->GetWightMax();
	int nMapHeightMax = pMapSystem->GetHeightMax();
	D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

	int nRNumber = m_nMapWight + 1;
	int nLNumber = m_nMapWight - 1;
	int nUNumber = m_nMapHeight - 1;
	int nDNumber = m_nMapHeight + 1;

	nRNumber = useful::RangeNumber(nMapWightMax, 0, nRNumber);
	nLNumber = useful::RangeNumber(nMapWightMax, 0, nLNumber);
	nUNumber = useful::RangeNumber(nMapWightMax, 0, nUNumber);
	nDNumber = useful::RangeNumber(nMapWightMax, 0, nDNumber);

	OKR = !pMapSystem->GetGritBool(nRNumber, m_nMapHeight);
	OKL = !pMapSystem->GetGritBool(nLNumber, m_nMapHeight);
	OKU = !pMapSystem->GetGritBool(m_nMapWight, nUNumber);
	OKD = !pMapSystem->GetGritBool(m_nMapWight, nDNumber);

	//�����̗����Ă���O���b�g�̒��S�ʒu�����߂�
	D3DXVECTOR3 MyGritPos = INITVECTOR3;
	float MapGritSize = pMapSystem->GetGritSize();

	D3DXVECTOR3 DevilPos = CGame::GetDevil()->GetDevilPos();
	D3DXVECTOR3 DevilSize = CGame::GetDevil()->GetDevilSize();

	//�����W�̃O���b�g�Ɉʒu��ݒ肷��
	MyGritPos.x = MapSystemPos.x + (m_nMapWight * MapGritSize);

	// �����̈ʒu���}�b�v�O�������ꍇ�ɔ��Α�����͂ݏo�����̈ʒu��ݒ肷��
	if (MyGritPos.x > DevilPos.x + (DevilSize.x))
	{
		MyGritPos.x = MyGritPos.x - (DevilPos.x + (DevilSize.x * 2.0f)) - MapGritSize;
	}

	//�����W�̃O���b�g�Ɉʒu��ݒ肷��
	MyGritPos.z = MapSystemPos.z - (m_nMapHeight * MapGritSize);

	// �����̈ʒu���}�b�v�O�������ꍇ�ɔ��Α�����͂ݏo�����̈ʒu��ݒ肷��
	if (MyGritPos.z < DevilPos.z - (DevilSize.z))
	{
		MyGritPos.z = MyGritPos.z + (DevilPos.z + (DevilSize.z * 2.0f)) + MapGritSize;
	}

	DebugProc::Print(DebugProc::POINT_LEFT, "�������� %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if (m_pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - m_size.x) &&
		m_pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - m_size.x) &&
		m_pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - m_size.z) &&
		m_pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - m_size.z))
	{// �O���b�g�̒��S�ʒu�ɗ����Ă���Ȃ瑀����󂯕t����
		m_OKR = OKR;	//�E
		m_OKL = OKL;	//��
		m_OKU = OKU;	//��
		m_OKD = OKD;	//��
	}
	else
	{
		m_OKR = false;	//�E
		m_OKL = false;	//��
		m_OKU = false;	//��
		m_OKD = false;	//��
	}
}

//====================================================================
// �}�b�v���f���̓����蔻��
//====================================================================
void CPlayer::CollisionDevilHole(useful::COLLISION XYZ)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == TYPE_DEVILHOLE)
			{//��ނ��f�r���z�[���̎�

				CDevilHole* pDevilHole = (CDevilHole*)pObj;	// �u���b�N���̎擾

				D3DXVECTOR3 pos = pDevilHole->GetPos();
				D3DXVECTOR3 posOld = pDevilHole->GetPosOld();
				D3DXVECTOR3 Size = pDevilHole->GetSize();

				// ��`�̓����蔻��
				if (useful::CollisionBlock(pos, posOld, INITVECTOR3, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
				{
					//�ҋ@��Ԃɂ���
					m_State = STATE_WAIT;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// �G�̓����蔻��
//====================================================================
void CPlayer::CollisionEnemy(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == TYPE_ENEMY3D)
			{//��ނ��G�̎�

				CEnemy* pEnemy = (CEnemy*)pObj;	// �A�C�e���̏��̎擾

				D3DXVECTOR3 pos = pEnemy->GetPos();
				D3DXVECTOR3 posOld = pEnemy->GetPosOld();
				D3DXVECTOR3 Size = pEnemy->GetSize();

				// �~�̓����蔻��
				if (useful::CollisionCircle(m_pos, pos, 30.0f) == true)
				{
					Death();
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// �X�e�[�W�O�̓����蔻��
//====================================================================
void CPlayer::CollisionStageOut(void)
{
	D3DXVECTOR3 D_pos = CGame::GetDevil()->GetDevilPos();
	D3DXVECTOR3 D_Size = CGame::GetDevil()->GetDevilSize();
	float G_Size = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

	if (m_pos.x + G_Size > D_pos.x + D_Size.x)
	{
		m_pos.x = D_pos.x + D_Size.x - G_Size;
		m_State = STATE_WAIT;
		m_move.x = 0.0f;
	}
	if (m_pos.x - G_Size < D_pos.x - D_Size.x)
	{
		m_pos.x = D_pos.x - D_Size.x + G_Size;
		m_State = STATE_WAIT;
		m_move.x = 0.0f;
	}
	if (m_pos.z + G_Size > D_pos.z + D_Size.z)
	{
		m_pos.z = D_pos.z + D_Size.z - G_Size;
		m_State = STATE_WAIT;
		m_move.z = 0.0f;
	}
	if (m_pos.z - G_Size < D_pos.z - D_Size.z)
	{
		m_pos.z = D_pos.z - D_Size.z + G_Size;
		m_State = STATE_WAIT;
		m_move.z = 0.0f;
	}
}

//====================================================================
// �v���C���[���}�b�v�̂ǂ̃}�X�ɑ��݂��Ă��邩�ݒ肷��
//====================================================================
void CPlayer::MapSystemNumber(void)
{
	CDevil* pDevil = CGame::GetDevil();
	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();
	D3DXVECTOR3 DevilSize = pDevil->GetDevilSize();
	D3DXVECTOR3 MapSystemPos = CMapSystem::GetInstance()->GetMapPos();
	int MapWightMax = CMapSystem::GetInstance()->GetWightMax();
	int MapHeightMax = CMapSystem::GetInstance()->GetHeightMax();
	float MapGritSize = CMapSystem::GetInstance()->GetGritSize();
	D3DXVECTOR3 GritPos = INITVECTOR3;

	for (int nCntW = 0; nCntW < MapWightMax; nCntW++)
	{
		float fCountPosX = MapSystemPos.x + (nCntW * MapGritSize);

		if (fCountPosX > DevilPos.x + (DevilSize.x))
		{
			fCountPosX = fCountPosX - (DevilPos.x + (DevilSize.x * 2.0f)) - MapGritSize;
		}

		if (m_pos.x < fCountPosX + (MapGritSize * 0.5f) &&
			m_pos.x >= fCountPosX - (MapGritSize * 0.5f))
		{
			m_nMapWight = nCntW;
			break;
		}
	}

	for (int nCntH = 0; nCntH < MapHeightMax; nCntH++)
	{
		float fCountPosZ = MapSystemPos.z - (nCntH * MapGritSize);

		if (fCountPosZ < DevilPos.z - (DevilSize.z))
		{
			fCountPosZ = fCountPosZ + (DevilPos.z + (DevilSize.z * 2.0f)) + MapGritSize;
		}

		if (m_pos.z < fCountPosZ + (MapGritSize * 0.5f) &&
			m_pos.z >= fCountPosZ - (MapGritSize * 0.5f))
		{
			m_nMapHeight = nCntH;
			break;
		}
	}
}

//====================================================================
//�J�����ʒu�X�V����
//====================================================================
void CPlayer::CameraPosUpdate(void)
{
	//�J�����ʒu�̍X�V
	m_CameraPos.x = m_pos.x;
	m_CameraPos.z = m_pos.z;

	if (m_bJump == false)
	{
		m_CameraPos.y = m_pos.y;
	}
}

//====================================================================
//�ʒu�X�V����
//====================================================================
void CPlayer::PosUpdate(void)
{
	//�����W��
	//m_move.x = m_move.x * 0.5f;
	//m_move.z = m_move.z * 0.5f;

	if (m_move.x <= 0.0001f && m_move.x >= -0.0001f)
	{
		m_move.x = 0.0f;
	}

	if (m_move.z <= 0.0001f && m_move.z >= -0.0001f)
	{
		m_move.z = 0.0f;
	}

	if (m_bJump == true)
	{
		m_Objmove.x = m_Objmove.x * 0.25f;
		if (m_Objmove.x <= 0.0001f && m_Objmove.x >= -0.0001f)
		{
			m_Objmove.x = 0.0f;
		}

		m_Objmove.z = m_Objmove.z * 0.25f;
		if (m_Objmove.z <= 0.0001f && m_Objmove.z >= -0.0001f)
		{
			m_Objmove.z = 0.0f;
		}
	}

	// �ϐ��錾
	float fSpeed = 1.0f;	// �X���[�p default1.0f�ŏ�����
	if (m_pSlow)
	{
		fSpeed = m_pSlow->GetValue();
	}

	//Y���̈ʒu�X�V
	m_pos.y += m_move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_Y);
	CollisionDevilHole(useful::COLLISION_Y);

	//X���̈ʒu�X�V
	m_pos.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_X);
	CollisionDevilHole(useful::COLLISION_X);

	//Z���̈ʒu�X�V
	m_pos.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_Z);
	CollisionDevilHole(useful::COLLISION_Z);
}

//====================================================================
//�����X�V����
//====================================================================
void CPlayer::RotUpdate(void)
{
	// �p�x�̍������Z�o
	D3DXVECTOR3 rotDiff = m_rotDest - m_rot;

	// ���K��
	useful::NormalizeAngle(&rotDiff);			

	// �����̍X�V����
	m_rot += (rotDiff * 0.5);
}

//====================================================================
//�_���[�W����
//====================================================================
void CPlayer::Death(void)
{
	if (m_State != STATE_EGG && m_State != STATE_DEATH)
	{
		m_nLife--;

		if (m_nLife < 0)
		{
			CGame::SetGameEnd(true);
			CGame::SetGameClear(false);
		}
		else
		{
			if (m_pLifeUi != nullptr)
			{
				m_pLifeUi->GetNumber()->SetNumber(m_nLife);
			}

			m_State = STATE_DEATH;
			m_nStateCount = 150;

			// �_���[�W��
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE_PLAYER);
		}
	}
}

//====================================================================
// �f�o�b�O�{�^��
//====================================================================
void CPlayer::DebugKey(void)
{
#ifdef _DEBUG

	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (pMouse->GetTrigger(pMouse->PUSH_WHEEL))
	{
		HitDamage(10.0f);
	}
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//�L�[�{�[�h�̈ړ�����
	if (pInputKeyboard->GetTrigger(DIK_3))
	{
		m_nLife++;
		m_pLifeUi->GetNumber()->SetNumber(m_nLife);;
	}
	if (pInputKeyboard->GetTrigger(DIK_4))
	{
		m_nLife--;
		m_pLifeUi->GetNumber()->SetNumber(m_nLife);;
	}

#endif // !_DEBUG
}

//====================================================================
// ���f���̃��[�V�����ݒ�
//====================================================================
void CPlayer::SetAction(ACTION_TYPE Action, float BlendTime)
{
	if (m_Action != Action)
	{
		m_Action = Action;
		m_pMotion->Set(Action, BlendTime);
	}
}

//====================================================================
// ���f���\���̐ݒ�
//====================================================================
void CPlayer::SetModelDisp(bool Sst)
{
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			m_apModel[nCnt]->SetDisp(Sst);
		}
	}
}

//====================================================================
//�}�b�v�I�u�W�F�N�g�ƕǂƂ̃\�[�g����
//====================================================================
bool CPlayer::SortObject(D3DXVECTOR3 pos)
{
	float fDistance = sqrtf((m_pos.x - pos.x) * (m_pos.x - pos.x) + (m_pos.z - pos.z) * (m_pos.z - pos.z));

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == TYPE_CUBEBLOCK)
			{//��ނ��u���b�N�̎�
				CCubeBlock* pBlock = (CCubeBlock*)pObj;

				if (pBlock->GetPos().y <= 200.0f)
				{
					if (useful::CollisionCircle(m_pos, pBlock->GetPos(), 250.0f) == true)
					{
						D3DXVECTOR3 CrossPos = INITVECTOR3;
						float ObjDistance = 10000.0f;

						//��_�̈ʒu�����߂�
						for (int nCnt = 0; nCnt < 4; nCnt++)
						{
							switch (nCnt)
							{
							case 0:
								CrossPos = useful::CrossIntersection(m_pos, pos,
									D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
									D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
									250.0f);
								break;

							case 1:
								CrossPos = useful::CrossIntersection(m_pos, pos,
									D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
									D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
									250.0f);
								break;

							case 2:
								CrossPos = useful::CrossIntersection(m_pos, pos,
									D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
									D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
									250.0f);
								break;

							case 3:
								CrossPos = useful::CrossIntersection(m_pos, pos,
									D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
									D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
									250.0f);
								break;
							}

							ObjDistance = sqrtf((m_pos.x - CrossPos.x) * (m_pos.x - CrossPos.x) + (m_pos.z - CrossPos.z) * (m_pos.z - CrossPos.z));

							//CManager::GetInstance()->GetDebugProc()->Print("[�G�F%f %f] [�v�F%f %f] [�ǁF%f %f] \n", m_pos.x, m_pos.z, pos.x, pos.z, CrossPos.x, CrossPos.z);
							//CManager::GetInstance()->GetDebugProc()->Print("%f   :   %f\n", fDistance, ObjDistance);

							if (ObjDistance < fDistance)
							{
								return true;
							}
						}
					}
				}
			}

			pObj = pObjNext;
		}
	}
	return false;
}

//====================================================================
//���[�h����
//====================================================================
void CPlayer::LoadLevelData(const char* pFilename)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen(pFilename, "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		int ModelParent = 0;
		D3DXVECTOR3 ModelPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 ModelRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		char ModelName[128] = {};
		int nCntModel = 0;
		int nCntParts = 0;
		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		char aString[128] = {};				//�S�~��
		char aMessage[128] = {};			//�X�^�[�g�ƃG���h�̃��b�Z�[�W
		char aBool[128] = {};				//bool�ϊ��p���b�Z�[�W

		// �ǂݍ��݊J�n-----------------------------------------------------
		while (1)
		{//�uSCRIPT�v��T��
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// ���f�����ǂݍ���-----------------------------------------------------
				while (1)
				{//�uNUM_MODEL�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%d", &m_nNumModel);		//���f�����̐ݒ�
						break;
					}
				}

				//���f���t�@�C���̓ǂݍ���
				while (1)
				{//�uMODEL_FILENAME�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%s", &ModelName[0]);		//�ǂݍ��ރ��f���̃p�X���擾

						m_apModel[nCntModel] = CModel::Create(&ModelName[0]);
						nCntModel++;
					}
					if (nCntModel >= m_nNumModel)
					{
						nCntModel = 0;
						break;
					}
				}

				// �L�����N�^�[���ǂݍ���-----------------------------------------------------
				while (1)
				{//�uPARTSSET�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "PARTSSET") == 0)
					{
						while (1)
						{//�e��ϐ���T��
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "INDEX") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &nCntModel);	//�C���f�b�N�X��ݒ�
							}
							if (strcmp(&aMessage[0], "PARENT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParent);	//�e���f���̃C���f�b�N�X��ݒ�

								if (ModelParent == -1)
								{
									m_apModel[nCntModel]->SetParent(nullptr);
								}
								else
								{
									m_apModel[nCntModel]->SetParent(m_apModel[ModelParent]);
								}
							}
							if (strcmp(&aMessage[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelPos.x);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�
								fscanf(pFile, "%f", &ModelPos.y);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�
								fscanf(pFile, "%f", &ModelPos.z);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�

								m_apModel[nCntModel]->SetPos(ModelPos);
								m_apModel[nCntModel]->SetStartPos(ModelPos);
							}
							if (strcmp(&aMessage[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelRot.x);				////�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.y);				////�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.z);				////�����̏����ݒ�

								m_apModel[nCntModel]->SetRot(ModelRot);
								m_apModel[nCntModel]->SetStartRot(ModelRot);
							}
							if (strcmp(&aMessage[0], "END_PARTSSET") == 0)
							{
								break;
							}
						}
						nCntModel++;
						if (nCntModel >= m_nNumModel)
						{
							break;
						}
					}
				}
			}
			if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***\n");
	}
}

//====================================================================
// �v���C���[�̎w�胂�f������
//====================================================================
void CPlayer::SetPartsDisp(int nParts, bool Set)
{
	if (m_apModel[nParts] != nullptr)
	{
		m_apModel[nParts]->SetDisp(Set);
	}
}
