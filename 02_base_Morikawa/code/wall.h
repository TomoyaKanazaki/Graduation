//========================================
//
// ウォール[wall.h]
// Author：森川駿弥
//
//========================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"
#include "object3D.h"
#include "object2D.h"

//========================================
// ウォールクラス
//========================================
class CWall : public CObject3D
{
public:
	CWall(int nPriority = 0);		//コンストラクタ
	~CWall();	//デストラクタ

	//メンバ関数
	static CWall* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetVertex3D(void) override;

private:
	//メンバ変数
	float m_aTexU;			//テクスチャのU値

	static 	LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャのポインタ
};

#endif
