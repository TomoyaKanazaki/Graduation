//============================================
//
//	キャラクターの処理 [character.cpp]
//	Author:sakai minato
//
//============================================

#include "objectcharacter.h"

#include "characterManager.h"

#include "manager.h"
#include "renderer.h"

#include "model.h"
#include "motion.h"

#include "game.h"

#include "objmeshField.h"

#include "shadow.h"
#include "mask.h"

#include "move.h"

//====================================================================
// 定数定義
//====================================================================
namespace
{
	const float SHADOW_SIZE = 100.0f;			// 丸影の大きさ
}

//====================================================================
//コンストラクタ
//====================================================================
CObjectCharacter::CObjectCharacter(int nPriority) : CObject(nPriority),
m_pShadow(nullptr),
m_bUseShadow(true),
m_State(STATE_WAIT),
m_OldState(STATE_WAIT),
m_Info(SInfo()),
m_BodyInfo(SBodyData()),
m_SpeedState(CMapMove::SPEED_NONE),
m_OldSpeedState(CMapMove::SPEED_NONE)
{
	m_pMotion = nullptr;
	m_nCharacterNum = -1;

	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;

	m_UseMultiMatrix = nullptr;

	m_bUseShadowMtx = false;

	m_pMoveState = nullptr;

	m_Grid.x = 0;
	m_Grid.z = 0;
	m_GridOld.x = 0;
	m_GridOld.z = 0;

	// 進行許可状況
	m_Progress.bOKD = true;
	m_Progress.bOKL = true;
	m_Progress.bOKR = true;
	m_Progress.bOKU = true;

	m_bGritCenter = true;
}

//====================================================================
//デストラクタ
//====================================================================
CObjectCharacter::~CObjectCharacter()
{

}

//====================================================================
// 生成処理
//====================================================================
CObjectCharacter* CObjectCharacter::Create(bool bShadow)
{
	// オブジェクトの生成処理
	CObjectCharacter* pInstance = new CObjectCharacter();

	// シャドウの有無
	pInstance->SetShadow(bShadow);

	// オブジェクトの初期化処理
	if (FAILED(pInstance->Init()))
	{// 初期化処理が失敗した場合
		return nullptr;
	}

	return pInstance;
}

//====================================================================
// 初期化処理
//====================================================================
HRESULT CObjectCharacter::Init(void)
{
	if (m_pShadow == nullptr && m_bUseShadow)
	{// 影生成
		m_pShadow = CShadow::Create(m_Info.pos, D3DXVECTOR3(SHADOW_SIZE, 0.0f, SHADOW_SIZE));
	}

	if (m_pMoveState == nullptr)
	{ // 移動状態設定
		m_pMoveState = new CStateStop();		// 停止状態
	}

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CObjectCharacter::Uninit(void)
{
	if (m_BodyInfo.ppModel != nullptr)
	{
		for (int i = 0; i < m_BodyInfo.nNumModel; i++)
		{
			CModel* pModel = m_BodyInfo.ppModel[i];
			if (pModel == nullptr) { continue; }
			pModel->Uninit();
			delete pModel;
		}

		delete[] m_BodyInfo.ppModel;
		m_BodyInfo.ppModel = nullptr;
	}

	// 影の終了
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	//モーションの終了処理
	if (m_pMotion != nullptr)
	{
		//モーションの破棄
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// 移動状態の破棄
	if (m_pMoveState != nullptr)
	{
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CObjectCharacter::Update(void)
{
	m_OldSpeedState = m_SpeedState;
	D3DXVECTOR3 pos = GetPos();

	//加減速状態の管理
	SpeedStateManager();

	if (m_pMotion != nullptr)
	{
		//モーションの更新
		m_pMotion->Update();
	}

	if (m_pShadow != nullptr)
	{// シャドウの更新
		m_pShadow->SetPos(D3DXVECTOR3(m_Info.pos.x, 1.0f, m_Info.pos.z));
		m_pShadow->SetBaseHeight(pos.y);
	}

	// 状態の保存
	m_OldState = m_State;
}

//====================================================================
//描画処理
//====================================================================
void CObjectCharacter::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_bUseShadowMtx == true)
	{
		D3DXMATRIX mtxShadow;	// シャドウマトリックス
		D3DLIGHT9 light;		// ライト情報
		D3DXVECTOR4 posLight;	// ライトの位置
		D3DXVECTOR3 pos, normal;	// 平面上の任意の点、法線ベクトル
		D3DXPLANE plane;		// 平面情報

		// ライトの位置設定
		pDevice->GetLight(0, &light);
		posLight = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.05f);

		// 平面情報を生成
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXPlaneFromPointNormal(&plane, &pos, &normal);

		// シャドウマトリックスの初期化


		// シャドウマトリックスの作成
		D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
		D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

		// シャドウマトリックスの設定
	}

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_UseMultiMatrix != nullptr)
	{
		//算出したマトリクスをかけ合わせる
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			m_UseMultiMatrix);
	}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

