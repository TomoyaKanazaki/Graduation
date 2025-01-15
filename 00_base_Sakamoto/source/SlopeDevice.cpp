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

#include "MapMove.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 ASCENT_ADD = D3DXVECTOR3(0.0f, 30.0f, 0.0f);		// 上昇量
	const D3DXVECTOR3 DESCENT_DEST = D3DXVECTOR3(0.0f, 30.0f, 0.0f);	// 下降量

	const D3DXVECTOR3 MAX_POS_HEIGHT = D3DXVECTOR3(0.0f, 380.0f, 0.0f);		// 縦傾きの最大上昇値
	const D3DXVECTOR3 MAX_POS_WIDTH = D3DXVECTOR3(0.0f,550.0f,0.0f);		// 横傾きの最大上小値

	const D3DXVECTOR3 MIN_POS_HEIGHT = D3DXVECTOR3(0.0f, -250.0f, 0.0f);	// 縦傾きの最小上昇値
	const D3DXVECTOR3 MIN_POS_WIDTH = D3DXVECTOR3(0.0f, -300.0f, 0.0f);		// 横傾きの最小上小値

	const D3DXVECTOR3 MOVE_SPEED_HEIGHT = D3DXVECTOR3(0.0f, 1.6f, 0.0f);	// 縦傾きの昇降移動量
	const D3DXVECTOR3 MOVE_SPEED_WIDTH = D3DXVECTOR3(0.0f, 2.25f, 0.0f);	// 横傾きの昇降移動量

	const D3DXVECTOR3 MOVE_SPEED_HEIGHT_RETRO = D3DXVECTOR3(0.0f, 28.0f, 0.0f);	// 縦傾きの昇降移動量（レトロ状態）
	const D3DXVECTOR3 MOVE_SPEED_WIDTH_RETRO = D3DXVECTOR3(0.0f, 34.5f, 0.0f);	// 横傾きの昇降移動量（レトロ状態）

	const D3DXVECTOR3 LIVER_ROT_MOVE = D3DXVECTOR3(0.01f, 0.0f, 0.0f);		// レバーの移動量
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
	m_posTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posTargetDef = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_rotleverDef = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bleverMove = false;

	m_State = STATE(0);

	m_LocateWorldType = LOCATE_WORLD_TYPE(0);

	m_bUseRetroMove = false;
		
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
CSlopeDevice* CSlopeDevice::Create(const std::string pModelNameSlopeDevice, const std::string pModelNameEnemy)
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
void CSlopeDevice::SetStateArrow(CScrollArrow::Arrow stateArrow)
{
	switch (stateArrow)
	{
	case CScrollArrow::STATE_UP:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_RIGHT)
		{
			SetState(STATE_ASCENT);
		}
		else
		{
			SetState(STATE_DESCENT);
		}

		break;
	case CScrollArrow::STATE_DOWN:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT)
		{
			SetState(STATE_ASCENT);
		}
		else
		{
			SetState(STATE_DESCENT);
		}

		break;

	case CScrollArrow::STATE_LEFT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_LEFT)
		{
			SetState(STATE_ASCENT);
		}
		else
		{
			SetState(STATE_DESCENT);
		}

		break;
	case CScrollArrow::STATE_RIGHT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_RIGHT)
		{
			SetState(STATE_ASCENT);
		}
		else
		{
			SetState(STATE_DESCENT);
		}

		break;
	}

	// マップ移動取得処理
	CMapMove* pMapMove = CMapSystem::GetInstance()->GetMove();

	// 状態取得用変数
	CMapMove::SCROLL_TYPE MoveType = CMapMove::SCROLL_TYPE_NORMAL;

	if (pMapMove != nullptr)
	{
		// マップ移動モード取得処理
		MoveType = pMapMove->GetScrollType();
	}

	if (stateArrow == CScrollArrow::STATE_UP ||
		stateArrow == CScrollArrow::STATE_DOWN)
	{
		if (MoveType == CMapMove::SCROLL_TYPE_NORMAL)
		{
			// 昇降移動量を設定
			m_move = MOVE_SPEED_HEIGHT;
		}
		else if (MoveType == CMapMove::SCROLL_TYPE_RETRO)
		{
			// 昇降移動量を設定（レトロ）
			m_move = MOVE_SPEED_HEIGHT_RETRO;
		}

		if (m_State == STATE_ASCENT)
		{
			// 最大上昇値を代入
			m_posTarget = MAX_POS_HEIGHT;
		}
		else if (m_State == STATE_DESCENT)
		{
			// 最小下降値を代入
			m_posTarget = MIN_POS_HEIGHT;
		}
	}
	else if (stateArrow == CScrollArrow::STATE_LEFT ||
			 stateArrow == CScrollArrow::STATE_RIGHT)
	{
		if (MoveType == CMapMove::SCROLL_TYPE_NORMAL)
		{
			// 昇降移動量を設定
			m_move = MOVE_SPEED_WIDTH;
		}
		else if (MoveType == CMapMove::SCROLL_TYPE_RETRO)
		{
			// 昇降移動量を設定（レトロ）
			m_move = MOVE_SPEED_WIDTH_RETRO;
		}

		if (m_State == STATE_ASCENT)
		{
			// 最大上昇値を代入
			m_posTarget = MAX_POS_WIDTH;
		}
		else if (m_State == STATE_DESCENT)
		{
			// 最小下降値を代入
			m_posTarget = MIN_POS_WIDTH;
		}
	}
}

