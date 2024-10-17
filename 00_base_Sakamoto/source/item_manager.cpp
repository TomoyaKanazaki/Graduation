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
CItemManager::SItems CItemManager::ms_pItems[MAX_ITEM][MAX_SET] = {};

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
	//// �g�p����z�u����null�`�F�b�N
	//if (&ms_pItems[nMap] == nullptr) { assert(false); return; }

	//// �g�p����z�u�f�[�^��ݒ�
	//m_pItems = &ms_pItems[nMap];

	//// �z�u�^�C�v��ݒ�
	//m_eType = type;

	//// �A�C�e���̐ݒu
	//ItemSeting();
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
	///*
	//* �ǂݍ��ރf�[�^�ʂ��c��ɂȂ�\��������̂�
	//* �K���������Ŏ擾���邱��
	//*/

	//// TODO �A�C�e���z�u�f�[�^�̍쐬
	//// TODO �f�[�^�̓ǂݍ���
	//while (1)
	//{
	//	FILE* pFile = nullptr;

	//	switch (ms_nNumMap)
	//	{
	//	case 0:
	//		//�t�@�C�����J��
	//		pFile = fopen("data\\TXT\\STAGE\\Item00.txt", "r");
	//		break;
	//	case 1:
	//		//�t�@�C�����J��
	//		pFile = fopen("data\\TXT\\STAGE\\Item01.txt", "r");
	//		break;
	//	default:
	//		break;
	//	}

	//	if (pFile != nullptr)
	//	{//�t�@�C�����J�����ꍇ

	//		char aString[128] = {};			//���b�Z�[�W�ۑ��p

	//		fscanf(pFile, "%s", &aString[0]);
	//		if (strcmp(&aString[0], "STARTSETSTAGE") == 0)
	//		{
	//			int nNumberCount = 0;
	//			while (1)
	//			{
	//				fscanf(pFile, "%s", &aString[0]);
	//				if (strcmp(&aString[0], "STARTSETITEM") == 0)
	//				{
	//					D3DXVECTOR3 pos;
	//					int Type;

	//					fscanf(pFile, "%s", &aString[0]);
	//					fscanf(pFile, "%f", &pos.x);
	//					fscanf(pFile, "%f", &pos.y);
	//					fscanf(pFile, "%f", &pos.z);

	//					fscanf(pFile, "%s", &aString[0]);
	//					fscanf(pFile, "%d", &Type);

	//					ms_pItems[ms_nNumMap].pPos[nNumberCount] = pos;
	//					ms_pItems[ms_nNumMap].pType[nNumberCount] = (CItem::TYPE)Type;
	//					nNumberCount++;

	//					fscanf(pFile, "%s", &aString[0]);
	//					if (strcmp(&aString[0], "ENDSETITEM") == 0)
	//					{
	//						break;
	//					}
	//				}
	//				else if (strcmp(&aString[0], "ENDSETSTAGE") == 0)
	//				{
	//					ms_pItems[ms_nNumMap].nNum = nNumberCount;
	//					break;
	//				}
	//			}
	//		}
	//		fclose(pFile);
	//	}
	//	else
	//	{//�t�@�C�����J���Ȃ������ꍇ
	//		break;
	//	}
	//}

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

	//// �p�ӂ���Ă���S�Ẵf�[�^��j������
	//for (int i = 0; i < ms_nNumMap; ++i)
	//{
	//	for (int j = 0; j < ms_pItems[i].nNum; ++j)
	//	{
	//		delete &ms_pItems[i].pType[j];
	//		delete &ms_pItems[i].pPos[j];
	//	}
	//	delete &ms_pItems[i];
	//}
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
