//============================================
//
//	エディット [Edit.cpp]
//	Author:sakamoto kai
//
//============================================
#include "Edit.h"
#include "object.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objmeshCube.h"
#include "CubeBlock.h"
#include "debugproc.h"
#include "MapModel.h"
#include "XModel.h"
#include "CubeEffect.h"
#include "useful.h"

//マクロ定義
namespace
{
	//const char* DATA_BLOCK_NAME("data\\TXT\\STAGE\\Block.txt");
	//const char* DATA_XMODEL_NAME("data\\TXT\\STAGE\\XModel.txt");
	//const char* DATA_STAIR_NAME("data\\TXT\\STAGE\\Stair.txt");
	//const char* DATA_GIMMICK_NAME("data\\TXT\\STAGE\\Gimmick.txt");

	// チュートリアル用
	const char* DATA_BLOCK_NAME("data\\TXT\\STAGE\\Block_Tutorial_000.txt");
	const char* DATA_XMODEL_NAME("data\\TXT\\STAGE\\XModel_Tutorial_000.txt");
	const char* DATA_STAIR_NAME("data\\TXT\\STAGE\\Stair_Tutorial_000.txt");
	const char* DATA_GIMMICK_NAME("data\\TXT\\STAGE\\Gimmick_Tutorial_000.txt");

	// ブロックの初期位置（Y）
	const float BLOCK_INIT_POS_Y[CEdit::BLOCK_INIT_TYPE_MAX] =
	{
		10.0f,
		100.0f,
		100.0f,
	};

	// ブロックの初期サイズ
	const D3DXVECTOR3 BLOCK_INIT_SIZE[CEdit::BLOCK_INIT_TYPE_MAX] =
	{
		D3DXVECTOR3(100.0f, 10.0f,100.0f),
		D3DXVECTOR3(10.0f,100.0f,100.0f),
		D3DXVECTOR3(100.0f,100.0f, 10.0f),
	};

	// ブロックのテクスチャの種類
	const char *BLOCK_TEXTURE_TYPE[CEdit::BLOCK_TEXTURE_MAX] =
	{
		"data\\TEXTURE\\Wood000.png",
		"data\\TEXTURE\\Wood001.png",
		"data\\TEXTURE\\Wood002.png",
		"data\\TEXTURE\\Wood003.png",
	};
}

