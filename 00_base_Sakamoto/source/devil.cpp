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
#include "objectcharacter.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "debugproc.h"
#include "camera.h"
#include "input.h"
#include "enemy.h"
#include "slowManager.h"
#include "Number.h"
#include "bowabowa.h"
#include "Cross.h"
#include "objmeshField.h"
#include "railblock.h"
#include "RollRock.h"
#include "sound.h"
#include "scrollarrow.h"
#include "fire.h"
#include "Scene.h"
#include "tile.h"
#include "wall.h"
#include "MapSystem.h"
#include "ScrollDevice.h"
#include "SlopeDevice.h"
#include "effect.h"
#include "signal.h"
#include "friedegg.h"
#include "MapMove.h"

//===========================================
// �萔��`
//===========================================
namespace
{

}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CDevil>* CDevil::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CDevil::CDevil(int nPriority) : CObjectCharacter(nPriority)
{
	SetSize(D3DXVECTOR3(750.0f, 0.0f, 550.0f));
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_fActionCount = 0.0f;
	m_Action = ACTION_NEUTRAL;
	m_AtkAction = ACTION_NEUTRAL;
	m_nStateCount = 0;
	m_CollisionRot = 0.0f;
	m_DevilPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MapDifference = INITVECTOR3;
	m_bSlope = false;
	m_MinGrid = CMapSystem::GRID(0, 0);
	m_MaxGrid = CMapSystem::GRID(NUM_WIGHT - 1, NUM_HEIGHT - 1);
	m_DevilArrow = 0;
	m_ScrollArrowOld = 0;
	m_SlopwArrowOld = 0;
	m_pSignal[0] = nullptr;
	m_pSignal[1] = nullptr;
	m_pSignal[2] = nullptr;
	m_pSignal[3] = nullptr;
	m_nStateNum = 0;
	m_SlopeType = 0;
	m_pMapMove = nullptr;
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
	//��ސݒ�
	SetType(CObject::TYPE_DEVIL);

	// �e��s�g�p�ɐݒ�
	SetShadow(false);

	// �L�����N�^�[�N���X�̏������i�p���j
	if (FAILED(CObjectCharacter::Init())) { assert(false); }

	// �L�����N�^�[�e�L�X�g�ǂݍ��ݏ���
	SetTxtCharacter("data\\TXT\\MOTION\\01_enemy\\motion_devil.txt", 0);

	if (m_pMapMove == nullptr)
	{
		m_pMapMove = CMapMove::Create();
	}

	// �X���[�̐���
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_PLAYER, CSlowManager::TAG_PLAYER);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CDevil>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CDevil::Uninit(void)
{
	if (m_pMapMove != nullptr)
	{
		m_pMapMove->Uninit();
		m_pMapMove = nullptr;
	}

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �L�����N�^�[�N���X�̏I���i�p���j
	CObjectCharacter::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CDevil::Update(void)
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (CGame::GetInstance()->GetEvent() == false &&
			m_pMapMove != nullptr)
		{
			//�}�b�v�̓���
			m_pMapMove->Update();
		}
	}

	// �L�����N�^�[�N���X�̍X�V�i�p���j
	CObjectCharacter::Update();

	//�C�x���g���[�V����
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (CGame::GetInstance()->GetEvent() == false)
		{
			//���[�V�����̊Ǘ�
			ActionState();
		}
	}
	else
	{
		//���[�V�����̊Ǘ�
		ActionState();
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CDevil::Draw(void)
{
	// �L�����N�^�[�N���X�̕`��i�p���j
	CObjectCharacter::Draw();
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CDevil::ActionState(void)
{
	if (m_pMapMove != nullptr)
	{
		// �ヂ�[�V����
		if (m_pMapMove->GetState() == CMapMove::MOVE_SCROLL_UP ||
			m_pMapMove->GetState() == CMapMove::MOVE_SLOPE_UP)
		{
			if (m_Action != ACTION_SIGNAL_UP)
			{
				m_Action = ACTION_SIGNAL_UP;
				m_nStateNum = ACTION_SIGNAL_UP;
				GetMotion()->Set(ACTION_SIGNAL_UP, 5);
			}
		}
		// �����[�V����
		else if (m_pMapMove->GetState() == CMapMove::MOVE_SCROLL_DOWN ||
			m_pMapMove->GetState() == CMapMove::MOVE_SLOPE_DOWN)
		{
			if (m_Action != ACTION_SIGNAL_DOWN)
			{
				m_Action = ACTION_SIGNAL_DOWN;
				m_nStateNum = ACTION_SIGNAL_DOWN;
				GetMotion()->Set(ACTION_SIGNAL_DOWN, 5);
			}
		}
		// �����[�V����
		else if (m_pMapMove->GetState() == CMapMove::MOVE_SCROLL_LEFT ||
			m_pMapMove->GetState() == CMapMove::MOVE_SLOPE_LEFT)
		{
			if (m_Action != ACTION_SIGNAL_LEFT)
			{
				m_Action = ACTION_SIGNAL_LEFT;
				m_nStateNum = ACTION_SIGNAL_LEFT;
				GetMotion()->Set(ACTION_SIGNAL_LEFT, 5);
			}
		}
		// �E���[�V����
		else if (m_pMapMove->GetState() == CMapMove::MOVE_SCROLL_RIGHT ||
			m_pMapMove->GetState() == CMapMove::MOVE_SLOPE_RIGHT)
		{
			if (m_Action != ACTION_SIGNAL_RIGHT)
			{
				m_Action = ACTION_SIGNAL_RIGHT;
				m_nStateNum = ACTION_SIGNAL_RIGHT;
				GetMotion()->Set(ACTION_SIGNAL_RIGHT, 5);
			}
		}
		// �j���[�g�������[�V����
		else
		{
			if (m_Action != ACTION_NEUTRAL)
			{
				m_Action = ACTION_NEUTRAL;
				GetMotion()->Set(ACTION_NEUTRAL, 5);
			}
		}
	}
}

//====================================================================
// ���f���̃��[�V�����ݒ�
//====================================================================
void CDevil::SetAction(ACTION_TYPE Action, float BlendTime)
{
	// ���[�V�����̎擾����
	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
	{
		return;
	}

	if (m_Action != Action)
	{
		m_Action = Action;
		pMotion->Set(Action, BlendTime);
	}
}

//====================================================================
// ���f���\���̐ݒ�
//====================================================================
void CDevil::SetModelDisp(bool Sst)
{
	// ���f�������擾
	int nNumModel = GetNumModel();

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// ���f���̎擾����
		CModel* pModel = GetModel(nCnt);

		if (pModel != nullptr)
		{
			// �\���ݒ�
			pModel->SetDisp(Sst);
		}
	}
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CDevil>* CDevil::GetList(void)
{
	return m_pList;
}

//====================================================================
//���X�g�̐擪�擾
//====================================================================
CDevil* CDevil::GetListTop(void)
{
	if (CDevil::GetList() == nullptr) { return nullptr; }
	std::list<CDevil*> list = CDevil::GetList()->GetList();    // ���X�g���擾
	CDevil* pDevil = list.front();

	return pDevil;
}
