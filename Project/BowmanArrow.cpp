//============================================
//
//	�|���̖� [BowmanArrow.cpp]
//	Author:sakamoto kai
//
//============================================
#include "BowmanArrow.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "modelEffect.h"
#include "player.h"
#include "CubeBlock.h"
#include "MapModel.h"
#include "Effect.h"

//�}�N����`
#define COLLISION_SIZE (D3DXVECTOR3(20.0f,20.0f,20.0f))		//�����蔻��
//====================================================================
//�R���X�g���N�^
//====================================================================
CBowmanArrow::CBowmanArrow(int nPriority) : CObjectX(nPriority)
{
	SetSize(COLLISION_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = NULL;			//�}�e���A���̐�
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_fLife = 150.0f;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CBowmanArrow::~CBowmanArrow()
{

}

//====================================================================
//��������
//====================================================================
CBowmanArrow* CBowmanArrow::Create(char* pModelName)
{
	CBowmanArrow* pSample = NULL;

	if (pSample == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample = new CBowmanArrow();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample->Init(pModelName)))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pSample;
}

//====================================================================
//����������
//====================================================================
HRESULT CBowmanArrow::Init(char* pModelName)
{
	SetType(CObject::TYPE_ARROW);

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


	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CBowmanArrow::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CBowmanArrow::Update(void)
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
void CBowmanArrow::TitleUpdate(void)
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
void CBowmanArrow::GameUpdate(void)
{
	//�X�V�O�̈ʒu���ߋ��̈ʒu�Ƃ���
	m_posOld = m_pos;

	if (!m_bStop)
	{
		// �ړ��ʂ����Z
		m_pos += m_move;

		CollisionMapObject(m_pos, m_posOld, useful::COLLISION_X);
		CollisionMapObject(m_pos, m_posOld, useful::COLLISION_Z);

		//�ʒu�X�V
		CObjectX::SetPos(m_pos);

		CModelEffect* pEffect = CModelEffect::Create("data\\MODEL\\weapon\\Arrow001.x");
		pEffect->SetPos(m_pos);
		pEffect->SetRot(GetRot());
		pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		//�v���C���[�Ƃ̓����蔻��
		CollisionPlayer();
	}

	//��ʊO����
	if (m_pos.y < 0.0f)
	{

	}

	//�傫���̐ݒ�
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//��ԊǗ�
	StateManager();

	//���_���̍X�V
	CObjectX::Update();

	if (m_fLife > 0)
	{
		m_fLife--;
	}
	else
	{
		SetDeathFlag(true);
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CBowmanArrow::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//��ԊǗ�
//====================================================================
void CBowmanArrow::StateManager(void)
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
//��ԊǗ�
//====================================================================
void CBowmanArrow::CollisionPlayer(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾
			float fAngle = 0.0f;

			if (type == TYPE_PLAYER3D)
			{//��ނ��v���C���[�̎�
				CPlayer* pPlayer = (CPlayer*)pObj;

				if (useful::CollisionCircle(m_pos, pPlayer->GetPos(), 70.0f) == true)
				{
					pPlayer->HitDamage(10.0f);
					Uninit();
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// �ǂƂ̓����蔻��
//====================================================================
void CBowmanArrow::CollisionMapObject(D3DXVECTOR3 Mypos, D3DXVECTOR3 MyposOld, useful::COLLISION XYZ)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
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
				D3DXVECTOR3 MyMove = m_move;
				bool JumpNull = false;
				D3DXVECTOR3 ObjMoveNull = INITVECTOR3;

				// ��`�̓����蔻��
				if (useful::CollisionBlock(pos, posOld, Move, Size, &Mypos, MyposOld, &MyMove, &ObjMoveNull, D3DXVECTOR3(5.0f, 5.0f, 5.0f), &JumpNull, XYZ) == true)
				{
					SetPos(MyposOld);
					m_bStop = true;
				}
			}
			else if (type == TYPE_MAPMODEL)
			{
				CMapModel* pMapModel = (CMapModel*)pObj; // �}�b�v�u���b�N���̎擾

				D3DXVECTOR3 pos = pMapModel->GetPos();
				D3DXVECTOR3 posOld = pMapModel->GetPosOld();
				D3DXVECTOR3 Move = pMapModel->GetMove();
				D3DXVECTOR3 Size = pMapModel->GetSize();
				D3DXVECTOR3 MyMove = m_move;
				bool JumpNull = false;
				D3DXVECTOR3 ObjMoveNull = INITVECTOR3;

				// ��`�̓����蔻��
				if (useful::CollisionBlock(pos, posOld, Move, Size, &Mypos, MyposOld, &MyMove, &ObjMoveNull, D3DXVECTOR3(5.0f, 5.0f, 5.0f), &JumpNull, XYZ) == true)
				{
					if (pMapModel->GetBreakEffect() == false)
					{
						SetPos(MyposOld);
						m_bStop = true;
					}
				}
			}


			pObj = pObjNext;
		}
	}
}