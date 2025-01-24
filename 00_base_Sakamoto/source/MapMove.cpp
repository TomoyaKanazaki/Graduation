//============================================
//
//	マップの動き [MapMove.cpp]
//	Author:sakamoto kai
//
//============================================
#include "MapMove.h"
#include "object2D.h"
#include "Fade.h"
#include "sound.h"
#include "objmeshField.h"
#include "enemy.h"
#include "ScrollDevice.h"
#include "SlopeDevice.h"
#include "signal.h"
#include "effect.h"

#include "bowabowa.h"
#include "Cross.h"
#include "player.h"
#include "railblock.h"
#include "RollRock.h"
#include "tile.h"
#include "friedegg.h"
#include "fire.h"
#include "wall.h"
#include "camera.h"

//===========================================
// 定数定義
//===========================================
namespace
{
	const int SCROOL_TIME = 300;						// スクロール時間

	const int SCROOL_MOVEGRID_01 = 3;					// スクロールの移動マス幅
	const float SCROOL_SPEED_01 = (CMapSystem::GetGritSize() * SCROOL_MOVEGRID_01 / SCROOL_TIME);				// スクロールの移動速度

	const int SCROOL_COUNT_02 = 12;					// スクロールの移動回数
	const int SCROOL_MOVEGRID_02 = 3;					// スクロールの移動マス幅
	const float SCROOL_SPEED_02 = (CMapSystem::GetGritSize() * SCROOL_MOVEGRID_02) / SCROOL_COUNT_02;			// スクロールの移動速度

	const int SLOPE_TIME = 300;						// 傾き操作時間
	const int SLOPE_RAND = 100;						// 傾き発生確率
	float STAGE_ROT_LIMIT = D3DX_PI * 0.15f;		// 傾きの角度制限

	const float SLOPE_SPEED01 = 0.00075f;			// 傾きの移動速度

	const float SLOPE_SPEED02 = 0.0125f;			// 傾きの移動速度

	const int EFFECT_NUM = 3; // 一度に生成するエフェクトの数
	const float EFFECT_RANGE = 1.5f; // エフェクトの生成間隔

	const float SLOPEUP_MAG = 1.5f;		// 傾き時の移動速度の倍率
	const float SLOPEDOWN_MAG = 0.5f;	// 傾き時の移動速度の倍率
}

//====================================================================
//コンストラクタ
//====================================================================
CMapMove::CMapMove() : 
	m_fEffectTime(0.0f)
{
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_fActionCount = 0.0f;
	m_State = MOVE_WAIT;
	m_SlopeOld = MOVE_WAIT;
	m_nStateCount = 0;
	m_CollisionRot = 0.0f;
	m_DevilPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MapDifference = INITVECTOR3;
	m_DevilRot = INITVECTOR3;
	m_bSlope = false;
	m_MinGrid = CMapSystem::GRID(0, 0);
	m_MaxGrid = CMapSystem::GRID(NUM_WIGHT - 1, NUM_HEIGHT - 1);
	m_DevilArrow = 0;
	m_ScrollArrowOld = 0;
	m_SlopwArrowOld = 0;
	m_ScrollType = SCROLL_TYPE_NORMAL;
	m_nStateNum = 0;
	m_SlopeType = 0;
	m_bScrollOK = false;
	m_fScrollMove = 0.0f;
	m_fScrollEndLine = 0.0f;

	m_moveMode = MOVEMODE_SCROLL;
}

//====================================================================
//デストラクタ
//====================================================================
CMapMove::~CMapMove()
{

}

