//========================================
//
//シューティングアクション[object3D.h]
//Author：森川駿弥
//
//========================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "main.h"
#include "object.h"

//========================================
// オブジェクト3Dクラス
//========================================
class CObject3D : public CObject
{
public:
	CObject3D(int nPriority = 3);	//コンストラクタ
	~CObject3D();	//デストラクタ

	//メンバ関数
	HRESULT Init(void);		//初期化
	void Uninit(void);		//終了
	void Update(void);		//更新
	void Draw(void);		//描画
	static CObject3D *Create();		//オブジェクトの生成
	void BindTexture(int pTexture);	//テクスチャ割り当て

	void SetVertex(void);
	void SetSize(float fWidth, float fHeight);

	virtual void SetVertex3D(void);
	void SetVerTexBillboard(void);
	void Collision(void);
	void MeshVertex(void);

	float GetField();

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() { return m_pVtxBuff; }		// 頂点情報の取得
	int GetIdxTexture() { return m_nIdxTexture; }		// テクスチャの番号

private:
	//メンバ変数
	LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点情報へのポインタ
	D3DXMATRIX m_mtxWorld;				//ワールドマトリックス
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		 //インデックスバッファへのポインタ

	int m_nIdxTexture;		//テクスチャの番号
};

#endif