//========================================
//
//モーション[motion.cpp]
//Author：森川駿弥
//
//========================================
#include "motion.h"
#include "debugproc.h"
#include "renderer.h"
#include "manager.h"

//========================================
//コンストラクタ
//========================================
CMotion::CMotion() :
	m_pParent(nullptr),	// 親モデルのポインタ
	m_nNumAll(0),		// モーションの総数
	m_nType(0),			// モーション種類
	m_nKey(0),			// 現在のキー数
	m_nCounter(0),		// フレームカウンター
	m_bFinish(true),	// 終了したかどうか
	m_nNumModel(0),		// モデルの総数
	m_nNumMotion(0),	// 現在のモーション
	m_nNumParts(0),		// パーツ数
	m_nPartsIndex(0),	// パーツ番号
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{//値クリア
	memset(&m_aInfo[0], 0, sizeof(m_aInfo));		//モーション情報
	memset(&m_ppModel[0], 0, sizeof(m_ppModel));	//モデル情報
}

//========================================
//デストラクタ
//========================================
CMotion::~CMotion()
{	
}

//========================================
//生成
//========================================
CMotion *CMotion::Create(void)
{
	//CMotion型のポインタ
	CMotion *pMotion = nullptr;

	if (pMotion == nullptr)
	{//nullptrの時
		//インスタンス生成
		pMotion = new CMotion;

		//初期化
		pMotion->Init();
	}

	//ポインタを返す
	return pMotion;
}

//========================================
//初期化
//========================================
HRESULT CMotion::Init(void)
{
	//各値の初期値設定
	memset(&m_aInfo[0], 0, sizeof(m_aInfo));		// モーション情報
	m_nNumAll = 0;				// モーション総数
	m_nType = 0;				// モーション種類
	m_nKey = 0;					// 現在のキーナンバー
	m_nCounter = 0;				// モーションカウンター
	m_nNumMotion = 0;			// 現在のモーション
	m_nNumParts = 0;			// パーツ数
	m_bFinish = true;			// 終了したかどうか

	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{// ダブルポインタの初期化
		m_ppModel[nCntModel] = nullptr;
	}

	// モデルの総数
	m_nNumModel = 0;	

	//成功を返す
	return S_OK;
}

//========================================
//終了
//========================================
void CMotion::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{// ダブルポインタの初期化
		if (m_ppModel[nCntModel] != nullptr)
		{
			m_ppModel[nCntModel]->Uninit();
			delete m_ppModel[nCntModel];
			m_ppModel[nCntModel] = nullptr;
		}
	}
}

//========================================
//更新
//========================================
void CMotion::Update(void)
{
	//目的の位置
	D3DXVECTOR3 diffPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//目的の向き
	D3DXVECTOR3 diffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//次の値
	int nNextKey = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;

	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{//パーツの最大分回す
		//モデルのポインタ
		CModel *pModel = m_ppModel[nCntParts];

		//位置取得
		D3DXVECTOR3 pos = pModel->GetPosition();

		//位置取得
		D3DXVECTOR3 rot = pModel->GetRot();

		//位置を算出
		diffPos = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntParts].pos - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].pos;

		//向きを算出
		diffRot = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntParts].rot - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].rot;

		//位置
		pos = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].pos
				+ diffPos
				* ((float)m_nCounter
				/ (float)m_aInfo[m_nType].aKeyInfo[nNextKey].nFrame);

		//向き
		rot = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].rot
				+ diffRot
				* ((float)m_nCounter 
				/ (float)m_aInfo[m_nType].aKeyInfo[nNextKey].nFrame);

		//位置反映
		pModel->SetPosition(pos + pModel->GetPos());

		//向き反映
		pModel->SetRot(rot + pModel->GetRotate());
	}

	if (m_nCounter >= m_aInfo[m_nType].aKeyInfo[nNextKey].nFrame)
	{// カウンターが再生フレームを超えたら
		if (m_aInfo[m_nType].bLoop == true)
		{// モーションがループの時
			// モーションカウンターリセット
			m_nCounter = 0;

			// 現在のキー加算
			m_nKey = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;
		}
		else
		{// ループしない時
			if (m_nKey < m_aInfo[m_nType].nNumKey)
			{// キーの総数が現在のキーを超えたら
				// モーションカウンターリセット
				m_nCounter = 0;

				// キー加算
				m_nKey++;
			}
			else
			{// 超えてないとき
				// モーションを終了する
				m_bFinish = true;
			}
		}
	}
	else
	{// カウンターが再生フレームを超えてなかったら
		// 現在のキーを1つ進める
		m_nCounter++;
	}
}

//========================================
//描画
//========================================
void CMotion::Draw(void)
{
	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{//モデルの描画(全パーツ)
 		if (m_ppModel[nCntParts] != nullptr)
		{//m_apModelがnullptrの時
			m_ppModel[nCntParts]->Draw();
		}
	}
}

