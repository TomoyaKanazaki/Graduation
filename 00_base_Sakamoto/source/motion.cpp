//============================================
//
//	モーション [motion.cpp]
//	Author:sakamoto kai
//
//============================================
#include "motion.h"
#include "model.h"
#include "manager.h"
#include <stdio.h>
#include "MapMove.h"
#include "MapSystem.h"

//===============================================
// 関数ポインタ
//===============================================
// 再生状態管理
CMotion::PLAYTYPE_FUNC CMotion::m_PlayTypeFunc[] =
{
	&CMotion::PlayTypeNormal,		// 通常
	&CMotion::PlayTypeBlend,		// ブレンド
};

//====================================================================
//コンストラクタ
//====================================================================
CMotion::CMotion() :
	// 上のメンバ変数
	m_fSlowVaule(1.0f),		// スローに掛ける倍率

	// 下のメンバ変数
	m_PlayInfo(SPlayInfo()),
	m_BodyData(SBodyData())
{
	// 値のクリア
	for (int i = 0; i < MAX_INFO; i++)
	{
		m_aInfo[i] = INFO();
	}
}

//====================================================================
//デストラクタ
//====================================================================
CMotion::~CMotion()
{

}

//====================================================================
//モーションの設定処理
//====================================================================
void CMotion::Set(int nType, float BlendTime)
{
	SPlayInfo* pInfo = &m_PlayInfo;			// 再生情報のポインタ
	SBlendInfo* pBlend = &pInfo->blendInfo;	// ブレンド情報のポインタ

	// ブレンド情報
	pBlend->fCounter = 0.0f;
	pBlend->fTime = BlendTime;
	pBlend->fTimeMax = pBlend->fTime;
	pBlend->nOldId = pInfo->nId;
	pBlend->nOldKey = pInfo->nKey;

	// 再生情報
	pInfo->nId = nType;					//モーションの種類の設定
	pInfo->nKey = 0;					//現在のキーをリセット
	pInfo->nKeyNext = (pInfo->nKey + 1) % m_aInfo[nType].NumKey;	// 次のキー設定
	pInfo->bFinish = false;				// 終了フラグをリセット
	pInfo->fCounter = 0.0f;				// 再生カウント

	// ブレンドしない場合は通常再生
	if (BlendTime == 0)
	{
		m_PlayInfo.type = PLAYTYPE::PLAYTYPE_NORMAL;
		return;
	}

	// ブレンドする場合はブレンド再生
	m_PlayInfo.type = PLAYTYPE::PLAYTYPE_BLEND;
}

//====================================================================
//モーションの更新処理
//====================================================================
void CMotion::Update(void)
{
	// 再生終了の場合
	if (m_PlayInfo.bFinish) { return; }

	// 再生設定
	(this->*(m_PlayTypeFunc[m_PlayInfo.type]))();
}

//====================================================================
//モデルのセット処理
//====================================================================
void CMotion::SetModel(CModel** ppModel, int nNumModel)
{
	// 階層モデル情報を設定
	m_BodyData.ppModel = ppModel;
	m_BodyData.nNumModel = nNumModel;
}