//静的メンバ変数宣言
CEdit::OBJINFO CEdit::m_aModelInfo[OBJ_HIT_MAX] =
{
	{ "data\\MODEL\\MapObj\\Props\\daruma.x","ダルマ" },
	{ "data\\MODEL\\MapObj\\Props\\kousi001.x","格子" },
	{ "data\\MODEL\\MapObj\\Props\\toro.x","灯籠" },
	{ "data\\MODEL\\MapObj\\Props\\wood.x","木" },
	{ "data\\MODEL\\MapObj\\Props\\andon.x","行灯" },
	{ "data\\MODEL\\MapObj\\Props\\Armor.x","鎧立て" },
	{ "data\\MODEL\\MapObj\\Props\\Barrel.x","樽" },
	{ "data\\MODEL\\MapObj\\Props\\Bucket.x","風呂桶" },
	{ "data\\MODEL\\MapObj\\Props\\castle001.x","城" },
	{ "data\\MODEL\\MapObj\\Props\\FoldingScreen.x","屏風" },
	{ "data\\MODEL\\MapObj\\Props\\Gate.x","門" },
	{ "data\\MODEL\\MapObj\\Props\\HandCandle.x","手燭" },
	{ "data\\MODEL\\MapObj\\Props\\JapaneseCushion.x","座布団" },
	{ "data\\MODEL\\MapObj\\Props\\Kakejiku000.x","掛け軸（鶴）" },
	{ "data\\MODEL\\MapObj\\Props\\Kakejiku001.x","掛け軸（亀）" },
	{ "data\\MODEL\\MapObj\\Props\\Kettle.x","やかん" },
	{ "data\\MODEL\\MapObj\\Props\\LookoutTower.x","見張り台の砦" },
	{ "data\\MODEL\\MapObj\\Props\\SwordRack.x","刀掛け" },
	{ "data\\MODEL\\MapObj\\Props\\SwordWallHanging.x","刀壁掛け" },
	{ "data\\MODEL\\MapObj\\Props\\VaseThin.x","細い壺" },
	{ "data\\MODEL\\MapObj\\Props\\Well.x","井戸" },
	{ "data\\MODEL\\MapObj\\Props\\VaseBig.x","大きい壺" },
	{ "data\\MODEL\\MapObj\\Props\\Pillar.x","柱" },
	{ "data\\MODEL\\MapObj\\Props\\wooddesk.x","木の机" },
	{ "data\\MODEL\\MapObj\\Props\\wood_many_bg.x","たくさんの木があります" },
	{ "data\\MODEL\\MapObj\\Props\\glassdesk.x","ガラスの机" },
	{ "data\\MODEL\\MapObj\\Props\\glassdesk_weapon.x","ガラスの机の中に武器" },
	{ "data\\MODEL\\MapObj\\Props\\Tatami_floor.x","小上がり畳" },
	{ "data\\MODEL\\MapObj\\Props\\rice_bales.x","米俵" },
	{ "data\\MODEL\\MapObj\\Props\\Boss_Fence.x","ボス用柵" },
	{ "data\\MODEL\\MapObj\\Props\\chest.x","棚" },
	{ "data\\MODEL\\MapObj\\Props\\Hearth.x","囲炉裏" },
	{ "data\\MODEL\\MapObj\\Props\\Fusuma000.x","襖（１枚）" },
	{ "data\\MODEL\\MapObj\\Props\\Fusuma001.x","襖（４枚）4（端）" },
	{ "data\\MODEL\\MapObj\\Props\\Fusuma002.x","襖（４枚）3（中）" },
	{ "data\\MODEL\\MapObj\\Props\\Fusuma003.x","襖（４枚）2（中）" },
	{ "data\\MODEL\\MapObj\\Props\\Fusuma004.x","襖（４枚）1（端）" },
	{ "data\\MODEL\\MapObj\\Props\\Kakejiku002.x","掛け軸（鬼）" },
	{ "data\\MODEL\\MapObj\\Props\\Kakejiku003.x","掛け軸（山）" },
	{ "data\\MODEL\\MapObj\\Props\\Torch.x","松明" },
	{ "data\\MODEL\\MapObj\\Props\\oni_kamen.x","鬼の仮面" },
	{ "data\\MODEL\\MapObj\\Props\\Foundation.x","城の土台" },
	{ "data\\MODEL\\MapObj\\Props\\Scaffolding.x","足場" },
	{ "data\\MODEL\\MapObj\\Props\\SupportPost.x","支え柱" },
};

//====================================================================
//コンストラクタ
//====================================================================
CEdit::CEdit()
{
	m_nPressCount = 0;
	m_nBlockNumber = 0;
	m_nModelNumber = 0;
	m_nGimmickNumber = 0;
	m_nBlockTextureIdx = 0;

	m_pEditBlock = nullptr;
	if (m_pEditBlock == nullptr)
	{
		m_pEditBlock = CObjmeshCube::Create();
		m_pEditBlock->SetSize(D3DXVECTOR3(100.0f, 10.0f, 100.0f));
		m_pEditBlock->SetTexture(BLOCK_TEXTURE_TYPE[m_nBlockTextureIdx]);
		m_pEditBlock->SetAppear(false);
	}

	m_pEditModel = nullptr;
	if (m_pEditModel == nullptr)
	{
		m_pEditModel = CMapModel::Create(m_aModelInfo[m_nModelNumber].pFilename);
		m_pEditModel->SetAppear(false);
		AppearCollision();
	}

	m_EditPos = INITVECTOR3;
	m_EditRot = INITVECTOR3;
	m_EditSize = D3DXVECTOR3(100.0f, 10.0f, 100.0f);
	m_EditType = EDITTYPE_MODELHIT;
	MoveWidth = 10.0f;
	m_fRotValue = D3DX_PI * 0.5f;
	m_bModelCollision = true;
	m_bAppearCollision = false;
}

//====================================================================
//デストラクタ
//====================================================================
CEdit::~CEdit()
{

}

