//============================================
//
//	チュートリアル画面 [tutorial.cpp]
//	Author:morikawa shunya
//
//============================================
#include "tutorial.h"
#include "fade.h"
#include "objmeshField.h"
#include "TutorialPlayer.h"
#include "player.h"
#include "camera.h"
#include "timer.h"
#include "Score.h"
#include "devil.h"
#include "DevilHole.h"
#include "renderer.h"
#include "slowManager.h"
#include "enemyYoungDevil.h"
#include "RailBlock.h"
#include "RollRock.h"
#include "bowabowa.h"
#include "ScrollDevice.h"
#include "SlopeDevice.h"
#include "mask.h"
#include "signal.h"
#include "pause.h"
#include "EventMovie.h"
#include "objmeshField.h"
#include "Cross.h"
#include "MapMove.h"
#include "pause.h"
#include "tutorialCheck.h"
#include "bible.h"
#include "tutorialUi.h"

#include "sound.h"
#include "shadow.h"

#include "BgObjManager.h"

//====================================================================
// 定数定義
//====================================================================
namespace
{
	const D3DXVECTOR3 CHECK_POS[]
	{// それぞれの位置
		{ 170.0f, 60.0f, 0.0f },  // 移動の座標
		{ 170.0f, 115.0f, 0.0f }, // 十字架座標
		{ 170.0f, 170.0f, 0.0f }, // ボワボワの座標
		{ 170.0f, 225.0f, 0.0f }, // 攻撃の座標
		{ 170.0f, 280.0f, 0.0f }, // 聖書の座標
		{ 170.0f, 334.0f, 0.0f }, // デビルホールの座標
	};

	const std::string TUTORIAL_TEX[]
	{// チュートリアルテキストのテクスチャパス
		{ "data\\TEXTURE\\UI\\tutorial_text_00.png" },	// 移動のテキスト
		{ "data\\TEXTURE\\UI\\tutorial_text_01.png" },	// 十字架座標
		{ "data\\TEXTURE\\UI\\tutorial_text_03.png" },	// ボワボワの座標
		{ "data\\TEXTURE\\UI\\tutorial_text_02.png" },	// 攻撃の座標
		{ "data\\TEXTURE\\UI\\tutorial_text_004.png" },	// 聖書の座標
		{ "data\\TEXTURE\\UI\\tutorial_text_05.png" },	// デビルホールの座標
	};

