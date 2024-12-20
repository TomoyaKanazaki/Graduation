//============================================
//
//	背景オブジェクト [bjObj.cpp]
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

// 定数定義
namespace
{
	const char* BOTTOM_FIELD_TEX = "data\\TEXTURE\\Field\\tile_test_02.png";	// 下床のテクスチャ
	const D3DXVECTOR3 BOTTOM_FIELD_POS = D3DXVECTOR3(0.0f, -1000.0f, 0.0f);		// 下床の位置

	const std::string SCROLL_DEVICE_MODEL_HEIGHT = "data\\TXT\\MOTION\\02_staging\\00_ScrollDevice\\motion_scrolldevice_h.txt";
	const std::string SCROLL_DEVICE_MODEL_WIDTH = "data\\TXT\\MOTION\\02_staging\\00_ScrollDevice\\motion_scrolldevice_w.txt";
	const std::string SCROLL_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";

	const std::string SLOPE_DEVICE_MODEL = "data\\TXT\\MOTION\\02_staging\\01_SlopeDevice\\motion_slopedevice.txt";
	const std::string SLOPE_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";
}

// 静的メンバ変数宣言
CBgObj* CBgObj::m_pBgObj = nullptr;

//====================================================================
//コンストラクタ
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
//デストラクタ
//====================================================================
CBgObj::~CBgObj()
{

}

//====================================================================
//インスタンス取得
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
//  初期化処理
//==========================================
HRESULT CBgObj::Init(void)
{
	return S_OK;
}

//==========================================
//  終了処理
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
//  更新処理
//==========================================
void CBgObj::Update(void)
{
	// カウント加算
	m_nCount++;

	// 山の更新処理
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
//  ゲームでの設置処理
//==========================================
void CBgObj::SetGame(CMapSystem::GRID& grid)
{
	// 下床の設置処理
	SetFieldBotton(grid);

	// マップ移動・傾き装置の設置処理
	SetScrollDevice();
	SetSlopeDevice();

	// 山オブジェクトの設置処理
	SetMountain();
}

//==========================================
//  下床設置処理
//==========================================
void CBgObj::SetFieldBotton(CMapSystem::GRID& grid)
{
	// 下床の生成
	CObjmeshField* pBottonField = CObjmeshField::Create(grid);
	pBottonField->SetTexture(BOTTOM_FIELD_TEX);
	pBottonField->SetPos(BOTTOM_FIELD_POS);
}

//==========================================
//  マップ移動装置の設置処理
//==========================================
void CBgObj::SetScrollDevice(void)
{
	// 上下
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

	// 左右
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
//  傾き装置の設置処理
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
//  山オブジェクトの設置処理
//==========================================
void CBgObj::SetMountain(void)
{
	//右山
	CObjectX* pRMountain = CObjectX::Create("data\\MODEL\\RightMountain.x");
	pRMountain->SetPos(D3DXVECTOR3(2000.0f, -1000.0f, 2000.0f));

	//左山
	CObjectX* pLMountain = CObjectX::Create("data\\MODEL\\LeftMountain.x");
	pLMountain->SetPos(D3DXVECTOR3(-1900.0f, -1000.0f, 1500.0f));

	//マグマ
	CObject3D* pMaguma01 = CObject3D::Create();
	pMaguma01->SetPos(D3DXVECTOR3(0.0f, -950.0f, 0.0f));
	pMaguma01->SetSize(D3DXVECTOR3(3000.0f, 0.0f, 6500.0f));
	pMaguma01->SetScrollSpeed(D3DXVECTOR2(0.0f, 0.0002f));
	pMaguma01->SetTexture("data\\TEXTURE\\Field\\maguma_02.jpg");

	//マグマ
	CObject3D* pMaguma00 = CObject3D::Create();
	pMaguma00->SetPos(D3DXVECTOR3(0.0f, -940.0f, 0.0f));
	pMaguma00->SetSize(D3DXVECTOR3(3000.0f, 0.0f, 6500.0f));
	pMaguma00->SetScrollSpeed(D3DXVECTOR2(0.0f, -0.0003f));
	pMaguma00->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	pMaguma00->SetTexture("data\\TEXTURE\\Field\\maguma_00.jpg");

	//メダマン
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