//====================================================================
//生成処理
//====================================================================
CMapMove* CMapMove::Create()
{
	CMapMove* pSystem = new CMapMove();

	//オブジェクトの初期化処理
	if (FAILED(pSystem->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pSystem;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CMapMove::Init(void)
{
	m_ScrollType = (SCROLL_TYPE)CManager::GetInstance()->GetScrollType();

	//状態関連の初期化
	m_State = MOVE_WAIT;
	m_SlopeOld = MOVE_WAIT;
	m_nStateCount = SCROOL_TIME * 0.5f;
	m_move = INITVECTOR3;
	m_MapDifference = INITVECTOR3;
	m_DevilRot = INITVECTOR3;
	m_bSlope = false;
	m_fScrollMove = 0.0f;
	m_fEffectTime = 0.0f;
	m_Objmove = INITVECTOR3;
	m_DevilPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MinGrid = CMapSystem::GRID(0, 0);
	m_MaxGrid = CMapSystem::GRID(NUM_WIGHT - 1, NUM_HEIGHT - 1);
	m_DevilArrow = 0;
	m_ScrollArrowOld = 0;
	m_SlopwArrowOld = 0;
	m_nStateNum = 0;
	m_SlopeType = 0;
	m_bScrollOK = false;
	m_fScrollEndLine = 0.0f;

	//カメラを振動させない
	CManager::GetInstance()->GetCamera(0)->SetBib(false);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CMapMove::Uninit(void)
{
	// 自身を削除する
	delete this;
}

//====================================================================
//更新処理
//====================================================================
void CMapMove::Update(void)
{
	// 移動量をリセット
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// マップの傾き
	m_DevilRot = CObjmeshField::GetListTop()->GetRot();

	//状態の管理
	StateManager();

	//デバッグキーの処理と設定
	DebugKey();

	//ステージ外にいるオブジェクトの処理
	CollisionOut();

	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	D3DXVECTOR3 InitPos = CMapSystem::GetInstance()->GetInitPos();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

	// エフェクトを生成
	switch (m_State)
	{
	case MOVE_WAIT:

		// 生成時間をリセット
		m_fEffectTime = EFFECT_RANGE;
		break;

	default:

		// 生成時間を更新
		m_fEffectTime -= DeltaTime::Get();

		// タイマー
		if (m_fEffectTime > 0.0f)
		{
			break;
		}

		// 生成時間をリセット
		m_fEffectTime = EFFECT_RANGE;

		// グリッド情報の取得
		CMapSystem* system = CMapSystem::GetInstance();
		CMapSystem::GRID maxGrid = system->GetMapGrid();
		D3DXMATRIX mat = *CObjmeshField::GetListTop()->GetMatrix();
		D3DXVECTOR3 rotEffect = useful::CalcMatrixToRot(mat);
		for (int i = 0; i < EFFECT_NUM; ++i)
		{
			// 生成情報を算出
			CMapSystem::GRID grid = { rand() % maxGrid.x, rand() % maxGrid.z };

			// 生成
			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_QUAKE, false, useful::CalcMatrix(grid.ToWorld(), INITVECTOR3, mat), rotEffect);
		}

		break;
	}
}

//====================================================================
//描画処理
//====================================================================
void CMapMove::Draw(void)
{

}

//====================================================================
//状態管理
//====================================================================
void CMapMove::StateManager(void)
{
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

	switch (m_State)
	{
	case MOVE_WAIT: // 待機状態 -> 次に移動が起こる

		// 状態終了時
		if (m_nStateCount <= 0)
		{
			// サウンドの停止
			StopSound();

			// 次の行動を抽選
			int nRand = rand() % 101;

			if (nRand <= SLOPE_RAND)
			{ // 傾きの指定％の時

				m_moveMode = MOVEMODE_SLOPE;	// 傾きモードにする
			}
			else
			{// 傾きの指定％じゃない時
				m_moveMode = MOVEMODE_SCROLL;	// スクロールモードにする
			}

			// 状態設定
			switch (m_moveMode)
			{
			case CMapMove::MOVEMODE_SCROLL:		// スクロール

				// スクロール時間設定
				m_nStateCount = SCROOL_TIME * 2;

				// 傾きの向き設定する
				SetSlopeRot();

				// スクロール状態(方向)設定
				switch (m_DevilArrow + MOVE_SCROLL_UP)
				{
				case MOVE_SCROLL_UP:		// 上
					m_State = MOVE_SCROLL_UP;

					// サウンド再生
					CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_UP);
					break;

				case MOVE_SCROLL_DOWN:		// 下
					m_State = MOVE_SCROLL_DOWN;

					// サウンド再生
					CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_DOWN);
					break;

				case MOVE_SCROLL_LEFT:		// 左
					m_State = MOVE_SCROLL_LEFT;

					// サウンド再生
					CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_LEFT);
					break;

				case MOVE_SCROLL_RIGHT:		// 右
					m_State = MOVE_SCROLL_RIGHT;

					// サウンド再生
					CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_RIGHT);
					break;
				}

				// 移動角度の状態設定
				SetRotState();

				m_bScrollOK = false;

				break;

			case CMapMove::MOVEMODE_SLOPE:		// 傾き

				// 傾いている状態かどうかを切り替える
				m_bSlope = !m_bSlope;

				if (m_bSlope)
				{// 傾き状態の時

					m_nStateCount = SLOPE_TIME;

					// 傾きの向き設定する
					SetSlopeRot();

					// 傾き状態(方向)設定
					switch (m_DevilArrow + MOVE_SLOPE_UP)
					{
					case MOVE_SLOPE_UP:		// 上
						m_State = MOVE_SLOPE_UP;

						// サウンド再生
						CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_UP);
						break;
					case MOVE_SLOPE_DOWN:	// 下
						m_State = MOVE_SLOPE_DOWN;

						// サウンド再生
						CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_DOWN);
						break;
					case MOVE_SLOPE_LEFT:	// 左
						m_State = MOVE_SLOPE_LEFT;

						// サウンド再生
						CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_LEFT);
						break;
					case MOVE_SLOPE_RIGHT:	// 右
						m_State = MOVE_SLOPE_RIGHT;

						// サウンド再生
						CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_RIGHT);
						break;
					}

					// 移動角度の状態設定
					SetRotState();
				}
				else
				{// 傾き戻し状態の時

					// サウンドの停止
					StopSound();

					// 傾きを戻す時だけ倍の時間を指定し、戻り切ったら傾き状態を終了とする
					m_nStateCount = SLOPE_TIME * 2;

					m_State = m_SlopeOld;

					//カメラを振動させる
					CManager::GetInstance()->GetCamera(0)->SetBib(true);

					// 傾き装置のリスト構造が無ければ抜ける
					if (CSlopeDevice::GetList() == nullptr) { return; }
					std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // リストを取得

					// 傾き装置のリストの中身を確認する
					for (CSlopeDevice* pSlopeDevice : list)
					{
						// 方向の傾き装置を上昇状態に変更
						pSlopeDevice->SetStateArrowBack((CScrollArrow::Arrow)m_SlopwArrowOld);
					}
				}


				break;
			
			default:
				assert(false);
				break;
			}
		}

		break;

	// スクロール中の状態 -> 待機状態になる
	case MOVE_SCROLL_UP:
	case MOVE_SCROLL_DOWN:
	case MOVE_SCROLL_LEFT:
	case MOVE_SCROLL_RIGHT:

		switch (m_ScrollType)
		{
		case CMapMove::SCROLL_TYPE_NORMAL:
			Move(m_DevilArrow);
			break;

		case CMapMove::SCROLL_TYPE_RETRO:

			if (m_nStateCount % (SCROOL_TIME / SCROOL_COUNT_02) == 0)
			{
				Move(m_DevilArrow);
			}

			break;

		default:

			break;
		}

		if (m_bScrollOK == true)
		{
			m_State = MOVE_WAIT;
			m_nStateCount = 120;

			// マップ移動装置のリスト構造が無ければ抜ける
			if (CScrollDevice::GetList() == nullptr) { return; }
			std::list<CScrollDevice*> list = CScrollDevice::GetList()->GetList();    // リストを取得

			// マップ移動装置のリストの中身を確認する
			for (CScrollDevice* pScrollDevice : list)
			{
				// 通常状態に変更
				pScrollDevice->SetState(CScrollDevice::STATE_NORMAL);
			}
		}

		break;

	// 傾き中の状態 -> 待機状態になる
	case MOVE_SLOPE_UP:
	case MOVE_SLOPE_DOWN:
	case MOVE_SLOPE_LEFT:
	case MOVE_SLOPE_RIGHT:
		switch (m_ScrollType)
		{
		case CMapMove::SCROLL_TYPE_NORMAL:

			if (m_bSlope)
			{
				Slope(m_DevilArrow);
			}
			else
			{
				BackSlope(); // 現在の傾き状態を基に戻す関数
			}
			break;

		case CMapMove::SCROLL_TYPE_RETRO:

			if (m_nStateCount % 25 == 0)
			{
				if (m_bSlope)
				{
					Slope(m_DevilArrow);
				}
				else
				{
					BackSlope();
				}

				// 傾き装置のリスト構造が無ければ抜ける
				if (CSlopeDevice::GetList() == nullptr) { return; }
				std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // リストを取得

				// 傾き移動装置のリストの中身を確認する
				for (CSlopeDevice* pSlopeDevice : list)
				{
					// レトロ用の傾き移動をオン
					pSlopeDevice->SetUseRetroMove(true);
				}
			}
			break;

		default:

			break;
		}

		if (m_nStateCount < 0)
		{
			m_State = MOVE_WAIT;
			m_nStateCount = 120;
		}

		break;
	}

	if (m_nStateCount >= 0)
	{
		m_nStateCount--;
	}

	DebugProc::Print(DebugProc::POINT_RIGHT, "スクロール状態 : ");

	auto str = magic_enum::enum_name(m_State);

	DebugProc::Print(DebugProc::POINT_RIGHT, str.data());
	DebugProc::Print(DebugProc::POINT_RIGHT, "\n");

}

