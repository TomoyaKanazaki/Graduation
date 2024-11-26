//========================================================================================
//
//　便利関数[useful.h]
//	Author:sakamoto kai
//
//========================================================================================
#ifndef _USEFUL_H_
#define _USEFUL_H_

//マクロ定義
#define SCREEN_WIDTH (1280)					//ウインドウの幅
#define SCREEN_WIDTHCENTER (SCREEN_WIDTH / 2)			//ウインドウの幅中央
#define SCREEN_HEIGHT (720)					//ウインドウの高さ
#define SCREEN_HEIGHTCENTER (SCREEN_HEIGHT / 2)			//ウインドウの高さ中央
#define SCREEN_WIDTH_F (1280.0f)					//ウインドウの幅
#define SCREEN_WIDTHCENTER_F (SCREEN_WIDTH * 0.5f)			//ウインドウの幅中央
#define SCREEN_HEIGHT_F (720.0f)					//ウインドウの高さ
#define SCREEN_HEIGHTCENTER_F (SCREEN_HEIGHT * 0.5f)			//ウインドウの高さ中央
#define MY_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					//頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		//頂点フォーマット
#define INITVECTOR3 (D3DXVECTOR3(0.0f,0.0f,0.0f))		//座標のデフォルト
#define SCREEN_CENTER (D3DXVECTOR3(SCREEN_WIDTHCENTER,SCREEN_HEIGHTCENTER,0.0f))		//スクリーン座標の中心
#define SCREEN_CENTER_F (D3DXVECTOR3(SCREEN_WIDTHCENTER_F, SCREEN_HEIGHTCENTER_F, 0.0f))		//スクリーン座標の中心

//頂点構造体[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//座標変換用係数(1.0fで固定)
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

//頂点構造体[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_3D;

int GetFps(void);

// メモリ開放マクロ
#define SAFE_UNINIT(p)		if ((p) != nullptr) { (p)->Uninit();		(p) = nullptr; }	// Uninit関数の破棄マクロ
#define SAFE_FREE(p)		if ((p) != nullptr) { free((p));			(p) = nullptr; }	// free関数の破棄マクロ
#define SAFE_RELEASE(p)		if ((p) != nullptr) { (p)->Release();		(p) = nullptr; }	// Release関数の破棄マクロ
#define SAFE_DEL_OBJECT(p)	if ((p) != nullptr) { DeleteObject((p));	(p) = nullptr; }	// DeleteObject関数の破棄マクロ
#define SAFE_REF_RELEASE(p)	if ((p) != nullptr) { (p)->Release((p)); }						// 参照ポインタ付きRelease関数の破棄マクロ
#define SAFE_DELETE(p)		if ((p) != nullptr) { delete(p);			(p) = nullptr; }	// 配列を使用しないdeleteを使用する破棄マクロ
#define SAFE_DEL_ARRAY(p)	if ((p) != nullptr) { delete[](p);			(p) = nullptr; }	// 配列を使用したdelete[]を使用する破棄マクロ

// 使用頻度の高い関数マクロ
#define NUM_ARRAY(a)	(sizeof((a)) / sizeof((a)[0]))	// 配列の要素数計算

#define PRIORITY_MAX (8)	//描画順の最大数

// 名前空間
namespace useful
{
	//当たり判定の使用軸
	enum COLLISION
	{
		COLLISION_XY = 0,
		COLLISION_YZ,
		COLLISION_ZX,
		COLLISION_X,
		COLLISION_Y,
		COLLISION_Z,
		COLLISION_MAX,
	};

	bool MoveIF(float Move1, float Move2);	// 移動量を比較し移動量１より移動量２が早いときtrueを返す
	int RangeNumber(int Max, int Min, int Number);	// 数字を設定した範囲内に納める処理
	bool CollisionRectangle2D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 Size1, D3DXVECTOR3 Size2, useful::COLLISION Shaft);
	bool PointSquareXZ(D3DXVECTOR3 Point, D3DXVECTOR3 pos, D3DXVECTOR3 Size);		//点が四角の中に存在するかどうか
	bool PushSquareXZ(D3DXVECTOR3 PosMy, D3DXVECTOR3 SizeMy, D3DXVECTOR3 MoveMy, D3DXVECTOR3 PosPair, D3DXVECTOR3 PosOldPair, D3DXVECTOR3 SizePair, D3DXVECTOR3* pObjMove, useful::COLLISION XYZ);		//矩形の押し出し判定
	bool CameraFront(D3DXVECTOR3 pos);
	bool CollisionLine(D3DXVECTOR3 Mypos, D3DXVECTOR3 PairPos, float Angle);
	bool SlashCollision(D3DXVECTOR3* EnemyPos, int Max, float SlashRot);
	void CollisionWindow(D3DXVECTOR3* pos);
	bool CollisionWindow(D3DXVECTOR3 pos);
	bool CollisionCircle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float nRadiusOut);	//円の当たり判定
	bool CollisionRectangle(D3DXVECTOR3 posPair, D3DXVECTOR3 sizePair, D3DXVECTOR3 posMy, D3DXVECTOR3 sizeMy);
	bool CollisionBlock(D3DXVECTOR3 PosPair, D3DXVECTOR3 PosOldPair, D3DXVECTOR3 MovePair, D3DXVECTOR3 SizePair, D3DXVECTOR3* pPosMy, D3DXVECTOR3 pPosOldMy, D3DXVECTOR3* pMoveMy, D3DXVECTOR3* pObjMoveMy, D3DXVECTOR3 SizeMy, bool* bJump, useful::COLLISION XYZ);
	void NormalizeAngle(D3DXVECTOR3* rot);
	void NormalizeRot(float* rot);
	void Vec3Project(D3DXVECTOR3* pOut, D3DXVECTOR3 v);	// 2D座標に変換
	D3DXVECTOR3 Point2Rot(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);		//単位ベクトルを取得
	D3DXVECTOR3 CrossIntersection(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, float SeatchDistance);		//外積の判定の交点との距離を求める
	bool DecisionAbsolute(float fDataMy, float fDec);
	bool DecisionAbsolute(D3DXVECTOR3 dataMy, float fDec);
	float PosDirection(D3DXVECTOR3 MyPos, D3DXVECTOR3 PairPos, float ChageRot);		//引数の位置の方向を向く処理
	float DegreesToRadians(float degrees);			//度からラジアン変換

	D3DXVECTOR3 CalcMatrix(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXMATRIX& mtx);
	D3DXVECTOR3 CalcMatrixToRot(const D3DXMATRIX& mtx);
}

#endif