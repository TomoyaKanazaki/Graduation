//============================================
//
//	�j�Ђ̏��� [debris.cpp]
//	Author:hashimoto kenta
//
//============================================
#include "debris.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "XModel.h"
#include "MapModel.h"

#include "CubeBlock.h"
#include "CubeColl.h"

#include "useful.h"

//�}�N����`
#define COLLISION_SIZE (D3DXVECTOR3(20.0f,20.0f,20.0f))		//�����蔻��
namespace
{
	char* MODEL_FILE_PATH_00 = "data\\MODEL\\debris\\CrashGlass00.x";	// ���f���̃p�X
	char* MODEL_FILE_PATH_01 = "data\\MODEL\\debris\\CrashGlass01.x";	// ���f���̃p�X
	char* MODEL_FILE_PATH_02 = "data\\MODEL\\debris\\CrashGlass02.x";	// ���f���̃p�X

	int SIZE_RANGE = 9;
	int SIZE_MIN = 8;
	int ROTMOVE_RANGE = 100;
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CDebris::CDebris(int nPriority) : CObjectX(nPriority)
{
	SetSize(COLLISION_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = NULL;			//�}�e���A���̐�
	m_Scaling = 1.0f;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CDebris::~CDebris()
{

}

//====================================================================
//��������
//====================================================================
CDebris* CDebris::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int ModelIdx)
{
	CDebris* pDebris = NULL;

	if (pDebris == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pDebris = new CDebris();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pDebris->Init(pos, move, ModelIdx)))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pDebris;
}

//====================================================================
//����������
//====================================================================
HRESULT CDebris::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int ModelIdx)
{
	char* pModelName = {};

	SetType(CObject::TYPE_DEBRIS);

	SetPos(pos);
	SetMove(move);
	SetRamScaling();
	SetRotMove();

	//X���f���̎擾
	CXModel::XModel* pXmodel = CManager::GetInstance()->GetXModel()->GetXModel(ModelIdx);

	//�}�e���A���f�[�^�ւ̃|�C���^����������
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)pXmodel->m_pBuffMat->GetBufferPointer();

	SetMatColor(pMat[(rand() % (int)pXmodel->m_dwNumMat)].MatD3D.Diffuse);

	pModelName = SetModelName();

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
void CDebris::Uninit()
{
	CObjectX::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CDebris::Update()
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
void CDebris::TitleUpdate()
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
void CDebris::GameUpdate()
{
	//�X�V�O�̈ʒu���ߋ��̈ʒu�Ƃ���
	m_posOld = m_pos;

	//�ʒu�X�V
	m_move.y -= 0.5f;
	m_pos += m_move;

	//�����X�V
	m_rot += m_rotmove;

	CollisionMapObject(m_pos, m_posOld, useful::COLLISION_X);
	CollisionMapObject(m_pos, m_posOld, useful::COLLISION_Y);
	CollisionMapObject(m_pos, m_posOld, useful::COLLISION_Z);

	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);

	//��ʊO����
	if (m_pos.y < 0.0f)
	{
		//m_pos.y = 0.0f;
		//m_move.y = 0.0f;

		Uninit();
	}

	//�傫���̐ݒ�
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//���_���̍X�V
	CObjectX::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CDebris::Draw()
{
	CObjectX::Draw();
}

//====================================================================
//���f���̃p�X��ݒ�
//====================================================================
char* CDebris::SetModelName()
{
	char* pModelName = {};
	int i = rand() % 3;

	switch (i)
	{
	case 0:
		pModelName = MODEL_FILE_PATH_00;
		break;

	case 1:
		pModelName = MODEL_FILE_PATH_01;
		break;

	case 2:
		pModelName = MODEL_FILE_PATH_02;
		break;

	default:
		break;
	}

	return pModelName;
}

//====================================================================
//���f���̃T�C�Y�������_���Őݒ�
//====================================================================
void CDebris::SetRamScaling()
{
	m_Scaling = (float)(rand() % SIZE_RANGE + SIZE_MIN) / 10;
}

//====================================================================
//��]�̈ړ��ʂ�ݒ�
//====================================================================
void CDebris::SetRotMove()
{
	m_rotmove.x = (float)(rand() % ROTMOVE_RANGE - ROTMOVE_RANGE / 2) / 100;
	m_rotmove.y = (float)(rand() % ROTMOVE_RANGE - ROTMOVE_RANGE / 2) / 100;
	m_rotmove.z = (float)(rand() % ROTMOVE_RANGE - ROTMOVE_RANGE / 2) / 100;
}

//====================================================================
// �ǂƂ̓����蔻��
//====================================================================
void CDebris::CollisionMapObject(D3DXVECTOR3 Mypos, D3DXVECTOR3 MyposOld, useful::COLLISION XYZ)
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
				if (useful::CollisionBlock(pos, posOld, Move, Size, &Mypos, MyposOld, &MyMove, &ObjMoveNull, GetSize() * 2.0f, &JumpNull, XYZ) == true)
				{
					m_move = MyMove;
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
				if (useful::CollisionBlock(pos, posOld, Move, Size, &Mypos, MyposOld, &MyMove, &ObjMoveNull, GetSize() * 2.0f, &JumpNull, XYZ) == true)
				{
					Uninit();
				}
			}


			pObj = pObjNext;
		}
	}
}
