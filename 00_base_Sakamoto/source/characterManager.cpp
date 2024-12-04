//============================================
//
//	キャラクターの管理 [characterManager.cpp]
//	Author:sakai minato
//
//============================================

#include "characterManager.h"
#include "renderer.h"
#include "manager.h"
#include "Xmodel.h"

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
// テクスチャの番号指定
//====================================================================
int CCharacterManager::Regist(const char* pFilename)
{
	for (int nCntCharacter = 0; nCntCharacter < m_nNumAll; nCntCharacter++)
	{
		if (strcmp(&m_aCharacterInfo[nCntCharacter].acFileName[0], pFilename) == 0)
		{
			return nCntCharacter;
		}
	}

	return -1;
}

//====================================================================
// モデルロード処理
//====================================================================
void CCharacterManager::LoadModel(const char* pFileName, int nNumCharacter)
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
				m_aCharacterInfo[nNumCharacter].ModelManager.nNumModel = nNumModel;

				for (int nCnt = 0; nCnt < nCntModel; nCnt++)
				{
					m_aCharacterInfo[nNumCharacter].ModelManager.aModelParts[nCnt] = ModelParts[nCntModel];
				}

				break;
			}
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}

//====================================================================
// モーションのロード処理
//====================================================================
void CCharacterManager::LoadMotion(const char* pFileName, int nNumCharacter)
{
	FILE* pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(pFileName, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		int nCntMotion = 0;
		int nCntKeyInfo = 0;
		int nCntKey = 0;

		int nNumMotion = 0;
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
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}