	const D3DXVECTOR3 TEXT_POS[]
	{// テキストの位置
		{ D3DXVECTOR3(5.0f, 0.0f, 0.0f) },	// 移動のテキスト
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f) },		// 十字架座標
		{ D3DXVECTOR3(115.0f, 0.0f, 0.0f) },	// ボワボワの座標
		{ D3DXVECTOR3(107.5f, 0.0f, 0.0f) },	// 攻撃の座標
		{ D3DXVECTOR3(10.0f, 5.0f, 0.0f) },		// 聖書の座標
		{ D3DXVECTOR3(115.0f, 0.0f, 0.0f) },	// デビルホールの座標
	};

	const D3DXVECTOR3 TEXT_SIZE[]
	{// それぞれのテキストのサイズ
		{ D3DXVECTOR3(250.0f, 100.0f, 0.0f) },	// 移動のテキスト
		{ D3DXVECTOR3(250.0f, 40.0f, 0.0f) },	// 十字架座標
		{ D3DXVECTOR3(500.0f, 50.0f, 0.0f) },	// ボワボワの座標
		{ D3DXVECTOR3(500.0f, 50.0f, 0.0f) },	// 攻撃の座標
		{ D3DXVECTOR3(275.0f, 80.0f, 0.0f) },	// 聖書の座標
		{ D3DXVECTOR3(500.0f, 50.0f, 0.0f) },	// デビルホールの座標
	};

	const int BIBLE_OUTGRIT = 2;			// 聖書がマップの外側から何マス内側にいるか
	const int STENCIL_REF_PLAYER = 2;		// プレイヤーのステンシルの参照値
	const int STENCIL_REF_ITEM = 4;			// アイテムのステンシルの参照値
	const int STENCIL_REF_MEDAMAN = 102;	// メダマンのステンシルの参照値
	const int WAVE_MIDDLE = 4;				// チュートリアル4段階目
	const int WAVE_MAX = 6;					// チュートリアル最大段階
	const int PLAYER_REF = 2;		// プレイヤーのステンシル参照値
	const int ITEM_REF = 4;			// アイテムのステンシル参照値
	const int MEDAMAN_REF = 102;	// メダマンのステンシル参照値

	const std::string BOTTOM_FIELD_TEX = "data\\TEXTURE\\Field\\outside.jpg";		// 下床のテクスチャ
	const std::string SCROLL_DEVICE_MODEL = "data\\TXT\\MOTION\\02_staging\\00_ScrollDevice\\motion_scrolldevice.txt";
	const std::string SCROLL_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";
	const std::string CHECK_MARKER_TEX = "data\\TEXTURE\\UI\\tutorial_check.png";	// チェックマーカーテクスチャ
	const std::string BUTTON_TEX = "data\\TEXTURE\\UI\\return_title.png";			// 遷移ボタンテクスチャ

	const std::string CHECKBOX_TEX = "data\\TEXTURE\\UI\\tutorial_check_box.png";		// チェックボックスのテクスチャ
	const std::string TUTORIAL_FRAME_TEX = "data\\TEXTURE\\UI\\tutorial_frame.png";		// チュートリアルガイドの外枠のテクスチャ
	const std::string ACHIEVEMENT_TEX = "data\\TEXTURE\\UI\\tutorial_achievement.png";	// 項目達成のテクスチャ

	const float CHECK_POS_Y = 200.0f;		// チェックマーカーのy座標

	const CMapSystem::GRID FIELD_GRID = { 64, 64 }; // 下の床のサイズ
	const CMapSystem::GRID BIBLE_POS = { 11, 10 };	// 聖書の位置

	const D3DXVECTOR3 BOTTOM_FIELD_POS = D3DXVECTOR3(0.0f, -1000.0f, 0.0f);	// 下床の位置
	const D3DXVECTOR3 GUIDE_SIZE = D3DXVECTOR3(420.0f, 360.0f, 0.0f);	// チュートリアルガイドのサイズ
	const D3DXVECTOR3 BOX_SIZE = D3DXVECTOR3(50.0f, 50.0f, 0.0f);		// チェックボックスのサイズ
	const D3DXVECTOR3 MARKER_SIZE = D3DXVECTOR3(50.0f, 50.0f, 0.0f);	// マーカーサイズ
	const D3DXVECTOR3 BUTTON_POS = D3DXVECTOR3(1100.0f, 650.0f, 0.0f);	// 遷移ボタンの位置
	const D3DXVECTOR3 BUTTON_SIZE = D3DXVECTOR3(300.0f, 250.0f, 0.0f);	// 遷移ボタンのサイズ
	const D3DXVECTOR3 TEXTURE_CENTER_POS = D3DXVECTOR3(225.0f, 200.0f, 0.0f);	// テクスチャの中心座標
	const D3DXVECTOR3 ACHIEVEMENT_SIZE = D3DXVECTOR3(300.0f, 100.0f, 0.0f);	// テクスチャの中心座標

	const D3DXCOLOR MASK_DEFAULT_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 通常のステンシルカラー(白)
	const D3DXCOLOR MASK_PLAYER_COLOR = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);			// タマゴンのステンシルカラー(緑)
	const D3DXCOLOR MASK_MULTI_PLAYER_COLOR = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	// 2Pタマゴンのステンシルカラー(水色)
	const D3DXCOLOR MASK_MEDAMAN_COLOR = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);			// メダマンのステンシルカラー(ピンク)
	const D3DXCOLOR MASK_BONBON_COLOR = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);			// ボンンボンのステンシルカラー(オレンジ)
	const D3DXCOLOR MASK_YUNGDEVIL_COLOR = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);		// 子デビルのステンシルカラー(青)
	const D3DXCOLOR MASK_ITEM_COLOR = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);			// アイテムのステンシルカラー(黄)
}

