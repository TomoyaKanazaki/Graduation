//=======================================
//
//ステンシルバッファに必要な2Dマスク[mask.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _MASK_H_	 //このマクロ定義が定義されていなかったら
#define _MASK_H_	 //2連インクルード防止のマクロを定義する

#include "object2d.h"

//=======================================
// マスククラス
//=======================================
class CMask : public CObject2D
{
public:
	CMask(int nPriority = 6);	//コンストラクタ
	~CMask();	//デストラクタ

	static CMask* Create();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
	//メンバ変数
};

#endif
