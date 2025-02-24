
//============================================
//
//	選択画面 [select.cpp]
//	Author:sakamoto kai
//
//============================================
#include "select.h"
#include "fade.h"
#include "object2D.h"
#include "texture.h"
#include "objmeshDome.h"
#include "objmeshField.h"
#include "light.h"
#include "Devil.h"
#include "MapMove.h"
#include "sound.h"
#include "number.h"
#include "MapSystem.h"

// 定数定義
namespace
{
	const char* MAP_BLOCK_PASS("data\\TXT\\STAGE\\Block_Tutorial_000.txt");
	const char* MAP_XMODEL_PASS("data\\TXT\\STAGE\\XModel_Tutorial_000.txt");
	const char* MAP_GIMMICK_PASS("data\\TXT\\STAGE\\Gimmick_Tutorial_000.txt");

	const D3DXVECTOR3 STAGE_POS = D3DXVECTOR3(490.0f, 80.0f, 0.0f);		// 選択項目の位置
	const D3DXVECTOR3 MAP_POS = D3DXVECTOR3(640.0f, 250.0f, 0.0f);			// サムネイルの位置
	const D3DXVECTOR2 STAGE_SIZE = D3DXVECTOR2(130.0f, 160.0f);				// 選択項目の大きさ
	const D3DXVECTOR2 MAP_SIZE = D3DXVECTOR2(400.0f, 300.0f);				// サムネイルの大きさ
	const D3DXVECTOR2 NUMBER_SIZE = D3DXVECTOR2(35.0f, 35.0f);				// サムネイルの大きさ
	const D3DXVECTOR2 STAGE_DISTANCE = D3DXVECTOR2(300.0f + (STAGE_SIZE.x * 1.0f), 0.0f + (STAGE_SIZE.y * 0.0f));	// 選択項目の幅
	const float NUMBER_DISTANCE = 80.0f;	// ステージと数字の幅

	const D3DXVECTOR3 SCROOL_POS = D3DXVECTOR3(440.0f, 500.0f, 0.0f);		// 選択項目の位置
	const D3DXVECTOR2 SCROOL_SIZE = D3DXVECTOR2(300.0f, 50.0f);				// 選択項目の大きさ
	const D3DXVECTOR2 SCROOL_DISTANCE = D3DXVECTOR2(100.0f + (STAGE_SIZE.x * 1.0f), 0.0f + (STAGE_SIZE.y * 0.0f));	// 選択項目の幅

	const D3DXCOLOR SELECT_COLOR_TRUE = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 選択状態項目の色
	const D3DXCOLOR SELECT_COLOR_FALSE = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);	// 選択してない項目の色

	const D3DXVECTOR3 BUTTON_POS = D3DXVECTOR3(640.0f, 670.0f, 0.0f);		// ボタンの位置
	const D3DXVECTOR2 BUTTON_SIZE = { 300.0f, 60.0f };						// ボタンの大きさ

	const float DOME_ROT_SPEED = 0.001f;	// メッシュドームの回転速度

	const float SCROOL_SPEED = 0.05f; // スクロールの速度
	const float SCROOL_SCALE = 0.1f; // スクロールの倍率
}

//静的メンバ変数宣言
CSelect* CSelect::m_pSelect = nullptr;
std::vector<CSelect::SStageSelect> CSelect::m_StageSelect = {};	// マップの情報

//====================================================================
//コンストラクタ
//====================================================================
CSelect::CSelect()
{
	for (int nCnt = 0; nCnt < NUM_SCROLLTYPE; nCnt++)
	{
		m_pScrollSelect[nCnt] = nullptr;
	}

	for (int nCnt = 0; nCnt < NUM_SCROLLTYPE; nCnt++)
	{
		m_pTexScroll[nCnt] = nullptr;
	}
	m_pTitleButton = nullptr;
	m_pMeshDome = nullptr;

	m_fCountFade = 0.0f;
	m_bStart = false;
	m_bTurn = false;
	m_bSlash = false;
	m_nSelect = 0;
	m_nStep = 0;
	m_nSetStage = 0;
	m_nTime = 0;
	m_fTex[0] = 0.0f;
	m_fTex[1] = 0.0f;
	m_Type = SCROLL_NONE;
}

