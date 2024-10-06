#pragma once
//==============================================================
//
//プレイヤーのテスト処理[tutorial_texture.h]
//Author:佐藤根詩音
//
//==============================================================

#include "object3D.h"

class CTutorialUI;

//チュートリアルUIクラスの定義
class CTestLine : public CObject3D
{
public:

	//チュートリアルのステップ
	enum STEP
	{
		STEP_MOVE = 0,		//移動
		STEP_JUMP,			//ジャンプ
		STEP_MAX
	};

	CTestLine();		//コンストラクタ
	CTestLine(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//コンストラクタ(オーバーロード)
	~CTestLine();		//デストラクタ

	static CTestLine* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//生成処理

	HRESULT Init(void);
	void UnInit(void);
	void Update(void);
	void Draw(void);

	void SetLinePos(const D3DXVECTOR3& pos) { m_pos = pos; CObject3D::SetPosition(m_pos); }

private:

	static LPDIRECT3DTEXTURE9 m_pTexture;
	static const char* m_apFilename[STEP_MAX];		//テクスチャの名前
	int m_nIdxTexture;		//テクスチャの番号

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_posOld;	//前回の位置

};
