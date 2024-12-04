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
// キャラクターの番号指定
//====================================================================
int CCharacterManager::Regist(CObjectCharacter* pObjCharacter, const char* pFilename)
{
	// キャラクターファイル検索処理
	for (int nCntCharacter = 0; nCntCharacter < m_nNumAll; nCntCharacter++)
	{
		if (strcmp(&m_aCharacterInfo[nCntCharacter].acFileName[0], pFilename) == 0)
		{
			// モデル・モーション割当処理
			SetModelData(pObjCharacter, m_nNumAll);
			SetMotionData(pObjCharacter, m_nNumAll);

			return nCntCharacter;
		}
	}

	// 読み込み割当処理
	if (m_nNumAll < MAX_CHARACTER)
	{
		m_nNumAll++;

		// モデル読み込み
		if (LoadModel(pFilename, m_nNumAll))
		{
			// モデル割当処理
			SetModelData(pObjCharacter,m_nNumAll);
		}
		else
		{
			// 失敗
			assert(("モデル読み込み失敗", false));
		}

		// モーション読み込み
		if (LoadMotion(pFilename, m_nNumAll))
		{
			// モーション割当処理
			SetMotionData(pObjCharacter, m_nNumAll);
		}
		else
		{
			// 失敗
			assert(("モーション読み込み失敗", false));
		}
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
	// モデル管理を取得
	ModelManager ModelManager = m_aCharacterInfo[nNumCharacter].ModelManager;
	
	// モデル使用数を取得・設定
	int nNumModel = ModelManager.nNumModel;
	pObjCharacter->SetNumModel(nNumModel);

	CModel* apModel[MAX_MODEL_PARTS] = {};

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// パーツ情報を取得
		ModelParts ModelParts = ModelManager.aModelParts[nCnt];

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
		apModel[nCnt]->SetPos(ModelParts.pos);
		apModel[nCnt]->SetRot(ModelParts.rot);

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
	MotionManager MotionManager = m_aCharacterInfo[nNumCharacter].MotionManager;

	// モーション使用数を取得・設定
	int nNumMotion = MotionManager.nNumMotion;

	// モデル使用数の取得
	int nNumModel = pObjCharacter->GetNumModel();

	CMotion* pMotion = new CMotion;

	if (pMotion != nullptr)
	{
		// 先頭モデルを取得
		CModel* pModel = pObjCharacter->GetModel(0);

		// モーションのモデル紐づけ
		pMotion->SetModel(&pModel, nNumModel);
	}

	for (int nCntMotion = 0; nCntMotion < nNumMotion; nCntMotion++)
	{
		// パーツ情報を取得
		MotionInfo MotionInfo = MotionManager.aMotionInfo[nCntMotion];

		// ループ・最大キー数を設定
		pMotion->SetInfoLoop(MotionInfo.bLoop, nCntMotion);
		pMotion->SetInfoNumKey(MotionInfo.nNumKey, nCntMotion);

		for (int nCntKey = 0; nCntKey < MotionInfo.nNumKey; nCntKey++)
		{
			// キーごとのフレーム数を設定
			pMotion->SetInfoKeyFrame(MotionInfo.aKeyInfo[nCntKey].nFrame, nCntMotion, nCntKey);

			for (int nCntParts = 0; nCntParts < nNumModel; nCntParts++)
			{
				// パーツごとの位置・向きを設定
				pMotion->SetInfoPartsPosX(MotionInfo.aKeyInfo[nCntKey].aKey[nCntParts].pos, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsPosY(MotionInfo.aKeyInfo[nCntKey].aKey[nCntParts].pos, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsPosZ(MotionInfo.aKeyInfo[nCntKey].aKey[nCntParts].pos, nCntMotion, nCntKey, nCntParts);

				pMotion->SetInfoPartsRotX(MotionInfo.aKeyInfo[nCntKey].aKey[nCntParts].rot, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsRotY(MotionInfo.aKeyInfo[nCntKey].aKey[nCntParts].rot, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsRotZ(MotionInfo.aKeyInfo[nCntKey].aKey[nCntParts].rot, nCntMotion, nCntKey, nCntParts);
			}
		}
	}
}

//====================================================================
// モデルロード処理
//====================================================================
bool CCharacterManager::LoadModel(const char* pFileName, int nNumCharacter)
{
	//ファイルを開く
	FILE* pFile = fopen(pFileName, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		int nCntModel = 0;

		int nNumModel = 0;
		ModelParts ModelParts[MAX_MODEL_PARTS] = {};

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

				for (int nCnt = 0; nCnt < nCntModel; nCnt++)
				{
					// モデル管理にパーツ情報代入
					m_aCharacterInfo[nNumCharacter].ModelManager.aModelParts[nCnt] = ModelParts[nCntModel];
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
bool CCharacterManager::LoadMotion(const char* pFileName, int nNumCharacter)
{
	//ファイルを開く
	FILE* pFile = fopen(pFileName, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		int nCntMotion = 0;
		int nCntKeyInfo = 0;
		int nCntKey = 0;

		MotionInfo MotionInfo[MAX_MOTION] = {};

		char aString[128] = {};			//ゴミ箱
		char aMessage[128] = {};		//スタートメッセージ
		char aBool[128] = {};			//bool変換用メッセージ

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
										fscanf(pFile, "%d", &MotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].nFrame);	//キーフレームを設定
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
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKey].pos.x);	//位置を設定
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKey].pos.y);	//位置を設定
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKey].pos.z);	//位置を設定
											}
											if (strcmp(&aMessage[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKey].rot.x);	//向きを設定
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKey].rot.y);	//向きを設定
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKey].rot.z);	//向きを設定
												break;
											}
										}
									}
									if (strcmp(&aMessage[0], "END_KEY") == 0)
									{
										nCntKey++;

										if (nCntKey >= nCntMotion)
										{
											break;
										}
									}
								}
							}
							if (strcmp(&aMessage[0], "END_KEYSET") == 0)
							{
								nCntKey = 0;
								nCntKeyInfo++;

								if (nCntKeyInfo >= MotionInfo[nCntMotion].nNumKey)
								{
									break;
								}
							}
						}
					}

					if (strcmp(&aMessage[0], "END_MOTIONSET") == 0)
					{
						nCntKeyInfo = 0;
						nCntMotion++;
					}
					else if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
					{
						// エラー回避用（すぐ消す）
						int nNumMotion = 0;

						// モデル管理にモーション数代入
						m_aCharacterInfo[nNumCharacter].MotionManager.nNumMotion = nNumMotion;

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