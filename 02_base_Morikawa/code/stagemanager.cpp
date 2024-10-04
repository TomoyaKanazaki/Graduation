//=============================================================================
// 
//  ステージマネージャー処理 [stagemanager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "stagemanager.h"
#include "stageobj.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TXT = "data\\TXT\\Stage.txt";
}

//==========================================================================
// コンストラクタ
//==========================================================================
CStageManager::CStageManager()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CStageManager::~CStageManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CStageManager* CStageManager::Create()
{
	// メモリの確保
	CStageManager* pObj = new CStageManager;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CStageManager::Init()
{
	// ステージロード処理
	StageLoad();

	for (int i = 0 ; i < 800; i++)
	{
		float distance = UtilFunc::Transformation::Random(-50, 50) * 10.0f;
		CStageObj::CreateBg(MyLib::Vector3(MyLib::Vector3(-2000.0f + i * 200.0f, 0.0f, 1200.0f + distance)));
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CStageManager::Uninit()
{
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CStageManager::Update()
{
	
}

//==========================================================================
// ステージロード
//==========================================================================
HRESULT CStageManager::StageLoad()
{
	// 変数を宣言
	int nEnd;							// テキスト読み込み終了の確認用
	std::string str;					// 文字列
	int nType = 0;						// 何らかのタイプ
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	std::ifstream file(TXT);  // 読み込むファイルのパスを指定

	// ファイルが開けなかった場合、失敗を返す
	if (!file.is_open()) { return E_FAIL; }

	while (str != "END_TXT")
	{  // 1行ずつ読み込む

		file >> str;

		if (str == "#")
		{ // # の場合

			// 1行読み込む
			std::getline(file, str);
		}

		if (str == "SET_BG")
		{ // 文字が背景を示していた場合

			while (file >> str)
			{  // 1行ずつ読み込む

				if (str == "END_SET_BG") { break; }
				if (str == "POS")
				{ // 位置を読み込んだ場合

					file >> str;
					file >> pos.x;
					file >> pos.y;
					file >> pos.z;
				}
			}

			CStageObj::CreateBg(MyLib::Vector3(pos));
		}

		if (str == "SET_OBSTACLE")
		{ // 文字が障害物を示していた場合

			while (file >> str)
			{  // 1行ずつ読み込む

				if (str == "END_SET_OBSTACLE") { break; }
				if (str == "TYPE")
				{ // 種類を読み込んだ場合

					file >> str;
					file >> nType;
				}
				else if (str == "POS")
				{ // 位置を読み込んだ場合

					file >> str;
					file >> pos.x;
					file >> pos.y;
					file >> pos.z;
				}
			}

			CStageObj::CreateObstacle(MyLib::Vector3(pos), nType);
		}

		if (str == "SET_REVERSE")
		{ // 文字が反転ポイントを示していた場合

			while (file >> str)
			{  // 1行ずつ読み込む

				if (str == "END_SET_REVERSE") { break; }
				if (str == "POS")
				{ // 位置を読み込んだ場合

					file >> str;
					file >> pos.x;
					file >> pos.y;
					file >> pos.z;
				}
			}

			CStageObj::CreateReverse(MyLib::Vector3(pos));
		}
	}

	// 成功を返す
	return S_OK;
}
