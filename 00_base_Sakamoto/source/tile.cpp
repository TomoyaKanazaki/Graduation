//============================================
//
//	�����f���̏��� [tile.cpp]
//	Author:Satone Shion
//
//============================================
#include "tile.h"
#include "renderer.h"
#include "manager.h"
#include "objmeshField.h"
#include "game.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	
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
	m_Grid.x = 0;
	m_Grid.z = 0;
}

//====================================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//====================================================================
CTile::CTile(int nPriority, CMapSystem::GRID gridCenter) : CObjectX(nPriority)
{
	SetSize(INITVECTOR3);
	SetPos(INITVECTOR3);
	m_Grid.x = gridCenter.x;	// �O���b�h�̈ʒuX
	m_Grid.z = gridCenter.z;	// �O���b�h�̈ʒuZ
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
CTile* CTile::Create(CMapSystem::GRID gridCenter)
{
	CTile* pSample = nullptr;

	if (pSample == nullptr)
	{
		// ���̐���
		pSample = new CTile(3, gridCenter);
	}

	// ���̏���������
	if (FAILED(pSample->Init("data\\MODEL\\floor.x")))
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
	CMapSystem* pMapSystem = CMapSystem::GetInstance();		// �}�b�v�V�X�e���̏��
	D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

	SetType(CObject::TYPE_TILE);

	CObjectX::Init(pModelName);

	//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetUseMultiMatrix(CGame::GetInstance()->GetMapField()->GetMatrix());

	D3DXVECTOR3 size = GetSize() * 2.0f;		// ���f���̃T�C�Y�擾

	// �ʒu�ݒ�
	CObjectX::SetPos(D3DXVECTOR3(MapSystemPos.x + m_Grid.x * size.x, 0.0f, MapSystemPos.z - m_Grid.z * size.z));

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