//========================================
//モーション種類の設定
//========================================
void CMotion::SetInfo(int nType)
{
	m_nType = nType;
}

//========================================
//設定
//========================================
void CMotion::Set(int nType)
{
	if (!m_aInfo[m_nType].bLoop && !m_bFinish)
	{
		return;
	}

	if (nType != m_nType)
	{// 現在のモーションと違ったら
		m_nType = nType;

		// キーリセット
		m_nKey = 0;

		m_nCounter = 0;

		// 終了状況をfalseにする
		m_bFinish = false;

		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{// モデルの総数分回す
			//位置
			m_ppModel[nCnt]->SetPosition(m_aInfo[nType].aKeyInfo[m_nKey].aKey[nCnt].pos);

			//向き
			m_ppModel[nCnt]->SetRot(m_aInfo[nType].aKeyInfo[m_nKey].aKey[nCnt].rot);
		}
	}
}

//========================================
//モデル設定
//========================================
void CMotion::SetModel(CModel *ppModel, int nNumModel)
{
	//ポインタ
	m_ppModel[nNumModel] = ppModel;

	//パーツ番号
	m_nNumModel++;
}

//========================================
//読み込み
//========================================
void CMotion::Load(std::string pfile)
{
	//変数宣言
	char garbage[640];		// 不要文字格納用
	char FileName[64];		// 各モデルのファイル名
	float move = 0.0f;		// 移動量
	int nIdx = 0;			// 何番目のパーツか
	int nParent = 0;		// 親子関係
	int nNum = 0;			// 読み込む番号
	int nMotion = 0;		// 何番目のモーションか
	int nCntKey = 0;		// 何個目のキーか
	int nLoop = 0;			// ループするかしないか
	int KeyCount = 0;		// モーションのキー数

	//FILEのポインタ
	FILE *pFile = nullptr;

	//ファイルを開く
	pFile = fopen(pfile.c_str(), "r");

	while (1)
	{//END_SCRIPTが来るまで繰り返す
		//不要な文字を捨てる
		fscanf(pFile, "%s", &garbage[0]);

		if (strcmp(&garbage[0], "END_SCRIPT") == 0)
		{//文字列がEND_SCRIPTのとき
			break;
		}

		//========================================
		//モデルファイルの読み込み
		//========================================
		if (strcmp(&garbage[0], "MODEL_FILENAME") == 0)
		{//文字列がMODEL_FILENAMEのとき
			//=捨てる
			fscanf(pFile, "%s", &garbage[0]);

			//ファイルの名前取得
			fscanf(pFile, "%s", &FileName[0]);

			//モデルの生成
			m_ppModel[nNum] = CModel::Create(&FileName[0]);

			//モデル設定
			SetModel(m_ppModel[nNum], nNum);

			//モデルの番号を加算する
			nNum++;
		}

		//========================================
		//モデル設定
		//========================================
		if (strcmp(&garbage[0], "CHARACTERSET") == 0)
		{//文字列がCHARACTERSETのとき
			while (1)
			{//文字列がEND_CHARACTERSETになるまで繰り返す
				fscanf(pFile, "%s", &garbage[0]);

				if (strcmp(&garbage[0], "END_CHARACTERSET") == 0)
				{//文字列がEND_CHARACTERSETの時
					break;
				}

				if (strcmp(&garbage[0], "NUM_PARTS") == 0)
				{//文字列がNUM_PARTSの時
					//=捨てる
					fscanf(pFile, "%s", &garbage[0]);

					//パーツ数を入れる
					fscanf(pFile, "%d", &m_nNumParts);
				}

				if (strcmp(&garbage[0], "PARTSSET") == 0)
				{//文字列がPARTSSETの時
					while (1)
					{//END_PARTSSETが来るまで回す
						fscanf(pFile, "%s", &garbage[0]);

						if (strcmp(&garbage[0], "END_PARTSSET") == 0)
						{//文字列がEND_PARTSSETの時
							//パーツ数を加算
							m_nPartsIndex++;

							break;
						}

						if (strcmp(&garbage[0], "INDEX") == 0)
						{//文字列がINDEXの時
							//=捨てる
							fscanf(pFile, "%s", &garbage[0]);

							//何番目のパーツかの設定
							fscanf(pFile, "%d", &nIdx);

							//モデルの番号設定
							m_ppModel[m_nPartsIndex]->SetIndex(m_ppModel[nIdx]);
						}

						if (strcmp(&garbage[0], "PARENT") == 0)
						{//文字列がPARENTの時
							//=捨てる
							fscanf(pFile, "%s", &garbage[0]);

							//親子関係設定
							fscanf(pFile, "%d", &nParent);

							if (nParent == -1)
							{//親パーツが無い場合
								m_ppModel[m_nPartsIndex]->SetParent(nullptr);
							}
							else
							{//親パーツがある場合
								m_ppModel[m_nPartsIndex]->SetParent(m_ppModel[nParent]);
							}
						}

						if (strcmp(&garbage[0], "POS") == 0)
						{//文字列がPARENTの時
							//=捨てる
							fscanf(pFile, "%s", &garbage[0]);

							fscanf(pFile, "%f", &m_pos.x);
							fscanf(pFile, "%f", &m_pos.y);
							fscanf(pFile, "%f", &m_pos.z);

							//位置を設定
							m_ppModel[m_nPartsIndex]->SetPos(m_pos);
						}

						if (strcmp(&garbage[0], "ROT") == 0)
						{//文字列がPARENTの時
							//=捨てる
							fscanf(pFile, "%s", &garbage[0]);

							fscanf(pFile, "%f", &m_rot.x);
							fscanf(pFile, "%f", &m_rot.y);
							fscanf(pFile, "%f", &m_rot.z);

							//向きを設定
							m_ppModel[m_nPartsIndex]->SetRotate(m_rot);
						}
					}
				}
			}
		}

		//========================================
		//モーション設定
		//========================================
		if (strcmp(&garbage[0], "MOTIONSET") == 0)
		{//文字列がMOTIONSETの時
			while (1)
			{//END_PARTSSETが来るまで回す
				fscanf(pFile, "%s", &garbage[0]);

				if (strcmp(&garbage[0], "END_MOTIONSET") == 0)
				{//文字列がEND_PARTSSETの時
					// モーションの総数を加算
					m_nNumAll++;

					// 現在のキーリセット
					m_nNowKey = 0;

					break;
				}

				if (strcmp(&garbage[0], "LOOP") == 0)
				{//文字列がINDEXの時
					//=捨てる
					fscanf(pFile, "%s", &garbage[0]);

					//親子関係設定
					fscanf(pFile, "%d", &nLoop);

					// ループするかしないか
					if (nLoop == 0)
					{
						m_aInfo[m_nNumAll].bLoop = false;
					}
					if (nLoop == 1)
					{
						m_aInfo[m_nNumAll].bLoop = true;
					}
				}

				if (strcmp(&garbage[0], "NUM_KEY") == 0)
				{//文字列がNUM_KEYの時
					//=捨てる
					fscanf(pFile, "%s", &garbage[0]);

					//キーの番号
					fscanf(pFile, "%d", &m_aInfo[m_nNumAll].nNumKey);
				}

				if (strcmp(&garbage[0], "KEYSET") == 0)
				{//文字列がPARENTの時
					while (1)
					{//END_KEYSETが来るまで繰り返す
						//=捨てる
						fscanf(pFile, "%s", &garbage[0]);

						if (strcmp(&garbage[0], "END_KEYSET") == 0)
						{//文字列がEND_PARTSSETの時
							//加算する
							m_nNowKey++;

							// カウントリセット
							KeyCount = 0;

							break;
						}

						if (strcmp(&garbage[0], "FRAME") == 0)
						{//文字列がFRAMEの時
							//=捨てる
							fscanf(pFile, "%s", &garbage[0]);

							//フレーム数
							fscanf(pFile, "%d", &m_aInfo[m_nNumAll].aKeyInfo[m_nNowKey].nFrame);
						}

						if (strcmp(&garbage[0], "KEY") == 0)
						{//文字列がKEYの時
							while (1)
							{//END_KEYが来るまで繰り返す
								//=捨てる
								fscanf(pFile, "%s", &garbage[0]);

								if (strcmp(&garbage[0], "END_KEY") == 0)
								{//文字列がEND_KEYの時

									// カウント加算する
									KeyCount++;

									break;
								}

								if (strcmp(&garbage[0], "POS") == 0)
								{//文字列がPOSの時
									//=捨てる
									fscanf(pFile, "%s", &garbage[0]);

									fscanf(pFile, "%f", &m_pos.x);
									fscanf(pFile, "%f", &m_pos.y);
									fscanf(pFile, "%f", &m_pos.z);

									KEY* pKey = &m_aInfo[m_nNumAll].aKeyInfo[m_nNowKey].aKey[KeyCount];

									//位置を設定
									pKey->pos = m_pos;
								}

								if (strcmp(&garbage[0], "ROT") == 0)
								{//文字列がROTの時
									//=捨てる
									fscanf(pFile, "%s", &garbage[0]);

									fscanf(pFile, "%f", &m_rot.x);
									fscanf(pFile, "%f", &m_rot.y);
									fscanf(pFile, "%f", &m_rot.z);

									KEY* pKey = &m_aInfo[m_nNumAll].aKeyInfo[m_nNowKey].aKey[KeyCount];

									//向きを設定
									pKey->rot = m_rot;
								}
							}
						}
					}
				}
			}
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}
