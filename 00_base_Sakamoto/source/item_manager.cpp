//==========================================
//
//  アイテム管理クラス(item_manager.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "item_manager.h"

//==========================================
//  静的メンバ変数宣言
//==========================================
int CItemManager::ms_nNumMap = 0;
CItemManager::SItems CItemManager::ms_pItems[MAX_ITEM][MAX_SET] = {};

//==========================================
//  コンストラクタ
//==========================================
CItemManager::CItemManager() : 
	m_eType(TYPE_NONE),
	m_pItems(nullptr)
{
}

//==========================================
//  デストラクタ
//==========================================
CItemManager::~CItemManager()
{

}

//==========================================
//  初期化処理
//==========================================
void CItemManager::Init(const int nMap, const EType type)
{
	//// 使用する配置情報のnullチェック
	//if (&ms_pItems[nMap] == nullptr) { assert(false); return; }

	//// 使用する配置データを設定
	//m_pItems = &ms_pItems[nMap];

	//// 配置タイプを設定
	//m_eType = type;

	//// アイテムの設置
	//ItemSeting();
}

//==========================================
//  終了処理
//==========================================
void CItemManager::Uninit()
{
	// 自身の終了処理
	delete this;
}

//==========================================
//  更新処理
//==========================================
void CItemManager::Update()
{
	// Do Nothing
}

//==========================================
//  描画処理
//==========================================
void CItemManager::Draw()
{
	// Do Nothing
}

//==========================================
//  生成処理
//==========================================
CItemManager* CItemManager::Create(const int nMap, const EType type)
{
	// インスタンス生成
	CItemManager* manager = new CItemManager;

	// メモリの確保に失敗した場合関数を抜ける
	if (manager == nullptr) { assert(false); return nullptr; }

	// 初期化処理
	manager->Init(nMap, type);

	return manager;
}

//==========================================
//  情報取得
//==========================================
HRESULT CItemManager::Load()
{
	///*
	//* 読み込むデータ量が膨大になる可能性があるので
	//* 必ず初期化で取得すること
	//*/

	//// TODO アイテム配置データの作成
	//// TODO データの読み込み
	//while (1)
	//{
	//	FILE* pFile = nullptr;

	//	switch (ms_nNumMap)
	//	{
	//	case 0:
	//		//ファイルを開く
	//		pFile = fopen("data\\TXT\\STAGE\\Item00.txt", "r");
	//		break;
	//	case 1:
	//		//ファイルを開く
	//		pFile = fopen("data\\TXT\\STAGE\\Item01.txt", "r");
	//		break;
	//	default:
	//		break;
	//	}

	//	if (pFile != nullptr)
	//	{//ファイルが開けた場合

	//		char aString[128] = {};			//メッセージ保存用

	//		fscanf(pFile, "%s", &aString[0]);
	//		if (strcmp(&aString[0], "STARTSETSTAGE") == 0)
	//		{
	//			int nNumberCount = 0;
	//			while (1)
	//			{
	//				fscanf(pFile, "%s", &aString[0]);
	//				if (strcmp(&aString[0], "STARTSETITEM") == 0)
	//				{
	//					D3DXVECTOR3 pos;
	//					int Type;

	//					fscanf(pFile, "%s", &aString[0]);
	//					fscanf(pFile, "%f", &pos.x);
	//					fscanf(pFile, "%f", &pos.y);
	//					fscanf(pFile, "%f", &pos.z);

	//					fscanf(pFile, "%s", &aString[0]);
	//					fscanf(pFile, "%d", &Type);

	//					ms_pItems[ms_nNumMap].pPos[nNumberCount] = pos;
	//					ms_pItems[ms_nNumMap].pType[nNumberCount] = (CItem::TYPE)Type;
	//					nNumberCount++;

	//					fscanf(pFile, "%s", &aString[0]);
	//					if (strcmp(&aString[0], "ENDSETITEM") == 0)
	//					{
	//						break;
	//					}
	//				}
	//				else if (strcmp(&aString[0], "ENDSETSTAGE") == 0)
	//				{
	//					ms_pItems[ms_nNumMap].nNum = nNumberCount;
	//					break;
	//				}
	//			}
	//		}
	//		fclose(pFile);
	//	}
	//	else
	//	{//ファイルが開けなかった場合
	//		break;
	//	}
	//}

	return S_OK;
}

//==========================================
//  情報破棄
//==========================================
void CItemManager::UnLoad()
{
	/*
	* ゲーム中に破棄すると再度読み込む必要があるため
	* 必ず終了で破棄すること
	*/

	//// 用意されている全てのデータを破棄する
	//for (int i = 0; i < ms_nNumMap; ++i)
	//{
	//	for (int j = 0; j < ms_pItems[i].nNum; ++j)
	//	{
	//		delete &ms_pItems[i].pType[j];
	//		delete &ms_pItems[i].pPos[j];
	//	}
	//	delete &ms_pItems[i];
	//}
}

//==========================================
//  アイテムの設置処理
//==========================================
void CItemManager::ItemSeting()
{
	// 設定される配置タイプによって分岐
	switch (m_eType)
	{
	case TYPE_BOWABOWA: // ボワボワ配置

		// 配置情報を捜査する
		for (int i = 0; i < m_pItems->nNum; ++i)
		{
			// 配置されるアイテムが聖書の場合次に進む
			if (m_pItems->pType[i] == CItem::TYPE_BIBLE) { continue; }

			// アイテムの生成
			CItem::Create(m_pItems->pType[i], m_pItems->pPos[i]);
		}

		break;

	case TYPE_BIBLE: // 聖書配置
		
		// 配置情報を捜査する
		for (int i = 0; i < m_pItems->nNum; ++i)
		{
			// 配置されるアイテムが聖書でない場合次に進む
			if (m_pItems->pType[i] != CItem::TYPE_BIBLE) { continue; }

			// アイテムの生成
			CItem::Create(m_pItems->pType[i], m_pItems->pPos[i]);
		}

		break;

	default:
		assert(false);
		break;
	}
}
