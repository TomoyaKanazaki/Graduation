//========================================================================================
//
// マップシステム[MapSystem.cpp]
// Author: 坂本　翔唯
//
//========================================================================================
#include "MapSystem.h"
#include "renderer.h"
#include "game.h"
#include "Devil.h"

#include "AStar.h"
#include "CubeBlock.h"

// 定数定義
namespace
{
	float GRID_SIZE = 100.0f;	// グリッドのサイズ
	D3DXVECTOR3 MAP_SIZE = D3DXVECTOR3(750.0f, 0.0f, 550.0f);		// 横の当たり判定
}

//静的メンバ変数宣言
CMapSystem* CMapSystem::pMapSystem = nullptr;
bool CMapSystem::m_nMapGrit[NUM_WIGHT][NUM_HEIGHT] = {false};

//====================================================================
//コンストラクタ
//====================================================================
CMapSystem::CMapSystem()
{
	for (int nCntW = 0; nCntW < NUM_WIGHT; nCntW++)
	{
		for (int nCntH = 0; nCntH < NUM_HEIGHT; nCntH++)
		{
			m_nMapGrit[nCntW][nCntH] = false;
		}
	}
	m_WightMax = NUM_WIGHT;
	m_HeightMax = NUM_HEIGHT;
	m_fGritSize = GRID_SIZE;
	m_MapPos = D3DXVECTOR3((((m_WightMax * 0.5f) * -100.0f) + m_fGritSize * 0.5f), 0.0f, (((m_HeightMax * 0.5f) * 100.0f) - m_fGritSize * 0.5f));
	m_InitPos = m_MapPos;
	m_MapSize = MAP_SIZE;
	m_MapSize = D3DXVECTOR3((NUM_WIGHT - 1) * 50.0f, 0.0f, (NUM_HEIGHT - 1) * 50.0f);
}

//====================================================================
//デストラクタ
//====================================================================
CMapSystem::~CMapSystem()
{

}

//====================================================================
//インスタンス取得
//====================================================================
CMapSystem* CMapSystem::GetInstance(void)
{
	if (pMapSystem == nullptr)
	{
		pMapSystem = new CMapSystem;
	}
	return pMapSystem;
}

//====================================================================
//マップシステムの初期化処理
//====================================================================
void CMapSystem::Init()
{
	for (int nCntW = 0; nCntW < NUM_WIGHT; nCntW++)
	{
		for (int nCntH = 0; nCntH < NUM_HEIGHT; nCntH++)
		{
			pMapSystem->m_nMapGrit[nCntW][nCntH] = false;
		}
	}

	m_MapPos = D3DXVECTOR3((((m_WightMax * 0.5f) * -100.0f) + m_fGritSize * 0.5f), 0.0f, (((m_HeightMax * 0.5f) * 100.0f) - m_fGritSize * 0.5f));
	m_InitPos = m_MapPos;

	// 経路探索用の情報を取得
	AStar::Generator::Create();
	auto generator = AStar::Generator::GetInstance();

	// 経路探索用情報の設定
	generator->setWorldSize({ NUM_WIGHT, NUM_HEIGHT }); // 世界の大きさ
	generator->setHeuristic(AStar::Heuristic::euclidean); // 最短ルート計測の種類
	generator->setDiagonalMovement(false); // 斜め移動をオフ
}

//====================================================================
//マップシステムの終了処理
//====================================================================
void CMapSystem::Uninit(void)
{
	if (pMapSystem != nullptr)
	{
		delete pMapSystem;
		pMapSystem = nullptr;
	}

	delete AStar::Generator::GetInstance();
}

//====================================================================
//マップシステムの更新処理
//====================================================================
void CMapSystem::Update(void)
{

}

//====================================================================
//マップシステムの描画処理
//====================================================================
void CMapSystem::Draw(void)
{

}

