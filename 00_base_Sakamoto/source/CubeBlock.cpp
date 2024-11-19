//============================================
//
//	キューブブロック [CubeBlock.cpp]
//	Author:sakamoto kai
//
//============================================
#include "CubeBlock.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "objmeshField.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float WAIGHT_SIZE = 11;		//横の頂点数
	const float HEIGHT_SIZE = 11;		//縦の頂点数
	const float WIGHT_CENTER = 0.5f;	//横の原点(0.0f～1.0f)
	const float HEIGHT_CENTER = 0.5f;	//縦の原点(0.0f～1.0f)
	const float FIELD_SIZE = 200.0f;	//床一枚の大きさ
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CCubeBlock>* CCubeBlock::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CCubeBlock::CCubeBlock(int nPriority) :CObjmeshCube(nPriority)
{
	m_nMapWidthNumber = 0;
	m_nMapHeightNumber = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CCubeBlock::~CCubeBlock()
{

}

//====================================================================
//生成処理
//====================================================================
CCubeBlock* CCubeBlock::Create(void)
{
	CCubeBlock* pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		//オブジェクト3Dの生成
		pObject3D = new CCubeBlock();
	}

	//オブジェクトの初期化処理
	if (FAILED(pObject3D->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pObject3D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CCubeBlock::Init(void)
{	
	CObjmeshCube::Init();

	SetUseMultiMatrix(CGame::GetMapField()->GetMatrix());
	//SetMultiMatrix(true);

	SetTexture("data\\TEXTURE\\Field\\00_wall.jpg");

	//SetColor(D3DXCOLOR(0.5f, 0.5f, 0.0f, 0.5f));

	SetType(TYPE_CUBEBLOCK);

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CCubeBlock>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CCubeBlock::Uninit(void)
{
	CObjmeshCube::Uninit();

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CCubeBlock::Update(void)
{
	CObjmeshCube::Update();
}

//====================================================================
//描画処理
//====================================================================
void CCubeBlock::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	SetUseMultiMatrix(CGame::GetMapField()->GetMatrix());
	CObjmeshCube::Draw();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 == (参照値 == ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// 2回目の描画
	CObjmeshCube::Draw();

	//ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//====================================================================
//ブロックの当たり判定
//====================================================================
bool CCubeBlock::CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ)
{
	D3DXVECTOR3 MyPos = GetPos();
	D3DXVECTOR3 MyPosOld = GetPosOld();
	D3DXVECTOR3 MySize = GetSize();
	D3DXVECTOR3 MyMove = GetMove();

	switch (XYZ)
	{
	case CObject::COLLISION_X:	// X軸

		//＋から－の面
		if (MyPos.x + MySize.x > pPos->x - Size.x &&
			(MyPosOld.x + MySize.x <= pPos->x - Size.x ||
			MyPos.x + MySize.x <= pPosOld.x - Size.x ||
			(MyPos.x + MySize.x >= pPosOld.x - Size.x &&
			MyPosOld.x + MySize.x <= pPosOld.x - Size.x &&
			MyPosOld.x + MySize.x >= pPos->x - Size.x)) &&
			MyPos.z + MySize.z > pPos->z - Size.z &&
			MyPos.z - MySize.z < pPos->z + Size.z &&
			MyPos.y + MySize.y > pPos->y &&
			MyPos.y - MySize.y < pPos->y + Size.y
			)
		{
			if (MyMove.x >= 0.0f)
			{
				pPos->x = pPosOld.x - Size.x + Size.x + MyMove.x;
			}
			else
			{
				pPos->x = MyPos.x + MySize.x + Size.x + MyMove.x;
			}
			pMove->x = 0.0f;
		}

		//－から＋の面
		if (MyPos.x - MySize.x < pPos->x + Size.x &&
			(MyPosOld.x - MySize.x >= pPos->x + Size.x ||
			MyPos.x - MySize.x >= pPosOld.x + Size.x ||
			(MyPos.x - MySize.x <= pPosOld.x + Size.x &&
			MyPosOld.x - MySize.x >= pPosOld.x + Size.x &&
			MyPosOld.x - MySize.x <= pPos->x + Size.x)) &&
			MyPos.z + MySize.z > pPos->z - Size.z &&
			MyPos.z - MySize.z < pPos->z + Size.z &&
			MyPos.y + MySize.y > pPos->y &&
			MyPos.y - MySize.y < pPos->y + Size.y
			)
		{
			if (MyMove.x <= 0.0f)
			{
				pPos->x = pPosOld.x + Size.x - Size.x + MyMove.x;
			}
			else
			{
				pPos->x = MyPos.x - MySize.x - Size.x + MyMove.x;
			}
			pMove->x = 0.0f;
		}

		break;
	case CObject::COLLISION_Y:

		//＋から－の面
		if (MyPos.y + MySize.y > pPos->y &&
			(MyPosOld.y + MySize.y <= pPos->y ||
			MyPos.y + MySize.y <= pPosOld.y ||
			(MyPos.y + MySize.y >= pPosOld.y &&
			MyPosOld.y + MySize.y <= pPosOld.y &&
			MyPosOld.y + MySize.y >= pPos->y)) &&
			MyPos.z + MySize.z > pPos->z - Size.z &&
			MyPos.z - MySize.z < pPos->z + Size.z &&
			MyPos.x + MySize.x > pPos->x - Size.x &&
			MyPos.x - MySize.x < pPos->x + Size.x
			)
		{
			if (MyMove.y >= 0.0f)
			{
				pPos->y = pPosOld.y + MyMove.y;
			}
			else
			{
				pPos->y = MyPos.y + MySize.y + MyMove.y;
			}
			pMove->y = 0.0f;
			*bJump = false;
			pObjMove->x = MyMove.x;
			pObjMove->y = MyMove.y;
			pObjMove->z = MyMove.z;
		}

		//－から＋の面
		if (MyPos.y - MySize.y < pPos->y + Size.y &&
			(MyPosOld.y - MySize.y >= pPos->y + Size.y ||
			MyPos.y - MySize.y >= pPosOld.y + Size.y ||
			(MyPos.y - MySize.y <= pPosOld.y + Size.y &&
			MyPosOld.y - MySize.y >= pPosOld.y + Size.y &&
			MyPosOld.y - MySize.y <= pPos->y + Size.y)) &&
			MyPos.z + MySize.z > pPos->z - Size.z &&
			MyPos.z - MySize.z < pPos->z + Size.z &&
			MyPos.x + MySize.x > pPos->x - Size.x &&
			MyPos.x - MySize.x < pPos->x + Size.x
			)
		{
			pPos->y = MyPos.y - MySize.y - Size.y + MyMove.y;
			pMove->y = MyMove.y;
		}

		break;
	case CObject::COLLISION_Z:

		//＋から－の面
		if (MyPos.z + MySize.z > pPos->z - Size.z &&
			(MyPosOld.z + MySize.z <= pPos->z - Size.z ||
			MyPos.z + MySize.z <= pPosOld.z - Size.z ||
			(MyPos.z + MySize.z >= pPosOld.z - Size.z &&
			MyPosOld.z + MySize.z <= pPosOld.z - Size.z &&
			MyPosOld.z + MySize.z >= pPos->z - Size.z)) &&
			MyPos.x + MySize.x > pPos->x - Size.x &&
			MyPos.x - MySize.x < pPos->x + Size.x &&
			MyPos.y + MySize.y > pPos->y &&
			MyPos.y - MySize.y < pPos->y + Size.y
			)
		{
			if (MyMove.z >= 0.0f)
			{
				pPos->z = pPosOld.z - Size.z + Size.z + MyMove.z;
			}
			else
			{
				pPos->z = MyPos.z + MySize.z + Size.z + MyMove.z;
			}
			pMove->z = 0.0f;
		}

		//－から＋の面
		if (MyPos.z - MySize.z < pPos->z + Size.z &&
			(MyPosOld.z - MySize.z >= pPos->z + Size.z ||
			MyPos.z - MySize.z >= pPosOld.z + Size.z ||
			(MyPos.z - MySize.z <= pPosOld.z + Size.z &&
			MyPosOld.z - MySize.z >= pPosOld.z + Size.z &&
			MyPosOld.z - MySize.z <= pPos->z + Size.z)) &&
			MyPos.x + MySize.x > pPos->x - Size.x &&
			MyPos.x - MySize.x < pPos->x + Size.x &&
			MyPos.y + MySize.y > pPos->y &&
			MyPos.y - MySize.y < pPos->y + Size.y
			)
		{
			if (MyMove.z <= 0.0f)
			{
				pPos->z = pPosOld.z + Size.z - Size.z + MyMove.z;
			}
			else
			{
				pPos->z = MyPos.z - MySize.z - Size.z + MyMove.z;
			}
			pMove->z = 0.0f;
		}

		break;
	}

	return false;
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CCubeBlock>* CCubeBlock::GetList(void)
{
	return m_pList;
}