//==========================================
//  静的警告処理
//==========================================
static_assert(NUM_ARRAY(CHECK_POS) == CTutorial::TYPE_MAX, "ERROR : Type Count Missmatch");
static_assert(NUM_ARRAY(TUTORIAL_TEX) == CTutorial::TYPE_MAX, "ERROR : Type Count Missmatch");
static_assert(NUM_ARRAY(TEXT_POS) == CTutorial::TYPE_MAX, "ERROR : Type Count Missmatch");
static_assert(NUM_ARRAY(TEXT_SIZE) == CTutorial::TYPE_MAX, "ERROR : Type Count Missmatch");

//====================================================================
// 静的メンバ変数宣言
//====================================================================
CTutorial* CTutorial::m_pTutorial = nullptr;

//====================================================================
// コンストラクタ
//====================================================================
CTutorial::CTutorial():
m_bTutorialEnd(false),		// ゲーム終了のフラグ
m_bDevilHoleFinish(false),	// デビルホールが4方向埋まったかどうか
m_BGColorA(1.0f),			// イベント背景の透明度
m_nTutorialWave(0),			// チュートリアルの段階
m_nNumBowabowa(0),			// ボワボワの総数
m_nNumCross(0),				// 十字架の総数
m_nNumEnemy(0),				// 敵の総数
m_pPause(nullptr),			// ポーズのポインタ
m_pDevil(nullptr),			// デビルのポインタ
m_bTutorialClear(false),	// ゲームクリアのフラグ
m_Wireframe(false),			// ワイヤーフレーム切り替え
m_Slow(false),				// スロー演出フラグ
InitPlayerPos(D3DXVECTOR3()),	// プレイヤーの初期位置
m_nNumBible(0)				// 聖書の総数
{
	for (int nCnt = 0; nCnt < NUM_CAMERA; ++nCnt)
	{// カメラ分回す
		CManager::GetInstance()->GetCamera(nCnt)->SetBib(false);
		CManager::GetInstance()->GetCamera(nCnt)->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);
	}

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_bSound[nCnt] = false;
	}

	for (int i = 0; i < TYPE_MAX; ++i)
	{// マーカー表示
		m_bCheck[i] = false;
	}
}

//====================================================================
// デストラクタ
//====================================================================
CTutorial::~CTutorial()
{
}

//====================================================================
// インスタンス取得
//====================================================================
CTutorial* CTutorial::GetInstance(void)
{
	if (m_pTutorial == nullptr)
	{// インスタンス生成
		m_pTutorial = new CTutorial;
	}

	return m_pTutorial;
}

