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
#include "effect.h"
#include "game.h"

#include "character.h"
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
CListManager<CSlopeDevice>* CSlopeDevice::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CSlopeDevice::CSlopeDevice(int nPriority) : CCharacter(nPriority)
{
	m_pos = INITVECTOR3;
	m_posOld = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = INITVECTOR3;

	m_State = STATE(0);
	m_nStateCount = 0;

	m_bMultiMatrix = false;
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
	CCharacter::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CSlopeDevice::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//タイトルでの更新処理
//====================================================================
void CSlopeDevice::TitleUpdate(void)
{
	// キャラクタークラスの更新（継承）
	CCharacter::Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CSlopeDevice::GameUpdate(void)
{
	//状態管理
	StateManager();

	// キャラクタークラスの更新（継承）
	CCharacter::Update();
}

//====================================================================
//描画処理
//====================================================================
void CSlopeDevice::Draw(void)
{
	// 無理やり一時的位置情報交換（pos・rotの置き換え完了次第削除）
	CCharacter::SetPos(GetPos());
	CCharacter::SetRot(GetRot());

	// キャラクタークラスの描画（継承）
	CCharacter::Draw();
}

//====================================================================
// モデル関連の初期化処理
//====================================================================
HRESULT CSlopeDevice::InitModel(const char* pModelNameSlopeDevice, const char* pModelNameEnemy)
{
	CCharacter::SetTxtCharacter(pModelNameSlopeDevice);

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
	case STATE_ROTATE:

		// ローラーと回し車回転処理
		Rotate(SETUP_TYPE_ROLLRE, ROTATE_ADD);
		Rotate(SETUP_TYPE_MAWASIGURMA, ROTATE_ADD);
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
void CSlopeDevice::Rotate(int nNldxModel,D3DXVECTOR3 rotate)
{
	// モデルの取得
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// モデルの回転軸を取得
	D3DXVECTOR3 rot = pModel->GetStartRot();
	
	if (m_rot.y == 0.0f)
	{
		// 回転量減算
		rot += rotate;
	}
	else if (m_rot.y == D3DX_PI)
	{
		// 回転量加算
		rot -= rotate;
	}

	// モデルの回転設定
	pModel->SetStartRot(rot);
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CSlopeDevice>* CSlopeDevice::GetList(void)
{
	return m_pList;
}