//====================================================================
//デストラクタ
//====================================================================
CSelect::~CSelect()
{
	m_StageSelect.clear();
}


//====================================================================
//インスタンス取得
//====================================================================
CSelect* CSelect::GetInstance(void)
{
	if (m_pSelect == nullptr)
	{
		m_pSelect = new CSelect;
	}
	return m_pSelect;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CSelect::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// マップ情報取得
	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	std::vector<CMapSystem::MapInfo> MapInfo = pMapSystem->GetMapInfo();
	int nMax = MapInfo.size();

	// 全てのステージ分の選択肢生成
	for (int nCnt = 0; nCnt < nMax; nCnt++)
	{
		// 選択肢を一つ増やす
		m_StageSelect.emplace_back();

		// マップサムネイル
		m_StageSelect.back().m_pMapTex = CObject2D::Create();
		m_StageSelect.back().m_pMapTex->SetPos(D3DXVECTOR3(MAP_POS.x + (STAGE_DISTANCE.x * nCnt), MAP_POS.y + (STAGE_DISTANCE.y * nCnt), MAP_POS.z));
		m_StageSelect.back().m_pMapTex->SetSize(D3DXVECTOR3(MAP_SIZE.x, MAP_SIZE.y, 0.0f));
		m_StageSelect.back().m_pMapTex->SetIdx(pTexture->Regist(MapInfo[nCnt].texture));

		// ボタン
		m_StageSelect.back().m_pSelectUI = CObject2D::Create();
		m_StageSelect.back().m_pSelectUI->SetPos(D3DXVECTOR3(STAGE_POS.x + (STAGE_DISTANCE.x * nCnt), STAGE_POS.y + (STAGE_DISTANCE.y * nCnt), STAGE_POS.z));
		m_StageSelect.back().m_pSelectUI->SetSize(D3DXVECTOR3(STAGE_SIZE.x, STAGE_SIZE.y, 0.0f));

		// 数字(１)
		m_StageSelect.back().m_pSelectNumber01 = CNumber::Create();		// 生成
		m_StageSelect.back().m_pSelectNumber01->SetPos(D3DXVECTOR3(		// 位置
			m_StageSelect.back().m_pSelectUI->GetPos().x + NUMBER_DISTANCE + 32.0f,
			m_StageSelect.back().m_pSelectUI->GetPos().y,
			m_StageSelect.back().m_pSelectUI->GetPos().z));
		m_StageSelect.back().m_pSelectNumber01->SetSize(D3DXVECTOR3(NUMBER_SIZE.x, NUMBER_SIZE.y, 0.0f));		// サイズ
		m_StageSelect.back().m_pSelectNumber01->SetNumber(nCnt + 1);							// 数字

		// 数字(１０)
		m_StageSelect.back().m_pSelectNumber10 = CNumber::Create();		// 生成
		m_StageSelect.back().m_pSelectNumber10->SetPos(D3DXVECTOR3(		// 位置
			m_StageSelect.back().m_pSelectUI->GetPos().x + NUMBER_DISTANCE,
			m_StageSelect.back().m_pSelectUI->GetPos().y,
			m_StageSelect.back().m_pSelectUI->GetPos().z));
		m_StageSelect.back().m_pSelectNumber10->SetSize(D3DXVECTOR3(NUMBER_SIZE.x, NUMBER_SIZE.y, 0.0f));		// サイズ
		m_StageSelect.back().m_pSelectNumber10->SetNumber((nCnt + 1) / 10);						// 数字

		if (m_StageSelect.back().m_pSelectUI != nullptr)
		{ // ボタンUIが生成されてたら

			// Stageテクスチャ
			m_StageSelect.back().m_pSelectUI->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\stage00.png"));
		}
	}

	// 最後の選択肢の番号
	int nLast = nMax - 1;

	// 最後を最初に持っていく
	m_StageSelect[nLast].m_pMapTex->SetPos(D3DXVECTOR3(MAP_POS.x - STAGE_DISTANCE.x, MAP_POS.y - STAGE_DISTANCE.y, MAP_POS.z));
	m_StageSelect[nLast].m_pSelectUI->SetPos(D3DXVECTOR3(STAGE_POS.x - STAGE_DISTANCE.x, STAGE_POS.y - STAGE_DISTANCE.y, STAGE_POS.z));

	// モード用選択肢
	for (int nCnt = 0; nCnt < NUM_SCROLLTYPE; nCnt++)
	{
		if (m_pScrollSelect[nCnt] == nullptr)
		{
			// ボタン
			m_pScrollSelect[nCnt] = CObject2D::Create();
			m_pScrollSelect[nCnt]->SetPos(D3DXVECTOR3(300.0f + (500.0f * nCnt), SCROOL_POS.y + (SCROOL_DISTANCE.y * nCnt), SCROOL_POS.z));
			m_pScrollSelect[nCnt]->SetSize(D3DXVECTOR3(400.0f, 200.0f, 0.0f));
			m_pScrollSelect[nCnt]->SetColor(SELECT_COLOR_FALSE);
		}
		if (m_pScrollSelect[nCnt] != nullptr)
		{
			switch (nCnt)
			{
			case 0:
				m_pScrollSelect[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\ScrollSelect00.png"));
				break;
			case 1:
				m_pScrollSelect[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\ScrollSelect01.png"));
				break;
			}
		}
	}

	// モード用の参考画像
	for (int nCnt = 0; nCnt < NUM_SCROLL; nCnt++)
	{
		if (m_pTexScroll[nCnt] == nullptr)
		{
			// スクロール
			m_pTexScroll[nCnt] = CObject2D::Create();
			m_pTexScroll[nCnt]->SetPos(D3DXVECTOR3(550.0f+ (470.0f * nCnt), SCROOL_POS.y + (SCROOL_DISTANCE.y * nCnt), SCROOL_POS.z));
			m_pTexScroll[nCnt]->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		}
		// スクロール
		if (m_pTexScroll[nCnt] != nullptr)
		{
			switch (nCnt)
			{
			case 0:
				m_pTexScroll[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\scroll.png"));
				break;
			case 1:
				m_pTexScroll[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\scroll.png"));
				break;
			}
		}
	}

	// 決定ボタン
	if (m_pTitleButton == nullptr)
	{
		m_pTitleButton = CObject2D::Create();
		m_pTitleButton->SetPos(BUTTON_POS);
		m_pTitleButton->SetSize(D3DXVECTOR3(600.0f, 200.0f, 0.0f));
		//m_pTitleButton->SetColor(SELECT_COLOR_TRUE);
	}

	if (m_pTitleButton != nullptr)
	{
		m_pTitleButton->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\pressbotton.png"));
	}

	// ライトの初期化
	CManager::GetInstance()->GetLight()->Init();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CSelect::Uninit(void)
{
	//全てのオブジェクトの破棄
	CObject::ReleaseAll();

	if (m_pSelect != nullptr)
	{
		delete m_pSelect;
		m_pSelect = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CSelect::Update(void)
{
	// スクロール処理
	m_fTex[0] += SCROOL_SPEED * SCROOL_SCALE;

	// テクスチャ[0]移動量加算
	m_pTexScroll[0]->SetScroll(D3DXVECTOR2(m_fTex[0],1.0f));

	m_nTime++;

	if (m_nTime >= 20)
	{
		// テクスチャ[1]移動量加算
		m_fTex[1] += SCROOL_SPEED;
		m_pTexScroll[1]->SetScroll(D3DXVECTOR2(m_fTex[1], 1.0f));

		m_nTime = 0;
	}

	if (CManager::GetInstance()->GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if (m_nStep == 0)
		{
			// 選択処理
			StageSelect();

			// 決定処理
			StageButton();
		}
		else
		{
			// 選択処理
			ScrollSelect();

			// 決定処理
			ScrollButton();
		}
	}
}

//====================================================================
//描画処理
//====================================================================
void CSelect::Draw(void)
{

}

//====================================================================
//選択処理
//====================================================================
void CSelect::StageSelect(void)
{
	int nMax = CMapSystem::GetInstance()->GetMapInfo().size();

	// 右
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_D) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RIGHT) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_RIGHT, 0) ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_RIGHT, 1))
	{
		m_nSelect++;

		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);

		// 最大越えたら最初に戻る
		if (m_nSelect >= nMax)
		{
			m_nSelect = 0;
		}
	}
	// 左
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_A) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_LEFT) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_LEFT, 0) ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_LEFT, 1))
	{
		m_nSelect--;

		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);

		// 最初より小さかったら最大に戻る
		if (m_nSelect < 0)
		{
			m_nSelect = nMax - 1;
		}
	}

	for (int nCnt = 0; nCnt < nMax; nCnt++)
	{
		if (m_nSelect == nCnt)
		{
			m_StageSelect[nCnt].m_pSelectUI->SetColor(SELECT_COLOR_TRUE);
			m_StageSelect[nCnt].m_pMapTex->SetColor(SELECT_COLOR_TRUE);

		}
		else
		{
			m_StageSelect[nCnt].m_pSelectUI->SetColor(SELECT_COLOR_FALSE);
			m_StageSelect[nCnt].m_pMapTex->SetColor(SELECT_COLOR_FALSE);
		}
	}

	int nLast = nMax - 1;		// 最後の選択肢の番号
	int nStart = -nLast;		// 最初の選択肢の番号

	// 位置とか
	for (int nCnt = 0; nCnt < nMax; nCnt++)
	{
		int nNumber = nCnt - m_nSelect;
		//if (m_nSelect - 2 <= nCnt && nCnt <= m_nSelect + 2)
		//{//重くなった時用
			if (m_StageSelect[nCnt].m_pSelectUI != nullptr)
			{
				// ボタン
				m_StageSelect[nCnt].m_pSelectUI->SetPos(D3DXVECTOR3(STAGE_POS.x + (STAGE_DISTANCE.x * (nNumber)), STAGE_POS.y + (STAGE_DISTANCE.y * nCnt), STAGE_POS.z));
				//m_pStageSelect[nCnt].m_pSelectUI->SetSize(D3DXVECTOR3(500.0f, STAGE_SIZE.y, 0.0f));
			}

			if (m_StageSelect[nCnt].m_pMapTex != nullptr)
			{
				// サムネイル
				m_StageSelect[nCnt].m_pMapTex->SetPos(D3DXVECTOR3(MAP_POS.x + (STAGE_DISTANCE.x * (nNumber)), MAP_POS.y + (STAGE_DISTANCE.y * nCnt), MAP_POS.z));
				//m_pStageSelect[nCnt].m_pSelectUI->SetSize(D3DXVECTOR3(500.0f, STAGE_SIZE.y, 0.0f));
				
				if (nNumber == nLast)
				{ // 最初が選択されてる場合、最後を最初の左に

					// 最後を最初に持っていく
					m_StageSelect[nLast].m_pMapTex->SetPos(D3DXVECTOR3(MAP_POS.x - STAGE_DISTANCE.x, MAP_POS.y - STAGE_DISTANCE.y, MAP_POS.z));
					m_StageSelect[nLast].m_pSelectUI->SetPos(D3DXVECTOR3(STAGE_POS.x - STAGE_DISTANCE.x, STAGE_POS.y - STAGE_DISTANCE.y, STAGE_POS.z));
				}
				else if (nNumber == nStart)
				{ // 最後が選択されてる場合、最初を最後の右に

					// 最初を最後に持っていく
					m_StageSelect[0].m_pMapTex->SetPos(D3DXVECTOR3(MAP_POS.x + (STAGE_DISTANCE.x * 1), MAP_POS.y + (STAGE_DISTANCE.y * nCnt), MAP_POS.z));
					m_StageSelect[0].m_pSelectUI->SetPos(D3DXVECTOR3(STAGE_POS.x + (STAGE_DISTANCE.x * 1), STAGE_POS.y + (STAGE_DISTANCE.y * nCnt), STAGE_POS.z));
				}
			}

			if (m_StageSelect[nCnt].m_pSelectNumber01 != nullptr)
			{
				m_StageSelect[nCnt].m_pSelectNumber01->SetPos(D3DXVECTOR3(
					m_StageSelect[nCnt].m_pSelectUI->GetPos().x + NUMBER_DISTANCE + 32.0f,
					m_StageSelect[nCnt].m_pSelectUI->GetPos().y,
					m_StageSelect[nCnt].m_pSelectUI->GetPos().z));

				if (m_nSelect == nCnt)
				{
					m_StageSelect[nCnt].m_pSelectNumber01->SetColor(SELECT_COLOR_TRUE);
				}
				else
				{
					m_StageSelect[nCnt].m_pSelectNumber01->SetColor(SELECT_COLOR_FALSE);
				}
			}

			if (m_StageSelect[nCnt].m_pSelectNumber10 != nullptr)
			{
				m_StageSelect[nCnt].m_pSelectNumber10->SetPos(D3DXVECTOR3(
					m_StageSelect[nCnt].m_pSelectUI->GetPos().x + NUMBER_DISTANCE,
					m_StageSelect[nCnt].m_pSelectUI->GetPos().y,
					m_StageSelect[nCnt].m_pSelectUI->GetPos().z));

				if (m_nSelect == nCnt)
				{
					m_StageSelect[nCnt].m_pSelectNumber10->SetColor(SELECT_COLOR_TRUE);
				}
				else
				{
					m_StageSelect[nCnt].m_pSelectNumber10->SetColor(SELECT_COLOR_FALSE);
				}
			}
		//}
	}
}

