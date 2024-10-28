//============================================
//
//	�f�r���̏���(��) [devil.cpp]
//	Author:sakamoto kai
//
//============================================
#include "devil.h"
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
#include "bowabowa.h"
#include "Cross.h"
#include "MapSystem.h"
#include "debugproc.h"
#include "objmeshField.h"
#include "sound.h"

#define COLLISION_SIZE (D3DXVECTOR3(750.0f,0.0f,550.0f))		//���̓����蔻��
#define SCROOL_SPEED (5.0f)									//�X�N���[���̈ړ����x
#define STAGE_ROT_LIMIT (D3DX_PI * 0.25f)					//�X�N���[���̈ړ����x

namespace
{

}

//====================================================================
//�R���X�g���N�^
//====================================================================
CDevil::CDevil(int nPriority) : CObject(nPriority)
{
	SetSize(COLLISION_SIZE);
	m_DevilSize = COLLISION_SIZE;
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_nActionCount = 0;
	m_Action = ACTION_WAIT;
	m_AtkAction = ACTION_WAIT;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_CollisionRot = 0.0f;
	m_pMotion = nullptr;
	m_DevilPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MapDifference = INITVECTOR3;
	m_DevilRot = INITVECTOR3;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CDevil::~CDevil()
{

}

//====================================================================
//��������
//====================================================================
CDevil* CDevil::Create()
{
	CDevil* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//�v���C���[�̐���
		pPlayer = new CDevil();
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
HRESULT CDevil::Init(void)
{
	if (CScene::GetMode() == CScene::MODE_GAME ||
		CScene::GetMode() == CScene::MODE_TUTORIAL)
	{
		MyObjCreate();
	}

	//��ސݒ�
	SetType(CObject::TYPE_DEVIL);

	//���f���̐���
	LoadLevelData("data\\TXT\\motion_Boss.txt");

	//���[�V�����̐���
	if (m_pMotion == nullptr)
	{
		//���[�V�����̐���
		m_pMotion = new CMotion;
	}

	//����������
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData("data\\TXT\\motion_Boss.txt");

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

	// �X���[�̐���
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_PLAYER, CSlowManager::TAG_PLAYER);

	return S_OK;
}

//====================================================================
//�������ێ�����I�u�W�F�N�g�̐���
//====================================================================
void CDevil::MyObjCreate(void)
{
	//�I�u�W�F�N�g����
}

//====================================================================
//�I������
//====================================================================
void CDevil::Uninit(void)
{
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
void CDevil::Update(void)
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
void CDevil::TitleUpdate(void)
{
	//���[�V�����̍X�V
	m_pMotion->Update();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CDevil::GameUpdate(void)
{
	// �ߋ��̈ʒu�ɑ��
	m_posOld = m_pos;

	// �ړ�����
	Move();

	// �����ړ�����
	Rot();

	// �}�b�v�̌X��
	m_DevilRot = CGame::GetMapField()->GetRot();

	//��Ԃ̊Ǘ�
	StateManager();

	if (m_pMotion != nullptr)
	{
		//���[�V�����̍X�V
		m_pMotion->Update();
	}

	CEffect* pTestEffect = nullptr;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pTestEffect = CEffect::Create();

		switch (nCnt)
		{
		case 0:
			pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x + m_DevilSize.x, m_DevilPos.y, m_DevilPos.z + m_DevilSize.z));
			break;
		case 1:
			pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x - m_DevilSize.x, m_DevilPos.y, m_DevilPos.z + m_DevilSize.z));
			break;
		case 2:
			pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x + m_DevilSize.x, m_DevilPos.y, m_DevilPos.z - m_DevilSize.z));
			break;
		case 3:
			pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x - m_DevilSize.x, m_DevilPos.y, m_DevilPos.z - m_DevilSize.z));
			break;
		}

		pTestEffect->SetLife(20);
		pTestEffect->SetRadius(20.0f);
		pTestEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	//���[�V�����̊Ǘ�
	ActionState();

	//�f�o�b�O�L�[�̏����Ɛݒ�
	DebugKey();

	D3DXVECTOR3 InitPos = CMapSystem::GetInstance()->GetInitPos();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

	//�f�o�b�O�\��
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v]�@�@�@�ʒu %f : %f\n", MapPos.x, MapPos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̍���]�ʒu %f : %f\n", m_MapDifference.x, m_MapDifference.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̃X�N���[��]�F���L�[\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̌X��X]�F[ X+ : 6 ] [ X- : 7 ]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̌X��Z]�F[ Z+ : 8 ] [ Z- : 9 ]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̌X�����Z�b�g]�F[ 5 ]\n");
}

