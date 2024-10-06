//============================================
//
//	ブロック全般のマネージャー [blockmanager.cpp]
//	Author:nakamura riku
//
//============================================
#include "blockmanager.h"
#include "block.h"
#include "Player2D.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"

// マクロ定義

// 静的メンバ変数宣言
CBlockManager* CBlockManager::pBlockManager = nullptr;

//====================================================================
// コンストラクタ
//====================================================================
CBlockManager::CBlockManager()
{
	listBlock.clear();
	m_nBlockCounter = 0;
	m_nBlockLevel = 0;
	m_nBlockLevelCount = 0;
}

//====================================================================
// デストラクタ
//====================================================================
CBlockManager::~CBlockManager()
{

}

//====================================================================
// 生成処理
//====================================================================
CBlockManager* CBlockManager::GetInstance()
{
	if (pBlockManager == nullptr)
	{
		pBlockManager = new CBlockManager;
	}
	return pBlockManager;
}

//====================================================================
// 初期化処理
//====================================================================
HRESULT CBlockManager::Init(void)
{
	listBlock.clear();
	return S_OK;
}

//====================================================================
// 終了処理
//====================================================================
void CBlockManager::Uninit(void)
{
	listBlock.clear();
}

//====================================================================
// 更新処理
//====================================================================
void CBlockManager::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
		GameUpdate();
		break;

	case CScene::MODE_TUTORIAL:
		TutorialUpdate();
		break;

	case CScene::MODE_RESULT:
		listBlock.clear();
		m_nBlockRandom = 1450;
		break;
	}
}

//====================================================================
// タイトルでの更新処理
//====================================================================
void CBlockManager::TitleUpdate(void)
{
	listBlock.clear();
}

//====================================================================
// ゲームでの更新処理
//====================================================================
void CBlockManager::GameUpdate(void)
{
	if (!CManager::GetInstance()->GetPause())
	{
		SetStage();
		
		// 各ブロックの更新
		for (auto itrBlock : listBlock)
		{
			// まだ破棄されていないなら更新
			if (itrBlock->GetDeathFlag() != true)
			{
				//itrBlock->Update();
			}
		}
	}

	// 各ブロックが破棄予定ならリストから消去
	std::list<CBlock*>  listBlockDef = listBlock;
	for (auto itrBlock : listBlock)
	{
		if (itrBlock->GetDeathFlag())
		{
			listBlockDef.remove(itrBlock);
		}
	}
	listBlock = listBlockDef;
}

//====================================================================
// チュートリアルでの更新処理
//====================================================================
void CBlockManager::TutorialUpdate(void)
{

}

//====================================================================
// 描画処理
//====================================================================
void CBlockManager::Draw(void)
{
	
}

//====================================================================
// 当たり判定処理
//====================================================================
void CBlockManager::Collision(CPlayer2D* pPlayer2D)
{
	// 各ブロックの更新
	for (auto itrBlock : listBlock)
	{
		// まだ破棄されていないなら更新
		if (itrBlock->GetDeathFlag() != true)
		{
			itrBlock->Collision(pPlayer2D);
		}
	}
}

