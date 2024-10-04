//========================================
//
//シューティングアクション[number.h]
//Author：森川駿弥
//
//========================================
#ifndef _NUMBER_H_
#define _NUMBER_H_
#include "main.h"
#include "object2D.h"

//========================================
//ナンバークラス
//========================================
class CNumber : public CObject2D
{
public:
	CNumber();		//コンストラクタ
	~CNumber();		//デストラクタ

	HRESULT Init(void);		//初期化
	void Uninit(void);		//終了
	void Update(void);		//更新
	void Draw(void);		//描画

	void SetNumber(const int nNum) { m_nNumber = nNum; } // 数値の設定

	//メンバ関数
	static CNumber *Create(void);	//プレイヤー生成

private:

	// メンバ関数
	void CalcUV(); // テクスチャ座標の計算

	// メンバ変数
	int m_nNumber; // 自身の数字
};

#endif