//====================================================================
//�`���[�g���A���ł̍X�V����
//====================================================================
void CDevil::TutorialUpdate(void)
{

}

//====================================================================
//�`�揈��
//====================================================================
void CDevil::Draw(void)
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
void CDevil::Move(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�L�[�{�[�h�̈ړ�����
	if (pInputKeyboard->GetPress(DIK_UP))
	{
		ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, SCROOL_SPEED));

	}
	if (pInputKeyboard->GetPress(DIK_DOWN))
	{
		ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, -SCROOL_SPEED));
	}

	if (pInputKeyboard->GetPress(DIK_LEFT))
	{
		ObjectScroll(D3DXVECTOR3(-SCROOL_SPEED, 0.0f, 0.0f));
	}
	if (pInputKeyboard->GetPress(DIK_RIGHT))
	{
		ObjectScroll(D3DXVECTOR3(SCROOL_SPEED, 0.0f, 0.0f));
	}

	if (pInputKeyboard->GetPress(DIK_UP) == false && pInputKeyboard->GetPress(DIK_LEFT) == false && pInputKeyboard->GetPress(DIK_DOWN) == false && pInputKeyboard->GetPress(DIK_RIGHT) == false)
	{
		//���X�e�B�b�N�ɂ��O��ړ�	
		m_DevilPos.z += pInputJoypad->Get_Stick_Left(0).y * SCROOL_SPEED;
		m_DevilPos.x += pInputJoypad->Get_Stick_Left(0).y * SCROOL_SPEED;

		//���X�e�B�b�N�ɂ�鍶�E�ړ�
		m_DevilPos.x += pInputJoypad->Get_Stick_Left(0).x * SCROOL_SPEED;
		m_DevilPos.z -= pInputJoypad->Get_Stick_Left(0).x * SCROOL_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_LEFT) == true || pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_RIGHT) == true)
	{
		float JunpPawer = NormarizeMove.y;
		NormarizeMove.y = 0.0f;

		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= SCROOL_SPEED;
		NormarizeMove.y = JunpPawer;
		NormarizeMove.z *= SCROOL_SPEED;
	}

	m_move += NormarizeMove;

}