//====================================================================
// 傾きの向きを設定
//====================================================================
void CMapMove::SetSlopeRot(void)
{
	//傾き方向指定処理
	m_DevilArrow = rand() % 2;

	if (m_SlopwArrowOld == ROTTYPE_UP || m_SlopwArrowOld == ROTTYPE_DOWN)
	{// 前回の傾き方向が上下だった場合

		// 今回の傾き方向は左右にする
		if (m_DevilArrow == ROTTYPE_UP)
		{ // 上の場合

			m_DevilArrow = ROTTYPE_LEFT;		// 左
		}
		else if (m_DevilArrow == ROTTYPE_DOWN)
		{ // 下の場合

			m_DevilArrow = ROTTYPE_RIGHT;		// 右
		}
	}

	// 今回の傾き方向を記録する
	m_SlopwArrowOld = m_DevilArrow;
	m_nStateNum = m_DevilArrow;
}

//====================================================================
// 移動角度の状態設定
//====================================================================
void CMapMove::SetRotState(void)
{
	// 移動状態設定
	switch (m_moveMode)
	{
	case CMapMove::MOVEMODE_SCROLL:		// スクロール
	{
		// マップ移動装置のリスト構造が無ければ抜ける
		if (CScrollDevice::GetList() == nullptr) { return; }
		std::list<CScrollDevice*> list = CScrollDevice::GetList()->GetList();    // リストを取得

		// マップ移動装置のリストの中身を確認する
		for (CScrollDevice* pScrollDevice : list)
		{
			// 方向指定状態設定（一致だと回転）
			pScrollDevice->SetStateArrow((CScrollArrow::Arrow)m_DevilArrow);
		}
	}
		break;
	case CMapMove::MOVEMODE_SLOPE:		// 傾き
	{
		//カメラを振動させる
		CManager::GetInstance()->GetCamera(0)->SetBib(true);

		// 傾き装置のリスト構造が無ければ抜ける
		if (CSlopeDevice::GetList() == nullptr) { return; }
		std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // リストを取得

		// 傾き装置のリストの中身を確認する
		for (CSlopeDevice* pSlopeDevice : list)
		{
			// 方向の傾き装置を上昇状態に変更
			pSlopeDevice->SetStateArrow((CScrollArrow::Arrow)m_DevilArrow);
		}
	}
		break;
	
	default:
		assert(false);
		break;
	}
	
}

