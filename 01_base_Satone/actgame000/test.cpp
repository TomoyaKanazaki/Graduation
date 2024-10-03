//==============================================================
//
//チュートリアル画像の処理[tutorial_texture.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "test.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "tutorial_texture.h"
#include "effect.h"
#include "testline.h"

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CTest::m_pTexture = NULL;
const char *CTest::m_apFilename[STEP_MAX] =
{
	"data\\TEXTURE\\death.png",		//移動
	"data\\TEXTURE\\death.png",		//ジャンプ
};

namespace
{
	const D3DXVECTOR3 LIMIT = D3DXVECTOR3(1300.0f, 700.0f, 0.0f);

	const int MAX_PLAYER = 2;
	const float WIDTH = 50.0f * 0.5f;
	const float HEIGHT = 50.0f * 0.5f;
	const float MOVE = 10.0f;		// 移動量

}

//==============================================================
//コンストラクタ
//==============================================================
CTest::CTest()
{
	m_nIdxTexture = -1;		//テクスチャの番号
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_distance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量

	m_nType = 0;

	m_pTest2 = nullptr;
	m_pCircle = nullptr;
}

//==============================================================
//コンストラクタ
//==============================================================
CTest::CTest(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	m_nIdxTexture = -1;		//テクスチャの番号
	m_pos = pos;		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_distance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_nType = nType;
	m_pTest2 = nullptr;
	m_pCircle = nullptr;

}

//==============================================================
//デストラクタ
//==============================================================
CTest::~CTest()
{

}

//==============================================================
//壁の生成処理
//==============================================================
CTest *CTest::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	CTest *pTest = NULL;

	if (pTest == NULL)
	{//メモリが使用されてなかったら

		//壁の生成
		pTest = new CTest(pos, rot, nType);
	}

	//初期化処理
	pTest->Init();

	return pTest;
}

//==============================================================
//壁の初期化処理
//==============================================================
HRESULT CTest::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//オブジェクト3Dの初期化処理
	CObject3D::Init();

	//種類の設定
	CObject::SetType(CObject::TYPE_WALL);

	//位置の設定
	CObject3D::SetPosition(m_pos);

	//壁の大きさ設定
	CObject3D::SetSize(WIDTH, HEIGHT);

	// プレイヤー1の色設定
	CObject3D::SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	m_pTest2 = CTutorialUI::Create(D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	m_pTestLine = CTestLine::Create(D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	// 円の生成
	m_pCircle = CObject3D::Create();

	//テクスチャの読み込み
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\item00.png");
	BindTexture(m_nIdxTexture);

	//テクスチャの割り当て
	m_pCircle->BindTexture(m_nIdxTexture);

	m_pCircle->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pCircle->SetSize(400.0f, 400.0f);
	m_pCircle->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0, 0.5f));

	return S_OK;
}

//==============================================================
//壁の終了処理
//==============================================================
void CTest::UnInit(void)
{
	if (m_pCircle != nullptr)
	{
		m_pCircle->Uninit();
		m_pCircle = nullptr;
	}

	//オブジェクト3Dの終了処理
	CObject3D::Uninit();
}

//==============================================================
//壁の更新処理
//==============================================================
void CTest::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	m_posOld = m_pos;

	m_pTest2->SetTest2Position(m_pos);

	m_distance = m_pTest2->GetPosition();

	m_fDistance = (m_pos.x - m_distance.x) * (m_pos.x - m_distance.x) + (m_pos.y - m_distance.y) * (m_pos.y - m_distance.y);
	m_fDistance = sqrtf(m_fDistance);

	CircleColllllll();

	MoveKeyboard();

	D3DXVECTOR3 vecline = m_distance - m_pos;
	D3DXVECTOR3 linepos = m_pos + (vecline * 0.5f);
	float fRot = atan2f(vecline.y, vecline.x);

	m_pTestLine->SetPosition(linepos);
	m_pTestLine->SetRot(D3DXVECTOR3(0.0f, 0.0f, fRot));
	m_pTestLine->SetSize(m_fDistance * 0.5f, 10.0f);

	m_pos += m_move;			//移動量加算

	ScreenLimit();

	//移動量を更新
	m_move.x += (0.0f - m_move.x) * 0.008f;
	m_move.y += (0.0f - m_move.y) * 0.008f;
	m_move.z += (0.0f - m_move.z) * 0.008f;

	CObject3D::SetPosition(m_pos);
	m_pCircle->SetPosition(m_pos);

	pDebugProc->Print("\nプレイヤー1の位置    (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("\n距離1：%f\n", m_fDistance);
	pDebugProc->Print("\n倍率1：%f\n", m_fScale);

}

//==============================================================
//壁の描画処理
//==============================================================
void CTest::Draw(void)
{
	//オブジェクト3Dの描画処理
	CObject3D::Draw();
}

//==============================================================
//プレイヤーキーボードの移動操作
//==============================================================
void CTest::MoveKeyboard(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得
	if (m_fScale < 0.01f)
	{
		m_fScale = 0.01f;
	}

	//移動
	if (pInputKeyboard->GetTrigger(DIK_D) == true)
	{//右

		m_move.x = sinf(pCamera->GetRotation().y + -D3DX_PI * 0.5f) * MOVE / m_fScale;
		m_move.z = cosf(pCamera->GetRotation().y + -D3DX_PI * 0.5f) * MOVE / m_fScale;
	}
	else if (pInputKeyboard->GetTrigger(DIK_A) == true)
	{//左
		m_move.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.5f) * MOVE / m_fScale;
		m_move.z = cosf(pCamera->GetRotation().y + D3DX_PI * 0.5f) * MOVE / m_fScale;
	}
	else if (pInputKeyboard->GetTrigger(DIK_W) == true)
	{//上

		m_move.x = sinf(pCamera->GetRotation().y + D3DX_PI * 1.0f) * MOVE / m_fScale;
		m_move.y = cosf(pCamera->GetRotation().y + D3DX_PI * 1.0f) * MOVE / m_fScale;
	}
	else if (pInputKeyboard->GetTrigger(DIK_S) == true)
	{//下

		m_move.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.0f) * MOVE / m_fScale;
		m_move.y = cosf(pCamera->GetRotation().y + D3DX_PI * 0.0f) * MOVE / m_fScale;
	}
}

//==============================================================
// 画面外判定
//==============================================================
void CTest::ScreenLimit(void)
{
	if (m_pos.x <= -LIMIT.x)
	{
		m_pos.x = -LIMIT.x;
		m_move.x *= -1.0f;
	}
	else if (m_pos.x >= LIMIT.x)
	{
		m_pos.x = LIMIT.x;
		m_move.x *= -1.0f;
	}

	if (m_pos.y <= -LIMIT.y)
	{
		m_pos.y = -LIMIT.y;
		m_move.y *= -1.0f;
	}
	else if (m_pos.y >= LIMIT.y)
	{
		m_pos.y = LIMIT.y;
		m_move.y *= -1.0f;
	}
}

//==============================================================
// 円の当たり判定
//==============================================================
void CTest::CircleColllllll(void)
{
	if (m_fDistance <= 400.0f + 400.0f)
	{
		float Base = 800.0f;

		m_fScale = (m_fDistance / Base);

		return;
	}

	m_fScale = 1.0f;
}