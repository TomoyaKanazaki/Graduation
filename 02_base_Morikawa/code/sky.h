//========================================
//
// フィールド[sky.h]
// Author：相馬靜雅
//
//========================================
#ifndef _SKY_H_
#define _SKY_H_
#include "main.h"
#include "object3D.h"

//========================================
//ポリゴンクラス
//========================================
class CSky : public CObject3D
{
public:
	CSky(int nPriority = 0);		//コンストラクタ
	~CSky();	//デストラクタ

	//メンバ関数
	static CSky* Create(void);	//ポリゴン生成

	HRESULT Init(void);		//初期化
	void Uninit(void);		//終了
	void Update(void);		//更新
	void Draw(void);		//描画

	virtual void SetVertex3D() override;
private:
	
	float m_fTexU;
	//メンバ変数
	static 	LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ
};

#endif
