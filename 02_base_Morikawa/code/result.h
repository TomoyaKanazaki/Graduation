//========================================
//
//  リザルト [result.h]
//  Author：相馬靜雅
//
//========================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "manager.h"

//========================================
// 前方宣言
//========================================
class CObject2D;
class CResultScore;

//========================================
// リザルトクラス
//========================================
class CResult : public CScene
{
public:
	CResult();	//コンストラクタ
	~CResult();	//デストラクタ

	// メンバ関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CResult* Create();

private:

	CObject2D* m_pObj2D;	// 背景
	CResultScore* m_pScore;	// 今回のスコア
};

#endif