//====================================================================
// 初期化処理
//====================================================================
HRESULT CTutorial::Init(void)
{
	//BGMの再生
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_STAGE1);

	if (m_pPause == nullptr)
	{// ポーズの生成
		// サウンドの停止
		CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_SE_SIGN_UP);
		CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_SE_SIGN_DOWN);
		CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_SE_SIGN_RIGHT);
		CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_SE_SIGN_LEFT);

		m_pPause = CPause::Create();
	}

	// プレイヤー・アイテム・メダマンのステンシルカラーの設定
	CMask::Create(PLAYER_REF, MASK_PLAYER_COLOR);
	CMask::Create(ITEM_REF, MASK_ITEM_COLOR);
	CMask::Create(MEDAMAN_REF, MASK_MEDAMAN_COLOR);

	//クリアフラグのデフォルトをオンにしておく
	m_bTutorialClear = true;

	//タイムの起動
	CTutorial::GetTime()->SetStopTime(false);

	// マップの生成
	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	pMapSystem->Init();
	pMapSystem->CreateMap(pMapSystem->GetSelectMap());

	//デビルの生成
	m_pDevil = CDevil::Create();

	// 十字架の総数保存
	if (CCross::GetList() == nullptr) { assert(false); }
	m_nNumCross = CCross::GetList()->GetList().size();

	// ボワボワの総数保存
	if (CBowabowa::GetList() == nullptr) { assert(false); }
	m_nNumBowabowa = CBowabowa::GetList()->GetList().size();

	// 敵の総数保存
	m_nNumEnemy = CEnemy::GetList()->GetList().size();

	// チュートリアルガイドの外枠
	CTutorialUi::Create(TEXTURE_CENTER_POS, GUIDE_SIZE, TUTORIAL_FRAME_TEX, 1.0f);
	
	for (int i = 0; i < TYPE_MAX; ++i)
	{// 列挙分チェックボックスの生成
		CTutorialUi::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[i].x,
							TEXTURE_CENTER_POS.y + CHECK_POS[i].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z),
							BOX_SIZE, CHECKBOX_TEX, 1.0f);

		//CTutorialUi::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x + CHECK_POS[i].x,
		//					TEXTURE_CENTER_POS.y + CHECK_POS[i].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z),
		//					BOX_SIZE, CHECKBOX_TEX, 1.0f);
	}

	for (int i = 0; i < TYPE_MAX; ++i)
	{// テキストの生成
		m_pText.push_back(CTutorialUi::Create(
			D3DXVECTOR3(TEXTURE_CENTER_POS.x + TEXT_POS[i].x, CHECK_POS[i].y - TEXT_POS[i].y, CHECK_POS[i].z),
						TEXT_SIZE[i], TUTORIAL_TEX[i], 1.0f));
	}

	// 遷移ボタンの生成
	CTutorialUi::Create(BUTTON_POS, BUTTON_SIZE, BUTTON_TEX, 1.0f);

	for (int i = 0; i < TYPE_MAX; ++i)
	{// チェックマーカー非表示
		m_bCheck[i] = false;
	}
	
	// 下床の生成
	auto grid = FIELD_GRID;
	CObjmeshField* pBottonField = CObjmeshField::Create(grid);
	pBottonField->SetTexture(BOTTOM_FIELD_TEX);
	pBottonField->SetPos(BOTTOM_FIELD_POS);

	// 背景モデル設定処理
	SetBgObjTest();

	for (int i = 0; i < CManager::GetInstance()->GetGameMode(); ++i)
	{// プレイヤーを生成する
		m_pPlayer.push_back(CTutorialPlayer::Create(i));
		m_gridPlayer.push_back(m_pPlayer.at(i)->GetGrid());
	}

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CTutorial::Uninit(void)
{
	if (m_pPause != nullptr)
	{// ポーズの終了
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}

	// プレイヤーの解放
	while (1)
	{
		if (m_pPlayer.size() <= 0) { m_pPlayer.clear(); break; }
		m_pPlayer.back()->SetDeathFlag(true);
		m_pPlayer.pop_back();
	}

	// サウンド停止
	CManager::GetInstance()->GetSound()->Stop();

	// スロー情報の全削除
	CSlowManager::ReleaseAll();

	// マップシステムの終了
	//CMapSystem::GetInstance()->Uninit();

	//全てのオブジェクトの破棄
	CObject::ReleaseAll();

	// シーンの終了
	CScene::Uninit();

	if (m_pTutorial != nullptr)
	{
		delete m_pTutorial;
		m_pTutorial = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CTutorial::Update(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	DebugProc::Print(DebugProc::POINT_LEFT, "ゲームスピード : %f\n", CManager::GetInstance()->GetGameSpeed());
	DebugProc::Print(DebugProc::POINT_CENTER, "チュートリアル段階 : %d\n", m_nTutorialWave);
	DebugProc::Print(DebugProc::POINT_CENTER, "3で聖書生成段階に設定\n");

	// マップシステムの更新
	CMapSystem::GetInstance()->Update();
	
	// プレイヤーリストを取得
	if (CPlayer::GetList() == nullptr) { assert(false); }
	std::list<CPlayer*> list = CPlayer::GetList()->GetList();

	// デビルホールリストを取得
	if (CDevilHole::GetList() == nullptr) { assert(false); }
	std::list<CDevilHole*> DevilHolelist = CDevilHole::GetList()->GetList();

	// エネミーリストを取得
	if (CEnemy::GetList() == nullptr) { assert(false); }
	std::list<CEnemy*> Enemylist = CEnemy::GetList()->GetList();

	for (CPlayer* player : list)
	{
		if (player->GetState() == CPlayer::STATE_WALK
			&& m_bCheck[TYPE_MOVE] == false)
		{// 座標が一致しなかったら
			CTutorialCheck::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[TYPE_MOVE].x,
				TEXTURE_CENTER_POS.y + CHECK_POS[TYPE_MOVE].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z));

			// マーカー表示
			m_bCheck[TYPE_MOVE] = true;

			if (m_bCheck[TYPE_MOVE] == true && m_bSound[TYPE_MOVE] == false)
			{
				// サウンド再生
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHECK);

				m_bSound[TYPE_MOVE] = true;
			}

			if (m_bCheck[TYPE_MOVE] == true)
			{// テキストの不透明度を下げる
				SetUIAlpha(TYPE_MOVE, 0.5f);
			}

			// チュートリアル段階を進める
			m_nTutorialWave += 1;
		}

		if (CCross::GetList() != nullptr
			&& player->GetItemType() == CPlayer::TYPE_CROSS
			&& m_bCheck[TYPE_CROSS] == false)
		{// 十字架取得
			CTutorialCheck::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[TYPE_CROSS].x,
									TEXTURE_CENTER_POS.y + CHECK_POS[TYPE_CROSS].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z));

			// マーカー表示
			m_bCheck[TYPE_CROSS] = true;

			if (m_bCheck[TYPE_CROSS] == true && m_bSound[TYPE_CROSS] == false)
			{
				// サウンド再生
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHECK);

				m_bSound[TYPE_CROSS] = true;
			}

			if (m_bCheck[TYPE_CROSS] == true)
			{// テキストの不透明度を下げる
				SetUIAlpha(TYPE_CROSS, 0.5f);
			}

			// チュートリアル段階を進める
			m_nTutorialWave += 1;
		}

		if (player->GetItemType() == CPlayer::TYPE_BIBLE
			&& m_bCheck[TYPE_BIBLE] == false)
		{// 聖書取得
			CTutorialCheck::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[TYPE_BIBLE].x,
									TEXTURE_CENTER_POS.y + CHECK_POS[TYPE_BIBLE].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z));

			// マーカー表示
			m_bCheck[TYPE_BIBLE] = true;

			if (m_bCheck[TYPE_BIBLE] == true && m_bSound[TYPE_BIBLE] == false)
			{
				// サウンド再生
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHECK);

				m_bSound[TYPE_BIBLE] = true;
			}

			if (m_bCheck[TYPE_BIBLE] == true)
			{// テキストの不透明度を下げる
				SetUIAlpha(TYPE_BIBLE, 0.5f);
			}

			// チュートリアル段階を進める
			m_nTutorialWave += 1;
		}
	}

	if (m_nNumEnemy != CEnemy::GetList()->GetList().size()
		&& m_bCheck[TYPE_ATTACK] == false)
	{// 敵の総数減少
		CTutorialCheck::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[TYPE_ATTACK].x,
								TEXTURE_CENTER_POS.y + CHECK_POS[TYPE_ATTACK].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z));

		// マーカー表示
		m_bCheck[TYPE_ATTACK] = true;

		if (m_bCheck[TYPE_ATTACK] == true && m_bSound[TYPE_ATTACK] == false)
		{
			// サウンド再生
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHECK);

			m_bSound[TYPE_ATTACK] = true;
		}

		if (m_bCheck[TYPE_ATTACK] == true)
		{// テキストの不透明度を下げる
			SetUIAlpha(TYPE_ATTACK, 0.5f);
		}

		// チュートリアル段階を進める
		m_nTutorialWave += 1;
	}

	if (CBowabowa::GetList() != nullptr 
		&& m_nNumBowabowa != CBowabowa::GetList()->GetList().size()
		&& m_bCheck[TYPE_BOWABOWA] == false)
	{// ボワボワの総数減少
		CTutorialCheck::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[TYPE_BOWABOWA].x,
								TEXTURE_CENTER_POS.y + CHECK_POS[TYPE_BOWABOWA].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z));

		// マーカー表示
		m_bCheck[TYPE_BOWABOWA] = true;

		if (m_bCheck[TYPE_BOWABOWA] == true && m_bSound[TYPE_BOWABOWA] == false)
		{
			// サウンド再生
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHECK);

			m_bSound[TYPE_BOWABOWA] = true;
		}

		if (m_bCheck[TYPE_BOWABOWA] == true)
		{// テキストの不透明度を下げる
			SetUIAlpha(TYPE_BOWABOWA, 0.5f);
		}

		// チュートリアル段階を進める
		m_nTutorialWave += 1;
	}

	if (m_nTutorialWave == WAVE_MIDDLE)
	{// チュートリアルが4段階目まで進んだら
		if (CBible::GetList() == nullptr)
		{// 1つだけ聖書の生成
			CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(BIBLE_POS));
		}
	}

	for (CDevilHole* pDevilHole : DevilHolelist)
	{// デビルホールの中身を確認
		if (pDevilHole->IsSet())
		{// デビルホールの4箇所の内1箇所埋まった
			CTutorialCheck::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[TYPE_DEVILHOLE].x,
									TEXTURE_CENTER_POS.y + CHECK_POS[TYPE_DEVILHOLE].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z));

			// マーカー表示
			m_bCheck[TYPE_DEVILHOLE] = true;

			if (m_bCheck[TYPE_DEVILHOLE] == true && m_bSound[TYPE_DEVILHOLE] == false)
			{
				// サウンド再生
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHECK);

				m_bSound[TYPE_DEVILHOLE] = true;
			}

			if (m_bCheck[TYPE_DEVILHOLE] == true)
			{// テキストの不透明度を下げる
				SetUIAlpha(TYPE_DEVILHOLE, 0.5f);
			}

			// チュートリアル段階を進める
			m_nTutorialWave += 1;
		}
	}

	if (m_nTutorialWave >= WAVE_MAX)
	{
		// 値がWAVE_MAX超えないように
		m_nTutorialWave = WAVE_MAX;

		// 項目達成テクスチャ表示
		//CTutorialUi::Create(TEXTURE_CENTER_POS, ACHIEVEMENT_SIZE, ACHIEVEMENT_TEX, 1.0f);
	}

