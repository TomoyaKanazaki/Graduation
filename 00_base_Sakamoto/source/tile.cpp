//============================================
//
//	�f�r���z�[���̏��� [Tile.cpp]
//	Author:sakamoto kai
//
//============================================
#include "Tile.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "objmeshField.h"
#include "game.h"
#include "tutorial.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	//const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(50.0f, 0.0f, 50.0f);		//�����蔻��
	//const int DIRECTION = 4;	// �f�r���z�[���̍ő����
}

//====================================================================
//�ÓI�����o�ϐ��錾
//====================================================================
CListManager<CTile>* CTile::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CTile::CTile(int nPriority) : CObjectX(nPriority)
{
	SetSize(INITVECTOR3);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//�}�e���A���̐�
	m_Grid.x = 0;
	m_Grid.z = 0;
	m_pos = INITVECTOR3;	
	m_posOld = INITVECTOR3;
	m_move = INITVECTOR3;
	
	//for (int nCnt = 0; nCnt < DIRECTION; nCnt++)
	//{
	//	m_bSet[nCnt] = false;			//�㉺���E�̌������܂��Ă��邩�ǂ���
	//	m_pHoleKey[nCnt] = nullptr;		//�㉺���E�̌��𖄂߂�|���S��
	//}
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CTile::~CTile()
{

}

//====================================================================
//��������
//====================================================================
CTile* CTile::Create(char* pModelName)
{
	CTile* pSample = nullptr;

	if (pSample == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample = new CTile();
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
HRESULT CTile::Init(char* pModelName)
{
	SetType(CObject::TYPE_DEVILHOLE);

	CObjectX::Init(pModelName);

	//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetUseMultiMatrix(CGame::GetInstance()->GetMapField()->GetMatrix());

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CTile>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CTile::Uninit(void)
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
void CTile::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	//�ʒu�X�V
	pos += m_move;

	SetPos(pos);

	//���_���̍X�V
	CObjectX::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CTile::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CTile>* CTile::GetList(void)
{
	return m_pList;
}