//====================================================================
//生成処理
//====================================================================
CEdit* CEdit::Create()
{
	CEdit* pMap = NULL;

	if (pMap == NULL)
	{
		//オブジェクト2Dの生成
		pMap = new CEdit();
	}

	return pMap;
}

//====================================================================
//終了処理
//====================================================================
void CEdit::Uninit(void)
{
	if (m_pEditBlock != nullptr)
	{
		m_pEditBlock->SetDeathFlag(true);
		m_pEditBlock = nullptr;
	}

	if (m_pEditModel != nullptr)
	{
		m_pEditModel->SetDeathFlag(true);
		m_pEditModel = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CEdit::Update(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//移動処理
	Move();

	if (pInputKeyboard->GetTrigger(DIK_9) == true)
	{
		// Y軸の位置初期化
		m_EditPos.y = 0.0f;

		if (m_EditType == EDITTYPE_MODELHIT)
		{
			m_EditType = EDITTYPE_BLOCK;
			m_pEditBlock->SetAppear(true);
			m_pEditModel->SetAppear(false);

			m_EditPos.y = BLOCK_INIT_POS_Y[m_nBlockNumber];
			m_EditSize = BLOCK_INIT_SIZE[m_nBlockNumber];
		}
		else if (m_EditType == EDITTYPE_BLOCK)
		{
			m_EditType = EDITTYPE_MODELHIT;
			m_pEditBlock->SetAppear(false);
			m_pEditModel->SetAppear(false);
		}
	}

	//デバッグ表示の取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("========== {エディットモード起動中} ==========\n");
	pDebugProc->Print("エディットの位置 [%f]:[%f]:[%f]\n", m_EditPos.x, m_EditPos.y, m_EditPos.z);
	pDebugProc->Print("変動の幅 [%f]\n", MoveWidth);
	pDebugProc->Print("向きの幅 [%f]\n", m_fRotValue);
	pDebugProc->Print("-----[操作説明]-----\n");
	pDebugProc->Print("エディット位置の移動 : [W][A][S][D][LShift][LControl]\n");
	pDebugProc->Print("カメラ位置の移動     : [T][F][G][H][RShift][RControl]\n");
	pDebugProc->Print("視点の移動           : [I][J][K][L]\n");
	pDebugProc->Print("移動幅調整/リセット  : [←][→] / [N]\n");
	pDebugProc->Print("向き幅調整/リセット  : [↑][↓] / [M]\n");
	pDebugProc->Print("設置情報の保存       : [F9]\n");

	switch (m_EditType)
	{
	case CEdit::EDITTYPE_BLOCK:

		UpdateBlock();

		break;
	case CEdit::EDITTYPE_MODELHIT:

		UpdateXModel();

		break;
	case CEdit::EDITTYPE_MODELNOT:
		break;
	case CEdit::EDITTYPE_ENEMY:
		break;
	case CEdit::EDITTYPE_MAX:
		break;
	default:
		break;
	}

	if (pInputKeyboard->GetTrigger(DIK_F9) == true)
	{
		SaveData();
	}

	pDebugProc->Print("========== {エディットモード起動中} ==========\n");
}

//====================================================================
//	ブロックの更新処理
//====================================================================
void CEdit::UpdateBlock(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	m_pEditBlock->SetAppear(true);
	m_pEditBlock->SetPos(m_EditPos);

	m_pEditBlock->SetSize(m_EditSize);

	if (pInputKeyboard->GetTrigger(DIK_E) == true)
	{
		m_EditSize.x += MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_Q) == true)
	{
		m_EditSize.x -= MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_Y) == true)
	{
		m_EditSize.y += MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_R) == true)
	{
		m_EditSize.y -= MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_O) == true)
	{
		m_EditSize.z += MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_U) == true)
	{
		m_EditSize.z -= MoveWidth;
	}

	// テンプレート種類変更（昇順）
	if (pInputKeyboard->GetTrigger(DIK_7) == true)
	{
		m_nBlockNumber--;

		if (m_nBlockNumber < BLOCK_INIT_TYPE_NORMAL)
		{
			m_nBlockNumber = BLOCK_INIT_TYPE_MAX - 1;
		}

		// 初期ブロック情報設定
		m_EditPos.y = BLOCK_INIT_POS_Y[m_nBlockNumber];
		m_EditSize = BLOCK_INIT_SIZE[m_nBlockNumber];
	}

	// テンプレート種類変更（降順）
	if (pInputKeyboard->GetTrigger(DIK_8) == true)
	{
		m_nBlockNumber++;

		if (m_nBlockNumber >= BLOCK_INIT_TYPE_MAX)
		{
			m_nBlockNumber = BLOCK_INIT_TYPE_NORMAL;
		}

		// 初期ブロック情報設定
		m_EditPos.y = BLOCK_INIT_POS_Y[m_nBlockNumber];
		m_EditSize = BLOCK_INIT_SIZE[m_nBlockNumber];
	}

	// テクスチャ種類変更（昇順）
	if (pInputKeyboard->GetTrigger(DIK_5) == true)
	{
		m_nBlockTextureIdx--;

		if (m_nBlockTextureIdx < BLOCK_TEXTURE_00)
		{
			m_nBlockTextureIdx = BLOCK_TEXTURE_MAX - 1;
		}

		m_pEditBlock->SetTexture(BLOCK_TEXTURE_TYPE[m_nBlockTextureIdx]);
	}

	// テクスチャ種類変更（降順）
	if (pInputKeyboard->GetTrigger(DIK_6) == true)
	{
		m_nBlockTextureIdx++;

		if (m_nBlockTextureIdx >= BLOCK_TEXTURE_MAX)
		{
			m_nBlockTextureIdx = BLOCK_TEXTURE_00;
		}

		m_pEditBlock->SetTexture(BLOCK_TEXTURE_TYPE[m_nBlockTextureIdx]);
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		CCubeBlock* pBlock = CCubeBlock::Create();
		pBlock->SetPos(m_pEditBlock->GetPos());
		pBlock->SetSize(m_pEditBlock->GetSize());
		pBlock->SetTexture(BLOCK_TEXTURE_TYPE[m_nBlockTextureIdx]);
		AppearCollision();
	}

	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true)
	{
		CollisionBlock();
	}

	//デバッグ表示の取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("ブロックのサイズ [%f]:[%f]:[%f]\n", m_pEditBlock->GetSize().x, m_pEditBlock->GetSize().y, m_pEditBlock->GetSize().z);
	pDebugProc->Print("サイズ変更キー : X軸 [Q- : E+] \n");
	pDebugProc->Print("サイズ変更キー : Y軸 [R- : Y+] \n");
	pDebugProc->Print("サイズ変更キー : Z軸 [U- : O+] \n");
	pDebugProc->Print("ブロックのテンプレート変更キー : [7 : 8] \n");
	pDebugProc->Print("ブロックのテクスチャ変更キー : [5 : 6] \n");
}

