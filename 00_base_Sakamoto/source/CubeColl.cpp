//============================================
//
//	当たり判定キューブ [CubeColl.cpp]
//	Author:酒井 南勝
//
//============================================

#include "CubeColl.h"

#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const int WAIGHT_SIZE = 11;		//横の頂点数
	const int HEIGHT_SIZE = 11;		//縦の頂点数
	const float WAIGHT_CENTER = 0.5f;	//横の原点(0.0f〜1.0f)
	const float HEIGHT_CENTER = 0.5f;	//縦の原点(0.0f〜1.0f)
	const float FIELD_SIZE = 200.0f;	//床一枚の大きさ
}

//====================================================================
//コンストラクタ
//====================================================================
CCubeColl::CCubeColl(int nPriority) :CObjmeshCube(nPriority)
{
}

//====================================================================
//デストラクタ
//====================================================================
CCubeColl::~CCubeColl()
{

}

//====================================================================
//生成処理
//====================================================================
CCubeColl* CCubeColl::Create(void)
{
	CCubeColl* pInstance = new CCubeColl();

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{// 初期化処理が失敗した場合
		return nullptr;
	}

	return pInstance;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CCubeColl::Init(void)
{
	CObjmeshCube::Init();

	SetTexture("data\\TEXTURE\\Wood003.png");
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	SetType(TYPE_CUBECOLL);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CCubeColl::Uninit(void)
{
	CObjmeshCube::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CCubeColl::Update(void)
{
	CObjmeshCube::Update();
}

//====================================================================
//描画処理
//====================================================================
void CCubeColl::Draw(void)
{
	CObjmeshCube::Draw();
}

//====================================================================
//ブロックの当たり判定
//====================================================================
bool CCubeColl::CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ)
{
	D3DXVECTOR3 MyPos = GetPos();
	D3DXVECTOR3 MyPosOld = GetPosOld();
	D3DXVECTOR3 MySize = GetSize();
	D3DXVECTOR3 MyMove = GetMove();

	switch (XYZ)
	{
	case CObject::COLLISION_X:	// X軸

		//＋から−の面
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

		//−から＋の面
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

		//＋から−の面
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

		//−から＋の面
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

		//＋から−の面
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

		//−から＋の面
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