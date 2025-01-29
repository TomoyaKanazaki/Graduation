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
	const D3DXVECTOR3 ROTATE_ADD_TOP = D3DXVECTOR3(0.0f, 0.0f, -0.05f);		// 上
	const D3DXVECTOR3 ROTATE_ADD_BOTTOM = D3DXVECTOR3(0.0f, 0.0f, 0.05f);	// 下
	const D3DXVECTOR3 ROTATE_ADD_LEFT = D3DXVECTOR3(0.0f, 0.0f, 0.05f);		// 左
	const D3DXVECTOR3 ROTATE_ADD_RIGHT = D3DXVECTOR3(0.0f, 0.0f, -0.05f);	// 右

	const D3DXVECTOR3 MEDAMAN_ROT_TOP_NOR = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	const D3DXVECTOR3 MEDAMAN_ROT_BOTTOM_NOR = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	const D3DXVECTOR3 MEDAMAN_ROT_LEFT_NOR = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	const D3DXVECTOR3 MEDAMAN_ROT_RIGHT_NOR = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);

	const D3DXVECTOR3 MEDAMAN_ROT_TOP = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	const D3DXVECTOR3 MEDAMAN_ROT_BOTTOM = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	const D3DXVECTOR3 MEDAMAN_ROT_LEFT = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	const D3DXVECTOR3 MEDAMAN_ROT_RIGHT = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
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

	m_LocateWorldType = LOCATE_WORLD_TYPE(0);

	m_rotMove = D3DXVECTOR3(0.0f,0.0f,0.0f);

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
CScrollDevice* CScrollDevice::Create(const std::string pModelNameSlopeDevice, const std::string pModelNameEnemy)
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
	if (m_pObjectCharacter != nullptr)
	{
		m_pObjectCharacter->Uninit();
		m_pObjectCharacter = nullptr;
	}

	if (m_pList != nullptr)
	{
		// リストから自身のオブジェクトを削除
		m_pList->DelList(m_iterator);

		if (m_pList->GetNumAll() == 0)
		{ // オブジェクトが一つもない場合

			// リストマネージャーの破棄
			m_pList->Release(m_pList);
		}
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
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// キャラクタークラスの描画（継承）
	CObjectCharacter::Draw();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 103);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 => (参照値 => ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// キャラクタークラスの描画（継承）
	CObjectCharacter::Draw();

	//ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
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

		// メダマンの向き設定
		SetRotMedaman();

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
// 特定条件（傾き）の状態設定処理
//====================================================================
void CScrollDevice::SetStateArrow(CScrollArrow::Arrow stateArrow)
{
	m_rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	switch (stateArrow)
	{
	case CScrollArrow::STATE_UP:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM)
		{
			// 上移動代入
			m_rotMove = ROTATE_ADD_TOP;

			// 方向
			SetState(STATE_ROTATE);

			// メダマンの向き設定
			SetRotMedaman(stateArrow);
		}

		break;
	case CScrollArrow::STATE_DOWN:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM)
		{
			// 下移動代入
			m_rotMove = ROTATE_ADD_BOTTOM;

			// 方向
			SetState(STATE_ROTATE);

			// メダマンの向き設定
			SetRotMedaman(stateArrow);
		}

		break;

	case CScrollArrow::STATE_LEFT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_RIGHT)
		{
			// 左移動量代入
			m_rotMove = ROTATE_ADD_LEFT;

			// 方向
			SetState(STATE_ROTATE);

			// メダマンの向き設定
			SetRotMedaman(stateArrow);
		}

		break;
	case CScrollArrow::STATE_RIGHT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_RIGHT)
		{
			// 右移動量代入
			m_rotMove = ROTATE_ADD_RIGHT;

			// 方向
			SetState(STATE_ROTATE);

			// メダマンの向き設定
			SetRotMedaman(stateArrow);
		}

		break;
	}
}

//====================================================================
// 全表示設定処理
//====================================================================
void CScrollDevice::SetAppearAll(bool bAppear)
{
	// 表示設定
	SetAppear(bAppear);

	// キャラクターの表示設定
	if (m_pObjectCharacter != nullptr)
	{
		m_pObjectCharacter->SetAppear(bAppear);
	}
}

//====================================================================
// モデル関連の初期化処理
//====================================================================
HRESULT CScrollDevice::InitModel(const std::string pModelNameScrollDevice, const std::string pModelNameEnemy)
{
	CObjectCharacter::SetTxtCharacter(pModelNameScrollDevice);

	if (m_pObjectCharacter != nullptr)
	{
		// キャラクターテキスト読み込み処理（メダマン）
		m_pObjectCharacter->SetTxtCharacter(pModelNameEnemy);

		// メダマンの親を土台に変更
		m_pObjectCharacter->GetModel(0)->SetParent(GetModel(SETUP_TYPE_FOUNDATION));

		// メダマンの位置を取得
		D3DXVECTOR3 pos = m_pObjectCharacter->GetModel(0)->GetStartPos();
		D3DXVECTOR3 rot = m_pObjectCharacter->GetModel(0)->GetStartRot();
		D3DXVECTOR3 posAdd = D3DXVECTOR3(0.0f, 75.0f, 0.0f);

		// メダマンを上に
		m_pObjectCharacter->GetModel(0)->SetStartPos(pos + posAdd);
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
		Rotate(rotMy,SETUP_TYPE_ROLLRE);
		Rotate(rotMy,SETUP_TYPE_MAWASIGURMA);
		break;
	}
}

//====================================================================
// 回転処理
//====================================================================
void CScrollDevice::Rotate(D3DXVECTOR3& rotMy,int nNldxModel)
{
	// モデルの取得
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// モデルの回転軸を取得
	D3DXVECTOR3 rot = pModel->GetStartRot();
	
	// 回転量加算
	rot += m_rotMove;

	// 角度の正規化
	useful::NormalizeAngle(&rot);

	// モデルの回転設定
	pModel->SetStartRot(rot);
}

//====================================================================
// 状態設定処理
//====================================================================
void CScrollDevice::SetRotMedaman(CScrollArrow::Arrow stateArrow)
{
	if (m_pObjectCharacter == nullptr)
	{
		return;
	}

	switch (m_State)
	{
	case CScrollDevice::STATE_NORMAL:

		switch (m_LocateWorldType)
		{
		case CScrollDevice::LOCATE_WORLD_TYPE_TOP:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_TOP_NOR);

			break;
		case CScrollDevice::LOCATE_WORLD_TYPE_BOTTOM:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_BOTTOM_NOR);

			break;
		case CScrollDevice::LOCATE_WORLD_TYPE_LEFT:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_LEFT_NOR);

			break;
		case CScrollDevice::LOCATE_WORLD_TYPE_RIGHT:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_RIGHT_NOR);

			break;
		}

		break;

	case CScrollDevice::STATE_ROTATE:

		switch (stateArrow)
		{
		case CScrollArrow::STATE_UP:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_TOP);

			break;
		case CScrollArrow::STATE_DOWN:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_BOTTOM);

			break;
		case CScrollArrow::STATE_LEFT:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_LEFT);

			break;
		case CScrollArrow::STATE_RIGHT:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_RIGHT);

			break;
		}

		break;
	}
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CScrollDevice>* CScrollDevice::GetList(void)
{
	return m_pList;
}