//====================================================================
//移動処理
//====================================================================
void CMapMove::Move(int Arroow)
{
	if (m_fScrollEndLine > 0.0f || m_fScrollEndLine < 0.0f)
	{
		D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

		switch (m_ScrollType)
		{
		case CMapMove::SCROLL_TYPE_NORMAL:

			switch (Arroow)
			{
			case 0:
				m_move.z = SCROOL_SPEED_01;
				if (m_fScrollEndLine <= MapPos.z)
				{
					m_bScrollOK = true;
				}
				break;

			case 1:
				m_move.z = -SCROOL_SPEED_01;
				if (m_fScrollEndLine >= MapPos.z)
				{
					m_bScrollOK = true;
				}
				break;

			case 2:
				m_move.x = -SCROOL_SPEED_01;
				if (m_fScrollEndLine >= MapPos.x)
				{
					m_bScrollOK = true;
				}
				break;

			case 3:
				m_move.x = SCROOL_SPEED_01;
				if (m_fScrollEndLine <= MapPos.x)
				{
					m_bScrollOK = true;
				}
				break;
			}
			break;

		case CMapMove::SCROLL_TYPE_RETRO:

			switch (Arroow)
			{
			case 0:
				m_move.z = SCROOL_SPEED_02;
				if (m_fScrollEndLine <= MapPos.z)
				{
					m_bScrollOK = true;
				}
				break;

			case 1:
				m_move.z = -SCROOL_SPEED_02;
				if (m_fScrollEndLine >= MapPos.z)
				{
					m_bScrollOK = true;
				}
				break;

			case 2:
				m_move.x = -SCROOL_SPEED_02;
				if (m_fScrollEndLine >= MapPos.x)
				{
					m_bScrollOK = true;
				}
				break;

			case 3:
				m_move.x = SCROOL_SPEED_02;
				if (m_fScrollEndLine <= MapPos.x)
				{
					m_bScrollOK = true;
				}
				break;
			}
			break;

		default:
			break;
		}

		if (m_bScrollOK == true)
		{
			MapPos.x = useful::RoundUp2(MapPos.x);
			MapPos.z = useful::RoundUp2(MapPos.z);

			CMapSystem::GetInstance()->SetMapPos(MapPos);

			m_fScrollMove = 0.0f;
			m_fScrollEndLine = 0.0f;
			m_move = INITVECTOR3;
		}
	}
	else
	{
		switch (m_ScrollType)
		{
		case CMapMove::SCROLL_TYPE_NORMAL:
			switch (Arroow)
			{
			case 0:
				m_move.z = SCROOL_SPEED_01;
				break;
			case 1:
				m_move.z = -SCROOL_SPEED_01;
				break;
			case 2:
				m_move.x = -SCROOL_SPEED_01;
				break;
			case 3:
				m_move.x = SCROOL_SPEED_01;
				break;
			}

			m_fScrollMove += SCROOL_SPEED_01;
			if (m_fScrollMove >= (SCROOL_MOVEGRID_01 * 100))
			{
				D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

				MapPos.x = useful::RoundUp2(MapPos.x);
				MapPos.z = useful::RoundUp2(MapPos.z);

				CMapSystem::GetInstance()->SetMapPos(MapPos);

				m_bScrollOK = true;
				m_fScrollMove = 0.0f;
				m_move = INITVECTOR3;
			}

			break;

		case CMapMove::SCROLL_TYPE_RETRO:
			switch (Arroow)
			{
			case 0:
				m_move.z = SCROOL_SPEED_02;
				break;
			case 1:
				m_move.z = -SCROOL_SPEED_02;
				break;
			case 2:
				m_move.x = -SCROOL_SPEED_02;
				break;
			case 3:
				m_move.x = SCROOL_SPEED_02;
				break;
			}

			m_fScrollMove += SCROOL_SPEED_02;
			if (m_fScrollMove > (SCROOL_MOVEGRID_02 * 100))
			{
				D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

				MapPos.x = useful::RoundUp2(MapPos.x);
				MapPos.z = useful::RoundUp2(MapPos.z);

				CMapSystem::GetInstance()->SetMapPos(MapPos);

				m_bScrollOK = true;
				m_fScrollMove = 0.0f;
				m_move = INITVECTOR3;
			}

			break;

		default:
			break;
		}
	}

	// マップを動かす
	GritScroll();
}

