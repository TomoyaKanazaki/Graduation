//============================================
//
//	�f�r���z�[���̏��� [DevilHole.cpp]
//	Author:sakamoto kai
//
//============================================
#include "DevilHole.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "effect.h"
#include "objmeshField.h"
#include "game.h"
#include "tutorial.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(50.0f, 0.0f, 50.0f);		//�����蔻��
	const int DIRECTION = 4;	// �f�r���z�[���̍ő����
}

//====================================================================
//�ÓI�����o�ϐ��錾
//====================================================================
CListManager<CDevilHole>* CDevilHole::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CDevilHole::CDevilHole(int nPriority) : CObjectX(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//�}�e���A���̐�
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_Grid.x = 0;
	m_Grid.z = 0;
	m_pos = INITVECTOR3;	
	m_posOld = INITVECTOR3;
	m_move = INITVECTOR3;
	m_rot = INITVECTOR3;
	
	for (int nCnt = 0; nCnt < DIRECTION; nCnt++)
	{
		m_bSet[nCnt] = false;			//�㉺���E�̌������܂��Ă��邩�ǂ���
		m_pHoleKey[nCnt] = nullptr;		//�㉺���E�̌��𖄂߂�|���S��
	}
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CDevilHole::~CDevilHole()
{

}

//====================================================================
//��������
//====================================================================
CDevilHole* CDevilHole::Create(char* pModelName)
{
	CDevilHole* pSample = nullptr;

	if (pSample == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample = new CDevilHole();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample->Init(pModelName)))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pSample;
}

//====================================================================
//����������
//====================================================================
HRESULT CDevilHole::Init(char* pModelName)
{
	SetType(CObject::TYPE_DEVILHOLE);

	CObjectX::Init(pModelName);

	//���[�h���Ƃɏ����l��ݒ�o����
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

	switch (CScene::GetMode())
	{
	case CScene::MODE_GAME:
		//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
		SetUseMultiMatrix(CGame::GetInstance()->GetMapField()->GetMatrix());
		break;
	case CScene::MODE_TUTORIAL:
		//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
		SetUseMultiMatrix(CTutorial::GetMapField()->GetMatrix());
		break;
	}

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CDevilHole>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CDevilHole::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	CObjectX::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CDevilHole::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//�^�C�g���ł̍X�V����
//====================================================================
void CDevilHole::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	//�ʒu�X�V
	pos += m_move;

	SetPos(pos);

	//���_���̍X�V
	CObjectX::Update();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CDevilHole::GameUpdate(void)
{
	//�X�V�O�̈ʒu���ߋ��̈ʒu�Ƃ���
	m_posOld = m_pos;

	//�z�[���������菈��
	CollisionOpen();

	//�N���A���菈��
	ClearJudge();

	m_pos = m_Grid.ToWorld();

	for (int nCnt = 0; nCnt < DIRECTION; nCnt++)
	{
		if (m_pHoleKey[nCnt] != nullptr)
		{
			switch (nCnt)
			{
			case 0:	//��
				m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + 20.0f));
				break;

			case 1:	//��
				m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z - 20.0f));
				break;

			case 2:	//�E
				m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(m_pos.x + 20.0f, m_pos.y, m_pos.z));
				break;

			case 3:	//��
				m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(m_pos.x - 20.0f, m_pos.y, m_pos.z));
				break;
			}
		}
	}

	//�ʒu�X�V
	CObjectX::SetPos(m_pos);

	//�傫���̐ݒ�
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//��ԊǗ�
	StateManager();

	//���_���̍X�V
	CObjectX::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CDevilHole::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//��ԊǗ�
//====================================================================
void CDevilHole::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ACTION:
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
void CDevilHole::CollisionOpen(void)
{
	// �v���C���[�̃��X�g�\����������Δ�����
	if (CPlayer::GetList() == nullptr) { return; }
	std::list<CPlayer*> list = CPlayer::GetList()->GetList();    // ���X�g���擾

	// �v���C���[���X�g�̒��g���m�F����
	for (CPlayer* pPlayer : list)
	{
		// �v���C���[�̈ʒu�E�I�u�W�F�N�gX�T�C�Y�擾
		D3DXVECTOR3 playerPos = pPlayer->GetPos();
		D3DXVECTOR3 ObjXSize = GetSize();

		if (pPlayer->GetItemType() != CPlayer::TYPE_BIBLE)
		{// �����ȊO�̎�
			continue;
		}

		for (int nCnt = 0; nCnt < DIRECTION; nCnt++)
		{
			D3DXVECTOR3 pos = m_pos;

			switch (nCnt)
			{
			case 0:	//��
				pos.z += 200.0f;
				break;

			case 1:	//��
				pos.z -= 200.0f;
				break;

			case 2:	//�E
				pos.x += 200.0f;
				break;

			case 3:	//��
				pos.x -= 200.0f;
				break;
			}

			// ��`�̓����蔻��
			if (useful::PointSquareXZ(playerPos, pos, ObjXSize) == true &&
				m_bSet[nCnt] == false)
			{
				m_pHoleKey[nCnt] = CObjectX::Create("data\\MODEL\\DevilKey.x");
				m_pHoleKey[nCnt]->SetUseMultiMatrix(CGame::GetInstance()->GetMapField()->GetMatrix());
				//m_pHoleKey[nCnt]->SetMultiMatrix(true);

				switch (nCnt)
				{
				case 0:	//��
					m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + 20.0f));
					break;

				case 1:	//��
					m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z - 20.0f));
					break;

				case 2:	//�E
					m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(m_pos.x + 20.0f, m_pos.y, m_pos.z));
					break;

				case 3:	//��
					m_pHoleKey[nCnt]->SetPos(D3DXVECTOR3(m_pos.x - 20.0f, m_pos.y, m_pos.z));
					break;
				}

				m_bSet[nCnt] = true;
				pPlayer->SetItemType(CPlayer::TYPE_NONE);
				return;
			}
		}
	}
}

//====================================================================
// �N���A���菈��
//====================================================================
void CDevilHole::ClearJudge(void)
{
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_bSet[nCnt] == false)
		{
			return;
		}
	}

	CGame::GetInstance()->SetDevilHoleFinish(true);
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CDevilHole>* CDevilHole::GetList(void)
{
	return m_pList;
}
