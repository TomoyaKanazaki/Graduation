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

//==========================================
//  �Q�[���ł̐ݒu����
//==========================================
void BgObj::SetGame(CMapSystem::GRID& grid)
{
	// �����̐ݒu����
	SetFieldBotton(grid);

	// �}�b�v�ړ��E�X�����u�̐ݒu����
	SetScrollDevice();
	SetSlopeDevice();
}

//==========================================
//  �����ݒu����
//==========================================
void BgObj::SetFieldBotton(CMapSystem::GRID& grid)
{
	// �����̐���
	CObjmeshField* pBottonField = CObjmeshField::Create(grid);
	pBottonField->SetTexture(BOTTOM_FIELD_TEX);
	pBottonField->SetPos(BOTTOM_FIELD_POS);
}

//==========================================
//  �}�b�v�ړ����u�̐ݒu����
//==========================================
void BgObj::SetScrollDevice(void)
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
void BgObj::SetSlopeDevice(void)
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