//============================================
//
//	キャラクターの管理 [characterManager.cpp]
//	Author:sakai minato
//
//============================================

#include "characterManager.h"

#include "renderer.h"
#include "manager.h"

#include "objectcharacter.h"
#include "model.h"
#include "motion.h"

// 定数定義
namespace
{
	const int MAX_MODEL_PARTS = 32;	// モデルパーツ数の最大数
}

//====================================================================
//コンストラクタ
//====================================================================
CCharacterManager::CCharacterManager()
{
	ZeroMemory(&m_aCharacterInfo[0], sizeof(m_aCharacterInfo));
	m_nNumAll = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CCharacterManager::~CCharacterManager()
{

}

//====================================================================
//読み込み
//====================================================================
HRESULT CCharacterManager::Load(void)
{
	return S_OK;
}

//====================================================================
//破棄
//====================================================================
void CCharacterManager::Unload(void)
{
	for (int nCnt = 0; nCnt < m_nNumAll; nCnt++)
	{
		// 情報取得
		int nMaxParts = m_aCharacterInfo[nCnt].ModelManager.nNumModel;		// パーツ数
		int nMaxMotion = m_aCharacterInfo[nCnt].MotionManager.nNumMotion;	// モーション数
		
		// モーションパーツの開放処理
		for (int nCntMotion = 0; nCntMotion < nMaxMotion; nCntMotion++)
		{
			// 情報取得
			int nMaxKey = m_aCharacterInfo[nCnt].MotionManager.aMotionInfo[nCntMotion].nNumKey;	// キー数

			for (int nCntKey = 0; nCntKey < nMaxKey; nCntKey++)
			{
				// キー情報の開放処理
				delete[] m_aCharacterInfo[nCnt].MotionManager.aMotionInfo[nCntMotion].aKeyManager[nCntKey].apKey;
				m_aCharacterInfo[nCnt].MotionManager.aMotionInfo[nCntMotion].aKeyManager[nCntKey].apKey = nullptr;
			}
		}

		// モデルパーツ情報の開放処理
		delete[] m_aCharacterInfo[nCnt].ModelManager.apModelParts;
		m_aCharacterInfo[nCnt].ModelManager.apModelParts = nullptr;
	}
}

//====================================================================
// キャラクターの番号指定
//====================================================================
int CCharacterManager::Regist(CObjectCharacter* pObjCharacter, const std::string pFilename)
{
	// キャラクターファイル検索処理
	for (int nCntCharacter = 0; nCntCharacter < m_nNumAll; nCntCharacter++)
	{
		if (strcmp(&m_aCharacterInfo[nCntCharacter].acFileName[0], pFilename.c_str()) == 0)
		{
			// モデル・モーション割当処理
			SetModelData(pObjCharacter, nCntCharacter);
			SetMotionData(pObjCharacter, nCntCharacter);

			return nCntCharacter;
		}
	}

	// 読み込み割当処理
	if (m_nNumAll < MAX_CHARACTER)
	{
		int nCharacterNum = m_nNumAll;

		// モデル読み込み
		if (LoadModel(pFilename, nCharacterNum))
		{
			// モデル割当処理
			SetModelData(pObjCharacter, nCharacterNum);
		}
		else
		{
			// 失敗
			assert(("モデル読み込み失敗", false));
		}

		// モデル数を取得
		int nNumModel = pObjCharacter->GetNumModel();

		// モーション読み込み
		if (LoadMotion(pFilename, nNumModel, nCharacterNum))
		{
			// モーション割当処理
			SetMotionData(pObjCharacter, nCharacterNum);
		}
		else
		{
			// 失敗
			assert(("モーション読み込み失敗", false));
		}

		// ファイル名を代入
		strcpy(&m_aCharacterInfo[m_nNumAll].acFileName[0], pFilename.c_str());

		m_nNumAll++;

		return nCharacterNum;
	}
	else
	{
		assert(("キャラクター管理の最大数オーバー", false));
	}

	return -1;
}

//====================================================================
// モデル割当処理
//====================================================================
void CCharacterManager::SetModelData(CObjectCharacter* pObjCharacter,int nNumCharacter)
{	
	// モデル使用数を取得・設定
	int nNumModel = m_aCharacterInfo[nNumCharacter].ModelManager.nNumModel;
	pObjCharacter->SetNumModel(nNumModel);

	CModel* apModel[MAX_MODEL_PARTS] = {};

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// パーツ情報を取得
		SModelParts ModelParts = m_aCharacterInfo[nNumCharacter].ModelManager.apModelParts[nCnt];

		// モデルの生成処理
		if (FAILED(apModel[nCnt] = CModel::Create(&ModelParts.acModelFileName[0]))) { assert(false); }
		apModel[nCnt]->SetColorType(CModel::COLORTYPE_FALSE);

		// 親子付け設定
		if (ModelParts.nParent == -1)
		{
			apModel[nCnt]->SetParent(nullptr);
		}
		else
		{
			apModel[nCnt]->SetParent(apModel[ModelParts.nParent]);
		}

		// 位置と向きを設定
		apModel[nCnt]->SetStartPos(ModelParts.pos);
		apModel[nCnt]->SetStartRot(ModelParts.rot);

		// モデルをキャラクタークラスに代入
		pObjCharacter->SetModel(apModel[nCnt], nCnt);
	}
}

