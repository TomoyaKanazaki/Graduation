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
#include "devil.h"
#include "texture.h"

#include "AStar.h"
#include "tile.h"
#include "wall.h"
#include "item.h"
#include "DevilHole.h"
#include "RailBlock.h"
#include "enemy.h"
#include "RollRock.h"
#include "objmeshField.h"
#include "MapMove.h"
#include "RailManager.h"

#ifdef _DEBUG
#include "objmeshField.h"
#endif

// 定数定義
namespace
{
	float GRID_SIZE = 100.0f;	// グリッドのサイズ
	D3DXVECTOR3 MAP_SIZE = D3DXVECTOR3(750.0f, 0.0f, 550.0f);		// 横の当たり判定
	int BOWABOWA_RATE = 5; // ボワボワの生成率 ( 0以下でエラー )
	const D3DXVECTOR3 EFFECT_SIZE = { 103.5f, 25.0f, 67.5f }; // エフェクトの倍率
	const char* MAP_FILE = "data\\TXT\\STAGE\\stage.txt";		// マップファイル

}

//静的メンバ変数宣言
CMapSystem* CMapSystem::m_pMapSystem = nullptr;
bool CMapSystem::m_bMapGrit[NUM_WIGHT][NUM_HEIGHT] = {false};
bool CMapSystem::m_bMapRailGrit[NUM_WIGHT][NUM_HEIGHT] = { false };

std::vector<std::tuple<>> CMapSystem::m_nData = {};	// 複数の値を保持
//std::vector<CMapSystem::GRID> CMapSystem::m_PosPlayer = {};	// プレイヤーの位置を保持
std::vector<CMapSystem::MapInfo> CMapSystem::m_MapInfo = {};	// マップの情報

//====================================================================
//コンストラクタ
//====================================================================
CMapSystem::CMapSystem() : 
	m_mapCenter(0, 0),
	m_mtxStage(nullptr)
{
	for (int nCntW = 0; nCntW < NUM_WIGHT; nCntW++)
	{
		for (int nCntH = 0; nCntH < NUM_HEIGHT; nCntH++)
		{
			m_bMapGrit[nCntW][nCntH] = false;
		}
	}
	m_WightMax = NUM_WIGHT;
	m_HeightMax = NUM_HEIGHT;
	m_fGritSize = GRID_SIZE;
	m_MapPos = D3DXVECTOR3((((NUM_WIGHT * 0.5f) * -GRID_SIZE) + GRID_SIZE * 0.5f), 0.0f, (((NUM_HEIGHT * 0.5f) * GRID_SIZE) - GRID_SIZE * 0.5f));
	m_InitPos = m_MapPos;
	m_MapSize = MAP_SIZE;
	m_MapSize = D3DXVECTOR3((NUM_WIGHT - 1) * 50.0f, 0.0f, (NUM_HEIGHT - 1) * 50.0f);
	m_pMapMove = nullptr;
	m_nNumMap = -1;
	m_nSelectMap = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CMapSystem::~CMapSystem()
{
	m_MapInfo.clear();
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
			m_pMapSystem->m_bMapGrit[nCntW][nCntH] = false;
		}
	}

	if (m_pMapMove == nullptr)
	{
		m_pMapMove = CMapMove::Create();
	}

	/*m_MapPos = D3DXVECTOR3((((m_WightMax * 0.5f) * -100.0f) + m_fGritSize * 0.5f), 0.0f, (((m_HeightMax * 0.5f) * 100.0f) - m_fGritSize * 0.5f));
	m_InitPos = m_MapPos;*/

	m_MapPos = m_InitPos;

	// 経路探索用の情報を取得
	auto generator = AStar::Generator::Create();

	// 経路探索用情報の設定
	generator->setWorldSize({ NUM_WIGHT, NUM_HEIGHT }); // 世界の大きさ

	// 中心を設定
	m_mapCenter = GRID(NUM_WIGHT / 2, NUM_HEIGHT / 2);

	// 床のマトリックスを取得
	m_mtxStage = CObjmeshField::GetListTop()->GetMatrix();
}

//====================================================================
//マップシステムの終了処理
//====================================================================
void CMapSystem::Uninit(void)
{
	if (m_pMapMove != nullptr)
	{
		m_pMapMove->Uninit();
		m_pMapMove = nullptr;
	}

	if (m_pMapSystem != nullptr)
	{
		delete m_pMapSystem;
		m_pMapSystem = nullptr;
	}

	AStar::Generator::GetInstance()->Uninit();
}