//====================================================================
//決定処理
//====================================================================
void CSelect::StageButton(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 1) == true)
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);

		m_nSetStage = m_nSelect;
		m_nSelect = 0;
		m_nStep++;
	}
	else if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_BACKSPACE) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_B, 0) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_B, 1) == true)
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);
		CScene::SetMode(MODE_TITLE);
	}
}

//====================================================================
//選択処理
//====================================================================
void CSelect::ScrollSelect(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_D) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RIGHT) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_RIGHT, 0) ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_RIGHT, 1))
	{
		m_nSelect++;

		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);

		if (m_nSelect >= NUM_SCROLLTYPE)
		{
			m_nSelect = 0;
		}
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_A) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_LEFT) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_LEFT, 0) ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_LEFT, 1))
	{
		m_nSelect--;

		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);

		if (m_nSelect < 0)
		{
			m_nSelect = NUM_SCROLLTYPE - 1;
		}
	}

	for (int nCnt = 0; nCnt < NUM_SCROLLTYPE; nCnt++)
	{
		if (m_pScrollSelect[nCnt] != nullptr)
		{
			if (m_nSelect == nCnt)
			{
				m_pScrollSelect[nCnt]->SetColor(SELECT_COLOR_TRUE);

				// スムーズタイプに変更
				m_Type = SCROLL_LAGGY;
			}
			else
			{
				m_pScrollSelect[nCnt]->SetColor(SELECT_COLOR_FALSE);

				// カクカクタイプに変更
				m_Type = SCROLL_SMOOTH;
			}
		}
	}
}

