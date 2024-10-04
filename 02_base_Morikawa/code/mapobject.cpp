//========================================
//
// マップオブジェクト[mapobject.cpp]
// Author：森川駿弥
//
//========================================
#include "mapobject.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//========================================
// 定数定義
//========================================
namespace
{
const char* OBJECT_PASS = "data\\MODEL\\stage\\tougijo_001.x";	// マップオブジェクトのパス
}

//========================================
// コンストラクタ
//========================================
CMapObject::CMapObject(int nPriority) :
	m_pModel(nullptr)
{
}

//========================================
// デストラクタ
//========================================
CMapObject::~CMapObject()
{
}

//========================================
// マップオブジェクト生成
//========================================
CMapObject* CMapObject::Create()
{
	// CMapObject型のポインタ
	CMapObject* pMapObject = new CMapObject;

	pMapObject->Init();

	//ポインタを返す
	return pMapObject;
}

//========================================
// 初期化
//========================================
HRESULT CMapObject::Init(void)
{
	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	if (m_pModel == nullptr)
	{
		m_pModel = CModel::Create(OBJECT_PASS);

		// 階層
		m_pModel->SetType(CModel::TYPE_NOT_HIERARCHY);
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CMapObject::Uninit(void)
{
	if (m_pModel != nullptr)
	{
		m_pModel->Uninit();
		m_pModel = nullptr;
	}

	// 自身の終了
	Release();
}

//========================================
// 更新
//========================================
void CMapObject::Update(void)
{
	if (m_pModel != nullptr)
	{// モデルの更新
		m_pModel->Update();
	}
}

//========================================
// 描画
//========================================
void CMapObject::Draw(void)
{
	if (m_pModel != nullptr)
	{// モデルの描画
		m_pModel->Draw();
	}
}