//====================================================================
//マップシステムの更新処理
//====================================================================
void CMapSystem::Update(void)
{
	if (CManager::GetInstance()->GetPause() == false)
	{
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			if (CGame::GetInstance()->GetEvent() == false &&
				CGame::GetInstance()->GetTrans() == false &&
				m_pMapMove != nullptr)
			{
				//マップの動き設定
				m_pMapMove->Update();

				//その他オブジェクトのスクロール
				CObject::ScrollAll();
			}
		}
		else if (CScene::GetMode() == CScene::MODE_TUTORIAL)
		{
			if (m_pMapMove != nullptr)
			{
				//マップの動き設定
				m_pMapMove->Update();

				//その他オブジェクトのスクロール
				CObject::ScrollAll();
			}
		}
	}

#ifdef _DEBUG
#if 0

	// デバッグ表示
	for (int i = 0; i < m_MapGrid.x; ++i)
	{
		for (int j = 0; j < m_MapGrid.z; ++j)
		{
			if (!m_bMapGrit[i][j]) { continue; }
			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_TRUE, false, useful::CalcMatrix(GRID(i, j).ToWorld(), INITVECTOR3, *CObjmeshField::GetListTop()->GetMatrix()), INITVECTOR3, {10.0f, 10.0f, 10.0f});
		}
	}

#endif // 0 or 1
#endif // _DEBUG
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

	DevilPos = CMapSystem::GetInstance()->GetMove()->GetDevilPos();

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
//  グリッドの移動可能フラグを設定
//==========================================
void CMapSystem::SetGritBool(int nWight, int nHeight, bool Set)
{
	if (nWight < 0 || nHeight < 0) { return; }
	if (nWight >= m_MapGrid.x || nHeight >= m_MapGrid.z) { return; }
	m_bMapGrit[nWight][nHeight] = Set;
}

//==========================================
//  グリッドの移動可能フラグを設定
//==========================================
void CMapSystem::SetGritBool(const GRID& grid, bool Set)
{
	if (grid.x < 0 || grid.z < 0) { return; }
	if (grid.x >= m_MapGrid.x || grid.z >= m_MapGrid.z) { return; }
	m_bMapGrit[grid.x][grid.z] = Set;
}

//==========================================
//  グリッドの移動可能フラグを取得
//==========================================
bool CMapSystem::GetGritBool(int nWight, int nHeight)
{
	if (nWight < 0 || nHeight < 0) { return false; }
	if (nWight >= m_MapGrid.x || nHeight >= m_MapGrid.z) { return false; }
	return m_bMapGrit[nWight][nHeight];
}

//==========================================
//  グリッドの移動可能フラグを取得
//==========================================
bool CMapSystem::GetGritBool(const GRID& grid)
{
	if (grid.x < 0 || grid.z < 0) { return false; }
	if (grid.x >= m_MapGrid.x || grid.z >= m_MapGrid.z) { return false; }
	return m_bMapGrit[grid.x][grid.z];
}

//==========================================
//  グリッドのレールフラグを設定
//==========================================
void CMapSystem::SetRailGritBool(const GRID& grid, bool Set)
{
	if (grid.x < 0 || grid.z < 0) { return; }
	if (grid.x >= m_MapGrid.x || grid.z >= m_MapGrid.z) { return; }
	m_bMapRailGrit[grid.x][grid.z] = Set;
}

//==========================================
//  グリッドのレールフラグを取得
//==========================================
bool CMapSystem::GetRailGritBool(const GRID& grid)
{
	if (grid.x < 0 || grid.z < 0) { return false; }
	if (grid.x >= m_MapGrid.x || grid.z >= m_MapGrid.z) { return false; }
	return m_bMapRailGrit[grid.x][grid.z];
}

//==========================================
//  １マスのサイズを取得
//==========================================
float CMapSystem::GetGritSize(void)
{
	return GRID_SIZE;
}

#if 0
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
// プレイヤーのグリッド取得
//==========================================
CMapSystem::GRID CMapSystem::GetPlayerGrid(unsigned int PlayNumber)
{
	// 位置
	if (PlayNumber < m_PosPlayer.size())
	{ // 読み込んだ数以内だったら

		// 座標入れる
		return m_PosPlayer[PlayNumber];
	}

	// 変なことすんな
	assert(false);
	return GRID();
}
#endif

