//============================================
//
//	オブジェクト2D [object2D.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include "main.h"
#include "object.h"

//オブジェクト2Dクラス
class CObject2D : public CObject
{
public:
	CObject2D(int nPriority = 5);
	~CObject2D();

	static CObject2D *Create(int nPriority = 5);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetVerTex(void);						//頂点座標の設定
	void SetAnim(D3DXVECTOR2 Tex);				//テクスチャ座標の設定
	void SetAnim(D3DXVECTOR2 TexMin, D3DXVECTOR2 TexMax);
	void SetAnimTex(const int nPattern, const int nWidthPtrn, const int nHeightPtrn);
	void SetScroll(D3DXVECTOR2 Tex);			//テクスチャ座標の設定
	void SetColor(D3DXCOLOR col);				//頂点カラーの設定
	void SetAlpha(float A);					//頂点カラーの設定
	void SetTexture(const std::string name);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXCOLOR GetColor(void) { return m_Color; }
	virtual void SetIdx(int Idx) { m_nIdxTexture = Idx; }
	virtual int GetIdx(void) { return m_nIdxTexture; }
	void SetAddDraw(bool Set) { m_AddDarw = Set; }
	void SetTargetTexture(LPDIRECT3DTEXTURE9 Texture) { m_pTargetTexture = Texture; }
	void SetMultiTarget(bool Set) { m_MultiTarget = Set; }
	void SetNULL(void);

protected:
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 vtx) { m_pVtxBuff = vtx; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }

private:
	int m_nIdxTexture;
	bool m_AddDarw;							//描画の時に加算合成をするかどうか
	D3DXCOLOR m_Color;						//色
	D3DXVECTOR3 m_pos;						//頂点の位置
	D3DXVECTOR3 m_rot;						//向き
	D3DXVECTOR3 m_size;						//サイズ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DTEXTURE9 m_pTargetTexture;	//テクスチャへのポインタ
	bool m_MultiTarget;						//マルチターゲットレンダリングをオンにするかどうか

};
#endif