//====================================================================
//移動方向処理(傾き戻す)
//====================================================================
void CMapMove::BackSlope(void)
{
	// ローカル変数宣言
	CObjmeshField* pMapField = CObjmeshField::GetListTop(); // マップの床
	D3DXVECTOR3 MapRot = pMapField->GetRot(); // マップの傾き
	bool bReset = false; // 傾きのリセットが完了したフラグ
	float fSlopeRate = 0.0f; // 傾きの速度

	// スクロールタイプによって使用する定数を設定
	switch (m_ScrollType)
	{
	case CMapMove::SCROLL_TYPE_NORMAL:
		fSlopeRate = SLOPE_SPEED01;
		break;

	case CMapMove::SCROLL_TYPE_RETRO:
		fSlopeRate = SLOPE_SPEED02;
		break;

	default: // スクロール状態が設定されていない場合
		assert(false);
		break;
	}

	// xが正に傾いている場合
	if (MapRot.x > 0.0f)
	{
		MapRot.x -= D3DX_PI * fSlopeRate;

		// 戻り過ぎの補正
		if (MapRot.x <= 0.0f)
		{
			bReset = true;
		}
	}

	// xが負に傾いている場合
	if (MapRot.x < 0.0f)
	{
		MapRot.x += D3DX_PI * fSlopeRate;
		
		// 戻り過ぎの補正
		if (MapRot.x >= 0.0f)
		{
			bReset = true;
		}
	}
	
	// zが正に傾いている場合
	if (MapRot.z > 0.0f)
	{
		MapRot.z -= D3DX_PI * fSlopeRate;

		// 戻り過ぎの補正
		if (MapRot.z <= 0.0f)
		{
			bReset = true;
		}
	}

	// zが負に傾いている場合
	if (MapRot.z < 0.0f)
	{
		MapRot.z += D3DX_PI * fSlopeRate;

		// 戻り過ぎの補正
		if (MapRot.z >= 0.0f)
		{
			bReset = true;
		}
	}

	// 傾きの動きが終了する場合
	if (bReset)
	{
		//カメラの振動をオフにする
		CManager::GetInstance()->GetCamera(0)->SetBib(false);

		// カウンターをリセットする TODO : マジックナンバー
		m_nStateCount = 60;

		// 傾きを0にする
		MapRot.x = MapRot.z = 0.0f;
	}

	// 傾きを設定
	pMapField->SetRot(MapRot);
}

