//========================================================================================
//
// マップシステム[MapSystem.cpp]
// Author: 坂本　翔唯
//
//========================================================================================
#include "MapSystem.h"
#include "renderer.h"
#include "game.h"
#include "tutorial.h"
#include "Devil.h"

#include "AStar.h"
#include "CubeBlock.h"
#include "tile.h"
#include "wall.h"
#include "item.h"
#include "DevilHole.h"
#include "RailBlock.h"
#include "enemy.h"
#include "RollRock.h"
#include "objmeshField.h"

// 定数定義
namespace
{
	float GRID_SIZE = 100.0f;	// グリッドのサイズ
	D3DXVECTOR3 MAP_SIZE = D3DXVECTOR3(750.0f, 0.0f, 550.0f);		// 横の当たり判定
	int BOWABOWA_RATE = 5; // ボワボワの生成率 ( 0以下でエラー )
}

//静的メンバ変数宣言
CMapSystem* CMapSystem::m_pMapSystem = nullptr;
bool CMapSystem::m_nMapGrit[NUM_WIGHT][NUM_HEIGHT] = {false};
std::vector<std::tuple<>> CMapSystem::m_nData = {};	// 複数の値を保持
std::vector<CMapSystem::GRID> CMapSystem::m_PosPlayer = {};	// プレイヤーの位置を保持

//====================================================================
//コンストラクタ
//====================================================================
CMapSystem::CMapSystem() : 
	m_mapCenter(0, 0)
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
	m_MapPos = D3DXVECTOR3((((NUM_WIGHT * 0.5f) * -GRID_SIZE) + GRID_SIZE * 0.5f), 0.0f, (((NUM_HEIGHT * 0.5f) * GRID_SIZE) - GRID_SIZE * 0.5f));
	m_InitPos = m_MapPos;
	m_MapSize = MAP_SIZE;
	m_MapSize = D3DXVECTOR3((NUM_WIGHT - 1) * 50.0f, 0.0f, (NUM_HEIGHT - 1) * 50.0f);
	//m_MapType = MAPTYPE_NONE;			// マップオブジェクトの種類
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
	if (m_pMapSystem == nullptr)
	{
		m_pMapSystem = new CMapSystem;
		m_pMapSystem->Init();
	}
	return m_pMapSystem;
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
			m_pMapSystem->m_nMapGrit[nCntW][nCntH] = false;
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

	// 中心を設定
	m_mapCenter = GRID(NUM_WIGHT / 2, NUM_HEIGHT / 2);
}

