//============================================
//
//	�w�i�I�u�W�F�N�g [bjObj.cpp]
//	Author:sakai minato
//
//============================================

#include "bgObj.h"

#include "objmeshField.h"

#include "ScrollDevice.h"
#include "SlopeDevice.h"

#include "objectX.h"
#include "object3D.h"

#include "objectcharacter.h"
#include "motion.h"

// �萔��`
namespace
{
	const char* BOTTOM_FIELD_TEX = "data\\TEXTURE\\Field\\tile_test_02.png";	// �����̃e�N�X�`��
	const D3DXVECTOR3 BOTTOM_FIELD_POS = D3DXVECTOR3(0.0f, -1000.0f, 0.0f);		// �����̈ʒu

	const std::string SCROLL_DEVICE_MODEL_HEIGHT = "data\\TXT\\MOTION\\02_staging\\00_ScrollDevice\\motion_scrolldevice_h.txt";
	const std::string SCROLL_DEVICE_MODEL_WIDTH = "data\\TXT\\MOTION\\02_staging\\00_ScrollDevice\\motion_scrolldevice_w.txt";
	const std::string SCROLL_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";

	const std::string SLOPE_DEVICE_MODEL = "data\\TXT\\MOTION\\02_staging\\01_SlopeDevice\\motion_slopedevice.txt";
	const std::string SLOPE_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";
}

// �ÓI�����o�ϐ��錾
CBgObj* CBgObj::m_pBgObj = nullptr;

//====================================================================
//�R���X�g���N�^
//====================================================================
CBgObj::CBgObj()
{
	for (int nCnt = 0; nCnt < MOUNTAIN_OBJ_NUM; nCnt++)
	{
		m_pBGCharacter[nCnt] = nullptr;
	}

	m_nCount = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CBgObj::~CBgObj()
{

}

//====================================================================
//�C���X�^���X�擾
//====================================================================
CBgObj* CBgObj::GetInstance(void)
{
	if (m_pBgObj == nullptr)
	{
		m_pBgObj = new CBgObj;
		m_pBgObj->Init();
	}

	return m_pBgObj;
}

//==========================================
//  ����������
//==========================================
HRESULT CBgObj::Init(void)
{
	return S_OK;
}

//==========================================
//  �I������
//==========================================
void CBgObj::Uninit(void)
{
	for (int nCnt = 0; nCnt < MOUNTAIN_OBJ_NUM; nCnt++)
	{
		if (m_pBGCharacter[nCnt] != nullptr)
		{
			m_pBGCharacter[nCnt]->Uninit();
			m_pBGCharacter[nCnt] = nullptr;
		}
	}

	if (m_pBgObj != nullptr)
	{
		delete m_pBgObj;
		m_pBgObj = nullptr;
	}

}

//==========================================
//  �X�V����
//==========================================
void CBgObj::Update(void)
{
	// �J�E���g���Z
	m_nCount++;

	// �R�̍X�V����
	for (int nCnt = 0; nCnt < MOUNTAIN_OBJ_NUM; nCnt++)
	{
		if (m_pBGCharacter[nCnt] != nullptr)
		{
			m_pBGCharacter[nCnt]->SetPos(D3DXVECTOR3(
				-1975.0f + sinf(D3DX_PI * (0.5f * (nCnt + (float)m_nCount * 0.005f))) * 400.0f,
				-200.0f,
				1500.0f + cosf(D3DX_PI * (0.5f * (nCnt + (float)m_nCount * 0.005f))) * 250.0f));

			m_pBGCharacter[nCnt]->SetRot(D3DXVECTOR3(
				0.0f,
				sinf(D3DX_PI * (0.5f * ((nCnt - 1) + (float)m_nCount * 0.005f))),
				0.0f));
		}
	}
}

//==========================================
//  �Q�[���ł̐ݒu����
//==========================================
void CBgObj::SetGame(CMapSystem::GRID& grid)
{
	// �����̐ݒu����
	SetFieldBotton(grid);

	// �}�b�v�ړ��E�X�����u�̐ݒu����
	SetScrollDevice();
	SetSlopeDevice();

	// �R�I�u�W�F�N�g�̐ݒu����
	SetMountain();
}

//==========================================
//  �����ݒu����
//==========================================
void CBgObj::SetFieldBotton(CMapSystem::GRID& grid)
{
	// �����̐���
	CObjmeshField* pBottonField = CObjmeshField::Create(grid);
	pBottonField->SetTexture(BOTTOM_FIELD_TEX);
	pBottonField->SetPos(BOTTOM_FIELD_POS);
}

//==========================================
//  �}�b�v�ړ����u�̐ݒu����
//==========================================
void CBgObj::SetScrollDevice(void)
{
	// �㉺
	CScrollDevice* pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL_HEIGHT, SCROLL_DEVICE_ENEMY_MODEL);
	pScrollDevice->SetPos(D3DXVECTOR3(0.0f, 75.0f, 700.0f));
	pScrollDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	pScrollDevice->SetLocateWorldType(CScrollDevice::LOCATE_WORLD_TYPE_TOP);
	pScrollDevice->SetState(CScrollDevice::STATE_NORMAL);

	pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL_HEIGHT, SCROLL_DEVICE_ENEMY_MODEL);
	pScrollDevice->SetPos(D3DXVECTOR3(0.0f, 75.0f, -700.0f));
	pScrollDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	pScrollDevice->SetLocateWorldType(CScrollDevice::LOCATE_WORLD_TYPE_BOTTOM);
	pScrollDevice->SetState(CScrollDevice::STATE_NORMAL);

	// ���E
	pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL_WIDTH, SCROLL_DEVICE_ENEMY_MODEL);
	pScrollDevice->SetPos(D3DXVECTOR3(1075.0f, 75.0f, 0.0f));
	pScrollDevice->SetLocateWorldType(CScrollDevice::LOCATE_WORLD_TYPE_LEFT);
	pScrollDevice->SetState(CScrollDevice::STATE_NORMAL);

	pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL_WIDTH, SCROLL_DEVICE_ENEMY_MODEL);
	pScrollDevice->SetPos(D3DXVECTOR3(-1075.0f, 75.0f, 0.0f));
	pScrollDevice->SetLocateWorldType(CScrollDevice::LOCATE_WORLD_TYPE_RIGHT);
	pScrollDevice->SetState(CScrollDevice::STATE_NORMAL);
}