//====================================================================
//�ړ���������
//====================================================================
void CDevil::Rot(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
	
	CObjmeshField *pMapField = CGame::GetMapField();
	D3DXVECTOR3 MapRot = pMapField->GetRot();

	if (pInputKeyboard->GetPress(DIK_5))
	{
		MapRot = INITVECTOR3;
	}

	if (pInputKeyboard->GetPress(DIK_6))
	{
		MapRot.x += D3DX_PI * 0.005f;

		if (MapRot.x > STAGE_ROT_LIMIT)
		{
			MapRot.x = STAGE_ROT_LIMIT;
		}
	}
	if (pInputKeyboard->GetPress(DIK_7))
	{
		MapRot.x -= D3DX_PI * 0.005f;

		if (MapRot.x < -STAGE_ROT_LIMIT)
		{
			MapRot.x = -STAGE_ROT_LIMIT;
		}
	}

	if (pInputKeyboard->GetPress(DIK_8))
	{
		MapRot.z += D3DX_PI * 0.005f;

		if (MapRot.z > STAGE_ROT_LIMIT)
		{
			MapRot.z = STAGE_ROT_LIMIT;
		}
	}
	if (pInputKeyboard->GetPress(DIK_9))
	{
		MapRot.z -= D3DX_PI * 0.005f;

		if (MapRot.z < -STAGE_ROT_LIMIT)
		{
			MapRot.z = -STAGE_ROT_LIMIT;
		}
	}

	pMapField->SetRot(MapRot);
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CDevil::ActionState(void)
{
	//�ړ����[�V����
	if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
	{
		if (m_Action != ACTION_WALK)
		{
			m_Action = ACTION_WALK;
			m_pMotion->Set(ACTION_WALK, 5);
		}
	}
	//�j���[�g�������[�V����
	else
	{
		if (m_Action != ACTION_WAIT)
		{
			m_Action = ACTION_WAIT;
			m_pMotion->Set(ACTION_WAIT, 5);
		}
	}
}

//====================================================================
//��ԊǗ�
//====================================================================
void CDevil::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		//	�X���[��default��
		CSlowManager::SetValueDefault();
		break;

	case STATE_DEATH:
		break;

	case STATE_WAIT:
		break;

	case STATE_DAMAGE:
		if (m_nStateCount == 0)
		{
			m_State = STATE_NORMAL;
		}
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
// �f�o�b�O�{�^��
//====================================================================
void CDevil::DebugKey(void)
{
#ifdef _DEBUG

	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (pMouse->GetTrigger(pMouse->PUSH_WHEEL))
	{
		HitDamage(10.0f);
	}

#endif // !_DEBUG
}

//====================================================================
// ���f���̃��[�V�����ݒ�
//====================================================================
void CDevil::SetAction(ACTION_TYPE Action, float BlendTime)
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
void CDevil::SetModelDisp(bool Sst)
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
// �I�u�W�F�N�g�̃X�N���[��
//====================================================================
void CDevil::ObjectScroll(D3DXVECTOR3 Move)
{
	GritScroll(Move);

	float m_GritSize = CMapSystem::GetInstance()->GetGritSize();

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == TYPE_CROSS)
			{//��ނ��G�̎�

				CCross* pCross = (CCross*)pObj;	// �u���b�N���̎擾

				D3DXVECTOR3 pos = pCross->GetPos();
				D3DXVECTOR3 Size = pCross->GetSize();

				pos += Move;

				if (Move.x > 0.0f)
				{
					if (m_DevilPos.x + m_DevilSize.x < pos.x - m_GritSize)
					{
						pos.x = -m_DevilSize.x + m_DevilPos.x - m_GritSize + Move.x;
					}
				}
				if (Move.x < 0.0f)
				{
					if (m_DevilPos.x - m_DevilSize.x > pos.x + m_GritSize)
					{
						pos.x = m_DevilSize.x + m_DevilPos.x + m_GritSize + Move.x;
					}
				}
				if (Move.z > 0.0f)
				{
					if (m_DevilPos.z + m_DevilSize.z < pos.z - m_GritSize)
					{
						pos.z = -m_DevilSize.z + m_DevilPos.z - m_GritSize + Move.z;
					}
				}
				if (Move.z < 0.0f)
				{
					if (m_DevilPos.z - m_DevilSize.z > pos.z + m_GritSize)
					{
						pos.z = m_DevilSize.z + m_DevilPos.z + m_GritSize + Move.z;
					}
				}

				pCross->SetPos(pos);
			}

			if (type == TYPE_BOWABOWA)
			{//��ނ��G�̎�

				CBowabowa* pBowabowa = (CBowabowa*)pObj;	// �u���b�N���̎擾

				D3DXVECTOR3 pos = pBowabowa->GetPos();
				D3DXVECTOR3 Size = pBowabowa->GetSize();

				pos += Move;

				if (Move.x > 0.0f)
				{
					if (m_DevilSize.x < pos.x)
					{
						pos.x = -m_DevilSize.x + Move.x - 100.0f;
					}
				}
				if (Move.x < 0.0f)
				{
					if (-m_DevilSize.x > pos.x)
					{
						pos.x = m_DevilSize.x + Move.x + 100.0f;
					}
				}
				if (Move.z > 0.0f)
				{
					if (m_DevilSize.z < pos.z)
					{
						pos.z = -m_DevilSize.z + Move.z - 100.0f;
					}
				}
				if (Move.z < 0.0f)
				{
					if (-m_DevilSize.z > pos.z)
					{
						pos.z = m_DevilSize.z + Move.z + 100.0f;
					}
				}

				pBowabowa->SetPos(pos);
			}

			if (type == TYPE_ENEMY3D)
			{//��ނ��G�̎�

				CEnemy* pEnemy = (CEnemy*)pObj;	// �u���b�N���̎擾

				D3DXVECTOR3 pos = pEnemy->GetPos();
				D3DXVECTOR3 Size = pEnemy->GetSize();

				pos += Move;

				pEnemy->SetPos(pos);
			}

			if (type == TYPE_PLAYER3D)
			{//��ނ��u���b�N�̎�

				CPlayer* pPlayer = (CPlayer*)pObj;	// �u���b�N���̎擾

				if (pPlayer->GetState() != CPlayer::STATE_EGG)
				{
					D3DXVECTOR3 pos = pPlayer->GetPos();
					D3DXVECTOR3 Size = pPlayer->GetSize();

					pos += Move;

					if (Move.x > 0.0f)
					{
						if (pos.x + (m_GritSize * 0.5f) > m_DevilPos.x + m_DevilSize.x)
						{
							pos.x = m_DevilPos.x + m_DevilSize.x - (m_GritSize * 0.5f) + Move.x;
							CollisionPressPlayer(pPlayer, pos, Size);

							CEffect* pEffect = CEffect::Create();
							pEffect->SetPos(pos);
							pEffect->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
							pEffect->SetRadius(100.0f);
						}
					}
					if (Move.x < 0.0f)
					{
						if (pos.x - (m_GritSize * 0.5f) < m_DevilPos.x - m_DevilSize.x)
						{
							pos.x = m_DevilPos.x - m_DevilSize.x + (m_GritSize * 0.5f) + Move.x;
							CollisionPressPlayer(pPlayer, pos, Size);

							CEffect* pEffect = CEffect::Create();
							pEffect->SetPos(pos);
							pEffect->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
							pEffect->SetRadius(100.0f);
						}
					}
					if (Move.z > 0.0f)
					{
						if (pos.z + (m_GritSize * 0.5f) > m_DevilPos.z + m_DevilSize.z)
						{
							pos.z = m_DevilPos.z + m_DevilSize.z - (m_GritSize * 0.5f) + Move.x;
							CollisionPressPlayer(pPlayer, pos, Size);

							CEffect* pEffect = CEffect::Create();
							pEffect->SetPos(pos);
							pEffect->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
							pEffect->SetRadius(100.0f);
						}
					}
					if (Move.z < 0.0f)
					{
						if (pos.z - (m_GritSize * 0.5f) < m_DevilPos.z - m_DevilSize.z)
						{
							pos.z = m_DevilPos.z - m_DevilSize.z + (m_GritSize * 0.5f) + Move.z;
							CollisionPressPlayer(pPlayer, pos, Size);

							CEffect* pEffect = CEffect::Create();
							pEffect->SetPos(pos);
							pEffect->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
							pEffect->SetRadius(100.0f);
						}
					}

					pPlayer->SetPos(pos);
				}
				else
				{// ����Ԃ̎��A�u���b�N�����݂��Ȃ��ʒu�Ƀz�[�~���O����

					int WightNumber = pPlayer->GetWightNumber();
					int HeightNumber = pPlayer->GetHeightNumber();
					if (!CMapSystem::GetInstance()->GetGritBool(WightNumber, HeightNumber))
					{
						D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
						D3DXVECTOR3 AnswerPos = INITVECTOR3;
						AnswerPos = CMapSystem::GetInstance()->GetGritPos(WightNumber, HeightNumber);

						if (pPlayer->GetGritCenter())
						{
							pPlayer->SetPos(AnswerPos);
						}
						else
						{
							PlayerPos += (AnswerPos - PlayerPos) * 0.5f;
							pPlayer->SetPos(PlayerPos);
						}
					}
				}
			}
			pObj = pObjNext;
		}
	}
}