//====================================================================
// 特定条件（傾き戻り）の状態設定処理
//====================================================================
void CSlopeDevice::SetStateArrowBack(CScrollArrow::Arrow stateArrow)
{
	switch (stateArrow)
	{
	case CScrollArrow::STATE_UP:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_RIGHT)
		{
			SetState(STATE_DESCENT);
		}
		else
		{
			SetState(STATE_ASCENT);
		}

		break;
	case CScrollArrow::STATE_DOWN:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT)
		{
			SetState(STATE_DESCENT);
		}
		else
		{
			SetState(STATE_ASCENT);
		}

		break;
	case CScrollArrow::STATE_LEFT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_LEFT)
		{
			SetState(STATE_DESCENT);
		}
		else
		{
			SetState(STATE_ASCENT);
		}

		break;
	case CScrollArrow::STATE_RIGHT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_RIGHT)
		{
			SetState(STATE_DESCENT);
		}
		else
		{
			SetState(STATE_ASCENT);
		}

		break;
	}

	if (stateArrow == CScrollArrow::STATE_UP ||
		stateArrow == CScrollArrow::STATE_DOWN)
	{
		// 目的位置を通常位置に変更
		m_posTarget = m_posTargetDef;
	}
	else if (stateArrow == CScrollArrow::STATE_LEFT ||
			 stateArrow == CScrollArrow::STATE_RIGHT)
	{
		// 目的位置を通常位置に変更
		m_posTarget = m_posTargetDef;
	}
}

//====================================================================
// 全値リセット処理
//====================================================================
void CSlopeDevice::ReSet(void)
{
	// 昇降部位のモデルの取得
	CModel* pModelElevatingParts = GetModel(SETUP_TYPE_ELEVATING_PART);
	CModel* pModelLever = GetModel(SETUP_TYPE_LIVER);

	if (pModelElevatingParts == nullptr && pModelLever == nullptr)
	{
		return;
	}

	// モデルの位置を取得
	D3DXVECTOR3 pos = pModelElevatingParts->GetStartPos();
	D3DXVECTOR3 rot = pModelLever->GetStartRot();

	// 下降量を加算
	pos = m_posTargetDef;
	rot = m_rotleverDef;

	// モデルの位置更新
	pModelElevatingParts->SetStartPos(pos);
	pModelLever->SetStartRot(rot);

	// 通常状態に変更
	SetState(STATE_NORMAL);
}

//====================================================================
// 全表示設定処理
//====================================================================
void CSlopeDevice::SetAppearAll(bool bAppear)
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
HRESULT CSlopeDevice::InitModel(const std::string pModelNameSlopeDevice, const std::string pModelNameEnemy)
{
	CObjectCharacter::SetTxtCharacter(pModelNameSlopeDevice, 0);

	// レバーの初期向きを取得
	m_rotleverDef = GetModel(SETUP_TYPE_LIVER)->GetStartRot();

	// デフォルト目的位置を取得・設定
	m_posTargetDef = GetModel(SETUP_TYPE_ELEVATING_PART)->GetStartPos();
	m_posTarget = m_posTargetDef;

	if (m_pObjectCharacter != nullptr)
	{
		// キャラクターテキスト読み込み処理（メダマン）
		m_pObjectCharacter->SetTxtCharacter(pModelNameEnemy, 0);

		// メダマンの親を土台に変更
		m_pObjectCharacter->GetModel(0)->SetParent(GetModel(SETUP_TYPE_LIVER));
		D3DXVECTOR3 rotlever = GetModel(SETUP_TYPE_LIVER)->GetStartRot();

		// メダマンの位置を取得
		D3DXVECTOR3 pos = m_pObjectCharacter->GetModel(0)->GetStartPos();
		D3DXVECTOR3 rot = m_pObjectCharacter->GetModel(0)->GetStartRot();
		D3DXVECTOR3 posAdd = D3DXVECTOR3(0.0f, 10.0f, 150.0f);
		D3DXVECTOR3 rotAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f) - rotlever;

		// メダマンを上に
		m_pObjectCharacter->GetModel(0)->SetStartPos(pos + posAdd);
		m_pObjectCharacter->GetModel(0)->SetStartRot(rot + rotAdd);
	}

	return S_OK;
}