//====================================================================
//マップシステムの終了処理
//====================================================================
void CMapSystem::Uninit(void)
{
	if (m_pMapSystem != nullptr)
	{
		delete m_pMapSystem;
		m_pMapSystem = nullptr;
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
	D3DXVECTOR3 DevilPos;

	DevilPos = CDevil::GetListTop()->GetDevilPos();

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

	CDevil* pDevil = nullptr;

	pDevil = CDevil::GetListTop();

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
//  １マスのサイズを取得
//==========================================
float CMapSystem::GetGritSize(void)
{
	return GRID_SIZE;
}

//==========================================
// プレイヤーの位置取得
//==========================================
D3DXVECTOR3 CMapSystem::GetPlayerPos(unsigned int PlayNumber)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置

	// 位置
	if (PlayNumber < m_PosPlayer.size())
	{ // 読み込んだ数以内だったら
		
		// 座標入れる
		pos = m_PosPlayer[PlayNumber].ToWorld();
	}
	return pos;
}

//==========================================
//  マップ情報の読み込み
//==========================================
void CMapSystem::Load(const char* pFilename)
{
	// 経路探索用の情報を取得
	auto generator = AStar::Generator::GetInstance();
	if (generator == nullptr)
	{
		assert(false);
		generator = AStar::Generator::Create();
	}

	// マップシステムの情報
	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	float fMapSystemGritSize = pMapSystem->GetGritSize() * 0.5f;

	// 読み込み用
	D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f,0.0f,0.0f);	// グリッド生成位置
	D3DXVECTOR3 posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// グリッド開始位置
	D3DXVECTOR2 charOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// グリッドのオフセット
	D3DXVECTOR3 size = D3DXVECTOR3(fMapSystemGritSize, 0.0f, fMapSystemGritSize);		// グリッドサイズ
	GRID MaxGrid;		// グリッドの最大数

	// グリッド設定の判定
	bool bGridSet = false;

	// ファイルを開く
	std::ifstream file(pFilename);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ブロックセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);
		return;
	}

	// ファイルを読込
	std::string str;	// 読込文字列
	while (std::getline(file, str))
	{ // ファイルの終端ではない場合ループ

		// カンマ区切りごとにデータを読込
		std::istringstream iss(str);	// 文字列ストリーム
		while (std::getline(iss, str, ','))
		{
			if (str == "START_POS")
			{
				// 開始位置を読込
				iss >> posStart.x >> posStart.y >> posStart.z;

				// 開始位置を生成位置に設定
				posOffset = posStart;
				pMapSystem->m_MapPos = posStart;		// マップの位置に設定
			}
			else if (str == "NUM_GRID")
			{
				// グリッドの行列数を読み込み
				iss >> MaxGrid.x >> MaxGrid.z;

				CObjmeshField* map = nullptr;
				//床の生成
				if (CScene::GetMode() == CScene::MODE_GAME)
				{
					CGame::GetInstance()->SetMapField(CObjmeshField::Create(MaxGrid));
					map = CGame::GetInstance()->GetMapField();
				}
				else if (CScene::GetMode() == CScene::MODE_TUTORIAL)
				{
					CTutorial::GetInstance()->SetMapField(CObjmeshField::Create(MaxGrid));
					map = CTutorial::GetInstance()->GetMapField();
				}
				map->SetPos(INITVECTOR3);
				map->SetAlpha(0.0f); // 描画をオフ

				// 経路探索用情報の設定
				generator->setWorldSize(MaxGrid.ToAStar()); // 世界の大きさ
			}

			else if (str == "STARTSETSTAGE")
			{
				while (std::getline(file, str))
				{ // ファイルの終端ではない場合ループ

					// 終端の場合ステージ生成を抜ける
					if (str == "ENDSETSTAGE") { break; }

					for (int nCntHeight = 0; nCntHeight < MaxGrid.z; nCntHeight++)
					{ // 列カウント

						// 横一行分の配列を拡張
						m_nData.emplace_back();

						// カンマ区切りごとにデータを読込
						std::istringstream issChar(str);	// 文字列ストリーム

						for (int nCntWidth = 0; nCntWidth < MaxGrid.x; nCntWidth++)
						{ // 行カウント

							// 1行ずつ読み込み
							std::getline(issChar, str, ',');

							// 行列数設定
							GRID grid = GRID(nCntWidth, nCntHeight);
							if (str == "") { continue; }	// 空白は無視する
							else if (str == "1")
							{ // 壁の場合

								// 壁モデルの生成
								CWall::Create(grid);

								// グリッド判定の設定
								pMapSystem->SetGritBool(grid.x, grid.z, true);

								// 経路探索用情報の設定
								generator->addCollision(grid.ToAStar()); // 通過不可地点を追加

								continue;
							}

							// 床モデルの生成
							CTile::Create(grid);

							// グリッド設定の判定
							bGridSet = false;

							// オブジェクトを設置
							if (str == "2")
							{ // 十字架

								// 十字架の生成
								CItem::Create(CItem::TYPE_CROSS, grid);
							}
							else if (str == "3")
							{ // デビルホールの生成範囲

								// グリッド設定の判定
								bGridSet = true;

								// 経路探索用情報の設定
								generator->addCollision(grid.ToAStar()); // 通過不可地点を追加
							}
							else if (str == "4")
							{ // デビルホール

								// デビルホールの生成
								CDevilHole::Create(grid);

								// マップの中心に設定
								pMapSystem->m_mapCenter = grid;

								// グリッド設定の判定
								bGridSet = true;

								// 経路探索用情報の設定
								generator->addCollision(grid.ToAStar()); // 通過不可地点を追加
							}
							else if (str == "5")
							{ // レールブロック

								// レールブロックの生成
								CRailBlock::Create(grid);

								// グリッド設定の判定
								bGridSet = true;

								// 経路探索用情報の設定
								generator->addCollision(grid.ToAStar()); // 通過不可地点を追加
							}
							else if (str == "6")
							{ // プレイヤー

								// プレイヤーのグリッド位置
								pMapSystem->m_PosPlayer.push_back(grid);
							}
							else if (str == "7")
							{ // 敵(メダマン)

								// メダマン生成
								CEnemy::Create(CEnemy::ENEMY_MEDAMAN, grid);

							}
							else if (str == "8")
							{ // 敵(ボンボン)

								// ボンボン生成
								CEnemy::Create(CEnemy::ENEMY_BONBON, grid);

							}
							else if (str == "9")
							{ // 敵(子デビル)

								// 子デビル生成
								CEnemy::Create(CEnemy::ENEMY_LITTLEDEVIL, grid);

							}
							else if (str == "10")
							{ // 転がる岩

								// 転がる岩生成
								CRollRock::Create(grid);

								// グリッド設定の判定
								bGridSet = true;

								// 経路探索用情報の設定
								generator->addCollision(grid.ToAStar()); // 通過不可地点を追加
							}
							else
							{ // ボワボワの生成
								if (BOWABOWA_RATE <= 0) { assert(false); }

								// ランダム生成
								if (!(rand() % BOWABOWA_RATE))
								{
									CItem::Create(CItem::TYPE_BOWABOWA, grid);
								}
							}

							// グリッド判定の設定
							pMapSystem->SetGritBool(grid, bGridSet);

						}

						// 次の行読み込み
						std::getline(file, str);
					}

				}
			}
		}
	}

	// ファイルを閉じる
	file.close();
}

