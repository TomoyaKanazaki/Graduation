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

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(50.0f, 0.0f, 50.0f);		//当たり判定
	const int DIRECTION = 4;	// デビルホールの最大方向
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CSlopeDevice>* CSlopeDevice::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CSlopeDevice::CSlopeDevice(int nPriority) : CObjectX(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//マテリアルの数
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
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
CSlopeDevice* CSlopeDevice::Create(char* pModelName)
{
	// オブジェクトの生成処理
	CSlopeDevice* pInstance = new CSlopeDevice();

	// オブジェクトの初期化処理
	if (FAILED(pInstance->Init(pModelName)))
	{// 初期化処理が失敗した場合
		return nullptr;
	}

	return pInstance;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CSlopeDevice::Init(char* pModelName)
{
	SetType(CObject::TYPE_DEVILHOLE);

	CObjectX::Init(pModelName);

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

	CObjectX::Uninit();
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
	D3DXVECTOR3 pos = GetPos();

	//位置更新
	pos += m_move;

	SetPos(pos);

	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CSlopeDevice::GameUpdate(void)
{
	//更新前の位置を過去の位置とする
	m_posOld = m_pos;

	//位置更新
	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);

	//大きさの設定
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//状態管理
	StateManager();

	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//描画処理
//====================================================================
void CSlopeDevice::Draw(void)
{
	CObjectX::Draw();
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
	case STATE_ACTION:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CSlopeDevice>* CSlopeDevice::GetList(void)
{
	return m_pList;
}
