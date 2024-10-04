//========================================
//
//オブジェクト管理[object.h]
//Author：森川駿弥
//
//========================================
#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "main.h"

//========================================
// 定数定義
//========================================
namespace
{
const int MAX_OBJ = 256;	// オブジェクトの最大数
const int NUM_PRIORITY = 8;	// 優先順位の総数
}

//========================================
//オブジェクトクラス
//========================================
class CObject
{
public: 
	CObject(int nPriority = 3);				//コンストラクタ
	virtual ~CObject();		//デストラクタ

	//純粋仮想関数
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	//メンバ関数
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void DeleteAll(void);

	// 位置の設定と取得
	virtual void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void);
	void SetOriginPosition(const MyLib::Vector3& pos) { m_posOrigin = pos; }	// 初期位置
	MyLib::Vector3 GetOriginPosition() { return m_posOrigin; }					// 初期位置
	void SetOldPosition(const MyLib::Vector3& pos) { m_posOld = pos; }			// 過去位置
	MyLib::Vector3 GetOldPosition() { return m_posOld; }						// 過去位置

	// 移動量の設定と取得
	void SetMove(D3DXVECTOR3 move);
	D3DXVECTOR3 GetMove();

	// 向きの設定と取得
	void SetRot(D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRot(void);
	
	void Delete(void);
	void BindTexture(int pTexture);		//テクスチャ割り当て

	void SetDrawFrag(bool bDraw);		// 描画するかしないか

	// ワールドマトリックスの設定と取得
	void SetmtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }
	D3DXMATRIX GetmtxWorld() { return m_mtxWorld; }

	// メッシュの頂点情報と設定と取得
	void SetMesh(LPD3DXMESH pMesh) { m_pMesh = pMesh; }
	LPD3DXMESH GetMesh() { return m_pMesh; }

	// マテリアルの取得と設定
	void SetBuffMat(LPD3DXBUFFER pBuffMat) { m_pBuffMat = pBuffMat; }
	LPD3DXBUFFER GetBuffMat() { return m_pBuffMat; }

	// マテリアルの設定と取得
	void SetdwNumMat(DWORD dwNumMat) {m_dwNumMat = dwNumMat;}
	DWORD GetdwNumMat() { return m_dwNumMat; }

	bool IsDeath() { return m_bRelease; }

protected:
	void Release(void);

private:
	// メンバ変数
	static int m_nNumAll;		// オブジェクト総数
	static CObject *m_pTop[NUM_PRIORITY];		//先頭
	static CObject *m_pCurrent[NUM_PRIORITY];	//最後尾

	int m_nID;				// 自分自身のID
	int m_nIdxTexture;		// テクスチャの番号
	int m_nPriority;		// 優先順位の位置

	bool m_bRelease;		//終了フラグ
	bool m_bDrawFrag;		//終了フラグ

	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス
	LPD3DXMESH m_pMesh;				//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;		//マテリアルへのポインタ
	DWORD m_dwNumMat;				//マテリアルの数

	D3DXVECTOR3 m_vtxMin, m_vtxMax;	//モデルの最大、最小値
	D3DXVECTOR3 m_pos;			// 位置
	MyLib::Vector3 m_posOrigin;	// 初期位置
	MyLib::Vector3 m_posOld;	// 過去位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_move;			// 移動量

	CObject *m_pNext;		//次
	CObject *m_pPrev;		//前回
};

#endif