//==========================================
//  �X�����u�̐ݒu����
//==========================================
void CBgObj::SetSlopeDevice(void)
{
	CSlopeDevice* pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
	pSlopeDevice->SetPos(D3DXVECTOR3(800.0f, BOTTOM_FIELD_POS.y, 450.0f));
	pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_TOP_LEFT);

	pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
	pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	pSlopeDevice->SetPos(D3DXVECTOR3(-800.0f, BOTTOM_FIELD_POS.y, 450.0f));
	pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_TOP_RIGHT);

	pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
	pSlopeDevice->SetPos(D3DXVECTOR3(800.0f, BOTTOM_FIELD_POS.y, -450.0f));
	pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_BOTTOM_LEFT);

	pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
	pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	pSlopeDevice->SetPos(D3DXVECTOR3(-800.0f, BOTTOM_FIELD_POS.y, -450.0f));
	pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_BOTTOM_RIGHT);
}

//==========================================
//  �R�I�u�W�F�N�g�̐ݒu����
//==========================================
void CBgObj::SetMountain(void)
{
	//�E�R
	CObjectX* pRMountain = CObjectX::Create("data\\MODEL\\RightMountain.x");
	pRMountain->SetPos(D3DXVECTOR3(2000.0f, -1000.0f, 2000.0f));

	//���R
	CObjectX* pLMountain = CObjectX::Create("data\\MODEL\\LeftMountain.x");
	pLMountain->SetPos(D3DXVECTOR3(-1900.0f, -1000.0f, 1500.0f));

	//�}�O�}
	CObject3D* pMaguma01 = CObject3D::Create();
	pMaguma01->SetPos(D3DXVECTOR3(0.0f, -950.0f, 0.0f));
	pMaguma01->SetSize(D3DXVECTOR3(3000.0f, 0.0f, 6500.0f));
	pMaguma01->SetScrollSpeed(D3DXVECTOR2(0.0f, 0.0002f));
	pMaguma01->SetTexture("data\\TEXTURE\\Field\\maguma_02.jpg");

	//�}�O�}
	CObject3D* pMaguma00 = CObject3D::Create();
	pMaguma00->SetPos(D3DXVECTOR3(0.0f, -940.0f, 0.0f));
	pMaguma00->SetSize(D3DXVECTOR3(3000.0f, 0.0f, 6500.0f));
	pMaguma00->SetScrollSpeed(D3DXVECTOR2(0.0f, -0.0003f));
	pMaguma00->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	pMaguma00->SetTexture("data\\TEXTURE\\Field\\maguma_00.jpg");

	//���_�}��
	for (int nCnt = 0; nCnt < MOUNTAIN_OBJ_NUM; nCnt++)
	{
		if (m_pBGCharacter[nCnt] == nullptr)
		{
			m_pBGCharacter[nCnt] = CObjectCharacter::Create(false);
			m_pBGCharacter[nCnt]->SetTxtCharacter(SCROLL_DEVICE_ENEMY_MODEL, 0);

			m_pBGCharacter[nCnt]->SetPos(D3DXVECTOR3(
				-1975.0f + sinf(D3DX_PI * (0.5f * nCnt)) * 400.0f,
				-200.0f,
				1500.0f + cosf(D3DX_PI * (0.5f * nCnt)) * 250.0f));

			m_pBGCharacter[nCnt]->SetRot(D3DXVECTOR3(
				0.0f,
				sinf(D3DX_PI * (0.5f * (nCnt - 1))),
				0.0f));

			m_pBGCharacter[nCnt]->GetMotion()->Set(1, 0);
		}
	}
}