#if _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_3) == true)
	{// チュートリアル段階を4にする
		m_nTutorialWave = WAVE_MIDDLE;
	}

	if (pInputKeyboard->GetTrigger(DIK_0) == true)
	{
		m_Wireframe = (m_Wireframe == true) ? false : true;

		if (m_Wireframe == true)
		{
			//レンダーステートの設定
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else
		{
			//レンダーステートの設定
			m_pDevice->SetRenderState(D3DRS_FILLMODE, 0);
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		float Speed = CManager::GetInstance()->GetGameSpeed();

		Speed -= 0.90f;

		CManager::GetInstance()->SetGameSpeed(Speed);
	}

	if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{
		float Speed = CManager::GetInstance()->GetGameSpeed();

		Speed += 0.90f;

		CManager::GetInstance()->SetGameSpeed(Speed);
	}

	if (CManager::GetInstance()->GetGameSpeed() <= 1.0f)
	{
		m_Slow = true;
	}
	else
	{
		m_Slow = false;
	}

#endif

	if (CManager::GetInstance()->GetFade()->GetFade() == CFade::FADE_NONE)
	{
		//ステージクリア条件の設定
		switch (CManager::GetInstance()->GetStage())
		{
		case 0:
			if (m_nTutorialWave == WAVE_MAX)
			{// 6段階目のとき
				m_bTutorialEnd = true;
			}
			break;
		}

		//ステージクリア時の処理
		if (m_bTutorialEnd == true)
		{
			if (pInputKeyboard->GetTrigger(DIK_RETURN))
			{// 好きなタイミングで遷移
				CFade::SetFade(CScene::MODE_TITLE);
			}
		}

		//ポーズの更新処理
		if (m_pPause != nullptr)
		{
			m_pPause->Update();
		}
	}
}

//====================================================================
//描画処理
//====================================================================
void CTutorial::Draw(void)
{
	if (m_pPause != nullptr)
	{
		m_pPause->Draw();
	}
}

//====================================================================
//ステージ進行処理
//====================================================================
void CTutorial::NextStage(void)
{
	// マップの生成
	CMapSystem::GetInstance()->GetMove()->Init();
	CObjmeshField::GetListTop()->SetRot(INITVECTOR3);

	//十字架の削除
	DeleteCross();

	// ソフトクリームの生成
	CItem::Create(CItem::TYPE_SOFTCREAM, CMapSystem::GetInstance()->GetCenter());

	//聖書の生成
	CreateBible();

	//ステージ情報を進める
	CManager::GetInstance()->SetStage(1);

	m_bTutorialEnd = false;
}

//====================================================================
//十字架の削除
//====================================================================
void CTutorial::DeleteCross(void)
{
	// 十字架のリスト構造が無ければ抜ける
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // リストを取得

	// 十字架リストの中身を確認する
	for (CCross* pCross : list)
	{
		pCross->Uninit();
	}
}

//====================================================================
//聖書の生成
//====================================================================
void CTutorial::CreateBible(void)
{
	//グリッド最大・最小位置取得
	CMapSystem::GRID GMax = CMapSystem::GetInstance()->GetMove()->GetMaxGrid();
	CMapSystem::GRID GMin = CMapSystem::GetInstance()->GetMove()->GetMinGrid();

	// 聖書生成
	CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GMin.x + BIBLE_OUTGRIT, GMin.z + BIBLE_OUTGRIT));
	CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GMax.x - BIBLE_OUTGRIT, GMin.z + BIBLE_OUTGRIT));
	CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GMin.x + BIBLE_OUTGRIT, GMax.z - BIBLE_OUTGRIT));
	CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GMax.x - BIBLE_OUTGRIT, GMax.z - BIBLE_OUTGRIT));
}

