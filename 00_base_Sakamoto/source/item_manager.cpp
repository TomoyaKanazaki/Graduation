//==========================================
//
//  �A�C�e���Ǘ��N���X(item_manager.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "item_manager.h"

//==========================================
//  �ÓI�����o�ϐ��錾
//==========================================
int CItemManager::ms_nNumMap = 0;
CItemManager::SItems* CItemManager::ms_pItems = nullptr;

//==========================================
//  �R���X�g���N�^
//==========================================
CItemManager::CItemManager() : 
	m_eType(TYPE_NONE),
	m_pItems(nullptr)
{
}

//==========================================
//  �f�X�g���N�^
//==========================================
CItemManager::~CItemManager()
{
}

//==========================================
//  ����������
//==========================================
void CItemManager::Init(const int nMap, const EType type)
{
	// �g�p����z�u����null�`�F�b�N
	if (&ms_pItems[nMap] == nullptr) { assert(false); return; }

	// �g�p����z�u�f�[�^��ݒ�
	m_pItems = &ms_pItems[nMap];

	// �z�u�^�C�v��ݒ�
	m_eType = type;

	// �A�C�e���̐ݒu
	ItemSeting();
}

//==========================================
//  �I������
//==========================================
void CItemManager::Uninit()
{
	// ���g�̏I������
	delete this;
}

//==========================================
//  �X�V����
//==========================================
void CItemManager::Update()
{
	// Do Nothing
}

//==========================================
//  �`�揈��
//==========================================
void CItemManager::Draw()
{
	// Do Nothing
}

//==========================================
//  ��������
//==========================================
CItemManager* CItemManager::Create(const int nMap, const EType type)
{
	// �C���X�^���X����
	CItemManager* manager = new CItemManager;

	// �������̊m�ۂɎ��s�����ꍇ�֐��𔲂���
	if (manager == nullptr) { assert(false); return nullptr; }

	// ����������
	manager->Init(nMap, type);

	return manager;
}

//==========================================
//  ���擾
//==========================================
HRESULT CItemManager::Load()
{
	/*
	* �ǂݍ��ރf�[�^�ʂ��c��ɂȂ�\��������̂�
	* �K���������Ŏ擾���邱��
	*/

	// TODO �A�C�e���z�u�f�[�^�̍쐬
	// TODO �f�[�^�̓ǂݍ���

	return S_OK;
}

//==========================================
//  ���j��
//==========================================
void CItemManager::UnLoad()
{
	/*
	* �Q�[�����ɔj������ƍēx�ǂݍ��ޕK�v�����邽��
	* �K���I���Ŕj�����邱��
	*/

	// �p�ӂ���Ă���S�Ẵf�[�^��j������
	for (int i = 0; i < ms_nNumMap; ++i)
	{
		for (int j = 0; j < ms_pItems[i].nNum; ++j)
		{
			delete &ms_pItems[i].pType[j];
			delete &ms_pItems[i].pPos[j];
		}
		delete &ms_pItems[i];
	}
}

//==========================================
//  �A�C�e���̐ݒu����
//==========================================
void CItemManager::ItemSeting()
{
	// �ݒ肳���z�u�^�C�v�ɂ���ĕ���
	switch (m_eType)
	{
	case TYPE_BOWABOWA: // �{���{���z�u

		// �z�u����{������
		for (int i = 0; i < m_pItems->nNum; ++i)
		{
			// �z�u�����A�C�e���������̏ꍇ���ɐi��
			if (m_pItems->pType[i] == CItem::TYPE_BIBLE) { continue; }

			// �A�C�e���̐���
			CItem::Create(m_pItems->pType[i], m_pItems->pPos[i]);
		}

		break;

	case TYPE_BIBLE: // �����z�u
		
		// �z�u����{������
		for (int i = 0; i < m_pItems->nNum; ++i)
		{
			// �z�u�����A�C�e���������łȂ��ꍇ���ɐi��
			if (m_pItems->pType[i] != CItem::TYPE_BIBLE) { continue; }

			// �A�C�e���̐���
			CItem::Create(m_pItems->pType[i], m_pItems->pPos[i]);
		}

		break;

	default:
		assert(false);
		break;
	}
}