//====================================================================
//	Xモデルの更新処理
//====================================================================
void CEdit::UpdateXModel(void)
{
	//各種情報の取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CXModel* pXModel = CManager::GetInstance()->GetXModel();

	m_pEditModel->SetAppear(true);
	m_pEditModel->SetPos(m_EditPos);

	if (pInputKeyboard->GetTrigger(DIK_4) == true)
	{
		m_bAppearCollision = !m_bAppearCollision;
		AppearCollision();
	}
	if (pInputKeyboard->GetTrigger(DIK_5) == true)
	{
		m_bModelCollision = m_pEditModel->GetCollision();

		m_bModelCollision = !m_bModelCollision;

		m_pEditModel->SetCollision(m_bModelCollision);
	}

	if (pInputKeyboard->GetTrigger(DIK_6) == true)
	{
		m_EditRot.y += D3DX_PI * 0.5f;
		m_pEditModel->SwapSize();

		if(m_EditRot.y >= D3DX_PI * 2.0f)
		{
			m_EditRot.y = 0.0f;
		}

		m_pEditModel->SetRot(m_EditRot);
	}

	if (pInputKeyboard->GetTrigger(DIK_7) == true)
	{
		m_nModelNumber--;

		if (m_nModelNumber < OBJ_HIT_DARUMA)
		{
			m_nModelNumber = OBJ_HIT_MAX - 1;
		}

		m_pEditModel->SetXModelIdx(pXModel->Regist(m_aModelInfo[m_nModelNumber].pFilename));
		m_pEditModel->SizeVtxSet();


		if (m_EditRot.y < 1.57f)
		{

		}
		else if (m_EditRot.y < 3.14f)
		{
			m_pEditModel->SwapSize();
		}
		else if (m_EditRot.y < 4.71f)
		{

		}
		else
		{
			m_pEditModel->SwapSize();
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_8) == true)
	{
		m_nModelNumber++;

		if (m_nModelNumber >= OBJ_HIT_MAX)
		{
			m_nModelNumber = OBJ_HIT_DARUMA;
		}

		m_pEditModel->SetXModelIdx(pXModel->Regist(m_aModelInfo[m_nModelNumber].pFilename));
		m_pEditModel->SizeVtxSet();

		if (m_EditRot.y < 1.57f)
		{

		}
		else if (m_EditRot.y < 3.14f)
		{
			m_pEditModel->SwapSize();
		}
		else if (m_EditRot.y < 4.71f)
		{

		}
		else
		{
			m_pEditModel->SwapSize();
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_X) == true)
	{
		m_EditRot.x += m_fRotValue;
		m_pEditModel->SwapSize();

		useful::NormalizeAngle(&m_EditRot);

		m_pEditModel->SetRot(m_EditRot);
	}

	if (pInputKeyboard->GetTrigger(DIK_Z) == true)
	{
		m_EditRot.y += m_fRotValue;
		m_pEditModel->SwapSize();

		useful::NormalizeAngle(&m_EditRot);

		m_pEditModel->SetRot(m_EditRot);
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		CMapModel* pModel = CMapModel::Create(m_aModelInfo[m_nModelNumber].pFilename);
		pModel->SetEditIdx(m_nModelNumber);
		pModel->SetPos(m_EditPos);
		pModel->SetRot(m_EditRot);

		if (m_EditRot.y < 1.57f)
		{

		}
		else if (m_EditRot.y < 3.14f)
		{
			pModel->SwapSize();
		}
		else if (m_EditRot.y < 4.71f)
		{

		}
		else
		{
			pModel->SwapSize();
		}

		pModel->SetCollision(m_bModelCollision);
		AppearCollision();
	}

	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true)
	{
		CollisionXModel();
	}

	m_pEditModel->UpdateBlock();

	//デバッグ表示の取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("モデルの当たり判定切り替え : [5]\n");
	pDebugProc->Print("向きの90度回転: [6]\n");
	pDebugProc->Print("モデル変更: [7 : 8]\n");
	if (m_bModelCollision == true)
	{
		pDebugProc->Print("モデルの当たり判定 : [ TRUE ]\n");
	}
	else
	{
		pDebugProc->Print("モデルの当たり判定 : [ FALSE ]\n");
	}
	pDebugProc->Print("モデルの説明: %s\n", m_aModelInfo[m_nModelNumber].pCommentname);
}

