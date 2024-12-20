//============================================
//
//	オブジェクトXファイル [objectX.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include "main.h"
#include "object.h"
#include "move.h"
#include "MapSystem.h"

//オブジェクトXクラス
class CObjectX : public CObject
{
public:
	CObjectX(int nPriority = 4);
	~CObjectX();

	static CObjectX *Create(const char *pModelName);

	virtual HRESULT Init(void);
	virtual HRESULT Init(const char* pModelName);
	virtual void Uninit(void);
	void SetNULL(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Draw(float fAlpha);
	void BindFile(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pdwNumMat);

	void SetVerTex(void);							//頂点座標の設定
	void SetAnim(D3DXVECTOR2 Tex);					//テクスチャ座標の設定
	void SetColor(D3DXCOLOR col);					//頂点カラーの設定

	void SizeVtxSet(void);
	void SwapSize(void);

	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMove(D3DXVECTOR3 pos) { m_move = pos; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetMatColorA(float SetColorA) { m_MatColor.a = SetColorA; m_bUseColor = true; }
	void SetMatColor(D3DXCOLOR SetColor) { m_MatColor = SetColor; m_bUseColor = true; }
	D3DXCOLOR GetMatColor(void) { return m_MatColor; }
	void SetMatAlpha(float SetAlpha) { m_StateMat.MatD3D.Diffuse.a = SetAlpha; }
	void SetDefColor(void) { m_bUseColor = false; }
	void SetScaling(D3DXVECTOR3 Scaling) { m_Scaling = Scaling; }
	D3DXVECTOR3 GetScaling(void) { return m_Scaling; }

	void SetXModelIdx(int Idx);
	int GetXModelIdx(void) { return m_IdxModelNumber; }
	void SetTextureIdx(int nCnt, int Idx);
	int GetTextureIdx(int nCnt) { return m_IdxTextureNumber[nCnt]; }
	void SetMtxWorld(D3DMATRIX mtx) { m_SetMtx = mtx; m_bUseMtx = true; }

	void SetUseMultiMatrix(D3DXMATRIX* Set);
	D3DXMATRIX* GetUseMultiMatrix(void) { return m_UseMultiMatrix; }

	void SetDisp(bool frag) { m_bDisp = frag; }
	bool GetDisp() { return m_bDisp; }

	// マップ番号の設定
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	virtual CMapSystem::GRID GetGrid(void) { return m_Grid; }
	virtual void ChangeMoveState(CMoveState* pMoveState) {}   // 移動状態変更

protected:

	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス

private:

	bool m_bDisp;							// 描画フラグ
	int m_IdxModelNumber;					//モデルの番号
	int m_IdxTextureNumber[128];			//テクスチャの番号
	LPDIRECT3DTEXTURE9 *m_pTexture;			//テクスチャへのポインタ
	LPD3DXMESH m_pMesh;						//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;				//マテリアルへのポインタ
	DWORD m_dwNumMat;						//マテリアルの数
	bool m_bUseMtx;							//マトリックスを使用するかどうか
	D3DMATRIX m_SetMtx;						//マトリックス
	D3DXVECTOR3 m_pos;						//頂点の位置
	D3DXVECTOR3 m_posOld;					//過去の位置
	D3DXVECTOR3 m_rot;						//向き
	D3DXVECTOR3 m_move;						//移動量
	D3DXVECTOR3 m_size;						//サイズ
	D3DXVECTOR3 m_Scaling;					//大きさ
	D3DXMATERIAL m_StateMat;				//状態管理時の色
	D3DXCOLOR m_MatColor;					//マテリアルの色
	bool m_bUseColor;						//色を変更しているかどうか
	CMapSystem::GRID m_Grid;		//グリット番号

	//マップとのマトリックス情報
	D3DXMATRIX* m_UseMultiMatrix;			//掛け合わせるマトリックス

};
#endif