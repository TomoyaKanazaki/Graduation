//============================================
//
//	キャラクターの処理 [character.cpp]
//	Author:sakai minato
//
//============================================

#include "character.h"

#include "manager.h"
#include "renderer.h"

#include "model.h"
#include "motion.h"

#include "game.h"

#include "objmeshField.h"

#include "shadow.h"

//====================================================================
// 定数定義
//====================================================================
namespace
{
	const float SHADOW_SIZE = 50.0f;			// 丸影の大きさ
}

//====================================================================
//コンストラクタ
//====================================================================
CCharacter::CCharacter(int nPriority) : CObject(nPriority),
m_pShadow(nullptr)
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
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = INITVECTOR3;

	m_UseMultiMatrix = nullptr;

	m_bUseStencil = false;
	m_bUseShadowMtx = false;
}


//====================================================================
//デストラクタ
//====================================================================
CCharacter::~CCharacter()
{

}

//====================================================================
//生成処理
//====================================================================
CCharacter* CCharacter::Create(const char* pModelName)
{
	CCharacter* pInstance = new CCharacter();

	//オブジェクトの初期化処理
	if (FAILED(pInstance->Init(pModelName)))
	{//初期化処理が失敗した場合

		// アサート
		assert(("キャラクタークラスの生成失敗", false));

		return nullptr;
	}

	return pInstance;
}

//====================================================================
// 初期化処理（継承以外での初期化処理）
//====================================================================
HRESULT CCharacter::Init(const char* pModelName)
{
	strcpy(&m_aModelName[0], pModelName);

	//モデルの生成
	LoadModel(pModelName);

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
		m_pMotion->LoadData(pModelName);
	}

	if (m_pShadow == nullptr)
	{// 影生成
		m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 1.0f, m_pos.z), SHADOW_SIZE, SHADOW_SIZE);
	}

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CCharacter::Uninit(void)
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

	//モーションの終了処理
	if (m_pMotion != nullptr)
	{
		//モーションの破棄
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	if (m_pShadow != nullptr)
	{// シャドウの破棄
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CCharacter::Update(void)
{
	if (m_pMotion != nullptr)
	{
		//モーションの更新
		m_pMotion->Update();
	}

	if (m_pShadow != nullptr)
	{// シャドウの更新
		m_pShadow->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + 1.0f, m_pos.z));
	}
}

//====================================================================
//描画処理
//====================================================================
void CCharacter::Draw(void)
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

	if (m_pShadow != nullptr)
	{// シャドウの描画
		m_pShadow->Draw();
	}

	//ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//====================================================================
// キャラクターテキスト設定処理
//====================================================================
void CCharacter::SetTxtCharacter(const char* pFilename)
{
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
}

//====================================================================
// モデル取得処理
//====================================================================
CModel* CCharacter::GetModel(int nCnt)
{
	if (m_apModel[nCnt] != nullptr)
	{
		return m_apModel[nCnt];
	}

	assert(("モデル取得失敗", false));

	return nullptr;
}

//====================================================================
// モーション取得処理
//====================================================================
CMotion* CCharacter::GetMotion(void)
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
void CCharacter::LoadModel(const char* pFilename)
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
		int nCntParts = 0;
		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		char aString[128] = {};				//ゴミ箱
		char aMessage[128] = {};			//スタートとエンドのメッセージ
		char aBool[128] = {};				//bool変換用メッセージ

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