//====================================================================
// �O���b�g�̃X�N���[��
//====================================================================
void CDevil::GritScroll(D3DXVECTOR3 Move)
{
	D3DXVECTOR3 InitPos = CMapSystem::GetInstance()->GetInitPos();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();
	int MapWightMax = CMapSystem::GetInstance()->GetWightMax();
	int MapHeightMax = CMapSystem::GetInstance()->GetHeightMax();
	float MapGrit = CMapSystem::GetInstance()->GetGritSize();
	MapPos += Move;
	m_MapDifference = -(InitPos - MapPos);

	if ((InitPos.x - MapPos.x) > 0.0f)
	{// ���͈�
		MapPos.x = InitPos.x + (m_DevilSize.x * 2.0f) + MapGrit + Move.x;
	}
	if ((InitPos.x - MapPos.x + MapGrit) < (-m_DevilSize.x * 2.0f))
	{// �E�͈�
		MapPos.x = InitPos.x + Move.x;
	}

	if ((InitPos.z - MapPos.z) < 0.0f)
	{// ��͈�
		MapPos.z = InitPos.z + (-m_DevilSize.z * 2.0f) - MapGrit + Move.z;
	}
	if ((InitPos.z - MapPos.z - MapGrit) > (m_DevilSize.z * 2.0f))
	{// ���͈�
		MapPos.z = InitPos.z + Move.z;
	}

	CMapSystem::GetInstance()->SetMapPos(MapPos);

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

				// �c���̃i���o�[�ƍ�����ݒ肷��
				D3DXVECTOR3 pos = INITVECTOR3;
				int BlockWight = pBlock->GetWightNumber();
				int BlockHeight = pBlock->GetHeightNumber();

				//�O���b�g�ԍ����ʒu�ɕϊ�
				pos = CMapSystem::GetInstance()->GetGritPos(BlockWight, BlockHeight);
				pos.y = 50.0f;

				pBlock->SetPos(pos);
			}
			pObj = pObjNext;
		}
	}