//====================================================================
//移動処理
//====================================================================
void CEdit::Move(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetPress(DIK_W) == true ||
		pInputKeyboard->GetPress(DIK_A) == true ||
		pInputKeyboard->GetPress(DIK_S) == true ||
		pInputKeyboard->GetPress(DIK_D) == true ||
		pInputKeyboard->GetPress(DIK_LSHIFT) == true ||
		pInputKeyboard->GetPress(DIK_LCONTROL) == true)
	{
		m_nPressCount++;
	}
	else
	{
		m_nPressCount = 0;
	}

	if (m_nPressCount >= 30 && m_nPressCount % 2 == 0)
	{
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{
			m_EditPos.z += MoveWidth;
		}

		if (pInputKeyboard->GetPress(DIK_S) == true)
		{
			m_EditPos.z -= MoveWidth;
		}

		if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			m_EditPos.x -= MoveWidth;
		}

		if (pInputKeyboard->GetPress(DIK_D) == true)
		{
			m_EditPos.x += MoveWidth;
		}

		if (pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			m_EditPos.y += MoveWidth;
		}

		if (pInputKeyboard->GetPress(DIK_LCONTROL) == true)
		{
			m_EditPos.y -= MoveWidth;
		}
	}
	else
	{
		if (pInputKeyboard->GetTrigger(DIK_W) == true)
		{
			m_EditPos.z += MoveWidth;
		}

		if (pInputKeyboard->GetTrigger(DIK_S) == true)
		{
			m_EditPos.z -= MoveWidth;
		}

		if (pInputKeyboard->GetTrigger(DIK_A) == true)
		{
			m_EditPos.x -= MoveWidth;
		}

		if (pInputKeyboard->GetTrigger(DIK_D) == true)
		{
			m_EditPos.x += MoveWidth;
		}

		if (pInputKeyboard->GetTrigger(DIK_LSHIFT) == true)
		{
			m_EditPos.y += MoveWidth;
		}

		if (pInputKeyboard->GetTrigger(DIK_LCONTROL) == true)
		{
			m_EditPos.y -= MoveWidth;
		}
	}

	//幅変更
	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		m_fRotValue += D3DX_PI * 0.125f;
		useful::NormalizeRot(&m_fRotValue);
	}

	if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		m_fRotValue -= D3DX_PI * 0.125f;
		useful::NormalizeRot(&m_fRotValue);
	}

	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{
		MoveWidth += 3.0f;
	}

	if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{
		MoveWidth -= 3.0f;
	}

	//幅リセット
	if (pInputKeyboard->GetTrigger(DIK_N) == true)
	{
		m_fRotValue = D3DX_PI * 0.5f;
	}

	if (pInputKeyboard->GetTrigger(DIK_M) == true)
	{
		MoveWidth = D3DX_PI * 0.5f;
	}
}

