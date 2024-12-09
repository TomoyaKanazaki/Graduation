//============================================
//
//	マップ移動装置の処理 [ScrollDevice.cpp]
//	Author:sakai minato
//
//============================================
#include "ScrollDevice.h"
#include "renderer.h"
#include "manager.h"

#include "objectcharacter.h"
#include "model.h"
#include "motion.h"

#include "objmeshField.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 ROTATE_ADD = D3DXVECTOR3(0.0f, 0.0f, 0.05f);
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CScrollDevice>* CScrollDevice::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CScrollDevice::CScrollDevice(int nPriority) : CObjectCharacter(nPriority)
{
	m_State = STATE(0);
	m_nStateCount = 0;

	m_pObjectCharacter = nullptr;
}

//====================================================================
//デストラクタ
//====================================================================
CScrollDevice::~CScrollDevice()
{

}

//====================================================================
//生成処理
//====================================================================
CScrollDevice* CScrollDevice::Create(const char* pModelNameSlopeDevice, const char* pModelNameEnemy)
{
	// オブジェクトの生成処理
	CScrollDevice* pInstance = new CScrollDevice();

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
HRESULT CScrollDevice::Init(void)
{
	SetType(CObject::TYPE_DEVILHOLE);

	// 影を不使用に設定
	SetShadow(false);

	// キャラクタークラスの初期化（継承）
	if (FAILED(CObjectCharacter::Init())) { assert(false); }

	// マップマトリックスと紐づけ
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	if (m_pObjectCharacter == nullptr)
	{
		// キャラクター（メダマン用）の生成処理
		if (FAILED(m_pObjectCharacter = CObjectCharacter::Create(false))) { assert(false); }

		// マップマトリックスと紐づけ
		m_pObjectCharacter->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
	}

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CScrollDevice>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CScrollDevice::Uninit(void)
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
void CScrollDevice::Update(void)
{
	// 値を取得
	D3DXVECTOR3 posMy = GetPos();			// 位置
	D3DXVECTOR3 posOldMy = GetPosOld();		// 前回の位置
	D3DXVECTOR3 rotMy = GetRot();			// 向き
	D3DXVECTOR3 sizeMy = GetSize();			// 大きさ

	// 過去の位置を記録
	posOldMy = posMy;

	//状態管理
	StateManager(rotMy);

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
void CScrollDevice::Draw(void)
{
	// キャラクタークラスの描画（継承）
	CObjectCharacter::Draw();
}

//====================================================================
// 状態設定処理
//====================================================================
void CScrollDevice::SetState(STATE state)
{
	// 状態設定
	m_State = state;

	switch (m_State)
	{
	case CScrollDevice::STATE_NORMAL:

		if (m_pObjectCharacter != nullptr)
		{
			// モーション設定処理
			m_pObjectCharacter->GetMotion()->Set(0, 5);
		}

		break;
	case CScrollDevice::STATE_ROTATE:

		if (m_pObjectCharacter != nullptr)
		{
			// モーション設定処理
			m_pObjectCharacter->GetMotion()->Set(1, 5);
		}

		break;
	}


}

//====================================================================
// モデル関連の初期化処理
//====================================================================
HRESULT CScrollDevice::InitModel(const char* pModelNameScrollDevice, const char* pModelNameEnemy)
{
	CObjectCharacter::SetTxtCharacter(pModelNameScrollDevice);

	if (m_pObjectCharacter != nullptr)
	{
		// キャラクターテキスト読み込み処理（メダマン）
		m_pObjectCharacter->SetTxtCharacter(pModelNameEnemy);

		// メダマンの親を土台に変更
		m_pObjectCharacter->GetModel(0)->SetParent(GetModel(SETUP_TYPE_FOUNDATION));

		// メダマンの位置を取得
		D3DXVECTOR3 pos = m_pObjectCharacter->GetModel(2)->GetStartPos();
		D3DXVECTOR3 rot = m_pObjectCharacter->GetModel(2)->GetStartRot();
		D3DXVECTOR3 posAdd = D3DXVECTOR3(-50.0f, 375.0f, 500.0f);
		D3DXVECTOR3 rotAdd = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

		// メダマンを上に
		m_pObjectCharacter->GetModel(0)->SetStartPos(pos + posAdd);
		m_pObjectCharacter->GetModel(0)->SetStartRot(rot + rotAdd);
	}

	return S_OK;
}

//====================================================================
//状態管理
//====================================================================
void CScrollDevice::StateManager(D3DXVECTOR3& rotMy)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ROTATE:

		// ローラーと回し車回転処理
		Rotate(rotMy,SETUP_TYPE_ROLLRE, ROTATE_ADD);
		Rotate(rotMy,SETUP_TYPE_MAWASIGURMA, ROTATE_ADD);
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
// 回転処理
//====================================================================
void CScrollDevice::Rotate(D3DXVECTOR3& rotMy,int nNldxModel,D3DXVECTOR3 rotate)
{
	// モデルの取得
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// モデルの回転軸を取得
	D3DXVECTOR3 rot = pModel->GetStartRot();
	
	// 本体の向き判定
	if (rotMy.y == 0.0f)
	{
		// 回転量加算
		rot += rotate;
	}
	else if (rotMy.y == D3DX_PI)
	{
		// 回転量減算
		rot -= rotate;
	}

	// 角度の正規化
	useful::NormalizeAngle(&rot);

	// モデルの回転設定
	pModel->SetStartRot(rot);
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CScrollDevice>* CScrollDevice::GetList(void)
{
	return m_pList;
}