//====================================================================
//移動方向処理(傾ける)
//====================================================================
void CMapMove::Slope(int Arroow)
{
	CObjmeshField* pMapField = CObjmeshField::GetListTop();
	if (pMapField == nullptr) { assert(false); }

	D3DXVECTOR3 MapRot = pMapField->GetRot();

	float fSlopeRate = 0.0f; // スクロールの速度

	// スクロールタイプによって使用する定数を設定
	switch (m_ScrollType)
	{
	case CMapMove::SCROLL_TYPE_NORMAL:
		fSlopeRate = SLOPE_SPEED01;
		break;

	case CMapMove::SCROLL_TYPE_RETRO:
		fSlopeRate = SLOPE_SPEED02;
		break;

	default: // スクロール状態が設定されていない場合
		assert(false);
		break;
	}

	switch (m_DevilArrow)
	{
	case 0:

		MapRot.x += D3DX_PI * fSlopeRate;

		if (MapRot.x > STAGE_ROT_LIMIT)
		{
			MapRot.x = STAGE_ROT_LIMIT;
			CManager::GetInstance()->GetCamera(0)->SetBib(false);	//カメラの振動をオフにする
		}
		// 右側
		m_SlopeType = 0;

		break;
	case 1:

		MapRot.x -= D3DX_PI * fSlopeRate;

		if (MapRot.x < -STAGE_ROT_LIMIT)
		{
			MapRot.x = -STAGE_ROT_LIMIT;
			CManager::GetInstance()->GetCamera(0)->SetBib(false);	//カメラの振動をオフにする
		}
		// 左側
		m_SlopeType = 1;

		break;
	case 2:

		MapRot.z += D3DX_PI * fSlopeRate;

		if (MapRot.z > STAGE_ROT_LIMIT)
		{
			MapRot.z = STAGE_ROT_LIMIT;
			CManager::GetInstance()->GetCamera(0)->SetBib(false);	//カメラの振動をオフにする
		}
		// 奥側
		m_SlopeType = 2;

		break;
	case 3:

		MapRot.z -= D3DX_PI * fSlopeRate;

		if (MapRot.z < -STAGE_ROT_LIMIT)
		{
			MapRot.z = -STAGE_ROT_LIMIT;
			CManager::GetInstance()->GetCamera(0)->SetBib(false);	//カメラの振動をオフにする
		}
		// 手前側
		m_SlopeType = 3;

		break;
	}

	m_SlopeOld = m_State;

	pMapField->SetRot(MapRot);

	DebugProc::Print(DebugProc::POINT_RIGHT, "マップの角度 : %f %f %f\n", MapRot.x, MapRot.y, MapRot.z);
}

