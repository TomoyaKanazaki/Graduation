//========================================
//
//キャラクター全般処理[character.cpp]
//Author：森川駿弥
//
//========================================
#include "character.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugproc.h"
#include "game.h"
#include "camera.h"
#include "particle.h"

//========================================
// 定数定義
//========================================
namespace
{
const float CHARACTER_INERTIA = 0.3f;	// キャラクターの慣性
}

//========================================
//コンストラクタ
//========================================
CCharacter::CCharacter(int nPriority) :
	m_pTexture(nullptr),
	m_nIdxTexture(0),				// テクスチャの番号
	m_pMesh(nullptr),				// メッシュ(頂点情報)へのポインタ
	m_pBuffMat(nullptr),			// マテリアルへのポインタ
	m_dwNumMat(0),					// マテリアルの数
	m_apNumModel(0), 				// モデル(パーツ)の総数
	m_RotDest(0.0f, 0.0f, 0.0f),	// 目的の向き
	m_nState(STATE_NONE),			// キャラの状態
	m_bJump(false),					// ジャンプ中
	m_bMove(false),					// 移動モーション
	m_bWait(false),					// 待機モーション
	m_pMotion(nullptr)				// モーションポインタ
{//値をクリア
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//モデルのポインタ
}

//========================================
//デストラクタ
//========================================
CCharacter::~CCharacter()
{
}

//========================================
// 初期化
//========================================
HRESULT CCharacter::Init(std::string pfile)
{
	D3DXVECTOR3 pos = GetPos();		// 位置取得
	D3DXVECTOR3 move = GetMove();	// 移動量取得

	// 向きの設定
	m_RotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 最小値
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 最大値
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 位置設定
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 向き設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	// 状態初期化
	m_nState = STATE_NONE;

	//モーションのポインタ
	m_pMotion = nullptr;

	if (m_pMotion == nullptr)
	{
		// モーション生成
		m_pMotion = CMotion::Create();

		// モーション読み込み
		m_pMotion->Load(pfile);

		// 待機
		m_bWait = true;
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CCharacter::Uninit(void)
{
	if (m_pMotion != nullptr)
	{//モーション破棄
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	//オブジェクト(自分自身)の破棄
	Release();
}

//========================================
// 更新
//========================================
void CCharacter::Update(void)
{
	// モーション更新
	m_pMotion->Update();
}

//========================================
// 描画
//========================================
void CCharacter::Draw(void)
{
	//描画
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//CRenderer型のポインタ
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//位置取得
	D3DXVECTOR3 pos = GetPos();

	// 向き取得
	D3DXVECTOR3 rot = GetRot();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モーション描画
	m_pMotion->Draw();
}
