//============================================
//
//	敵の処理 [enemy.cpp]
//	Author:sakamoto kai
//
//============================================
#include "modelEffect.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "Particle.h"
#include "objGauge2D.h"
#include "number.h"
#include "input.h"
#include "CubeBlock.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
	const float MOVE_SPEED = 3.0f;		//移動速度
}

//==========================================
// 静的メンバ変数宣言
//==========================================
CListManager<CModelEffect>* CModelEffect::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CModelEffect::CModelEffect(int nPriority) : CObjectX(nPriority)
{
	SetSize(COLLISION_SIZE);
	m_nIdxXModel = 0;
	m_nIdxTexture = -1;
	m_pos = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//====================================================================
//デストラクタ
//====================================================================
CModelEffect::~CModelEffect()
{

}

//====================================================================
//生成処理
//====================================================================
CModelEffect* CModelEffect::Create(char* pModelName)
{
	CModelEffect* pBlock = nullptr;

	if (pBlock == nullptr)
	{
		//オブジェクト2Dの生成
		pBlock = new CModelEffect();
	}

	//オブジェクトの初期化処理
	if (FAILED(pBlock->Init(pModelName)))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pBlock;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CModelEffect::Init(char* pModelName)
{
	SetType(CObject::TYPE_EFFECT);

	CObjectX::Init(pModelName);

	// 静的メンバ変数
	static CListManager<CModelEffect>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CModelEffect>::AIterator m_iterator; // イテレーター

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CModelEffect::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CModelEffect::Update(void)
{
	if (m_color.a > 0.0f)
	{
		SetMatColor(m_color);
		m_color.a -= 0.05f;
	}
	else
	{
		Uninit();
	}

	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);
}

//====================================================================
//描画処理
//====================================================================
void CModelEffect::Draw(void)
{
	CObjectX::Draw();
}