//====================================================================
// ステージ外との当たり判定
//====================================================================
void CMapMove::CollisionOut()
{
	// キューブブロックのリスト構造が無ければ抜ける
	if (CEnemy::GetList() == nullptr) { return; }
	std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // リストを取得

	// キューブブロックリストの中身を確認する
	for (CEnemy* pEnemy : list)
	{
		CMapSystem::GRID EnemyGrid = pEnemy->GetGrid();
		D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
		D3DXVECTOR3 EnemyMove = pEnemy->GetMove();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float GritSize = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		m_MinGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x - MapSize.x - GritSize);	//左
		m_MinGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z + MapSize.z + GritSize);	//上
		m_MaxGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x + MapSize.x - GritSize);	//右
		m_MaxGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z - MapSize.z + GritSize);	//下

		if (EnemyGrid.x == -1)
		{
			if (EnemyPos.x > 0.0f)
			{
				m_MinGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x - MapSize.x - GritSize);	//左
				EnemyPos = CMapSystem::GRID(m_MinGrid.x, pEnemy->GetGrid().z).ToWorld();
			}
			else
			{
				m_MaxGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x + MapSize.x - GritSize);	//右
				EnemyPos = CMapSystem::GRID(m_MaxGrid.x, pEnemy->GetGrid().z).ToWorld();
			}
		}
		else if (EnemyGrid.z == -1)
		{
			if (EnemyPos.z < 0.0f)
			{
				m_MinGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z + MapSize.z + GritSize);	//上
				EnemyPos = CMapSystem::GRID(pEnemy->GetGrid().x, m_MinGrid.z).ToWorld();
			}
			else
			{
				m_MaxGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z - MapSize.z + GritSize);	//下
				EnemyPos = CMapSystem::GRID(pEnemy->GetGrid().x, m_MaxGrid.z).ToWorld();
			}
		}

		pEnemy->SetPos(EnemyPos);
	}
}

//==========================================
//  黙らせる処理
//==========================================
void CMapMove::StopSound()
{
	// サウンドの取得
	CSound* sound = CManager::GetInstance()->GetSound();

	// サウンドの停止
	sound->Stop(CSound::SOUND_LABEL_SE_SIGN_UP);
	sound->Stop(CSound::SOUND_LABEL_SE_SIGN_DOWN);
	sound->Stop(CSound::SOUND_LABEL_SE_SIGN_RIGHT);
	sound->Stop(CSound::SOUND_LABEL_SE_SIGN_LEFT);
}

//====================================================================
// デバッグボタン
//====================================================================
void CMapMove::DebugKey(void)
{
#ifdef _DEBUG

	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//キーボードの移動処理
	if (pInputKeyboard->GetPress(DIK_UP))
	{
		m_DevilArrow = 0;
		Move(0);

	}
	if (pInputKeyboard->GetPress(DIK_DOWN))
	{
		m_DevilArrow = 1;
		Move(1);
	}

	if (pInputKeyboard->GetPress(DIK_LEFT))
	{
		m_DevilArrow = 2;
		Move(2);
	}
	if (pInputKeyboard->GetPress(DIK_RIGHT))
	{
		m_DevilArrow = 3;
		Move(3);
	}

	if (pInputKeyboard->GetPress(DIK_5))
	{
		CObjmeshField* pMapField = CObjmeshField::GetListTop();
		D3DXVECTOR3 MapRot = pMapField->GetRot();
		MapRot = INITVECTOR3;
		pMapField->SetRot(MapRot);
	}

#endif // _DEBUG
}