//====================================================================
//マップオブジェクトの削除
//====================================================================
void CTutorial::DeleteMap(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::OBJECT_TYPE type = pObj->GetType();			//種類を取得

			if (type == CObject::TYPE_CUBEBLOCK ||
				type == CObject::TYPE_ENEMY3D ||
				type == CObject::TYPE_MAPMODEL)
			{//種類がマップ関連の時
				pObj->Uninit();
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// レールブロックの読み込み配置
//====================================================================
void CTutorial::LoadStageRailBlock(const std::string pFilename)
{
	//ファイルを開く
	FILE* pFile = fopen(pFilename.c_str(), "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		char aString[128] = {};			//ゴミ箱
		char aStartMessage[32] = {};	//スタートメッセージ
		char aSetMessage[32] = {};		//セットメッセージ
		char aEndMessage[32] = {};		//終了メッセージ

		fscanf(pFile, "%s", &aStartMessage[0]);
		if (strcmp(&aStartMessage[0], "STARTSETSTAGE") == 0)
		{
			CMapSystem* pMapSystem = CMapSystem::GetInstance();
			D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

			while (1)
			{
				fscanf(pFile, "%s", &aSetMessage[0]);
				if (strcmp(&aSetMessage[0], "STARTSETRAILBLOCK") == 0)
				{
					int WightNumber, HeightNumber, nMax, RailMove[64];

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &WightNumber);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &HeightNumber);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &nMax);

					fscanf(pFile, "%s", &aString[0]);

					for (int nCnt = 0; nCnt < nMax; nCnt++)
					{
						fscanf(pFile, "%d", &RailMove[nCnt]);
					}

					CMapSystem::GetInstance()->SetGritBool(WightNumber, HeightNumber, true);
					/*CRailBlock* pBlock = CRailBlock::Create(WightNumber, HeightNumber, false, nMax, &RailMove[0]);
					pBlock->SetPos(D3DXVECTOR3(pBlock->GetPos().x, 50.0f, pBlock->GetPos().z));
					pBlock->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));*/

					fscanf(pFile, "%s", &aEndMessage[0]);
				}
				else if (strcmp(&aSetMessage[0], "ENDSETSTAGE") == 0)
				{
					break;
				}
			}
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}

//====================================================================
// テスト用背景オブジェクト設定処理
//====================================================================
void CTutorial::SetBgObjTest(void)
{

	// マップ移動装置
	{
		//CScrollDevice* pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL, SCROLL_DEVICE_ENEMY_MODEL);
		//pScrollDevice->SetPos(D3DXVECTOR3(1300.0f, 0.0f, 0.0f));

		//pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL, SCROLL_DEVICE_ENEMY_MODEL);
		//pScrollDevice->SetPos(D3DXVECTOR3(-1300.0f, 0.0f, 0.0f));
	}
}

//==========================================
//  UIの透明度を変更
//==========================================
void CTutorial::SetUIAlpha(const CHECKTYPE type, const float fAlpha)
{
	m_pText.at(type)->SetAlpha(fAlpha);
}
