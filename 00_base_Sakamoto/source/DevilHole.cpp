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

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(50.0f, 0.0f, 50.0f);		//�����蔻��
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CDevilHole::CDevilHole(int nPriority) : CObjectX(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = NULL;			//�}�e���A���̐�
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	
	for (int nCnt = 0; nCnt < 4; nCnt++)
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
	CDevilHole* pSample = NULL;

	if (pSample == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample = new CDevilHole();
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


	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CDevilHole::Uninit(void)
{
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

	//�ʒu�X�V
	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);

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
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == TYPE_PLAYER3D)
			{//��ނ��v���C���[�̎�

				//* pBlock = (CCubeBlock*)pObj;	// �u���b�N���̎擾

				//D3DXVECTOR3 pos = pBlock->GetPos();
				//D3DXVECTOR3 posOld = pBlock->GetPosOld();
				//D3DXVECTOR3 Move = pBlock->GetMove();
				//D3DXVECTOR3 Size = pBlock->GetSize();

				//// ��`�̓����蔻��
				//if (useful::CollisionBlock(pos, posOld, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
				//{
				//	//�ҋ@��Ԃɂ���
				//	m_State = STATE_WAIT;
				//}
			}

			pObj = pObjNext;
		}
	}
}