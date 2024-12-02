//============================================
//
//	マップ移動装置の処理 [ScrollDevice.cpp]
//	Author:sakai minato
//
//============================================
#include "ScrollDevice.h"
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

	//モードごとに初期値を設定出来る
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		break;

	case CScene::MODE_RESULT:
		break;
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
// モデル関連の初期化処理
//====================================================================
HRESULT CScrollDevice::InitModel(const char* pModelNameScrollDevice, const char* pModelNameEnemy)
{
	CObjectCharacter::SetTxtCharacter(pModelNameScrollDevice);

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
