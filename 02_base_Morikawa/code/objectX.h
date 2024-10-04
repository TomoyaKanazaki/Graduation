//========================================
//
//objectX処理[objectX.h]
//Author：森川駿弥
//
//========================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_
#include "main.h"
#include "object.h"
#include <functional>

//========================================
// 定数定義
//========================================
namespace
{
const int MAX_OBJX = 160;		// オブジェクトXの最大数
const int MAX_GARBAGE = 900;	// 不要文字読み込む
const int OBJ_DETA = 83;		// オブジェクトの読み込む数
const char* CHARACTER_INERTIA = "data\\FILE\\Object.txt";	// キャラクターの慣性
}

//=======================================
//objectXクラス
//=======================================
class CObjectX	: public CObject
{
public:
	CObjectX(int nPriority = 3);		//コンストラクタ
	~CObjectX();	//デストラクタ

	//メンバ関数
	HRESULT Init(void);		//初期化
	void Uninit(void);		//終了
	void Update(void);		//更新
	void Draw(void);		//描画
	void SetVertex(void);
	void SetSize(float fWidht, float fHeight);
	void SetPosition(D3DXVECTOR3 pos);
	void Load(void);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }		//位置取得
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; }	//モデルの最小値取得
	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; }	//モデルの最大値取得

private:
	//メンバ変数
	LPDIRECT3DTEXTURE9 m_pTexture;	//共有テクスチャ
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス
	LPD3DXMESH m_pMesh;				//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;		//マテリアルへのポインタ
	DWORD m_dwNumMat;				//マテリアルの数
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	D3DXVECTOR3 m_vtxMin, m_vtxMax;	//モデルの最大、最小値

	std::function<float(float,float)> m_funcTest;
};

#endif