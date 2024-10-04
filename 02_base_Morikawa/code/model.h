//========================================
//
//モデル[model.h]
//Author：森川駿弥
//
//========================================
#ifndef _MODEL_H_
#define _MODEL_H_
#include "main.h"
#include "object.h"

//=======================================
//モデルクラス
//=======================================
class CModel
{
public:
	enum HIERARCHY
	{// 階層持ってるか
		TYPE_HIERARCHY = 0,
		TYPE_NOT_HIERARCHY,
		TYPE_MAX
	};

	CModel();		//コンストラクタ
	~CModel();		//デストラクタ

	//メンバ関数
	HRESULT Init(const char *pFilename);		//初期化
	void Uninit(void);		//終了
	void Update(void);		//更新
	void Draw(void);		//描画

	void BindXData(int nIdxXFile);
	void BindTexture();	// テクスチャ割り当て

	static CModel *Create(const char *pFilename);	//オブジェクト生成
	void SetParent(CModel *pModel);	//親
	D3DXMATRIX GetMtxWorld(void);	//ワールドマトリックスの取得

	void SetVertex(void);
	void SetSize(float fWidht, float fHeight);
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void);

	//向き
	void SetRot(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetRot(void);

	//番号
	void SetIndex(CModel *pModel);

	//位置
	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void);

	//向き
	void SetRotate(D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotate(void);

	// スケール
	void SetScale(const MyLib::Vector3& scale) { m_scale = scale; }
	MyLib::Vector3 GetScale() { return m_scale; }

	// 階層
	void SetType(HIERARCHY type) { m_nType = type; }

private:
	//メンバ変数
	HIERARCHY m_nType;

	std::vector<int> m_nIdxTexture;	//共有テクスチャ
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス
	LPD3DXMESH m_pMesh;				//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;		//マテリアルへのポインタ
	DWORD m_dwNumMat;				//マテリアルの数
	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_rot;				//向き
	MyLib::Vector3 m_scale;			// スケール
	CModel *m_pParent;				//親モデルへのポインタ
	int m_nIdxXFile;				// Xファイルのインデックス番号

	D3DXVECTOR3 m_setPos;
	D3DXVECTOR3 m_setRot;
};

#endif