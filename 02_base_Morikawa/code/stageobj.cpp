//=============================================================================
// 
//  ステージの配置物処理 [transferBeacon.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "stageobj.h"
#include "manager.h"
#include "calculation.h"
#include "model.h"
#include "input.h"

// 派生クラス
#include "obstacle.h"
#include "reverse.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* MODEL[] = 
	{
		"data\\MODEL\\obstacle\\wood00.x",
		"data\\MODEL\\obstacle\\wood01.x",
	};
	const float ENABLERANGE = 2000.0f;	// 有効範囲
}

namespace StateTime
{
	const float APPEARANCE = 0.3f;	// 登場
	const float LEAVE = 0.7f;		// 退場
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CStageObj::SAMPLE_FUNC CStageObj::m_SampleFuncList[] =
{
	&CStageObj::StateNone,			// なし
	&CStageObj::StateAppearance,	// 登場
	&CStageObj::StateLeave,			// 退場
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CStageObj> CStageObj::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CStageObj::CStageObj(int nPriority) : CObject(nPriority),
m_fStateTime(0.0f),		// 状態タイマー
m_state(State::STATE_NONE),	// 状態
m_pModel(nullptr),		// モデル
m_bWorking(false)		// 稼働判定
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CStageObj::~CStageObj()
{
	
}

//==========================================================================
// 背景の生成処理
//==========================================================================
CStageObj* CStageObj::CreateBg(const MyLib::Vector3& pos)
{
	// メモリの確保
	CStageObj* pObj = new CStageObj;

	if (pObj != nullptr)
	{
		// 引数設定
		pObj->m_type = TYPE_BG;
		pObj->SetPos(pos);
		pObj->SetOriginPosition(pos);

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 障害物の生成処理
//==========================================================================
CStageObj* CStageObj::CreateObstacle(const MyLib::Vector3& pos, const int type)
{
	// メモリの確保
	CStageObj* pObj = CObstacle::Create((CObstacle::EType)(type), pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (pObj != nullptr)
	{
		// 引数設定
		pObj->m_type = TYPE_OBSTACLE;
		pObj->SetPos(pos);
		pObj->SetOriginPosition(pos);

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 反転ポイントの生成処理
//==========================================================================
CStageObj* CStageObj::CreateReverse(const MyLib::Vector3& pos)
{
	// メモリの確保
	CStageObj* pObj = new CReverse;

	if (pObj != nullptr)
	{
		// 引数設定
		pObj->m_type = TYPE_REVERSE;
		pObj->SetPos(pos);
		pObj->SetOriginPosition(pos);

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CStageObj::Init()
{
	// リストに追加
	m_List.Regist(this);

	// モデル生成
	if (m_pModel == nullptr &&
		m_type != Type::TYPE_REVERSE)
	{
		int randNum = (rand() % 2);
		m_pModel = CModel::Create(MODEL[randNum]);
	}

	if (m_pModel != nullptr)
	{
		m_pModel->SetType(CModel::TYPE_NOT_HIERARCHY);
		m_pModel->SetPosition(GetPos());
		m_pModel->SetScale(0.0f);
	}

	// 登場
	SetState(State::STATE_NONE);
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CStageObj::Uninit()
{
	if (m_pModel != nullptr)
	{// モデルの削除
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// リストから削除
	m_List.Delete(this);

	// 終了
	Release();
}

//==========================================================================
// 位置
//==========================================================================
void CStageObj::SetPos(D3DXVECTOR3 pos)
{
	CObject::SetPos(pos);

	if (m_pModel != nullptr)
	{
		m_pModel->SetPosition(pos);
	}
}

//==========================================================================
// 削除
//==========================================================================
void CStageObj::Kill()
{
	if (m_pModel != nullptr)
	{// モデルの削除
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// リストから削除
	m_List.Delete(this);

	// 終了
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CStageObj::Update()
{
	// 状態更新
	UpdateState();

	if (m_pModel != nullptr)
	{// モデルの更新
		m_pModel->SetPosition(GetPos());
		m_pModel->Update();
	}

	// CInputKeyboard型のポインタ
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

#ifdef _DEBUG
	
	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		SetState(State::STATE_LEAVE);
	}
#endif
}

//==========================================================================
// 範囲判定
//==========================================================================
void CStageObj::CollisionRange(const MyLib::Vector3& rPos)
{
	MyLib::Vector3 pos = GetPos();

	if (m_bWorking &&
		m_state != State::STATE_LEAVE &&
		rPos.x >= pos.x + ENABLERANGE * 0.5f)
	{// 退場範囲
		SetState(State::STATE_LEAVE);
		m_bWorking = false;
		return;
	}

	if (!m_bWorking &&
		m_state == State::STATE_NONE &&
		rPos.x >= pos.x - ENABLERANGE)
	{// 登場範囲
		SetState(State::STATE_APPEARANCE);
		m_bWorking = true;
	}
	
}

//==========================================================================
// プレイヤーとの当たり判定
//==========================================================================
bool CStageObj::Collision(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rSize, const CPlayer::PLAYERSTATE state)
{
	return false;
}

//==========================================================================
// 状態更新
//==========================================================================
void CStageObj::UpdateState()
{
	// 状態カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 状態別処理
	(this->*(m_SampleFuncList[m_state]))();
}

//==========================================================================
// なし
//==========================================================================
void CStageObj::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// 登場
//==========================================================================
void CStageObj::StateAppearance()
{
	MyLib::Vector3 posOrigin = GetOriginPosition();	// 初期位置
	MyLib::Vector3 posDest = GetOriginPosition();	// 目標位置
	posDest.y = 0.0f;

	// 線形補間で移動
	MyLib::Vector3 pos = posOrigin;
	pos.y = UtilFunc::Correction::EaseOutBack(posOrigin.y, posDest.y, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetPos(pos);

	// 線形補間で拡縮
	float scale = UtilFunc::Correction::EaseOutBack(0.0f, 1.0f, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	
	if (m_pModel != nullptr)
	{
		m_pModel->SetScale(scale);
	}
}

//==========================================================================
// 退場
//==========================================================================
void CStageObj::StateLeave()
{
	MyLib::Vector3 posOrigin = GetOriginPosition();	// 初期位置
	MyLib::Vector3 posDest = GetOriginPosition();	// 目標位置
	posDest.y = 0.0f;

	// 線形補間で移動
	MyLib::Vector3 pos = posOrigin;
	pos.y = UtilFunc::Correction::EaseInBack(0.0f, posOrigin.y, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetPos(pos);

	// 線形補間で拡縮
	float scale = UtilFunc::Correction::EaseInBack(1.0f, 0.0f, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	
	if (m_pModel != nullptr)
	{
		m_pModel->SetScale(scale);
	}

	if (m_fStateTime >= StateTime::APPEARANCE)
	{// 時間経過
		Kill();
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CStageObj::SetState(const State& state)
{
	m_fStateTime = 0.0f;	// 状態カウンター
	m_state = state;		// 状態
}

//==========================================================================
// 描画処理
//==========================================================================
void CStageObj::Draw()
{
	if (m_pModel != nullptr)
	{// モデルの描画
		m_pModel->Draw();
	}
}