//====================================================================
// スクロールのずれ修正
//====================================================================
void CMapMove::ScrollReset()
{

}

//====================================================================
// グリットのスクロール
//====================================================================
void CMapMove::GritScroll()
{
	D3DXVECTOR3 InitPos = CMapSystem::GetInstance()->GetInitPos();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();
	float MapGrit = CMapSystem::GetInstance()->GetGritSize();
	MapPos += m_move;
	m_MapDifference = -(InitPos - MapPos);
	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();

	if ((InitPos.x - MapPos.x) > 0.0f)
	{// 左範囲
		MapPos.x = InitPos.x + (MapSize.x * 2.0f) + MapGrit + m_move.x;
	}
	if ((InitPos.x - MapPos.x + MapGrit) < (-MapSize.x * 2.0f))
	{// 右範囲
		MapPos.x = InitPos.x + m_move.x;
	}

	if ((InitPos.z - MapPos.z) < 0.0f)
	{// 上範囲
		MapPos.z = InitPos.z + (-MapSize.z * 2.0f) - MapGrit + m_move.z;
	}
	if ((InitPos.z - MapPos.z - MapGrit) > (MapSize.z * 2.0f))
	{// 下範囲
		MapPos.z = InitPos.z + m_move.z;
	}

	CMapSystem::GetInstance()->SetMapPos(MapPos);
}

//====================================================================
//傾き中の移動量変動
//====================================================================
float CMapMove::MoveSlopeX(float Move, SPEED& Speed)
{
	float fSlopeMove = 1.0f;

	D3DXVECTOR3 DevilRot = m_DevilRot;

	if (Move > 0.0f)
	{// 右側に移動
		fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.z));

		if (DevilRot.z < 0.0f)
		{// 左に傾いた時の加速
			fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.z)) * SLOPEUP_MAG;
		}
		else if (DevilRot.z > 0.0f)
		{// 右に傾いた時の減速
			fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.z)) * SLOPEDOWN_MAG;
		}
	}
	else if (Move < 0.0f)
	{// 左側に移動
		fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.z));

		if (DevilRot.z > 0.0f)
		{// 右に傾いた時の加速
			fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.z)) * SLOPEUP_MAG;
		}
		else if (DevilRot.z < 0.0f)
		{// 左に傾いた時の減速
			fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.z)) * SLOPEDOWN_MAG;
		}
	}

	if (fSlopeMove > 1.0f)
	{
		Speed = SPEED_UP;
	}
	else if (fSlopeMove < 1.0f)
	{
		Speed = SPEED_DOWN;
	}
	else
	{
		Speed = SPEED_NONE;
	}

	return fSlopeMove;
}

//====================================================================
//傾き中の移動量変動
//====================================================================
float CMapMove::MoveSlopeZ(float Move, SPEED& Speed)
{
	float fSlopeMove = 1.0f;

	D3DXVECTOR3 DevilRot = m_DevilRot;

	if (Move > 0.0f)
	{// 奥側に移動
		fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.x));

		if (DevilRot.x < 0.0f)
		{// 手前に傾いた時の減速
			fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.x)) * SLOPEDOWN_MAG;
		}
		else if (DevilRot.x > 0.0f)
		{// 奥に傾いた時の加速
			fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.x)) * SLOPEUP_MAG;
		}
	}
	else if (Move < 0.0f)
	{// 手前側に移動
		fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.x));

		if (DevilRot.x > 0.0f)
		{// 奥に傾いた時の減速
			fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.x)) * SLOPEDOWN_MAG;
		}
		else if (DevilRot.x < 0.0f)
		{// 手前に傾いた時の加速
			fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.x)) * SLOPEUP_MAG;
		}
	}

	if (fSlopeMove > 1.0f)
	{
		Speed = SPEED_UP;
	}
	else if (fSlopeMove < 1.0f)
	{
		Speed = SPEED_DOWN;
	}
	else
	{
		Speed = SPEED_NONE;
	}

	return fSlopeMove;
}