//============================================
//
//	オブジェクト3D [object3D.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "main.h"
#include "object.h"

//オブジェクト3Dクラス
class CObject3D : public CObject
{
public:
	CObject3D(int nPriority = 4);
	~CObject3D();

	static CObject3D *Create(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	void SetNULL(void);
	virtual void Update(void);
	virtual void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetVerPos(D3DXVECTOR3 Pos0, D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3);	//頂点座標の設定
	void SetVerTex(void);																	//頂点座標の設定
	void SetAnim(void);																		//テクスチャ座標の設定
	void SetAnim(D3DXVECTOR2 Tex);															//テクスチャ座標の設定
	void SetColor(D3DXCOLOR col);															//頂点カラーの設定
	void SetAlpha(float A) { m_Color.a = A; SetColor(m_Color); }							//頂点不透明度の設定
	float GetAlpha() { return m_Color.a; }							//頂点不透明度の設定
	void SetTexture(const char *name);

	void SetVtx(const D3DXVECTOR3& size);

	virtual void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPosOld(const D3DXVECTOR3& pos) { m_posOld = pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(const D3DXVECTOR3& size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	void SetAddDorw(bool Set) { m_AddDarw = Set; }
	bool GetAddDorw(void) { return m_AddDarw; }
	void SetLighting(bool Set) { m_Lighting = Set; }
	bool GetLighting(void) { return m_Lighting; }
	void SetScrollSpeed(D3DXVECTOR2 Speed) { m_ScrollSpeed = Speed; }
	D3DXVECTOR2 GetScrollSpeed(void) { return m_ScrollSpeed; }

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	virtual int GetIdx(void) { return m_nIdxTexture; }

	void SetUseMultiMatrix(D3DXMATRIX* Set) { m_UseMultiMatrix = Set; }
	D3DXMATRIX* GetUseMultiMatrix(void) { return m_UseMultiMatrix; }

protected:
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス

private:
	int m_nIdxTexture;
	bool m_AddDarw;							//加算合成
	bool m_Lighting;						//ライティング
	D3DXVECTOR3 m_pos;						//現在の位置
	D3DXVECTOR3 m_posOld;					//過去の位置
	D3DXVECTOR3 m_rot;						//向き
	D3DXVECTOR3 m_size;						//サイズ
	D3DXVECTOR2 m_Scroll;					//スクロールアニメーション
	D3DXVECTOR2 m_ScrollSpeed;				//スクロールの速度
	D3DXCOLOR m_Color;						//色

	//マップとのマトリックス情報
	D3DXMATRIX* m_UseMultiMatrix;			//掛け合わせるマトリックス
};
#endif