//==========================================
// プレイヤーの位置取得
//==========================================
D3DXVECTOR3 CMapSystem::GetPlayerPos(unsigned int PlayNumber, unsigned int nNumMap)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置

	// 位置
	if (PlayNumber < m_MapInfo[nNumMap].posPlayer.size())
	{ // 読み込んだ数以内だったら

		// 座標入れる
		pos = m_MapInfo[nNumMap].posPlayer[PlayNumber].ToWorld();
	}
	return pos;
}

//==========================================
// プレイヤーのグリッド取得
//==========================================
CMapSystem::GRID CMapSystem::GetPlayerGrid(unsigned int PlayNumber, unsigned int nNumMap)
{
	// 位置
	if (PlayNumber < m_MapInfo[nNumMap].posPlayer.size())
	{ // 読み込んだ数以内だったら

		// 座標入れる
		return m_MapInfo[nNumMap].posPlayer[PlayNumber];
	}

	// 変なことすんな
	assert(false);
	return GRID();
}

//==========================================
// 全てのマップ情報の読み込み
//==========================================
HRESULT CMapSystem::LoadAll()
{
	// ファイルを開く
	std::ifstream file(MAP_FILE);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ブロックセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// ファイル読み込み用変数
	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	std::string FileName;	// 読込文字列
	int nNum = -1;			// マップ数
	int nNumTex = 0;		// テクスチャ数

	// ファイルを読込
	std::string str, comment;	// 読込文字列
	while (std::getline(file, str))
	{ // ファイルの終端ではない場合ループ

		// カンマ区切りごとにデータを読込
		std::istringstream iss(str);	// 文字列ストリーム
		while (std::getline(iss, str, ' '))
		{
			if (str == "NUM_MAP")
			{ // マップ数

				// マップ数を読込
				iss >> comment >> nNum;
				pMapSystem->m_nNumMap = nNum;

			}
			else if (str == "SETFILENAME")
			{ // マップファイル名

				// 次の行読み込み
				std::getline(file, str);

				for (int nCnt = 0; nCnt < nNum; nCnt++)
				{
					std::istringstream issMapFile(str);	// 文字列ストリーム
					// マップファイルのパスを読込
					issMapFile >> FileName;

					// マップ情報の読み込み
					LoadMap(FileName.c_str());

					// 次の行読み込み
					std::getline(file, str);
				}
			}
			else if (str == "SETTEXTURE")
			{ // マップテクスチャ名

				// 次の行読み込み
				std::getline(file, str);

				while (str != "END_SETTEXTURE")
				{ // テクスチャ読み込みが終わるまで
					std::istringstream issTexFile(str);	// 文字列ストリーム

					// マップファイルのパスを読込
					issTexFile >> FileName;

					m_MapInfo[nNumTex].texture = FileName.c_str();

					// テクスチャの読み込み
					//pTexture->Regist(FileName.c_str());

					// 次の行読み込み
					std::getline(file, str);

					nNumTex++;
				}
			}
		}
	}

	// ファイルを閉じる
	file.close();

	return S_OK;
}