//====================================================================
// グリットの開始位置取得
//====================================================================
D3DXVECTOR3 CMapSystem::GetStartGritPos(float Wight, float Height)
{
	D3DXVECTOR3 Pos;

	if (Wight < 0 || Wight >= (float)m_WightMax ||
		Height < 0 || Height >= (float)m_HeightMax)
	{
		return Pos;
	}

	//　グリットの位置にエフェクトを表示
	Pos.x = m_InitPos.x + (Wight * m_fGritSize);
	Pos.y = 0.0f;
	Pos.z = m_InitPos.z - (Height * m_fGritSize);

	return Pos;
}

//====================================================================
// グリットの位置取得
//====================================================================
D3DXVECTOR3 CMapSystem::GetGritPos(const GRID& grid)
{
	D3DXVECTOR3 Pos;

	D3DXVECTOR3 DevilPos = CGame::GetDevil()->GetDevilPos();

	// グリット番号が最大値以上や最小値以下の時、範囲内に納める処理
	CMapSystem::GRID temp = grid;
	temp.x = useful::RangeNumber(m_WightMax, 0, grid.x);
	temp.z = useful::RangeNumber(m_HeightMax, 0, grid.z);

	// グリットの横番号の位置を設定する
	Pos.x = m_MapPos.x + (temp.x * m_fGritSize);

	//境界線の外側にグリットがある場合反対側に移動させる
	if (Pos.x > DevilPos.x + (m_MapSize.x))
	{
		Pos.x = Pos.x - (DevilPos.x + (m_MapSize.x * 2.0f)) - m_fGritSize;
	}

	// 高さの位置を設定する
	Pos.y = 0.0f;

	// グリットの縦番号の位置を設定する
	Pos.z = m_MapPos.z - (temp.z * m_fGritSize);

	//境界線の外側にグリットがある場合反対側に移動させる
	if (Pos.z < DevilPos.z - (m_MapSize.z))
	{
		Pos.z = Pos.z + (DevilPos.z + (m_MapSize.z * 2.0f)) + m_fGritSize;
	}

	return Pos;
}

//==========================================
//  グリッド座標の算出
//==========================================
CMapSystem::GRID CMapSystem::CalcGrid(const D3DXVECTOR3& pos)
{
	// 演算用変数
	GRID grid = GRID(-1, -1);
	CDevil* pDevil = CGame::GetDevil();
	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();

	// x座標の算出
	for (int i = 0; i < m_WightMax; i++)
	{
		float fCountPosX = m_MapPos.x + (i * m_fGritSize);

		if (fCountPosX > DevilPos.x + (m_MapSize.x))
		{
			fCountPosX = fCountPosX - (DevilPos.x + (m_MapSize.x * 2.0f)) - m_fGritSize;
		}

		if (pos.x < fCountPosX + (m_fGritSize * 0.5f) &&
			pos.x >= fCountPosX - (m_fGritSize * 0.5f))
		{
			grid.x = i;
		}
	}

	// z座標の算出
	for (int i = 0; i < m_HeightMax; i++)
	{
		float fCountPosZ = m_MapPos.z - (i * m_fGritSize);

		if (fCountPosZ < DevilPos.z - (m_MapSize.z))
		{
			fCountPosZ = fCountPosZ + (DevilPos.z + (m_MapSize.z * 2.0f)) + m_fGritSize;
		}

		if (pos.z < fCountPosZ + (m_fGritSize * 0.5f) &&
			pos.z >= fCountPosZ - (m_fGritSize * 0.5f))
		{
			grid.z = i;
		}
	}

	return grid;
}

