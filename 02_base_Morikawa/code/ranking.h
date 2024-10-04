//========================================
//
//  リザルト [ranking.h]
//  Author：相馬靜雅
//
//========================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "manager.h"

//========================================
// 前方宣言
//========================================
class CObject2D;
class CRankingScore;
class CResultScore;

//========================================
// ランキングクラス
//========================================
class CRanking : public CScene
{
public:
	CRanking();	//コンストラクタ
	~CRanking();	//デストラクタ

	// メンバ関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CRanking* Create(float nowScore);

private:

	//=============================
	// メンバ関数
	//=============================
	void Save();	// セーブ
	void Load();	// ロード
	void Sort();	// ソート

	//=============================
	// メンバ変数
	//=============================
	float m_fNowScore;		// 今回のスコア
	float m_fRanking[3];	// ランキング
	bool m_bNewRecord;		// ニューレコード
	CObject2D* m_pRankNum[3];
	CResultScore* m_pScore[3];
};

#endif
