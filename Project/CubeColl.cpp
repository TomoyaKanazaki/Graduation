//============================================
//
//	ìñÇΩÇËîªíËÉLÉÖÅ[Éu [CubeColl.cpp]
//	Author:éà‰ ìÏèü
//
//============================================

#include "CubeColl.h"

#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"

#define WAIGHT_SIZE (11)		//â°ÇÃí∏ì_êî
#define HEIGHT_SIZE (11)		//ècÇÃí∏ì_êî
#define WAIGHT_CENTER (0.5f)	//â°ÇÃå¥ì_(0.0fÅ`1.0f)
#define HEIGHT_CENTER (0.5f)	//ècÇÃå¥ì_(0.0fÅ`1.0f)
#define FIELD_SIZE (200.0f)		//è∞àÍñáÇÃëÂÇ´Ç≥

//====================================================================
//ÉRÉìÉXÉgÉâÉNÉ^
//====================================================================
CCubeColl::CCubeColl(int nPriority) :CObjmeshCube(nPriority)
{
}

//====================================================================
//ÉfÉXÉgÉâÉNÉ^
//====================================================================
CCubeColl::~CCubeColl()
{

}

//====================================================================
//ê∂ê¨èàóù
//====================================================================
CCubeColl* CCubeColl::Create(void)
{
	CCubeColl* pInstance = new CCubeColl();

	// èâä˙âªèàóù
	if (FAILED(pInstance->Init()))
	{// èâä˙âªèàóùÇ™é∏îsÇµÇΩèÍçá
		return nullptr;
	}

	return pInstance;
}

//====================================================================
//èâä˙âªèàóù
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
//èIóπèàóù
//====================================================================
void CCubeColl::Uninit(void)
{
	CObjmeshCube::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//çXêVèàóù
//====================================================================
void CCubeColl::Update(void)
{
	CObjmeshCube::Update();
}

//====================================================================
//ï`âÊèàóù
//====================================================================
void CCubeColl::Draw(void)
{
	CObjmeshCube::Draw();
}

//====================================================================
//ÉuÉçÉbÉNÇÃìñÇΩÇËîªíË
//====================================================================
bool CCubeColl::CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ)
{
	D3DXVECTOR3 MyPos = GetPos();
	D3DXVECTOR3 MyPosOld = GetPosOld();
	D3DXVECTOR3 MySize = GetSize();
	D3DXVECTOR3 MyMove = GetMove();

	switch (XYZ)
	{
	case CObject::COLLISION_X:	// Xé≤

		//Å{Ç©ÇÁÅ|ÇÃñ 
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

		//Å|Ç©ÇÁÅ{ÇÃñ 
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

		//Å{Ç©ÇÁÅ|ÇÃñ 
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

		//Å|Ç©ÇÁÅ{ÇÃñ 
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

		//Å{Ç©ÇÁÅ|ÇÃñ 
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

		//Å|Ç©ÇÁÅ{ÇÃñ 
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