//====================================================================
// モーション割当処理
//====================================================================
void CCharacterManager::SetMotionData(CObjectCharacter* pObjCharacter, int nNumCharacter)
{
	// モーション管理を取得
	SMotionManager MotionManager = m_aCharacterInfo[nNumCharacter].MotionManager;

	// モーション使用数を取得・設定
	int nNumMotion = MotionManager.nNumMotion;

	// モデル使用数の取得
	int nNumModel = pObjCharacter->GetNumModel();

	CMotion* pMotion = new CMotion;

	if (pMotion != nullptr)
	{
		// 先頭モデルを取得
		CModel** pModel = pObjCharacter->GetModel();

		// モーションのモデル紐づけ
		pMotion->SetModel(pModel, nNumModel);
	}

	for (int nCntMotion = 0; nCntMotion < nNumMotion; nCntMotion++)
	{
		// パーツ情報を取得
		SMotionInfo MotionInfo = MotionManager.aMotionInfo[nCntMotion];

		// ループ・最大キー数を設定
		pMotion->SetInfoLoop(MotionInfo.bLoop, nCntMotion);
		pMotion->SetInfoNumKey(MotionInfo.nNumKey, nCntMotion);

		for (int nCntKey = 0; nCntKey < MotionInfo.nNumKey; nCntKey++)
		{
			// キー管理情報を取得
			SKeyManager KeyManager = MotionInfo.aKeyManager[nCntKey];

			// キーごとのフレーム数を設定
			pMotion->SetInfoKeyFrame(KeyManager.nFrame, nCntMotion, nCntKey);

			for (int nCntParts = 0; nCntParts < nNumModel; nCntParts++)
			{
				// キー情報を取得
				SKey Key = KeyManager.apKey[nCntParts];

				// パーツごとの位置・向きを設定
				pMotion->SetInfoPartsPosX(Key.pos.x, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsPosY(Key.pos.y, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsPosZ(Key.pos.z, nCntMotion, nCntKey, nCntParts);

				pMotion->SetInfoPartsRotX(Key.rot.x, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsRotY(Key.rot.y, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsRotZ(Key.rot.z, nCntMotion, nCntKey, nCntParts);
			}
		}
	}

	// モデルをキャラクタークラスに代入
	pObjCharacter->SetMotion(pMotion);
}

//====================================================================
// モデルロード処理
//====================================================================
bool CCharacterManager::LoadModel(const std::string pFilename,int nNumCharacter)
{
	//ファイルを開く
	FILE* pFile = fopen(pFilename.c_str(), "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		int nCntModel = 0;

		int nNumModel = 0;
		SModelParts ModelParts[MAX_MODEL_PARTS] = {};

		char aString[128] = {};				//ゴミ箱
		char aMessage[128] = {};			//スタートとエンドのメッセージ

		// 読み込み開始-----------------------------------------------------
		while (1)
		{//「SCRIPT」を探す
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// モデル数読み込み-----------------------------------------------------
				while (1)
				{//「NUM_MODEL」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%d", &nNumModel);		//モデル数の設定
						break;
					}
				}

				//モデルファイルの読み込み
				while (1)
				{//「MODEL_FILENAME」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%s", &ModelParts[nCntModel].acModelFileName[0]);		//読み込むモデルのパスを取得

						nCntModel++;
					}
					if (nCntModel >= nNumModel)
					{
						nCntModel = 0;
						break;
					}
				}

				// キャラクター情報読み込み-----------------------------------------------------
				while (1)
				{//「PARTSSET」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "PARTSSET") == 0)
					{
						while (1)
						{//各種変数を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "INDEX") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParts[nCntModel].nIndex);		//インデックスを設定
								nCntModel = ModelParts[nCntModel].nIndex;
							}
							if (strcmp(&aMessage[0], "PARENT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParts[nCntModel].nParent);	//親モデルのインデックスを設定
							}
							if (strcmp(&aMessage[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelParts[nCntModel].pos.x);	// 位置(オフセット)の初期設定
								fscanf(pFile, "%f", &ModelParts[nCntModel].pos.y);	// 位置(オフセット)の初期設定
								fscanf(pFile, "%f", &ModelParts[nCntModel].pos.z);	// 位置(オフセット)の初期設定
							}
							if (strcmp(&aMessage[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelParts[nCntModel].rot.x);	// 向き（オフセット）の初期設定
								fscanf(pFile, "%f", &ModelParts[nCntModel].rot.y);	// 向き（オフセット）の初期設定
								fscanf(pFile, "%f", &ModelParts[nCntModel].rot.z);	// 向き（オフセット）の初期設定
							}

							if (strcmp(&aMessage[0], "END_PARTSSET") == 0)
							{
								break;
							}
						}

						nCntModel++;

						if (nCntModel >= nNumModel)
						{
							break;
						}
					}
				}
			}
			if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
			{
				// モデル管理に最大モデル数代入
				m_aCharacterInfo[nNumCharacter].ModelManager.nNumModel = nNumModel;

				// 動的確保
				m_aCharacterInfo[nNumCharacter].ModelManager.apModelParts = new SModelParts[nNumModel];

				for (int nCnt = 0; nCnt < nCntModel; nCnt++)
				{
					// モデル管理にパーツ情報代入
					m_aCharacterInfo[nNumCharacter].ModelManager.apModelParts[nCnt] = ModelParts[nCnt];
				}

				break;
			}
		}

		fclose(pFile);

		return true;
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");

		return false;
	}
}

//====================================================================
// モーションのロード処理
//====================================================================
bool CCharacterManager::LoadMotion(const std::string pFileName, int nNumModel, int nNumCharacter)
{
	//ファイルを開く
	FILE* pFile = fopen(pFileName.c_str(), "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		int nCntMotion = 0;
		int nCntKeyManager = 0;
		int nCntParts = 0;
		int nMaxParts = m_aCharacterInfo[nNumCharacter].ModelManager.nNumModel;

		SMotionInfo MotionInfo[MAX_MOTION] = {};

		char aString[128] = {};			//ゴミ箱
		char aMessage[128] = {};		//スタートメッセージ
		char aBool[128] = {};			//bool変換用メッセージ

		// モデルのパーツ数の有無判定
		if (nMaxParts == 0)
		{
			return false;
		}

		// 読み込み開始-----------------------------------------------------
		while (1)
		{//「SCRIPT」を探す
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// モーション情報読み込み-----------------------------------------------------
				while (1)
				{//「MOTIONSET」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MOTIONSET") == 0)
					{
						while (1)
						{//各種変数を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "LOOP") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%s", &aBool[0]);	//ループするかどうかを設定

								MotionInfo[nCntMotion].bLoop = (strcmp(&aBool[0], "1") == 0 ? true : false);			//bool型の書き方
							}
							if (strcmp(&aMessage[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &MotionInfo[nCntMotion].nNumKey);	//キーの総数を設定
								break;
							}
						}

						// キー情報読み込み-----------------------------------------------------
						while (1)
						{//「KEYSET」を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "KEYSET") == 0)
							{
								while (1)
								{//「FRAME」を探す
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "FRAME") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%d", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].nFrame);	//キーフレームを設定

										// キー要素のパーツ数動的確保
										MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey = new SKey[nMaxParts];

										break;
									}
								}

								while (1)
								{//「KEY」を探す
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "KEY") == 0)
									{
										while (1)
										{//各種変数を探す
											fscanf(pFile, "%s", &aMessage[0]);
											if (strcmp(&aMessage[0], "POS") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey[nCntParts].pos.x);	//位置を設定
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey[nCntParts].pos.y);	//位置を設定
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey[nCntParts].pos.z);	//位置を設定
											}
											if (strcmp(&aMessage[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey[nCntParts].rot.x);	//向きを設定
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey[nCntParts].rot.y);	//向きを設定
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey[nCntParts].rot.z);	//向きを設定
												break;
											}
										}
									}
									if (strcmp(&aMessage[0], "END_KEY") == 0)
									{
										nCntParts++;

										if (nCntParts >= nNumModel)
										{
											break;
										}
									}
								}
							}
							if (strcmp(&aMessage[0], "END_KEYSET") == 0)
							{
								nCntParts = 0;
								nCntKeyManager++;

								if (nCntKeyManager >= MotionInfo[nCntMotion].nNumKey)
								{
									break;
								}
							}
						}
					}

					if (strcmp(&aMessage[0], "END_MOTIONSET") == 0)
					{
						nCntKeyManager = 0;
						nCntMotion++;
					}
					else if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
					{
						// モデル管理にモーション数代入
						m_aCharacterInfo[nNumCharacter].MotionManager.nNumMotion = nCntMotion;

						for (int nCnt = 0; nCnt < nCntMotion; nCnt++)
						{
							// モーション管理にモーション情報代入
							m_aCharacterInfo[nNumCharacter].MotionManager.aMotionInfo[nCnt] = MotionInfo[nCnt];
						}

						break;
					}
				}
			}
			if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
		fclose(pFile);

		return true;
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");

		return false;
	}
}