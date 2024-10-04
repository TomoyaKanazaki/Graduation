//========================================
//
// メイン[main.h]
// Author：森川駿弥
//
//========================================
#ifndef _MAIN_H_
#define _MAIN_H_

//========================================
//ライブラリのリンク
//========================================
#pragma comment(lib,"d3d9.lib")     //描画処理に必要
#pragma comment(lib,"d3dx9.lib")    //[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")   //DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")    //システム時刻取得に必要

//========================================
//マクロ定義
//========================================
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//========================================
// 定数定義
//========================================
namespace
{
const int SCREEN_WIDTH = 1280;	// ウィンドウの幅
const int SCREEN_HEIGHT = 720;	// ウィンドウの高さ
const int MAX_PARTS = 15;	// //プレイヤーモデル(パーツ)の最大数
}

//========================================
//頂点情報[2D]の構造体を定義
//========================================
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//座標変換用係数(1.0fで固定)
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

//=======================================
//頂点情報[3D]の構造体を定義
//=======================================
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_3D;

#endif