//==========================================
//  マップ情報の読み込み
//==========================================
HRESULT CMapSystem::LoadMap(const char* pFilename)
{
	// 経路探索用の情報を取得
	auto generator = AStar::Generator::Create();

	// マップシステムの情報
	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	float fMapSystemGritSize = pMapSystem->GetGritSize() * 0.5f;

	// 読み込み用
	D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// グリッド生成位置
	D3DXVECTOR3 posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// グリッド開始位置
	D3DXVECTOR2 charOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// グリッドのオフセット
	D3DXVECTOR3 size = D3DXVECTOR3(fMapSystemGritSize, 0.0f, fMapSystemGritSize);		// グリッドサイズ
	MAPTYPE mapType;

	// ファイルを開く
	std::ifstream file(pFilename);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ブロックセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// ファイルを読込
	std::string str = {};		// 読込文字列
	m_MapInfo.emplace_back();	// 配列を拡張
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
				pMapSystem->m_InitPos = posStart;		// 開始地点の設定
			}
			else if (str == "NUM_GRID")
			{
				// グリッドの行列数を読み込み
				iss >> pMapSystem->m_MapGrid.x >> pMapSystem->m_MapGrid.z;

				// 経路探索用情報の設定
				generator->setWorldSize(pMapSystem->m_MapGrid.ToAStar()); // 世界の大きさ
			}

			else if (str == "STARTSETSTAGE")
			{
				while (std::getline(file, str))
				{ // ファイルの終端ではない場合ループ

					// 終端の場合ステージ生成を抜ける
					if (str == "ENDSETSTAGE") { break; }

					for (int nCntHeight = 0; nCntHeight < pMapSystem->m_MapGrid.z; nCntHeight++)
					{ // 列カウント

						// 横一行分の配列を拡張
						m_nData.emplace_back();

						// カンマ区切りごとにデータを読込
						std::istringstream issChar(str);	// 文字列ストリーム

						for (int nCntWidth = 0; nCntWidth < pMapSystem->m_MapGrid.x; nCntWidth++)
						{ // 行カウント

							// 1行ずつ読み込み
							std::getline(issChar, str, ',');

							// 行列数設定
							GRID grid = GRID(nCntWidth, nCntHeight);
							if (str == "") { continue; }	// 空白は無視する
							else if (str == "1")
							{ // 壁の場合

								// 種類保存
								m_MapInfo.back().type.push_back(MAPTYPE_WALL);
								m_MapInfo.back().grid.push_back(grid);

								continue;
							}

							// 床
							mapType = MAPTYPE_NONE;

							// オブジェクトを設置
							switch (std::stoi(str))
							{
							case MAPTYPE_CROSS:		// 十字架
								// 種類保存
								mapType = MAPTYPE_CROSS;
								break;

							case MAPTYPE_DEVILHOLLRANGE:		// デビルホールの範囲
								// 種類保存
								mapType = MAPTYPE_DEVILHOLLRANGE;
								break;

							case MAPTYPE_DEVILHOLL:				// デビルホール
								// 種類保存
								mapType = MAPTYPE_DEVILHOLL;
								break;

							case MAPTYPE_RAILBLOCK:				// レールブロック
								// 種類保存
								mapType = MAPTYPE_RAILBLOCK;
								break;

							case MAPTYPE_PLAYER:				// プレイヤー
								// 種類保存
								mapType = MAPTYPE_PLAYER;

								// プレイヤーのグリッド位置
								m_MapInfo.back().posPlayer.push_back(grid);
								break;

							case MAPTYPE_MEDAMAN:				// メダマン
								// 種類保存
								mapType = MAPTYPE_MEDAMAN;
								break;

							case MAPTYPE_BONBON:				// ボンボン
								// 種類保存
								mapType = MAPTYPE_BONBON;
								break;

							case MAPTYPE_LITTLEDEVIL:			// 子デビル
								// 種類保存
								mapType = MAPTYPE_LITTLEDEVIL;
								break;

							case MAPTYPE_ROLLROCK:				// 転がる岩
								// 種類保存
								mapType = MAPTYPE_ROLLROCK;
								break;

							case MAPTYPE_RAIL:					// レール
								// 種類保存
								mapType = MAPTYPE_RAIL;
								break;

							default:
								break;
							}

							// 種類保存
							m_MapInfo.back().type.push_back(mapType);

							// グリッド保存
							m_MapInfo.back().grid.push_back(grid);
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

	return S_OK;
}

//==========================================
// 指定されたマップの生成
//==========================================
HRESULT CMapSystem::CreateMap(unsigned int nSelect)
{
	// 空の場合返す
	if (m_MapInfo.empty()) { return E_FAIL; }

	m_MapPos = m_InitPos;		// マップの位置初期化

	// マップシステムの情報
	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	CObjmeshField* map = nullptr;
	CRailManager* pRailManager = new CRailManager();		// レールマネージャーを生成

	// グリッド設定の判定
	bool bGridSet = false;
	bool bRailGridSet = false;		// レール

	// 経路探索用の情報を取得
	auto generator = AStar::Generator::Create();

	switch (CScene::GetMode())
	{// 床の生成
	case CScene::MODE_GAME:
		CGame::GetInstance()->SetMapField(CObjmeshField::Create(pMapSystem->m_MapGrid));
		map = CGame::GetInstance()->GetMapField();
		break;

	case CScene::MODE_TUTORIAL:
		CTutorial::GetInstance()->SetMapField(CObjmeshField::Create(pMapSystem->m_MapGrid));
		map = CTutorial::GetInstance()->GetMapField();
		break;

	default:
		break;
	}

	map->SetPos(INITVECTOR3);
	map->SetDisp(false); // 描画をオフ

	for (int nCntHeight = 0; nCntHeight < m_MapGrid.z; nCntHeight++)
	{ // 列カウント
		for (int nCntWidth = 0; nCntWidth < m_MapGrid.x; nCntWidth++)
		{ // 行カウント

			int nNumGrid = nCntWidth + (nCntHeight * m_MapGrid.x);
			GRID grid = m_MapInfo[nSelect].grid[nNumGrid];

			// グリッド判定の初期化
			pMapSystem->SetGritBool(grid, false);

			// グリッドのレール判定の初期化
			pMapSystem->SetRailGritBool(grid, false);

			if (m_MapInfo[nSelect].type[nNumGrid] == MAPTYPE_WALL)
			{ // 壁の場合
				CWall::Create(grid);
				// グリッド判定の設定
				pMapSystem->SetGritBool(grid.x, grid.z, true);

				// 経路探索用情報の設定
				generator->addCollision(grid.ToAStar()); // 通過不可地点を追加

				continue;
			}

			// 床の生成
			CTile::Create(grid);

			// オブジェクトを生成
			switch (m_MapInfo[nSelect].type[nNumGrid])
			{
			case MAPTYPE_CROSS:		// 十字架
				CItem::Create(CItem::TYPE_CROSS, grid);
				break;

			case MAPTYPE_DEVILHOLLRANGE:		// デビルホールの範囲
				// グリッド設定の判定
				bGridSet = true;

				// 経路探索用情報の設定
				generator->addCollision(grid.ToAStar()); // 通過不可地点を追加
				break;

			case MAPTYPE_DEVILHOLL:				// デビルホール
				// マップの中心に設定
				pMapSystem->m_mapCenter = grid;
				CDevilHole::Create(grid);

				// グリッド設定の判定
				bGridSet = true;

				// 経路探索用情報の設定
				generator->addCollision(grid.ToAStar()); // 通過不可地点を追加
				break;

			case MAPTYPE_RAILBLOCK:				// レールブロック
				CRailBlock::Create(grid);
				pRailManager->Set(grid);
				// グリッド設定の判定
				bGridSet = true;
				bRailGridSet = true;

				// 経路探索用情報の設定
				generator->addCollision(grid.ToAStar()); // 通過不可地点を追加
				break;

			case MAPTYPE_PLAYER:				// プレイヤー
				break;

			case MAPTYPE_MEDAMAN:				// メダマン
				CEnemy::Create(CEnemy::ENEMY_MEDAMAN, grid);
				break;

			case MAPTYPE_BONBON:				// ボンボン
				CEnemy::Create(CEnemy::ENEMY_BONBON, grid);
				break;

			case MAPTYPE_LITTLEDEVIL:			// 子デビル
				CEnemy::Create(CEnemy::ENEMY_LITTLEDEVIL, grid);
				break;

			case MAPTYPE_ROLLROCK:				// 岩
				CRollRock::Create(grid);
				// グリッド設定の判定
				bGridSet = true;

				// 経路探索用情報の設定
				generator->addCollision(grid.ToAStar()); // 通過不可地点を追加
				break;

			case MAPTYPE_RAIL:					// レール
				pRailManager->Set(grid);
				bRailGridSet = true;

				break;

			default:
				// ボワボワの生成
				if (BOWABOWA_RATE <= 0) { assert(false); }

				// ランダム生成
				if (!(rand() % BOWABOWA_RATE))
				{
					CItem::Create(CItem::TYPE_BOWABOWA, grid);
				}
				break;
			}

			// グリッド判定の設定
			pMapSystem->SetGritBool(grid, bGridSet);

			// グリッドのレール判定の設定
			pMapSystem->SetRailGritBool(grid, bRailGridSet);

			// グリッド設定の判定
			bGridSet = false;
			bRailGridSet = false;

		}
	}

	// レールの向き設定
	pRailManager->Init();

	// レールマネージャーの破棄
	if (pRailManager != nullptr)
	{
		pRailManager->Uninit();
		delete pRailManager;
		pRailManager = nullptr;
	}

	return S_OK;
}

//==========================================
// マップのリセット
//==========================================
void CMapSystem::ResetMap(void)
{
	// 位置を初期化
	m_MapPos = m_InitPos;

	bool bGridSet = false;
	bool bRailGridSet = false;

	// グリッド判定の再設定
	for (int nCntHeight = 0; nCntHeight < m_MapGrid.z; nCntHeight++)
	{ // 列カウント
		for (int nCntWidth = 0; nCntWidth < m_MapGrid.x; nCntWidth++)
		{ // 行カウント
			int nNumGrid = nCntWidth + (nCntHeight * m_MapGrid.x);
			GRID grid = m_MapInfo[m_nSelectMap].grid[nNumGrid];

			// グリッド判定の初期化
			SetGritBool(grid, false);

			// グリッドのレール判定の初期化
			SetRailGritBool(grid, false);

			if (m_MapInfo[m_nSelectMap].type[nNumGrid] == MAPTYPE_WALL)
			{ // 壁の場合
				// グリッド判定の設定
				SetGritBool(grid.x, grid.z, true);
				continue;
			}

			// オブジェクトを生成
			switch (m_MapInfo[m_nSelectMap].type[nNumGrid])
			{
			case MAPTYPE_DEVILHOLLRANGE:		// デビルホールの範囲
				// グリッド設定の判定
				bGridSet = true;
				break;

			case MAPTYPE_DEVILHOLL:				// デビルホール
				// マップの中心に設定
				m_mapCenter = grid;
				// グリッド設定の判定
				bGridSet = true;

				break;

			case MAPTYPE_RAILBLOCK:				// レールブロック
				// グリッド設定の判定
				bGridSet = true;
				bRailGridSet = true;

				break;

			case MAPTYPE_PLAYER:				// プレイヤー
				break;

			case MAPTYPE_ROLLROCK:				// 岩
				// グリッド設定の判定
				bGridSet = true;

				break;

			case MAPTYPE_RAIL:					// レール
				bRailGridSet = true;

				break;

			default:
				
				break;
			}

			// グリッド判定の設定
			SetGritBool(grid, bGridSet);

			// グリッドのレール判定の設定
			SetRailGritBool(grid, bRailGridSet);

			// グリッド設定の判定
			bGridSet = false;
			bRailGridSet = false;
		}
	}

	// 岩のリスト構造が無ければ抜ける
	if (CRollRock::GetList() == nullptr) { /*return;*/ }
	else
	{
		std::list<CRollRock*> list = CRollRock::GetList()->GetList();    // リストを取得

		// 岩のリストの中身を確認する
		for (CRollRock* pRollRock : list)
		{
			// 岩のリセット
			pRollRock->Reset();
		}
	}

	// レールブロックのリスト構造が無ければ抜ける
	if (CRailBlock::GetList() == nullptr) { /*return;*/ }
	else
	{
		std::list<CRailBlock*> list = CRailBlock::GetList()->GetList();    // リストを取得

		// レールブロックのリストの中身を確認する
		for (CRailBlock* pRailBlock : list)
		{
			// レールブロックのリセット
			pRailBlock->Reset();
		}
	}

	// 敵のリスト構造が無ければ抜ける
	if (CEnemy::GetList() == nullptr) { }
	else
	{
		std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // リストを取得

		// 敵のリストの中身を確認する
		for (CEnemy* pEnemy : list)
		{
			// 敵のリセット
			pEnemy->Reset();
		}
	}
}

//==========================================
//  マップの削除
//==========================================
void CMapSystem::MapDelete()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::OBJECT_TYPE type = pObj->GetType();			//種類を取得

			if (type == CObject::TYPE_TILE ||
				type == CObject::TYPE_CROSS ||
				type == CObject::TYPE_DEVILHOLE ||
				type == CObject::TYPE_RAILBLOCK ||
				type == CObject::TYPE_RAIL ||
				type == CObject::TYPE_ENEMY3D ||
				type == CObject::TYPE_ROLLROCK
				)
			{//種類がマップ関連の時
				pObj->Uninit();
			}

			pObj = pObjNext;
		}
	}
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
	D3DXVECTOR3 DevilPos = CMapSystem::GetInstance()->GetMove()->GetDevilPos();
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