#ifdef _DEBUG

	//�@�O���b�g�̈ʒu�ɃG�t�F�N�g��\��
	for (int nCntW = 0; nCntW < MapWightMax; nCntW++)
	{
		for (int nCntH = 0; nCntH < MapHeightMax; nCntH++)
		{
			//�O���b�g�ԍ����ʒu�ɕϊ�
			D3DXVECTOR3 CountPos = CMapSystem::GetInstance()->GetGritPos(nCntW, nCntH);
			CountPos.y = 50.0f;

			if (CMapSystem::GetInstance()->GetGritBool(nCntW, nCntH))
			{// �u���b�N�����݂���O���b�g�̂݃G�t�F�N�g��\��

				CEffect* pEffect = CEffect::Create();
				pEffect->SetPos(CountPos);
				pEffect->SetLife(10);
			}
		}
	}

#endif // _DEBUG
}

//====================================================================
// �v���C���[���ׂ���鎞�̏���
//====================================================================
void CDevil::CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size)
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

				D3DXVECTOR3 Blockpos = pBlock->GetPos();
				D3DXVECTOR3 BlockSize = pBlock->GetSize();

				if (useful::CollisionRectangle2D(pos, Blockpos, Size, BlockSize, useful::COLLISION::COLLISION_ZX))
				{
					pPlayer->Death();
					return;
				}

			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//���[�h����
//====================================================================
void CDevil::LoadLevelData(const char* pFilename)
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
								fscanf(pFile, "%f", &ModelRot.x);				//�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.y);				//�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.z);				//�����̏����ݒ�

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
