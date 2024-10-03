//==============================================================
//
//チュートリアル画像の処理[tutorial_texture.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "testline.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "tutorial_texture.h"
#include "effect.h"

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CTestLine::m_pTexture = NULL;
const char* CTestLine::m_apFilename[STEP_MAX] =
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
CTestLine::CTestLine()
{
	m_nIdxTexture = -1;		//テクスチャの番号
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置

}

//==============================================================
//コンストラクタ
//==============================================================
CTestLine::CTestLine(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	m_nIdxTexture = -1;		//テクスチャの番号
	m_pos = pos;		//位置


}

//==============================================================
//デストラクタ
//==============================================================
CTestLine::~CTestLine()
{

}

//==============================================================
//壁の生成処理
//==============================================================
CTestLine* CTestLine::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	CTestLine* pTest = NULL;

	if (pTest == NULL)
	{//メモリが使用されてなかったら

		//壁の生成
		pTest = new CTestLine(pos, rot, nType);
	}

	//初期化処理
	pTest->Init();

	return pTest;
}

//==============================================================
//壁の初期化処理
//==============================================================
HRESULT CTestLine::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//テクスチャの読み込み
	//m_nIdxTexture = pTexture->Regist(m_apFilename[m_nType]);

	//テクスチャの割り当て
	CObject3D::BindTexture(m_nIdxTexture);

	//オブジェクト3Dの初期化処理
	CObject3D::Init();

	//種類の設定
	CObject::SetType(CObject::TYPE_WALL);

	//位置の設定
	CObject3D::SetPosition(m_pos);

	//壁の大きさ設定
	CObject3D::SetSize(WIDTH, HEIGHT);

	return S_OK;
}

//==============================================================
//壁の終了処理
//==============================================================
void CTestLine::UnInit(void)
{
	//オブジェクト3Dの終了処理
	CObject3D::Uninit();
}

//==============================================================
//壁の更新処理
//==============================================================
void CTestLine::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	pDebugProc->Print("\nエフェクトの位置    (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);

}

//==============================================================
//壁の描画処理
//==============================================================
void CTestLine::Draw(void)
{
	//オブジェクト3Dの描画処理
	CObject3D::Draw();
}
