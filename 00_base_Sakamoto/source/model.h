//============================================
//
//	モデルの処理 [model.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "object.h"

//オブジェクトプレイヤークラス
class CModel
{
public:
	CModel();
	~CModel();

	enum COLORTYPE
	{
		COLORTYPE_FALSE = 0,
		COLORTYPE_TRUE_ALL,
		COLORTYPE_TRUE_A,
		COLORTYPE_MAX,
	};

	static CModel *Create(const char *pModelName);
	HRESULT Init(const char *pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CModel* GetParent(void) { return m_pParent; }
	void SetParent(CModel *pModel) { m_pParent = pModel; }
	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetWorldPos(void) { return m_WorldPos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetStartPos(D3DXVECTOR3 pos) { m_Startpos = pos; }
	D3DXVECTOR3 GetStartPos(void) { return m_Startpos; }
	void SetStartRot(D3DXVECTOR3 rot) { m_Startrot = rot; }
	D3DXVECTOR3 GetStartRot(void) { return m_Startrot; }
	void SetColor(D3DCOLORVALUE Color) { m_Color = Color; }
	D3DCOLORVALUE GetColor(void) { return m_Color; }
	void SetColorType(COLORTYPE Set) { m_ColorType = Set; }
	COLORTYPE GetColorType(void) { return m_ColorType; }
	void SetDisp(bool Set) { m_bDisp = Set; }
	bool GetDisp(void) { return m_bDisp; }
	void SetColorA(float ColorA) { m_Color.a = ColorA; }
	char* GetModelName(void) { return &m_acModelName[0]; }
	void SetScaling(D3DXVECTOR3 Scaling) { m_Scaling = Scaling; }

private:
	LPDIRECT3DTEXTURE9 *m_pTexture;	//テクスチャへのポインタ
	LPD3DXMESH m_pMesh;				//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;		//マテリアルへのポインタ
	DWORD m_dwNumMat;				//マテリアルの数
	char m_acModelName[128];		//モデルの名前

	D3DXVECTOR3 m_Startpos;			//オフセットの位置
	D3DXVECTOR3 m_Startrot;			//オフセットの位置
	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_rot;				//向き
	D3DXVECTOR3 m_Scaling;			//拡大率
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス
	D3DXVECTOR3 m_WorldPos;			//ワールド座標
	CModel *m_pParent;				//親モデルへのポインタ
	D3DCOLORVALUE m_Color;			//色
	COLORTYPE m_ColorType;			//色を変更するかどうか
	bool m_bDisp;					//画面に表示するかどうか
};

#endif