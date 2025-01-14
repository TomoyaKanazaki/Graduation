//========================================
//
// テクスチャ管理[texture.h]
// Author：森川駿弥
//
//========================================

#include "texture.h"
#include "renderer.h"
#include "manager.h"

//========================================
//定数定義
//========================================
namespace
{
	// テクスチャ一括管理
	const std::string str[]
	{
		{ "data\\TEXTURE\\UI\\tutorial_text_00.png" },	// 移動のテキスト
		{ "data\\TEXTURE\\UI\\tutorial_text_01.png" },	// 十字架座標
		{ "data\\TEXTURE\\UI\\tutorial_text_03.png" },	// ボワボワの座標
		{ "data\\TEXTURE\\UI\\tutorial_text_02.png" },	// 攻撃の座標
		{ "data\\TEXTURE\\UI\\tutorial_text_004.png" },	// 聖書の座標
		{ "data\\TEXTURE\\UI\\tutorial_text_05.png" },	// デビルホールの座標
	};
}

//========================================
//静的メンバ変数
//========================================
int CTexture::m_nNumAll = 0;

//========================================
//コンストラクタ
//========================================
CTexture::CTexture()
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		m_apTexture[nCntTex] = {};	//テクスチャのポインタ
		m_apPath[nCntTex] = "\0";
	}
}

//========================================
//デストラクタ
//========================================
CTexture::~CTexture()
{

}

//========================================
//テクスチャ一括読み込み
//========================================
HRESULT CTexture::Load(void)
{
	////デバイスの取得
	//CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();
	//LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//指定のテクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\NULL.png", &m_apTexture[0]);

	//総数をカウントアップ
	//m_nNumAll++;

	return S_OK;
}

//========================================
//テクスチャ破棄
//========================================
void CTexture::Unload(void)
{
	for (int nCntTex = 0; nCntTex < m_nNumAll; nCntTex++)
	{//テクスチャの終了処理
		if (m_apTexture[nCntTex] != nullptr)
		{//m_apTexture[nCntTex]がnullptrじゃないとき
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = nullptr;
		}
	}
}

//========================================
//テクスチャ個別割り当て
//========================================
int CTexture::Regist(const std::string pfile)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		if (m_apPath[nCntTex] == pfile)
		{//ファイル名が一致したとき
			//nCntTex番目を返す
			return nCntTex;
		}

		if (m_apTexture[nCntTex] == nullptr)
		{//nullptrの時
			//CRenderer型のポインタ
			CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

			//デバイスの取得
			LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

			//指定のテクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pfile.c_str(), &m_apTexture[nCntTex]);

			//ファイル名を入れる
			m_apPath[nCntTex] = pfile;

			//総数をカウントアップ
			m_nNumAll++;

			//nCntTex番目を返す
			return nCntTex;
		}
	}

	if (m_nNumAll > MAX_TEXTURE)
	{// 総数がテクスチャの最大数を超えたら
		assert(false);
	}

	// -1を返す
	return -1;
}

//========================================
//テクスチャのアドレス取得
//========================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (nIdx >= 0 && nIdx <= m_nNumAll)
	{
		return m_apTexture[nIdx];
	}

	//nullptrを返す
	return nullptr;
}
