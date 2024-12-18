//========================================
//
// テクスチャ管理[texture.h]
// Author：森川駿弥
//
//========================================

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//=======================================
// 定数定義
//=======================================
namespace
{
	const int MAX_TEXTURE = 128;	// テクスチャの最大数
}

//========================================
//テクスチャクラス
//========================================
class CTexture
{
public:
	enum Texture
	{// テクスチャ管理
		TYPE_TITLE = 0,
		TYPE_MAX
	};

	CTexture();		//コンストラクタ
	~CTexture();	//デストラクタ

	//メンバ関数
	HRESULT Load(void);		//テクスチャ読み込み
	void Unload(void);		//テクスチャ破棄

	int Regist(const std::string pfile);			//テクスチャ割り当て
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);	//テクスチャのアドレス取得

private:
	//メンバ変数
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE];	//テクスチャの配列
	std::string m_apPath[MAX_TEXTURE];
	static int m_nNumAll;		//総数
};

#endif
