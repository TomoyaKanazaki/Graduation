//=======================================
//
//ライト[light.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================
#include "light.h"
#include "renderer.h"
#include "manager.h"

//=======================================
//コンストラクタ
//=======================================
CLight::CLight()
{

}

//=======================================
//デストラクタ
//=======================================
CLight::~CLight()
{

}

//=======================================
//ライトの初期化処理
//=======================================
HRESULT CLight::Init(void)
{
	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXVECTOR3 vecDir[3];	//設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < 3; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&m_alight[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		m_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		m_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_alight[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);
		m_alight[2].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

		//ライトの方向を設定
		vecDir[0] = D3DXVECTOR3(0.22f, -0.97f, 0.54f);
		vecDir[1] = D3DXVECTOR3(0.38f, 0.78f, -0.54f);
		vecDir[2] = D3DXVECTOR3(-0.9f, -0.21f, -0.44f);

		//ベクトルを正規化する
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);

		m_alight[nCntLight].Direction = vecDir[nCntLight];

		//ライトを設定
		pDevice->SetLight(nCntLight, &m_alight[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}

	//成功を返す
	return S_OK;
}

//=======================================
//ライトの終了処理
//=======================================
void CLight::Uninit(void)
{

}

//=======================================
//ライトの更新処理
//=======================================
void CLight::Update(void)
{

}