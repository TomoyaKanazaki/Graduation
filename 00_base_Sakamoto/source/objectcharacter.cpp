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
m_OldState(STATE_WAIT)
{
	for (int nCnt = 0; nCnt < MODEL_NUM; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
		m_aModelName[nCnt] = {};
	}

	m_pMotion = nullptr;
	m_nNumModel = 0;

	m_pos = INITVECTOR3;
	m_posOld = INITVECTOR3;
	m_move = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_size = INITVECTOR3;
	m_Objmove = INITVECTOR3;

	m_UseMultiMatrix = nullptr;

	m_bUseStencil = false;
	m_bUseShadowMtx = false;

	m_pMoveState = nullptr;

	m_Grid.x = 0;
	m_Grid.z = 0;

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
		m_pShadow = CShadow::Create(m_pos, D3DXVECTOR3(SHADOW_SIZE, 0.0f, SHADOW_SIZE));
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
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != nullptr)
		{
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = nullptr;
		}
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
	D3DXVECTOR3 pos = GetPos();

	if (m_pMotion != nullptr)
	{
		//モーションの更新
		m_pMotion->Update();
	}

	if (m_pShadow != nullptr)
	{// シャドウの更新
		m_pShadow->SetPos(D3DXVECTOR3(m_pos.x, 1.0f, m_pos.z));
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
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
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

	if (m_bUseStencil == true)
	{
		//ステンシルバッファ有効
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

		//ステンシルバッファと比較する参照値の設定 => ref
		pDevice->SetRenderState(D3DRS_STENCILREF, 1);

		//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
		pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

		//ステンシルバッファの比較方法 => (参照値 => ステンシルバッファの参照値)なら合格
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

		//ステンシルテスト結果に対しての反映設定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Zテスト・ステンシルテスト成功
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功
	}

	//モデルの描画(全パーツ)
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != nullptr)
		{
			m_apModel[nCntModel]->Draw();
		}
	}

	//ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
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
}

//====================================================================
// キャラクターテキスト設定処理
//====================================================================
void CObjectCharacter::SetTxtCharacter(const char* pFilename)
{
	strcpy(&m_aModelName[0], pFilename);

	CCharacterManager* pCharacterManager = CManager::GetInstance()->GetCharacterManager();

	if (pCharacterManager == nullptr)
	{
		assert(("キャラクター管理クラスがない", false));
		return;
	}

	int nNum = pCharacterManager->Regist(this,pFilename);

#if 0 // 酒井のデバッグ用（モデル読み込みが正常かわかるまでコメントアウト）

	strcpy(&m_aModelName[0], pFilename);

	//モデルの生成
	LoadModel(pFilename);

	//モーションの生成
	if (m_pMotion == nullptr)
	{
		//モーションの生成
		m_pMotion = new CMotion;
	}

	//初期化処理
	if (m_pMotion != nullptr)
	{
		m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
		m_pMotion->LoadData(pFilename);
	}

#endif

}

//====================================================================
// モデル設定処理
//====================================================================
void CObjectCharacter::SetModel(CModel* pModel, int nCnt)
{
	if (m_apModel[nCnt] == nullptr)
	{
		m_apModel[nCnt] = pModel;
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
	if (m_apModel[nCnt] != nullptr)
	{
		return m_apModel[nCnt];
	}

	assert(("モデル取得失敗", false));

	return nullptr;
}

//====================================================================
// モデル取得処理
//====================================================================
CModel** CObjectCharacter::GetModel(void)
{
	if (m_apModel[0] != nullptr)
	{
		return &m_apModel[0];
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
void CObjectCharacter::LoadModel(const char* pFilename)
{
	//ファイルを開く
	FILE* pFile = fopen(pFilename, "r");

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
						fscanf(pFile, "%d", &m_nNumModel);		//モデル数の設定
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
						fscanf(pFile, "%s", &ModelName[0]);		//読み込むモデルのパスを取得

						m_apModel[nCntModel] = CModel::Create(&ModelName[0]);
						m_apModel[nCntModel]->SetColorType(CModel::COLORTYPE_FALSE);
						nCntModel++;
					}
					if (nCntModel >= m_nNumModel)
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
								fscanf(pFile, "%d", &nCntModel);	//インデックスを設定
							}
							if (strcmp(&aMessage[0], "PARENT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParent);	//親モデルのインデックスを設定

								if (ModelParent == -1)
								{
									m_apModel[nCntModel]->SetParent(nullptr);
								}
								else
								{
									m_apModel[nCntModel]->SetParent(m_apModel[ModelParent]);
								}
							}
							if (strcmp(&aMessage[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelPos.x);				//位置(オフセット)の初期設定
								fscanf(pFile, "%f", &ModelPos.y);				//位置(オフセット)の初期設定
								fscanf(pFile, "%f", &ModelPos.z);				//位置(オフセット)の初期設定

								m_apModel[nCntModel]->SetPos(ModelPos);
								m_apModel[nCntModel]->SetStartPos(ModelPos);
							}
							if (strcmp(&aMessage[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelRot.x);				////向きの初期設定
								fscanf(pFile, "%f", &ModelRot.y);				////向きの初期設定
								fscanf(pFile, "%f", &ModelRot.z);				////向きの初期設定

								m_apModel[nCntModel]->SetRot(ModelRot);
								m_apModel[nCntModel]->SetStartRot(ModelRot);
							}
							if (strcmp(&aMessage[0], "END_PARTSSET") == 0)
							{
								break;
							}
						}
						nCntModel++;
						if (nCntModel >= m_nNumModel)
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
void CObjectCharacter::SetModelColor(CModel::COLORTYPE Type, D3DXCOLOR Col)
{
	// モデル数の取得
	int nNumModel = GetNumModel();

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// モデルの取得
		CModel* pModel = GetModel(nCnt);

		if (pModel != nullptr)
		{
			pModel->SetColorType(Type);
			pModel->SetColor(Col);
		}
	}
}