//====================================================================
//モーションのロード処理
//====================================================================
void CMotion::LoadData(const std::string pFilename)
{
	FILE* pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(pFilename.c_str(), "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		char aString[128] = {};				//ゴミ箱
		char aMessage[128] = {};		//スタートメッセージ
		char aBool[128] = {};				//bool変換用メッセージ

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
								m_aInfo[nCntMotion].bLoop = (strcmp(&aBool[0], "1") == 0 ? true : false);			//bool型の書き方
							}
							if (strcmp(&aMessage[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &m_aInfo[nCntMotion].NumKey);	//キーの総数を設定
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
										fscanf(pFile, "%d", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].nFrame);	//キーフレームを設定
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
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosX);	//位置を設定
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosY);	//位置を設定
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosZ);	//位置を設定
											}
											if (strcmp(&aMessage[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotX);	//位置を設定
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotY);	//位置を設定
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotZ);	//位置を設定
												break;
											}
										}
									}
									if (strcmp(&aMessage[0], "END_KEY") == 0)
									{
										nCntKey++;
										if (nCntKey >= m_BodyData.nNumModel)
										{
											break;
										}
									}
								}
							}
							if (strcmp(&aMessage[0], "END_KEYSET") == 0)
							{
								nCntKey = 0;
								nCntKeySet++;
								if (nCntKeySet >= m_aInfo[nCntMotion].NumKey)
								{
									break;
								}
							}
						}
					}
					if (strcmp(&aMessage[0], "END_MOTIONSET") == 0)
					{
						nCntKeySet = 0;
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

//====================================================================
// 通常再生
//====================================================================
void CMotion::PlayTypeNormal()
{
	SPlayInfo* pInfo = &m_PlayInfo;		// 再生情報のポインタ
	SBodyData* pBody = &m_BodyData;		// 階層データのポインタ
	if (m_BodyData.ppModel == nullptr) { return; }

	// ローカルに格納
	int id = pInfo->nId;				// 再生中モーション番号
	int key = pInfo->nKey;				// 再生中キー番号
	int keynext = pInfo->nKeyNext;		// 次のキー
	float count = pInfo->fCounter;		// カウント


	// マップの動作がレトロの場合モーションをかくかくに : TODO いらなかったら消して
	CMapMove* pMove = CMapSystem::GetInstance()->GetMove();
	if (pMove->GetScrollType() == CMapMove::SCROLL_TYPE::SCROLL_TYPE_RETRO)
	{
		count = 0.0f;
	}

	//全モデル(パーツ)の更新
	for (int nCntModel = 0; nCntModel < pBody->nNumModel; nCntModel++)
	{
		// モデル取得
		CModel* pModel = pBody->ppModel[nCntModel];

		//キーの情報から位置・向きを算出
		float POSX = m_aInfo[id].aKeyInfo[keynext].aKey[nCntModel].fPosX - m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosX;
		float POSY = m_aInfo[id].aKeyInfo[keynext].aKey[nCntModel].fPosY - m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosY;
		float POSZ = m_aInfo[id].aKeyInfo[keynext].aKey[nCntModel].fPosZ - m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosZ;
		float ROTX = m_aInfo[id].aKeyInfo[keynext].aKey[nCntModel].fRotX - m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotX;
		float ROTY = m_aInfo[id].aKeyInfo[keynext].aKey[nCntModel].fRotY - m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotY;
		float ROTZ = m_aInfo[id].aKeyInfo[keynext].aKey[nCntModel].fRotZ - m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotZ;

		//パーツの位置・向きを設定
		pModel->SetPos(D3DXVECTOR3
		(
			pModel->GetStartPos().x + m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosX + POSX * (count / ((float)m_aInfo[id].aKeyInfo[key].nFrame)),
			pModel->GetStartPos().y + m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosY + POSY * (count / ((float)m_aInfo[id].aKeyInfo[key].nFrame)),
			pModel->GetStartPos().z + m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosZ + POSZ * (count / ((float)m_aInfo[id].aKeyInfo[key].nFrame))
		));

		pModel->SetRot(D3DXVECTOR3
		(
			pModel->GetStartRot().x + m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotX + ROTX * (count / ((float)m_aInfo[id].aKeyInfo[key].nFrame)),
			pModel->GetStartRot().y + m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotY + ROTY * (count / ((float)m_aInfo[id].aKeyInfo[key].nFrame)),
			pModel->GetStartRot().z + m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotZ + ROTZ * (count / ((float)m_aInfo[id].aKeyInfo[key].nFrame))
		));
	}

	// 現在の再生カウントを加算
	pInfo->fCounter += CManager::GetInstance()->GetGameSpeed() * m_fSlowVaule;

	// 次のキーに移動
	int frame = m_aInfo[id].aKeyInfo[key].nFrame;
	if (pInfo->fCounter >= static_cast<int>(frame))
	{
		// カウントリセット
		pInfo->fCounter = 0.0f;

		// カウントを次に進める
		pInfo->nKey = (pInfo->nKey + 1) % m_aInfo[pInfo->nId].NumKey;
		pInfo->nKeyNext = (pInfo->nKey + 1) % m_aInfo[pInfo->nId].NumKey;

		// 再生未完了
		if (pInfo->nKey != 0 && pInfo->nKeyNext != 0) { return; }

		// ループしない場合再生終了
		if (m_aInfo[id].bLoop) { return; }

		pInfo->bFinish = true;
	}
}

//====================================================================
// ブレンド再生
//====================================================================
void CMotion::PlayTypeBlend()
{
	SPlayInfo* pInfo = &m_PlayInfo;		// 再生情報のポインタ
	SBodyData* pBody = &m_BodyData;		// 階層データのポインタ
	SBlendInfo* pBlend = &pInfo->blendInfo;	// ブレンド情報のポインタ

	if (m_BodyData.ppModel == nullptr) { return; }

	// ローカルに格納
	int id = pInfo->nId;				// 再生中モーション番号
	int key = pInfo->nKey;				// 再生中キー番号
	int blendid = pBlend->nOldId;		// ブレンド用モーション番号
	int blendkey = pBlend->nOldKey;		// ブレンド用キー番号

	for (int nCntModel = 0; nCntModel < pBody->nNumModel; nCntModel++)
	{
		CModel* pModel = pBody->ppModel[nCntModel];
		//キーの情報から位置・向きを算出
		float POSX = m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosX - m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fPosX;
		float POSY = m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosY - m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fPosY;
		float POSZ = m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosZ - m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fPosZ;
		float ROTX = m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotX - m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fRotX;
		float ROTY = m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotY - m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fRotY;
		float ROTZ = m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotZ - m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fRotZ;

		//パーツの位置・向きを設定
		pModel->SetPos(D3DXVECTOR3
		(

			pModel->GetStartPos().x + m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fPosX + POSX * (pBlend->fCounter / pBlend->fTimeMax),
			pModel->GetStartPos().y + m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fPosY + POSY * (pBlend->fCounter / pBlend->fTimeMax),
			pModel->GetStartPos().z + m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fPosZ + POSZ * (pBlend->fCounter / pBlend->fTimeMax)
		));

		pModel->SetRot(D3DXVECTOR3
		(
			pModel->GetStartRot().x + m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fRotX + ROTX * (pBlend->fCounter / pBlend->fTimeMax),
			pModel->GetStartRot().y + m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fRotY + ROTY * (pBlend->fCounter / pBlend->fTimeMax),
			pModel->GetStartRot().z + m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fRotZ + ROTZ * (pBlend->fCounter / pBlend->fTimeMax)
		));
	}

	// 現在のカウント加算、タイマー減算
	pBlend->fCounter += CManager::GetInstance()->GetGameSpeed() * m_fSlowVaule;
	pBlend->fTime -= CManager::GetInstance()->GetGameSpeed() * m_fSlowVaule;

	// ブレンド終了後デフォルトに戻す
	if (pBlend->fTime <= 0.0f)
	{
		m_PlayInfo.type = PLAYTYPE::PLAYTYPE_NORMAL;
	}
}

//====================================================================
// ループ取得
//====================================================================
bool CMotion::GetInfoLoop(int nMotion)
{
	// 範囲外
	if (nMotion < 0 || nMotion >= MAX_INFO)
	{
		assert(false);
	}

	return m_aInfo[nMotion].bLoop;
}

//====================================================================
// キー総数取得
//====================================================================
int CMotion::GetInfoNumKey(int nMotion)
{
	// 範囲外
	if (nMotion < 0 || nMotion >= MAX_INFO)
	{
		assert(false);
	}

	return m_aInfo[nMotion].NumKey;
}

//====================================================================
// 指定キーのフレーム数取得
//====================================================================
int CMotion::GetInfoKeyFrame(int nMotion, int nKey)
{
	// 範囲外
	if (nMotion < 0 || nMotion >= MAX_INFO)
	{
		assert(false);
	};

	// 範囲外
	if (nKey < 0 || nKey >= MAX_KEY)
	{
		assert(false);
	};

	return m_aInfo[nMotion].aKeyInfo[nKey].nFrame;
}