//====================================================================
//ブロックの当たり判定処理
//====================================================================
void CEdit::CollisionBlock(void)
{
	if (m_pEditBlock != nullptr)
	{
		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//オブジェクトを取得
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();			//種類を取得

				if (type == CObject::TYPE_CUBEBLOCK)
				{//種類がブロックの時
					CCubeBlock* pBlock = (CCubeBlock*)pObj;

					D3DXVECTOR3 MyPos = m_pEditBlock->GetPos();
					D3DXVECTOR3 MySize = m_pEditBlock->GetSize() * 2.0f;
					D3DXVECTOR3 BlockPos = pBlock->GetPos();
					D3DXVECTOR3 BlockSize = pBlock->GetSize();
					D3DXVECTOR3 BlockMove = pBlock->GetMove();

					if (BlockPos.x + BlockSize.x > MyPos.x &&
						BlockPos.x - BlockSize.x < MyPos.x &&
						BlockPos.z + BlockSize.z > MyPos.z &&
						BlockPos.z - BlockSize.z < MyPos.z &&
						BlockPos.y + BlockSize.y > MyPos.y &&
						BlockPos.y - BlockSize.y < MyPos.y)
					{
						pBlock->Uninit();
					}
				}
				pObj = pObjNext;
			}
		}
	}
}

//====================================================================
//ブロックの当たり判定処理
//====================================================================
void CEdit::CollisionXModel(void)
{
	if (m_pEditModel != nullptr)
	{
		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//オブジェクトを取得
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();			//種類を取得

				if (type == CObject::TYPE_MAPMODEL && pObj != m_pEditModel)
				{//種類がブロックの時
					CMapModel* pModel = (CMapModel*)pObj;

					D3DXVECTOR3 MyPos = m_pEditModel->GetPos();
					D3DXVECTOR3 MySize = m_pEditModel->GetSize();
					D3DXVECTOR3 BlockPos = pModel->GetPos();
					D3DXVECTOR3 BlockSize = pModel->GetSize();

					if (MyPos.x + MySize.x > BlockPos.x &&
						MyPos.x - MySize.x < BlockPos.x &&
						MyPos.z + MySize.z > BlockPos.z &&
						MyPos.z - MySize.z < BlockPos.z &&
						MyPos.y + MySize.y > BlockPos.y &&
						MyPos.y - MySize.y < BlockPos.y)
					{
						pModel->Uninit();
					}
				}
				pObj = pObjNext;
			}
		}
	}
}