//モデルの描画(全パーツ)
	if (m_BodyInfo.ppModel != nullptr)
	{
		for (int i = 0; i < m_BodyInfo.nNumModel; i++)
		{
			CModel* pModel = m_BodyInfo.ppModel[i];
			if (pModel == nullptr) { continue; }
			pModel->Draw();
		}
	}
}

//==========================================
// 加減速状態の管理
//==========================================
void CObjectCharacter::SpeedStateManager()
{
	switch (m_SpeedState)
	{
	case CMapMove::SPEED_UP:	//加速状態

		DebugProc::Print(DebugProc::POINT_RIGHT, "加速\n");

		break;

	case CMapMove::SPEED_DOWN:	//減速状態

		DebugProc::Print(DebugProc::POINT_RIGHT, "減速\n");

		break;

	case CMapMove::SPEED_NONE:	//通常状態
		break;
	}
}

//==========================================
// 移動状態変更処理
//==========================================
void CObjectCharacter::ChangeMoveState(CMoveState* pMoveState)
{
	if (m_pMoveState != nullptr)
	{
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	m_pMoveState = pMoveState;
	m_pMoveState->Init();
}

//==========================================
// 移動状態変更処理
//==========================================
void CObjectCharacter::SetNumModel(int nNumModel)
{
	// 既に使用済みなら削除
	if (m_BodyInfo.ppModel != nullptr)
	{
		for (int i = 0; i < m_BodyInfo.nNumModel; i++)
		{
			CModel* pModel = m_BodyInfo.ppModel[i];
			if (pModel == nullptr) { continue; }
			pModel->Uninit();
			delete pModel;
		}

		delete[] m_BodyInfo.ppModel;
		m_BodyInfo.ppModel = nullptr;
	}

	// 新たなデータ数を設定
	m_BodyInfo.nNumModel = nNumModel;

	// ポインタ生成
	if (m_BodyInfo.ppModel == nullptr)
	{
		m_BodyInfo.ppModel = new CModel * [m_BodyInfo.nNumModel];

		// null入れる
		for (int i = 0; i < m_BodyInfo.nNumModel; i++)
		{
			m_BodyInfo.ppModel[i] = nullptr;
		}
	}
}


//====================================================================
// キャラクターテキスト設定処理
//====================================================================
void CObjectCharacter::SetTxtCharacter(const std::string pFilename)
{
	CCharacterManager* pCharacterManager = CManager::GetInstance()->GetCharacterManager();

	if (pCharacterManager == nullptr)
	{
		assert(("キャラクター管理クラスがない", false));
		return;
	}

	// キャラクター割当
	m_nCharacterNum = pCharacterManager->Regist(this, pFilename);

	// キャラクター番号正常判定
	if (m_nCharacterNum == -1)
	{
		assert(("キャラクター割当失敗", false));
		return;
	}
}

//====================================================================
// モデル設定処理
//====================================================================
void CObjectCharacter::SetModel(CModel* pModel, int nCnt)
{
	// モデルのダブルポインタ
	CModel** ppModel = m_BodyInfo.ppModel;

	// 生成されていない
	if (ppModel == nullptr)
	{
		assert(("モデル生成していない", false));
	}

	if (nCnt < 0 || nCnt >= m_BodyInfo.nNumModel)
	{
		assert(("モデル生成していない", false));

	}

	if (ppModel[nCnt] == nullptr)
	{
		ppModel[nCnt] = pModel;
	}
	else
	{
		assert(("モデル上書き生成", false));
		return;
	}
}

//====================================================================
// モデル取得処理
//====================================================================
CModel* CObjectCharacter::GetModel(int nCnt)
{
	// モデルのダブルポインタ
	CModel** ppModel = m_BodyInfo.ppModel;

	// 生成されていない
	if (ppModel == nullptr)
	{
		assert(("モデル生成していない", false));
	}

	if (nCnt < 0 || nCnt >= m_BodyInfo.nNumModel)
	{
		assert(("モデル生成していない", false));

	}

	if (ppModel[nCnt] != nullptr)
	{
		return ppModel[nCnt];
	}

	assert(("モデル取得失敗", false));

	return nullptr;
}


//====================================================================
// モデル取得処理
//====================================================================
CModel** CObjectCharacter::GetModel(void)
{
	// モデルのダブルポインタ
	CModel** ppModel = m_BodyInfo.ppModel;

	if (ppModel != nullptr)
	{
		return ppModel;
	}

	assert(("モデル取得失敗", false));

	return nullptr;
}


//====================================================================
// モーション設定処理
//====================================================================
void CObjectCharacter::SetMotion(CMotion* pMotion)
{
	if (m_pMotion == nullptr)
	{
		m_pMotion = pMotion;
	}
	else
	{
		assert(("モーション上書き生成", false));
		return;
	}
}

//====================================================================
// モーション取得処理
//====================================================================
CMotion* CObjectCharacter::GetMotion(void)
{
	if (m_pMotion != nullptr)
	{
		return m_pMotion;
	}

	assert(("モーション取得失敗", false));

	return nullptr;
}

//====================================================================
// モデルロード処理
//====================================================================
void CObjectCharacter::LoadModel(const std::string pFilename)
{
	//ファイルを開く
	FILE* pFile = fopen(pFilename.c_str(), "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合
		int ModelParent = 0;
		D3DXVECTOR3 ModelPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 ModelRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		char ModelName[128] = {};
		int nCntModel = 0;

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
						fscanf(pFile, "%d", &m_BodyInfo.nNumModel);		//モデル数の設定

						// ポインタ生成
						if (m_BodyInfo.ppModel == nullptr)
						{
							m_BodyInfo.ppModel = new CModel * [m_BodyInfo.nNumModel];

							// null入れる
							for (int i = 0; i < m_BodyInfo.nNumModel; i++)
							{
								m_BodyInfo.ppModel[i] = nullptr;
							}
						}

						break;
					}
				}

				//モデルファイルの読み込み
				while (1)
				{//「MODEL_FILENAME」を探す

					// モデルのダブルポインタ
					CModel** ppModel = m_BodyInfo.ppModel;

					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%s", &ModelName[0]);		//読み込むモデルのパスを取得

						ppModel[nCntModel] = CModel::Create(&ModelName[0]);
						ppModel[nCntModel]->SetColorType(CModel::COLORTYPE_FALSE);
						nCntModel++;
					}
					if (nCntModel >= m_BodyInfo.nNumModel)
					{
						nCntModel = 0;
						break;
					}
				}

				// キャラクター情報読み込み-----------------------------------------------------
				while (1)
				{//「PARTSSET」を探す

					// モデルのダブルポインタ
					CModel** ppModel = m_BodyInfo.ppModel;

					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "PARTSSET") == 0)
					{
						while (1)
						{//各種変数を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "INDEX") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &nCntModel);	//インデックスを設定
							}
							if (strcmp(&aMessage[0], "PARENT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParent);	//親モデルのインデックスを設定

								if (ModelParent == -1)
								{
									ppModel[nCntModel]->SetParent(nullptr);
								}
								else
								{
									ppModel[nCntModel]->SetParent(ppModel[ModelParent]);
								}
							}
							if (strcmp(&aMessage[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelPos.x);				//位置(オフセット)の初期設定
								fscanf(pFile, "%f", &ModelPos.y);				//位置(オフセット)の初期設定
								fscanf(pFile, "%f", &ModelPos.z);				//位置(オフセット)の初期設定

								ppModel[nCntModel]->SetPos(ModelPos);
								ppModel[nCntModel]->SetStartPos(ModelPos);
							}
							if (strcmp(&aMessage[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelRot.x);				////向きの初期設定
								fscanf(pFile, "%f", &ModelRot.y);				////向きの初期設定
								fscanf(pFile, "%f", &ModelRot.z);				////向きの初期設定

								ppModel[nCntModel]->SetRot(ModelRot);
								ppModel[nCntModel]->SetStartRot(ModelRot);
							}
							if (strcmp(&aMessage[0], "END_PARTSSET") == 0)
							{
								break;
							}
						}
						nCntModel++;
						if (nCntModel >= m_BodyInfo.nNumModel)
						{
							break;
						}
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
// 指定モデルカラー変更
//====================================================================
void CObjectCharacter::SetModelColor(CModel::COLORTYPE Type, const D3DXCOLOR& Col)
{
	// モデルのダブルポインタ
	CModel** ppModel = m_BodyInfo.ppModel;

	// 生成されていない
	if (ppModel == nullptr)
	{
		assert(("モデル生成していない", false));
	}

	// モデル数の取得
	int nNumModel = GetNumModel();

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// モデルの取得
		CModel* pModel = ppModel[nCnt];

		if (pModel != nullptr)
		{
			pModel->SetColorType(Type);
			pModel->SetColor(Col);
		}
	}
}