//============================================
//
//	傾き装置の処理 [SlopeDevice.cpp]
//	Author:sakai minato
//
//============================================
#include "SlopeDevice.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "game.h"

#include "objectcharacter.h"
#include "model.h"
#include "motion.h"

#include "objmeshField.h"


//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 ASCENT_ADD = D3DXVECTOR3(0.0f, 1.5f, 0.0f);	// 上昇量
	const D3DXVECTOR3 DESCENT_DEST = D3DXVECTOR3(0.0f, 1.5f, 0.0f);	// 下降量
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CSlopeDevice>* CSlopeDevice::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CSlopeDevice::CSlopeDevice(int nPriority) : CObjectCharacter(nPriority)
{
	m_State = STATE(0);
	m_nStateCount = 0;

	m_LocateWorldType = LOCATE_WORLD_TYPE(0);

	m_pObjectCharacter = nullptr;
}

//====================================================================
//デストラクタ
//====================================================================
CSlopeDevice::~CSlopeDevice()
{

}

//====================================================================
//生成処理
//====================================================================
CSlopeDevice* CSlopeDevice::Create(const char* pModelNameSlopeDevice, const char* pModelNameEnemy)
{
	// オブジェクトの生成処理
	CSlopeDevice* pInstance = new CSlopeDevice();

	// オブジェクトの初期化処理
	if (FAILED(pInstance->Init()))
	{// 初期化処理が失敗した場合
		return nullptr;
	}

	// モデル関連初期化処理
	if (FAILED(pInstance->InitModel(pModelNameSlopeDevice, pModelNameEnemy)))
	{// 初期化処理が失敗した場合
		return nullptr;
	}

	return pInstance;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CSlopeDevice::Init(void)
{
	SetType(CObject::TYPE_DEVILHOLE);

	// 影を不使用に設定
	SetShadow(false);

	// キャラクタークラスの初期化（継承）
	if (FAILED(CObjectCharacter::Init())) { assert(false); }

	if (m_pObjectCharacter == nullptr)
	{
		// キャラクター（メダマン用）の生成処理
		m_pObjectCharacter = CObjectCharacter::Create(false);
	}

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CSlopeDevice>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CSlopeDevice::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// キャラクタークラスの終了（継承）
	CObjectCharacter::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CSlopeDevice::Update(void)
{
	// 値を取得
	D3DXVECTOR3 posMy = GetPos();			// 位置
	D3DXVECTOR3 posOldMy = GetPosOld();		// 前回の位置
	D3DXVECTOR3 rotMy = GetRot();			// 向き
	D3DXVECTOR3 sizeMy = GetSize();			// 大きさ

	// 過去の位置を記録
	posOldMy = posMy;

	//状態管理
	StateManager();

	// キャラクタークラスの更新（継承）
	CObjectCharacter::Update();

	// 値更新
	SetPos(posMy);			// 位置
	SetPosOld(posOldMy);	// 前回の位置
	SetRot(rotMy);			// 向き
	SetSize(sizeMy);		// 大きさ
}

//====================================================================
//描画処理
//====================================================================
void CSlopeDevice::Draw(void)
{
	// キャラクタークラスの描画（継承）
	CObjectCharacter::Draw();
}

//====================================================================
// 特定条件（傾き）の状態設定処理
//====================================================================
void CSlopeDevice::SetState(STATE state, CScrollArrow::Arrow stateArrow)
{
	switch (stateArrow)
	{
	case CScrollArrow::STATE_UP:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_RIGHT)
		{
			m_State = state;
		}
		break;
	case CScrollArrow::STATE_DOWN:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT)
		{
			m_State = state;
		}

		break;
	case CScrollArrow::STATE_LEFT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_LEFT)
		{
			m_State = state;
		}
		break;
	case CScrollArrow::STATE_RIGHT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_RIGHT)
		{
			m_State = state;
		}
		break;
	}
}

//====================================================================
// モデル関連の初期化処理
//====================================================================
HRESULT CSlopeDevice::InitModel(const char* pModelNameSlopeDevice, const char* pModelNameEnemy)
{
	CObjectCharacter::SetTxtCharacter(pModelNameSlopeDevice);

	if (m_pObjectCharacter != nullptr)
	{
		// キャラクターテキスト読み込み処理（メダマン）
		m_pObjectCharacter->SetTxtCharacter(pModelNameEnemy);

		// メダマンの親を土台に変更
		m_pObjectCharacter->GetModel(0)->SetParent(GetModel(SETUP_TYPE_JACK));

		// メダマンの位置を取得
		D3DXVECTOR3 pos = m_pObjectCharacter->GetModel(0)->GetStartPos();
		D3DXVECTOR3 rot = m_pObjectCharacter->GetModel(0)->GetStartRot();
		D3DXVECTOR3 posAdd = D3DXVECTOR3(0.0f, 0.0f, 400.0f);
		D3DXVECTOR3 rotAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// メダマンを上に
		m_pObjectCharacter->GetModel(0)->SetStartPos(pos + posAdd);
		m_pObjectCharacter->GetModel(0)->SetStartRot(rot + rotAdd);
	}

	return S_OK;
}

//====================================================================
//状態管理
//====================================================================
void CSlopeDevice::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ASCENT:

		Ascent(SETUP_TYPE_ELEVATING_PART, ASCENT_ADD,D3DXVECTOR3(0.0f,200.0f,0.0f));

		break;
	case STATE_DESCENT:

		Descent(SETUP_TYPE_ELEVATING_PART, DESCENT_DEST, D3DXVECTOR3(0.0f, 80.0f, 0.0f));

		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
// 上昇処理
//====================================================================
void CSlopeDevice::Ascent(int nNldxModel, D3DXVECTOR3 ascent, D3DXVECTOR3 ascentPosMax)
{
	// モデルの取得
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// モデルの位置を取得
	D3DXVECTOR3 pos = pModel->GetStartPos();

	// 最大上昇位置判定
	if (pos.y <= ascentPosMax.y)
	{
		// 上昇量を加算
		pos += ascent;
	}

	// モデルの位置更新
	pModel->SetStartPos(pos);
}

//====================================================================
// 下降処理
//====================================================================
void CSlopeDevice::Descent(int nNldxModel, D3DXVECTOR3 descent, D3DXVECTOR3 descentPosMin)
{
	// モデルの取得
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// モデルの位置を取得
	D3DXVECTOR3 pos = pModel->GetStartPos();

	// 最大下降位置判定
	if (pos.y >= descentPosMin.y)
	{
		// 下降量を減算
		pos -= descent;
	}

	// モデルの位置更新
	pModel->SetStartPos(pos);
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CSlopeDevice>* CSlopeDevice::GetList(void)
{
	return m_pList;
}
