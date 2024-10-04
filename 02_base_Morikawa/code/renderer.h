//========================================
//
//シューティングアクション[renderer.h]
//Author：森川駿弥
//
//========================================
#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "main.h"

//========================================
//レンダラークラス
//========================================
class CRenderer
{
public:
	CRenderer();		//コンストラクタ
	~CRenderer();		//デストラクタ

	//メンバ関数
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void);

private:
	//メンバ変数
	LPDIRECT3D9 m_pD3D;					//Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;		//Direct3Dデバイスへのポインタ
};

#endif