//====================================================================
//当たり判定の可視化
//====================================================================
void CEdit::AppearCollision(void)
{
	if (m_pEditModel != nullptr)
	{
		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//オブジェクトを取得
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();			//種類を取得

				if (type == CObject::TYPE_MAPMODEL)
				{//種類がマップモデル時
					CMapModel* pModel = (CMapModel*)pObj;

					if (m_bAppearCollision == true)
					{//可視化がオンの時

						//可視化ブロックの生成
						pModel->CreateBlock();
					}
					else
					{//可視化がオフの時

						//可視化ブロックの削除
						if (pModel->GetDebugBlock() != nullptr)
						{
							pModel->GetDebugBlock()->Uninit();
							pModel->SetDebugBlock(nullptr);
						}
					}
				}

				pObj = pObjNext;
			}
		}
	}
}

//====================================================================
//保存処理
//====================================================================
void CEdit::SaveData(void)
{
	SaveBlock();
	SaveXModel();
}

//====================================================================
//ブロックの保存処理
//====================================================================
void CEdit::SaveBlock(void)
{
	FILE* pFile; //ファイルポインタを宣言

//ファイルを開く
	pFile = fopen(DATA_BLOCK_NAME, "w");

	if (pFile != NULL)
	{//ファイルが開けた場合

	 //ステージをセーブする開始の合図
		fprintf(pFile, "%s\n\n", "STARTSETSTAGE");

		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//オブジェクトを取得
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();				//種類を取得

				if (type == CObject::TYPE_CUBEBLOCK)
				{//種類がブロックの時
					CCubeBlock* pBlock = (CCubeBlock*)pObj;

					fprintf(pFile, "%s\n", "STARTSETBLOCK");

					//ステージをセーブした終了の合図
					fprintf(pFile, "POS %f %f %f\n", pBlock->GetPos().x, pBlock->GetPos().y, pBlock->GetPos().z);
					fprintf(pFile, "SIZE %f %f %f\n", pBlock->GetSize().x, pBlock->GetSize().y, pBlock->GetSize().z);
					fprintf(pFile, "TEXTURE %s\n", pBlock->GetTextureName());

					fprintf(pFile, "%s\n\n", "ENDSETBLOCK");
				}

				pObj = pObjNext;
			}
		}

		//ステージをセーブした終了の合図
		fprintf(pFile, "%s", "ENDSETSTAGE");

		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}

//====================================================================
//Xモデルの保存処理
//====================================================================
void CEdit::SaveXModel(void)
{
	FILE* pFile; //ファイルポインタを宣言

//ファイルを開く
	pFile = fopen(DATA_XMODEL_NAME, "w");

	if (pFile != NULL)
	{//ファイルが開けた場合

	 //ステージをセーブする開始の合図
		fprintf(pFile, "%s\n\n", "STARTSETSTAGE");

		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//オブジェクトを取得
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();				//種類を取得

				if (type == CObject::TYPE_MAPMODEL && pObj != m_pEditModel)
				{//種類がブロックの時
					CMapModel* pModel = (CMapModel*)pObj;

					fprintf(pFile, "%s\n", "STARTSETXMODEL");

					//ステージをセーブした終了の合図
					fprintf(pFile, "NAME %s\n", m_aModelInfo[pModel->GetEditIdx()].pFilename);
					fprintf(pFile, "NUMBER %d\n", pModel->GetEditIdx());
					fprintf(pFile, "POS %f %f %f\n", pModel->GetPos().x, pModel->GetPos().y, pModel->GetPos().z);
					fprintf(pFile, "ROT %f %f %f\n", pModel->GetRot().x, pModel->GetRot().y, pModel->GetRot().z);
					if (pModel->GetCollision() == true)
					{
						fprintf(pFile, "COLLISION 1\n");
					}
					else
					{
						fprintf(pFile, "COLLISION 0\n");
					}

					fprintf(pFile, "%s\n\n", "ENDSETXMODEL");
				}

				pObj = pObjNext;
			}
		}

		//ステージをセーブした終了の合図
		fprintf(pFile, "%s", "ENDSETSTAGE");

		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}