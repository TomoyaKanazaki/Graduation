//=======================================
//
//メッシュフィールド処理[meshfield.h]
//Author　:　MORIKAWA SHUNYA
//
//=======================================

#ifndef _IDXMESHFIELD_H_     //このマクロ定義が定義されていなかったら
#define _IDXMESHFIELD_H_	 //2連インクルード防止のマクロを定義する

#include "main.h"
#include "object3D.h"

//=======================================
//メッシュフィールドクラス
//=======================================
class CIdxMeshField : public CObject3D
{
public:
	CIdxMeshField();	//コンストラクタ
	~CIdxMeshField();	//デストラクタ

	//メンバ関数
	static HRESULT Load(void);
	static void Unload(void);
	static CIdxMeshField *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtx;					//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffMeshField;		 //インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_rot;			//向き
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
};

#endif