//====================================================================
// 通常状態の状態管理
//====================================================================
void CSlopeDevice::StateManager(void)
{
	// マップ移動取得処理
	CMapMove* pMapMove = CMapSystem::GetInstance()->GetMove();

	// 状態取得用変数
	CMapMove::SCROLL_TYPE MoveType = CMapMove::SCROLL_TYPE_NORMAL;
	CMapMove::MOVE MoveState = CMapMove::MOVE_WAIT;	

	if (pMapMove != nullptr)
	{
		// マップ移動モード取得処理
		MoveType = pMapMove->GetScrollType();

		// マップ状態取得
		MoveState = pMapMove->GetState();
	}

	if (MoveType == CMapMove::SCROLL_TYPE_RETRO &&
		m_bUseRetroMove == false)
	{
		return;
	}
	
	// 状態判定
	switch (m_State)
	{
	case STATE_NORMAL:
		break;

		// 上昇状態
	case STATE_ASCENT:

		// 移動状態判定（上 or 下）
		if (MoveState == CMapMove::MOVE_SLOPE_UP ||
			MoveState == CMapMove::MOVE_SLOPE_DOWN)
		{
			// 上昇処理
			Ascent(SETUP_TYPE_ELEVATING_PART);
			
			// レバー動作処理
			ActiveLever(SETUP_TYPE_LIVER);
		}
		// 移動状態判定（右 or 左）
		else if (MoveState == CMapMove::MOVE_SLOPE_RIGHT ||
				 MoveState == CMapMove::MOVE_SLOPE_LEFT)
		{
			// 上昇処理
			Ascent(SETUP_TYPE_ELEVATING_PART);

			// レバー動作処理
			ActiveLever(SETUP_TYPE_LIVER);
		}

		break;
	case STATE_DESCENT:

		// 移動状態判定（上 or 下）
		if (MoveState == CMapMove::MOVE_SLOPE_UP ||
			MoveState == CMapMove::MOVE_SLOPE_DOWN)
		{
			// 下降処理
			Descent(SETUP_TYPE_ELEVATING_PART);

			// レバー動作処理
			ActiveLever(SETUP_TYPE_LIVER);
		}
		// 移動状態判定（右 or 左）
		else if (MoveState == CMapMove::MOVE_SLOPE_RIGHT ||
				 MoveState == CMapMove::MOVE_SLOPE_LEFT)
		{
			// 下降処理
			Descent(SETUP_TYPE_ELEVATING_PART);

			// レバー動作処理
			ActiveLever(SETUP_TYPE_LIVER);
		}

		break;
	}

	if (m_bUseRetroMove == true)
	{
		m_bUseRetroMove = false;
	}
}

//====================================================================
// 上昇処理
//====================================================================
void CSlopeDevice::Ascent(int nNldxModel)
{
	// モデルの取得
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// モデルの位置を取得
	D3DXVECTOR3 pos = pModel->GetStartPos();

	// 上昇最大値判定
	if (pos.y < m_posTarget.y)
	{
		// 上昇量を加算
		pos += m_move;
	}
	else
	{
		// 通常状態に変更
		SetState(STATE_NORMAL);
	}

	// モデルの位置更新
	pModel->SetStartPos(pos);
}

//====================================================================
// 下降処理
//====================================================================
void CSlopeDevice::Descent(int nNldxModel)
{
	// モデルの取得
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// モデルの位置を取得
	D3DXVECTOR3 pos = pModel->GetStartPos();

	// 下降最小値判定
	if (pos.y > m_posTarget.y)
	{
		// 下降量を加算
		pos -= m_move;
	}
	else
	{
		// 通常状態に変更
		SetState(STATE_NORMAL);
	}

	// モデルの位置更新
	pModel->SetStartPos(pos);
}

//====================================================================
// レバー動作状態処理
//====================================================================
void CSlopeDevice::ActiveLever(int nNldxModel)
{
	// モデルの取得
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// モデルの位置を取得
	D3DXVECTOR3 rot = pModel->GetStartRot();

	if (m_bleverMove)
	{
		// 下降量を加算
		rot -= LIVER_ROT_MOVE;

		// 下降最小値判定
		if (rot.x < m_rotleverDef.x)
		{
			m_bleverMove = false;
		}
	}
	else
	{
		// 上昇量を加算
		rot += LIVER_ROT_MOVE;

		// 下降最小値判定
		if (rot.x > 0.0f)
		{
			m_bleverMove = true;
		}
	}


	// モデルの位置更新
	pModel->SetStartRot(rot);
}

//====================================================================
// 状態設定処理
//====================================================================
void CSlopeDevice::SetState(STATE state)
{
	m_State = state;

	switch (m_State)
	{
	case STATE_NORMAL:

		if (m_pObjectCharacter != nullptr)
		{
			// モーション設定処理
			m_pObjectCharacter->GetMotion()->Set(0, 5);
		}

		break;
	case STATE_ASCENT:

		if (m_pObjectCharacter != nullptr)
		{
			// モーション設定処理
			m_pObjectCharacter->GetMotion()->Set(1, 5);
		}

		break;

	case STATE_DESCENT:

		if (m_pObjectCharacter != nullptr)
		{
			// モーション設定処理
			m_pObjectCharacter->GetMotion()->Set(1, 5);
		}

		break;
	}
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CSlopeDevice>* CSlopeDevice::GetList(void)
{
	return m_pList;
}
