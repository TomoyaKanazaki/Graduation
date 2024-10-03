//==============================================================
//
//チュートリアル画像の処理[tutorial_texture.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "tutorial_texture.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "test.h"
#include "debugproc.h"

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CTutorialUI::m_pTexture = NULL;
const char *CTutorialUI::m_apFilename[STEP_MAX] =
{
	"data\\TEXTURE\\tutorial000.png",		//移動
	"data\\TEXTURE\\tutorial001.png",		//ジャンプ
	"data\\TEXTURE\\tutorial002.png",		//ダッシュ
	"data\\TEXTURE\\tutorial003.png",		//通り抜け
	"data\\TEXTURE\\tutorial004.png",		//アイテム
	"data\\TEXTURE\\tutorial005.png",		//敵
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
CTutorialUI::CTutorialUI()
{
	m_nIdxTexture = -1;		//テクスチャの番号
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_distance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_pCircle = nullptr;

	m_nType = 0;
}

//==============================================================
//コンストラクタ
//==============================================================
CTutorialUI::CTutorialUI(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	m_nIdxTexture = -1;		//テクスチャの番号
	m_pos = pos;		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_distance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_pCircle = nullptr;

	m_nType = nType;	//壁の種類

}

//==============================================================
//デストラクタ
//==============================================================
CTutorialUI::~CTutorialUI()
{

}

//==============================================================
//壁の生成処理
//==============================================================
CTutorialUI *CTutorialUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	CTutorialUI *pWall = NULL;

	if (pWall == NULL)
	{//メモリが使用されてなかったら

		//壁の生成
		pWall = new CTutorialUI(pos, rot, nType);
	}

	//初期化処理
	pWall->Init();

	return pWall;
}

//==============================================================
//壁の初期化処理
//==============================================================
HRESULT CTutorialUI::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//テクスチャの読み込み
	//m_nIdxTexture = pTexture->Regist(m_apFilename[m_nType]);

	//テクスチャの割り当て
	//CObject3D::BindTexture(m_nIdxTexture);

	//ビルボードの初期化処理
	CObject3D::Init();

	//種類の設定
	CObject::SetType(CObject::TYPE_WALL);

	//位置の設定
	CObject3D::SetPosition(m_pos);

	//壁の大きさ設定
	CObject3D::SetSize(WIDTH, HEIGHT);

	// プレイヤー2の色設定
	CObject3D::SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

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
void CTutorialUI::UnInit(void)
{
	//オブジェクト3Dの終了処理
	CObject3D::Uninit();
}

//==============================================================
//壁の更新処理
//==============================================================
void CTutorialUI::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	m_posOld = m_pos;

	m_distance = m_posTest2;

	m_fDistance = (m_pos.x - m_distance.x) * (m_pos.x - m_distance.x) + (m_pos.y - m_distance.y) * (m_pos.y - m_distance.y);
	m_fDistance = sqrtf(m_fDistance);

	CircleColllllll();

	MoveKeyboard();

	m_pos += m_move;			//移動量加算

	ScreenLimit();

	//移動量を更新
	m_move.x += (0.0f - m_move.x) * 0.008f;
	m_move.y += (0.0f - m_move.y) * 0.008f;
	m_move.z += (0.0f - m_move.z) * 0.008f;

	CObject3D::SetPosition(m_pos);
	m_pCircle->SetPosition(m_pos);

	pDebugProc->Print("\nプレイヤー2の位置    (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("\n距離2：%f\n", m_fDistance);
	pDebugProc->Print("\n倍率2：%f\n", m_fScale);
}

//==============================================================
//壁の描画処理
//==============================================================
void CTutorialUI::Draw(void)
{
	//オブジェクト3Dの描画処理
	CObject3D::Draw();
}

//==============================================================
//プレイヤーキーボードの移動操作
//==============================================================
void CTutorialUI::MoveKeyboard(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得
	if (m_fScale < 0.01f)
	{
		m_fScale = 0.01f;
	}

	//移動
	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{//右

		m_move.x = sinf(pCamera->GetRotation().y + -D3DX_PI * 0.5f) * MOVE / m_fScale;
		m_move.z = cosf(pCamera->GetRotation().y + -D3DX_PI * 0.5f) * MOVE / m_fScale;
	}
	else if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{//左
		m_move.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.5f) * MOVE / m_fScale;
		m_move.z = cosf(pCamera->GetRotation().y + D3DX_PI * 0.5f) * MOVE / m_fScale;
	}
	else if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{//上

		m_move.x = sinf(pCamera->GetRotation().y + D3DX_PI * 1.0f) * MOVE / m_fScale;
		m_move.y = cosf(pCamera->GetRotation().y + D3DX_PI * 1.0f) * MOVE / m_fScale;
	}
	else if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{//下

		m_move.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.0f) * MOVE / m_fScale;
		m_move.y = cosf(pCamera->GetRotation().y + D3DX_PI * 0.0f) * MOVE / m_fScale;
	}
}

//==============================================================
// 画面外判定
//==============================================================
void CTutorialUI::ScreenLimit(void)
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
void CTutorialUI::CircleColllllll(void)
{
	if (m_fDistance <= 400.0f + 400.0f)
	{
		float Base = 800.0f;

		m_fScale = (m_fDistance / Base);

		return;
	}

	m_fScale = 1.0f;
}