//==========================================
//  マップ情報の読み込み
//==========================================
void CMapSystem::Load(const char* pFilename)
{
	// TODO : csv対応したい

	// 経路探索用の情報を取得
	auto generator = AStar::Generator::GetInstance();
	if (generator == nullptr)
	{
		assert(false);
		generator = AStar::Generator::Create();
	}

		//ファイルを開く
	FILE* pFile = fopen(pFilename, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		char Getoff[32] = {};
		char boolLife[32] = {};
		char aString[128] = {};			//ゴミ箱
		char aStartMessage[32] = {};	//スタートメッセージ
		char aSetMessage[32] = {};		//セットメッセージ
		char aEndMessage[32] = {};		//終了メッセージ

		fscanf(pFile, "%s", &aStartMessage[0]);
		if (strcmp(&aStartMessage[0], "STARTSETSTAGE") == 0)
		{
			CMapSystem* pMapSystem = CMapSystem::GetInstance();
			D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();
			float MapSystemGritSize = pMapSystem->GetGritSize() * 0.5f;

			while (1)
			{
				fscanf(pFile, "%s", &aSetMessage[0]);
				if (strcmp(&aSetMessage[0], "STARTSETBLOCK") == 0)
				{
					int WightNumber, HeightNumber;

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &WightNumber);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &HeightNumber);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%s", &aString[0]);

					pMapSystem->SetGritBool(WightNumber, HeightNumber, true);
					CCubeBlock* pBlock = CCubeBlock::Create();
					pBlock->SetWightNumber(WightNumber);
					pBlock->SetHeightNumber(HeightNumber);
					pBlock->SetPos(D3DXVECTOR3(MapSystemPos.x + WightNumber * 100.0f, 50.0f, MapSystemPos.z - HeightNumber * 100.0f));
					pBlock->SetSize(D3DXVECTOR3(MapSystemGritSize, MapSystemGritSize, MapSystemGritSize));
					pBlock->SetTexture(&aString[0]);

					// 経路探索用情報の設定
					generator->addCollision({ WightNumber, HeightNumber }); // 通過不可地点を追加

					fscanf(pFile, "%s", &aEndMessage[0]);
				}
				else if (strcmp(&aSetMessage[0], "ENDSETSTAGE") == 0)
				{
					break;
				}
			}
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}

}

//==========================================
//  グリッドを算出
//==========================================
int CMapSystem::CalcGridX(const float posX)
{
	// 算出に使用する変数
	CDevil* pDevil = CGame::GetDevil();
	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();

	// x座標の算出
	for (int i = 0; i < m_WightMax; i++)
	{
		float fCountPosX = m_MapPos.x + (i * m_fGritSize);

		if (fCountPosX > DevilPos.x + (m_MapSize.x))
		{
			fCountPosX = fCountPosX - (DevilPos.x + (m_MapSize.x * 2.0f)) - m_fGritSize;
		}

		if (posX < fCountPosX + (m_fGritSize * 0.5f) &&
			posX >= fCountPosX - (m_fGritSize * 0.5f))
		{
			return i;
		}
	}

	// グリット外なら-1を返す
	return -1;
}

//==========================================
//  グリッドを算出
//==========================================
int CMapSystem::CalcGridZ(const float posZ)
{
	// 算出に使用する変数
	CDevil* pDevil = CGame::GetDevil();
	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();

	for (int i = 0; i < m_HeightMax; i++)
	{
		float fCountPosZ = m_MapPos.z - (i * m_fGritSize);

		if (fCountPosZ < DevilPos.z - (m_MapSize.z))
		{
			fCountPosZ = fCountPosZ + (DevilPos.z + (m_MapSize.z * 2.0f)) + m_fGritSize;
		}

		if (posZ < fCountPosZ + (m_fGritSize * 0.5f) &&
			posZ >= fCountPosZ - (m_fGritSize * 0.5f))
		{
			return i;
		}
	}

	// グリット外なら-1を返す
	return -1;
}

//==========================================
//  代入演算子のオペレータ
//==========================================
CMapSystem::GRID& CMapSystem::GRID::operator=(const AStar::Vec2i& vec)
{
	x = vec.x;
	z = vec.y;
	return *this;
}

//==========================================
//  グリッド同士の比較演算子
//==========================================
bool CMapSystem::GRID::operator==(const GRID& grid)
{
	return (x == grid.x && z == grid.z);
}

//==========================================
//  グリッド同士の比較演算子
//==========================================
bool CMapSystem::GRID::operator!=(const GRID& grid)
{
	return (x != grid.x || z != grid.z);
}
