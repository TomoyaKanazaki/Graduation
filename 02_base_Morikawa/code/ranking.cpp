//========================================
//
//  リザルト [ranking.h]
//  Author：相馬靜雅
//
//========================================
#include "ranking.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "texture.h"
#include "sound.h"
#include "timer.h"
#include "resultscore.h"
#include "calculation.h"

namespace
{
	const int NUM = 3;
	const std::string TEXTURE[] = 
		
	{
		"data\\TEXTURE\\1.png",
		"data\\TEXTURE\\2.png",
		"data\\TEXTURE\\3.png",
	};
}

//=======================================
//コンストラクタ
//=======================================
CRanking::CRanking() : 
m_bNewRecord(false)		// ニューレコード
{
}

//=======================================
//デストラクタ
//=======================================
CRanking::~CRanking()
{
}

//=======================================
//生成
//=======================================
CRanking* CRanking::Create(float nowScore)
{
	//タイトルのポインタ
	CRanking* pTutorial = nullptr;

	//インスタンス生成
	pTutorial = new CRanking;

	if (pTutorial != nullptr)
	{
		// 今回のスコア
		pTutorial->m_fNowScore = nowScore;
		pTutorial->Init();
	}

	//ポインタを返す
	return pTutorial;
}

//=======================================
//初期化
//=======================================
HRESULT CRanking::Init(void)
{
	
	Load();
	Sort();
	Save();

	for (int i = 0; i < 3; i++)
	{
		m_pRankNum[i] = CObject2D::Create();

		// テクスチャ設定
		int texID = CTexture::GetInstance()->Regist(TEXTURE[i]);
		m_pRankNum[i]->BindTexture(texID);

		// サイズ設定
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// 横幅を元にサイズ設定
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// 縦幅を元にサイズ設定
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, 180.0f);
#endif
		m_pRankNum[i]->SetSize(size.x, size.y);
		m_pRankNum[i]->SetSizeOrigin(size);

		// 位置
		m_pRankNum[i]->SetPos(MyLib::Vector3(200.0f + i * 400.0f, 480.0f, 0.0f));
	}

	for (int i = 0; i < 3; i++)
	{
		m_pScore[i] = CResultScore::Create(MyLib::Vector3(200.0f + i * 400.0f, 550.0f, 0.0f), m_fRanking[i], 50.0f);
	}

	//成功を返す
	return S_OK;
}

//=======================================
// 終了
//=======================================
void CRanking::Uninit(void)
{
}

//=======================================
// 更新
//=======================================
void CRanking::Update(void)
{
	
}

//=======================================
// 描画
//=======================================
void CRanking::Draw(void)
{
}

//=======================================
// 描画
//=======================================
void CRanking::Sort()
{
	// 降順処理
	for (int nCntData = 0; nCntData < NUM - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < NUM; nCntSort++)
		{
			if (m_fRanking[nCntData] < m_fRanking[nCntSort])
			{// 要素1より要素2が大きかったら

				int nTemp = m_fRanking[nCntData];
				m_fRanking[nCntData] = m_fRanking[nCntSort];
				m_fRanking[nCntSort] = nTemp;
			}
		}
	}

	// ランキング最大値よりも今回のが小さかったら
	if (m_fRanking[NUM - 1] < m_fNowScore)
	{// 入れ替え

		m_fRanking[NUM - 1] = m_fNowScore;	// 最小値と今回のデータ入れ替え
		m_bNewRecord = true;	// フラグON
	}

	// 降順処理
	for (int nCntData = 0; nCntData < NUM - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < NUM; nCntSort++)
		{
			if (m_fRanking[nCntData] < m_fRanking[nCntSort])
			{// 要素1より要素2が大きかったら

				int nTemp = m_fRanking[nCntData];
				m_fRanking[nCntData] = m_fRanking[nCntSort];
				m_fRanking[nCntSort] = nTemp;
			}
		}
	}
}

//=======================================
// セーブ
//=======================================
void CRanking::Save()
{
	// ファイルを開く
	std::ofstream File("data\\FILE\\ranking.bin", std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	// 書き込み
	File.write(reinterpret_cast<char*>(m_fRanking), sizeof(m_fRanking));

	// ファイルを閉じる
	File.close();
}

//=======================================
// ロード
//=======================================
void CRanking::Load()
{
	// ファイルを開く
	std::ifstream File("data\\FILE\\ranking.bin", std::ios::binary);
	if (!File.is_open()) {
		
		m_fRanking[0] = 20.0f;	// ランキング
		m_fRanking[1] = 15.0f;	// ランキング
		m_fRanking[2] = 10.0f;	// ランキング

		return;
	}

	// 書き込み
	File.read(reinterpret_cast<char*>(m_fRanking), sizeof(m_fRanking));

	// ファイルを閉じる
	File.close();
}

