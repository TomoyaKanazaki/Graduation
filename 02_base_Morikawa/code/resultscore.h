#pragma once
//==========================================
//
//  タイマークラス(resultscore.h)
//  Author : 相馬靜雅
//
//==========================================
#include "object.h"

//==========================================
//  前方宣言
//==========================================
class CNumber;

//==========================================
//  クラス定義
//==========================================
class CResultScore : public CObject
{
public:

	CResultScore();
	~CResultScore();

	HRESULT Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	static CResultScore* Create(const MyLib::Vector3& pos, float time, float size);

private:

	// メンバ関数
	void CalcNum(); // 計算

	// メンバ変数
	float m_fTimer; // 現在の時間
	float m_fSize;	// サイズ
	CNumber* m_pNumber[5]; // 数字

};
