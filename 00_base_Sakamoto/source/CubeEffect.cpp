//============================================
//
//	キューブエフェクト [CubeEffect.cpp]
//	Author:sakamoto kai
//
//============================================
#include "CubeEffect.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const int WAIGHT_SIZE = 11;		//横の頂点数
	const int HEIGHT_SIZE = 11;		//縦の頂点数
	const float WAIGHT_CENTER = 0.5f;	//横の原点(0.0f～1.0f)
	const float HEIGHT_CENTER = 0.5f;	//縦の原点(0.0f～1.0f)
	const float FIELD_SIZE = 200.0f;		//床一枚の大きさ
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CCubeEffect>* CCubeEffect::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CCubeEffect::CCubeEffect(int nPriority) :CObjmeshCube(nPriority)
{
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_nLife = -1;
	m_bFall = false;
}

//====================================================================
//デストラクタ
//====================================================================
CCubeEffect::~CCubeEffect()
{

}

//====================================================================
//生成処理
//====================================================================
CCubeEffect* CCubeEffect::Create(void)
{
	CCubeEffect* pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		//オブジェクト3Dの生成
		pObject3D = new CCubeEffect();
	}

	//オブジェクトの初期化処理
	if (FAILED(pObject3D->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pObject3D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CCubeEffect::Init(void)
{
	CObjmeshCube::Init();

	SetTexture("data\\TEXTURE\\Test.jpg");
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	SetType(TYPE_CUBEEFFECT);

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CCubeEffect>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CCubeEffect::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	CObjmeshCube::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CCubeEffect::Update(void)
{
	m_pos = GetPos();

	if (m_bFall == true)
	{
		m_move.y -= 0.98f;
	}

	m_pos += m_move;

	CObjmeshCube::SetPos(m_pos);
	CObjmeshCube::Update();

	if (m_nLife > 0 && m_nLife != -1)
	{
		m_nLife--;
	}
	else if(m_nLife == 0)
	{
		Uninit();
		return;
	}

	if (m_bFall == true)
	{
		if (m_pos.y < 0.0f)
		{
			Uninit();
			return;
		}
	}
}

//====================================================================
//描画処理
//====================================================================
void CCubeEffect::Draw(void)
{
	CObjmeshCube::Draw();
}