//====================================================================
// ステージの配置処理
//====================================================================
void  CBlockManager::SetStage(void)
{
	if (!CManager::GetInstance()->GetStop())
	{
		m_nBlockCounter++;

		if (m_nBlockCounter > m_nBlockRandom)
		{
			m_nBlockLevelCount++;

			if (m_nBlockLevelCount % 15 == 0)
			{
				m_nBlockLevel++;
			}

			int blockPosX = 0;
			while (1)
			{
				blockPosX = (int)(rand() % 3) - 1;

				if (m_nBlockRandomPos != blockPosX)
					break;
			}
			m_nBlockRandomPos = blockPosX;

			int blockWight = (rand() % 5);

			CBlock* pBlock = nullptr;

			switch (m_nBlockLevel)
			{
			case 0:
				pBlock = CBlockBase::Create(D3DXVECTOR3(640.0f + ((float)blockPosX * 200.0f), 800.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 100.0f + (50.0f + (float)blockWight * 35.0f), 25.0f);
				m_nBlockRandom = 45 - m_nBlockLevel * 3; // 頻度
				break;
			case 1:
				pBlock = CBlockBase::Create(D3DXVECTOR3(640.0f + ((float)blockPosX * 200.0f), 800.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f - m_nBlockLevel * 0.25f, 0.0f), 100.0f + (10.0f + (float)blockWight * 50.0f), 25.0f);
				m_nBlockRandom = 45 - m_nBlockLevel * 3; // 頻度
				break;
			case 2:
				pBlock = CBlockBase::Create(D3DXVECTOR3(640.0f + ((float)blockPosX * 200.0f), 800.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f - m_nBlockLevel * 0.25f, 0.0f), 100.0f + (10.0f + (float)blockWight * 45.0f), 25.0f);
				m_nBlockRandom = 45 - m_nBlockLevel * 3; // 頻度
				break;
			case 3:
				pBlock = CBlockBase::Create(D3DXVECTOR3(640.0f + ((float)blockPosX * 200.0f), 800.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f - m_nBlockLevel * 0.25f, 0.0f), 100.0f + (10.0f + (float)blockWight * 40.0f), 25.0f);
				m_nBlockRandom = 45 - m_nBlockLevel * 3; // 頻度
				break;
			case 4:
				pBlock = CBlockBase::Create(D3DXVECTOR3(640.0f + ((float)blockPosX * 200.0f), 800.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f - m_nBlockLevel * 0.25f, 0.0f), 100.0f + (10.0f + (float)blockWight * 35.0f), 25.0f);
				m_nBlockRandom = 45 - m_nBlockLevel * 3; // 頻度
				break;
			case 5:
				pBlock = CBlockBase::Create(D3DXVECTOR3(640.0f + ((float)blockPosX * 200.0f), 800.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f - m_nBlockLevel * 0.25f, 0.0f), 100.0f + (10.0f + (float)blockWight * 30.0f), 25.0f);
				m_nBlockRandom = 45 - m_nBlockLevel * 3; // 頻度
				break;
			case 6:
				pBlock = CBlockBase::Create(D3DXVECTOR3(640.0f + ((float)blockPosX * 200.0f), 800.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f - m_nBlockLevel * 0.25f, 0.0f), 100.0f + (10.0f + (float)blockWight * 25.0f), 25.0f);
				m_nBlockRandom = 45 - m_nBlockLevel * 3; // 頻度
				break;
			case 7:
				pBlock = CBlockBase::Create(D3DXVECTOR3(640.0f + ((float)blockPosX * 200.0f), 800.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f - m_nBlockLevel * 0.25f, 0.0f), 100.0f + (10.0f + (float)blockWight * 20.0f), 25.0f);
				m_nBlockRandom = 23; // 頻度
				break;
			default:
				pBlock = CBlockBase::Create(D3DXVECTOR3(640.0f + ((float)blockPosX * 200.0f), 800.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f - m_nBlockLevel * 0.25f, 0.0f), 100.0f + (10.0f + (float)blockWight * 15.0f), 25.0f);
				break;
			}

			listBlock.push_back(pBlock);
			int RandSpike = rand() % 101;
			int RamdBane = rand() % 101;
			int Ran = rand() % 101;
			int RandPosX = rand() % 641;
			int RandPosX2 = rand() % 641;

			switch (m_nBlockLevel)
			{
			case 0:
				if (RandSpike <= 5)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX, 860.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 25.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick000.png");
				}

				break;
			case 1:
				if (RandSpike <= 10)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX, 860.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 25.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick000.png");
				}
				break;
			case 2:
				if (RandSpike <= 15)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX, 860.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 25.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick000.png");
				}
				break;
			case 3:
				if (RandSpike <= 20)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX, 860.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 25.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick000.png");
				}

				if (RamdBane <= 20)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX2, 860.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 50.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick001.png");
				}
				break;
			case 4:
				if (RandSpike <= 25)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX, 860.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 25.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick000.png");
				}

				if (RamdBane <= 25)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX2, 840.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 50.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick001.png");
				}
				break;
			case 5:
				if (RandSpike <= 30)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX, 860.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 25.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick000.png");
				}

				if (RamdBane <= 30)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX2, 840.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 50.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick001.png");
				}
				break;
			case 6:
				if (RandSpike <= 35)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX, 860.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 25.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick000.png");
				}

				if (RamdBane <= 35)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX2, 840.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 50.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick001.png");
				}
				break;
			case 7:
				if (RandSpike <= 40)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX, 860.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 25.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick000.png");
				}

				if (RamdBane <= 40)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX2, 840.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 50.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick001.png");
				}
				break;
			default:
				if (RandSpike <= 45)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX, 860.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 25.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick000.png");
				}

				if (RamdBane <= 45)
				{
					pBlock = CBlockSpike::Create(D3DXVECTOR3(320.0f + (float)RandPosX2, 840.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), 50.0f, 25.0f);
					pBlock->SetTexture("data\\TEXTURE\\HGS\\gimmick001.png");
				}
				break;
			}

			listBlock.push_back(pBlock);


			m_nBlockCounter = 0;
		}
	}
}

//====================================================================
// チュートリアルステージの配置処理
//====================================================================
void  CBlockManager::SetTutorial(void)
{
	CBlock* pBlock = CBlockTitle::Create(D3DXVECTOR3(350.0f, 350.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 400.0f, 200.0f);
		pBlock->SetTexture("data\\TEXTURE\\HGS\\Pause_logo.png");
	listBlock.push_back(pBlock);

	pBlock = CBlockTutorial::Create(D3DXVECTOR3(900.0f, 250.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 300.0f, 150.0f);
	listBlock.push_back(pBlock);

	pBlock = CBlockTutorial::Create(D3DXVECTOR3(940.0f, 650.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 400.0f, 200.0f);
	pBlock->SetTexture("data\\TEXTURE\\HGS\\tutorial002.png");
	listBlock.push_back(pBlock);

	pBlock = CBlockBase::Create(D3DXVECTOR3(350.0f, 750.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 300.0f, 50.0f);
	listBlock.push_back(pBlock);

	pBlock = CBlockTutorial::Create(D3DXVECTOR3(740.0f, 1200.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 700.0f, 200.0f);
	pBlock->SetTexture("data\\TEXTURE\\HGS\\tutorial001.png");
	listBlock.push_back(pBlock);

	pBlock = CBlockBase::Create(D3DXVECTOR3(540.0f, 1400.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 700.0f, 50.0f);
	listBlock.push_back(pBlock);

	pBlock = CBlockBase::Create(D3DXVECTOR3(740.0f, 1500.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 700.0f, 50.0f);
	listBlock.push_back(pBlock);

	pBlock = CBlockBase::Create(D3DXVECTOR3(540.0f, 1600.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 700.0f, 50.0f);
	listBlock.push_back(pBlock);
}