//==========================================
//  グリッドを算出
//==========================================
int CMapSystem::CalcGridX(const float posX)
{
	// 算出に使用する変数
	CDevil* pDevil = nullptr;

	pDevil = CDevil::GetListTop();

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
	CDevil* pDevil = nullptr;

	pDevil = CDevil::GetListTop();

	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();

	for (int i = 0; i < m_HeightMax; i++)
	{
		float fCountPosZ = m_MapPos.z - (i * m_fGritSize);

		if (fCountPosZ < DevilPos.z - (m_MapSize.z))
		{
			fCountPosZ = fCountPosZ + (DevilPos.z + (m_MapSize.z * 2.0f)) + m_fGritSize;
		}

		if (posZ <= fCountPosZ + (m_fGritSize * 0.5f) &&
			posZ > fCountPosZ - (m_fGritSize * 0.5f))
		{
			return i;
		}
	}

	// グリット外なら-1を返す
	return -1;
}

//==========================================
//  グリッド座標を世界の座標に変換する
//==========================================
D3DXVECTOR3 CMapSystem::GRID::ToWorld()
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 DevilPos = CDevil::GetListTop()->GetDevilPos();
	CMapSystem* map = GetInstance();

	// グリット番号が最大値以上や最小値以下の時、範囲内に納める処理
	CMapSystem::GRID temp = GRID(x, z);
	temp.x = useful::RangeNumber(map->m_WightMax, 0, x);
	temp.z = useful::RangeNumber(map->m_HeightMax, 0, z);

	// グリットの横番号の位置を設定する
	pos.x = map->m_MapPos.x + (temp.x * map->m_fGritSize);

	//境界線の外側にグリットがある場合反対側に移動させる
	if (pos.x > DevilPos.x + (map->m_MapSize.x))
	{
		pos.x = pos.x - (DevilPos.x + (map->m_MapSize.x * 2.0f)) - map->m_fGritSize;
	}

	// 高さの位置を設定する
	pos.y = 0.0f;

	// グリットの縦番号の位置を設定する
	pos.z = map->m_MapPos.z - (temp.z * map->m_fGritSize);

	//境界線の外側にグリットがある場合反対側に移動させる
	if (pos.z < DevilPos.z - (map->m_MapSize.z))
	{
		pos.z = pos.z + (DevilPos.z + (map->m_MapSize.z * 2.0f)) + map->m_fGritSize;
	}

	return pos;
}

//==========================================
//  A*用の座標系に変換する
//==========================================
AStar::Vec2i CMapSystem::GRID::ToAStar()
{
	// 変数宣言
	AStar::Vec2i vec;

	// 値を代入
	vec.x = x;
	vec.y = z;

	return vec;
}

//==========================================
//  グリッド座標に変換する
//==========================================
CMapSystem::GRID CMapSystem::GRID::ToGrid(const D3DXVECTOR3& pos)
{
	return CMapSystem::GetInstance()->CalcGrid(pos);
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