//====================================================================
//決定処理
//====================================================================
void CSelect::ScrollButton(void)
{
	CMapSystem* pMapSystem = CMapSystem::GetInstance();

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 1) == true)
	{
		CManager::GetInstance()->SetTypeInput(CManager::GetInstance()->TYPE_MNK);	// 入力タイプ：キーマウ
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);

		// 指定のマップ番号を設定
		pMapSystem->SetSelectMap(m_nSetStage);

		switch (m_nSelect)
		{
		case 0:
			CManager::GetInstance()->SetScrollType(CMapMove::SCROLL_TYPE_NORMAL);
			CFade::SetFade(CScene::MODE_GAME);
			break;
		case 1:
			CManager::GetInstance()->SetScrollType(CMapMove::SCROLL_TYPE_RETRO);
			CFade::SetFade(CScene::MODE_GAME);
			break;
		}

		CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_BGM_TITLE);
	}
	else if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_BACKSPACE) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_B, 0) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_B, 1) == true)
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);

		for (int nCnt = 0; nCnt < NUM_SCROLLTYPE; nCnt++)
		{
			if (m_pScrollSelect[nCnt] != nullptr)
			{
				m_pScrollSelect[nCnt]->SetColor(SELECT_COLOR_FALSE);
			}
		}

		m_nSelect = 0;
		m_nStep--;
		m_nSelect = m_nSetStage;
	}
}