//========================================================================================
//
//　便利関数[useful.cpp]
//	Author:sakamoto kai
//
//========================================================================================
#include "useful.h"
#include "manager.h"
#include "camera.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	constexpr float GIMBAL_LOCK_THRESHOLD = 0.9999f; // ジンバルロックの閾値
}

//====================================================================
// 移動量を比較し移動量１より移動量２が早いときtrueを返す
//====================================================================
float useful::RoundUp2(float Num)
{
	// 100単位で四捨五入
	int nNum = static_cast<int>(Num / 100.0f);
	float remainder = fmod(Num, 100.0f);

	if (remainder >= 50.0f)
	{
		return (nNum + 1) * 100.0f;
	}
	else if (remainder <= -50.0f)
	{
		return (nNum - 1) * 100.0f;
	}
	else
	{
		return nNum * 100.0f;
	}
}

//====================================================================
// 移動量を比較し移動量１より移動量２が早いときtrueを返す
//====================================================================
bool useful::MoveIF(float Move1, float Move2)
{
	if (Move1 > Move2)
	{
		return true;
	}

	return false;
}

//====================================================================
// 数字を設定した範囲内に納める処理
//====================================================================
int useful::RangeNumber(int Max, int Min, int Number)
{
	int nNumber = Number;

	if (nNumber >= Max)
	{
		nNumber = Min;
		return nNumber;
	}

	if (nNumber < Min)
	{
		nNumber = Max - 1;
		return nNumber;
	}

	return nNumber;
}

//====================================================================
//
//====================================================================
bool useful::CollisionRectangle2D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 Size1, D3DXVECTOR3 Size2, useful::COLLISION Shaft)
{
	bool Hit = false;

	switch (Shaft)
	{
	case useful::COLLISION_XY:
		if (pos1.x + Size1.x > pos2.x - Size2.x &&
			pos1.x - Size1.x < pos2.x + Size2.x &&
			pos1.y + Size1.y > pos2.y - Size2.y &&
			pos1.y - Size1.y < pos2.y + Size2.y)
		{
			Hit = true;
		}
		break;

	case useful::COLLISION_YZ:
		if (pos1.y + Size1.y < pos2.y - Size2.y &&
			pos1.y - Size1.y > pos2.y + Size2.y &&
			pos1.z + Size1.z < pos2.z - Size2.z &&
			pos1.z - Size1.z > pos2.z + Size2.z)
		{
			Hit = true;
		}
		break;

	case useful::COLLISION_ZX:
		if (pos1.z + Size1.z > pos2.z - Size2.z &&
			pos1.z - Size1.z < pos2.z + Size2.z &&
			pos1.x + Size1.x > pos2.x - Size2.x &&
			pos1.x - Size1.x < pos2.x + Size2.x)
		{
			Hit = true;
		}
		break;
	}

	return Hit;
}

//====================================================================
//点が四角の中に存在するかどうか
//====================================================================
bool useful::PointSquareXZ(D3DXVECTOR3 Point, D3DXVECTOR3 pos, D3DXVECTOR3 Size)
{
	bool Hit = false;

	if (Point.z < pos.z + Size.z &&
		Point.z > pos.z - Size.z &&
		Point.x < pos.x + Size.x &&
		Point.x > pos.x - Size.x)
	{
		Hit = true;
	}

	return Hit;
}

//====================================================================
//矩形の押し出し判定
//====================================================================
bool useful::PushSquareXZ(D3DXVECTOR3 PosMy, D3DXVECTOR3 SizeMy, D3DXVECTOR3 MoveMy, D3DXVECTOR3 PosPair, D3DXVECTOR3 PosOldPair, D3DXVECTOR3 SizePair, D3DXVECTOR3* pObjMove, useful::COLLISION XYZ)
{
	switch (XYZ)
	{
	case useful::COLLISION_X:
		//// 押し出し判定
		//if (PosMy.x - SizeMy.x + MoveMy.x < PosPair.x + SizePair.x &&
		//	PosMy.x + SizeMy.x + MoveMy.x > PosPair.x - SizePair.x &&
		//	PosMy.z + SizeMy.z > PosPair.z - SizePair.z &&
		//	PosMy.z - SizeMy.z < PosPair.z + SizePair.z
		//	)
		//{
		//	return true;
		//}

		// 押し出し判定
		if (PosMy.x - SizeMy.x < PosPair.x + SizePair.x &&
			PosMy.x - SizeMy.x >= PosOldPair.x + SizePair.x &&
			PosMy.z + SizeMy.z > PosPair.z - SizePair.z &&
			PosMy.z - SizeMy.z < PosPair.z + SizePair.z
			)
		{
			pObjMove->x = MoveMy.x;
			return true;
		}

		// 押し出し判定
		if (PosMy.x + SizeMy.x > PosPair.x - SizePair.x &&
			PosMy.x + SizeMy.x <= PosOldPair.x - SizePair.x &&
			PosMy.z + SizeMy.z > PosPair.z - SizePair.z &&
			PosMy.z - SizeMy.z < PosPair.z + SizePair.z
			)
		{
			return true;
		}

		break;

	case useful::COLLISION_Z:

		// 押し出し判定
		if (PosMy.z - SizeMy.z + MoveMy.z < PosPair.z + SizePair.z &&
			PosMy.z + SizeMy.z + MoveMy.z > PosPair.z - SizePair.z &&
			PosMy.x + SizeMy.x > PosPair.x - SizePair.x &&
			PosMy.x - SizeMy.x < PosPair.x + SizePair.x
			)
		{
			return true;
		}

		break;
	}

	return false;
}

//====================================================================
//カメラの前側に存在しているかどうか
//====================================================================
bool useful::CameraFront(D3DXVECTOR3 pos)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
	D3DXVECTOR3 CamPos = pCamera->GetPosV();
	float CamRot = pCamera->GetRot().y;

	D3DXVECTOR3 vecToPos;
	D3DXVECTOR3 vecLine;
	float A;

	vecToPos.x = (CamPos.x) - (pos.x);
	vecToPos.z = (CamPos.z) - (pos.z);

	vecLine.x = sinf(CamRot + D3DX_PI * 0.5f);
	vecLine.y = 0.0f;
	vecLine.z = cosf(CamRot + D3DX_PI * 0.5f);

	A = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);

	if (A > 0)
	{
		return true;
	}
	return false;
}

//====================================================================
// 引数１の位置から引数３の角度の線を引いた時に引数２の位置が線の右にあるかどうか
//====================================================================
bool useful::CollisionLine(D3DXVECTOR3 Mypos, D3DXVECTOR3 PairPos, float Angle)
{
	D3DXVECTOR3 vecToPos;
	D3DXVECTOR3 vecLine;
	float A;

	vecToPos.x = (Mypos.x) - (PairPos.x);
	vecToPos.z = (Mypos.z) - (PairPos.z);

	vecLine.x = sinf(Angle + D3DX_PI * 0.5f);
	vecLine.y = 0.0f;
	vecLine.z = cosf(Angle + D3DX_PI * 0.5f);

	A = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);

	if (A > 0)
	{
		return true;
	}
	return false;
}

//====================================================================
//斬撃の当たり判定
//====================================================================
bool useful::SlashCollision(D3DXVECTOR3 *EnemyPos, int Max, float SlashRot)
{
	//プレイヤーと敵の位置関係を示す外積
	int Left = 0;
	int	Right = 0;

	for (int nCnt = 0; nCnt < Max; nCnt++, EnemyPos++)
	{
		D3DXVECTOR3 vecToPos;
		D3DXVECTOR3 vecLine;
		float A;

		vecToPos.x = (EnemyPos->x) - (SCREEN_CENTER.x);
		vecToPos.y = (EnemyPos->y) - (SCREEN_CENTER.y);

		vecLine.x = sinf(SlashRot);
		vecLine.y = cosf(SlashRot);
		vecLine.z = 0.0f;

		A = (vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y);

		if (A < 0)
		{
			Right++;
		}
		else
		{
			Left++;
		}
	}

	//敵の点の間に攻撃時の線が入っていた場合
	if (Left > 0 && Right > 0)
	{
		return true;
	}

	return false;
}

//====================================================================
//ウィンドウ画面の当たり判定
//====================================================================
void useful::CollisionWindow(D3DXVECTOR3* pos)
{
	if (pos->x >= SCREEN_WIDTH)
	{
		pos->x = SCREEN_WIDTH;
	}
	else if (pos->x <= 0.0f)
	{
		pos->x = 0.0f;
	}

	if (pos->y >= SCREEN_HEIGHT)
	{
		pos->y = SCREEN_HEIGHT;
	}
	else if (pos->y <= 0.0f)
	{
		pos->y = 0.0f;
	}
}

//====================================================================
//ウィンドウ画面の当たり判定
//====================================================================
bool useful::CollisionWindow(D3DXVECTOR3 pos)
{
	bool bHit = false;

	if (pos.x >= SCREEN_WIDTH)
	{
		bHit = true;
	}
	else if (pos.x <= 0.0f)
	{
		bHit = true;
	}

	if (pos.y >= SCREEN_HEIGHT)
	{
		bHit = true;
	}
	else if (pos.y <= 0.0f)
	{
		bHit = true;
	}

	return bHit;
}

//====================================================================
//円の当たり判定
//====================================================================
bool useful::CollisionCircle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float nRadiusOut)
{
	if (sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.y - pos2.y) * (pos1.y - pos2.y)) <= nRadiusOut)
	{//円の判定が当たった
		if (sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
			+ (pos1.z - pos2.z) * (pos1.z - pos2.z)) <= nRadiusOut)
		{//円の判定が当たった
			return true;
		}
	}

	return false;
}

//====================================================================
// 矩形の当たり判定（酒井）
//====================================================================
bool useful::CollisionRectangle(D3DXVECTOR3 posMy, D3DXVECTOR3 sizeMy, D3DXVECTOR3 posPair, D3DXVECTOR3 sizePair)
{
	if (posMy.x + sizeMy.x >= posPair.x - sizePair.x &&
		posMy.x - sizeMy.x <= posPair.x + sizePair.x &&
		posMy.y + sizeMy.y >= posPair.y - sizePair.y &&
		posMy.y - sizeMy.y <= posPair.y + sizePair.y &&
		posMy.z + sizeMy.z >= posPair.z - sizePair.z &&
		posMy.z - sizeMy.z <= posPair.z + sizePair.z)
	{
		return true;
	}

	return false;
}

//====================================================================
// 動く面に対する当たり判定
//====================================================================
bool useful::CollisionBlock(
	D3DXVECTOR3 PosPair, D3DXVECTOR3 PosOldPair, D3DXVECTOR3 MovePair, D3DXVECTOR3 SizePair,
	D3DXVECTOR3* pPosMy, D3DXVECTOR3 pPosOldMy, D3DXVECTOR3* pMoveMy, D3DXVECTOR3* pObjMoveMy, D3DXVECTOR3 SizeMy, bool* bJump, useful::COLLISION XYZ)
{
	switch (XYZ)
	{
	case useful::COLLISION_X:

		// 相手の右と自分の左
		//＋から−の面
		if ((PosOldPair.x + SizePair.x <= pPosOldMy.x - SizeMy.x) &&	// 前回の位置同士が当たってない
			(PosPair.x + SizePair.x > pPosMy->x - SizeMy.x) &&			// 現在の位置同士が当たってる

			(PosPair.z + SizePair.z > pPosMy->z - SizeMy.z) &&			// Z軸の判定
			(PosPair.z - SizePair.z < pPosMy->z + SizeMy.z))
		{
			// 位置を更新
			pPosMy->x = PosPair.x + SizePair.x + SizeMy.x - pMoveMy->x + MovePair.x;
			pMoveMy->x = 0.0f;

			return true;
		}

		// 相手の左と自分の右
		//−から＋の面
		if ((PosOldPair.x - SizePair.x >= pPosOldMy.x + SizeMy.x) &&	// 前回の位置同士が当たってない
			(PosPair.x - SizePair.x < pPosMy->x + SizeMy.x) &&			// 現在の位置同士が当たってる

			(PosPair.z + SizePair.z > pPosMy->z - SizeMy.z) &&			// Z軸の判定
			(PosPair.z - SizePair.z < pPosMy->z + SizeMy.z))
		{
			// 位置を更新
			pPosMy->x = PosPair.x - SizePair.x - SizeMy.x - pMoveMy->x + MovePair.x;
			pMoveMy->x = 0.0f;

			return true;
		}

#if 0
		if (PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			(PosOldPair.x + SizePair.x <= pPosMy->x - SizeMy.x ||
				PosPair.x + SizePair.x <= pPosOldMy.x - SizeMy.x ||
				(PosPair.x + SizePair.x >= pPosOldMy.x - SizeMy.x &&
					PosOldPair.x + SizePair.x <= pPosOldMy.x - SizeMy.x &&
					PosOldPair.x + SizePair.x >= pPosMy->x - SizeMy.x)) &&
			PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			PosPair.z - SizePair.z < pPosMy->z + SizeMy.z/* &&
			PosPair.y + SizePair.y > pPosMy->y &&
			PosPair.y - SizePair.y < pPosMy->y + SizeMy.y*/
			)
		{
			//if (MovePair.x >= 0.0f)
			{ // 相手が右に進んでる場合
				pPosMy->x = pPosOldMy.x + MovePair.x + pMoveMy->x;
			}
			//else
			//{ // 相手が左に進んでる場合
			//	pPosMy->x = PosPair.x + SizePair.x + SizeMy.x + MovePair.x + pMoveMy->x;
			//}
			pMoveMy->x = 0.0f;
			return true;
		}
		// 相手の左と自分の右
		//−から＋の面
		if (PosPair.x - SizePair.x < pPosMy->x + SizeMy.x &&
			(PosOldPair.x - SizePair.x >= pPosMy->x + SizeMy.x ||
				PosPair.x - SizePair.x >= pPosOldMy.x + SizeMy.x ||
				(PosPair.x - SizePair.x <= pPosOldMy.x + SizeMy.x &&
					PosOldPair.x - SizePair.x >= pPosOldMy.x + SizeMy.x &&
					PosOldPair.x - SizePair.x <= pPosMy->x + SizeMy.x)) &&
			PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			PosPair.z - SizePair.z < pPosMy->z + SizeMy.z/* &&
			PosPair.y + SizePair.y >= pPosMy->y &&
			PosPair.y - SizePair.y <= pPosMy->y + SizeMy.y*/
			)
		{
			//if (MovePair.x <= 0.0f)
			{ // 相手が左に進んでる場合
				pPosMy->x = pPosOldMy.x + SizeMy.x - SizeMy.x + MovePair.x + pMoveMy->x;
			}
			//else
			//{ // 相手が右に進んでる場合
			//	pPosMy->x = PosPair.x - SizePair.x - SizeMy.x + MovePair.x + pMoveMy->x;
			//}
			pMoveMy->x = 0.0f;
			return true;
		}
#endif // 0


		break;

	case useful::COLLISION_Y:

		//＋から−の面
		if (PosPair.y + SizePair.y > pPosMy->y &&
			(PosOldPair.y + SizePair.y <= pPosMy->y ||
				PosPair.y + SizePair.y <= pPosOldMy.y ||
				(PosPair.y + SizePair.y >= pPosOldMy.y &&
					PosOldPair.y + SizePair.y <= pPosOldMy.y &&
					PosOldPair.y + SizePair.y >= pPosMy->y)) &&
			PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			PosPair.z - SizePair.z < pPosMy->z + SizeMy.z &&
			PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			PosPair.x - SizePair.x < pPosMy->x + SizeMy.x
			)
		{
			if (MovePair.y >= 0.0f)
			{
				pPosMy->y = pPosOldMy.y + MovePair.y;
			}
			else
			{
				pPosMy->y = PosPair.y + SizePair.y + MovePair.y;
			}
			pMoveMy->y = 0.0f;
			*bJump = false;
			pObjMoveMy->x = MovePair.x;
			pObjMoveMy->y = MovePair.y;
			pObjMoveMy->z = MovePair.z;
			return true;
		}

		//−から＋の面
		if (PosPair.y - SizePair.y < pPosMy->y + SizeMy.y &&
			(PosOldPair.y - SizePair.y >= pPosMy->y + SizeMy.y ||
				PosPair.y - SizePair.y >= pPosOldMy.y + SizeMy.y ||
				(PosPair.y - SizePair.y <= pPosOldMy.y + SizeMy.y &&
					PosOldPair.y - SizePair.y >= pPosOldMy.y + SizeMy.y &&
					PosOldPair.y - SizePair.y <= pPosMy->y + SizeMy.y)) &&
			PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			PosPair.z - SizePair.z < pPosMy->z + SizeMy.z &&
			PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			PosPair.x - SizePair.x < pPosMy->x + SizeMy.x
			)
		{
			pPosMy->y = PosPair.y - SizePair.y - SizeMy.y + MovePair.y;
			pMoveMy->y = MovePair.y;
			return true;
		}

		break;

	case useful::COLLISION_Z:

		//＋から−の面
		if ((PosOldPair.z + SizePair.z <= pPosOldMy.z - SizeMy.z) &&	// 前回の位置同士が当たってない
			(PosPair.z + SizePair.z > pPosMy->z - SizeMy.z) &&			// 現在の位置同士が当たってる

			(PosPair.x + SizePair.x > pPosMy->x - SizeMy.x) &&			// X軸の判定
			(PosPair.x - SizePair.x < pPosMy->x + SizeMy.x))
		{
			// 位置を更新
			pPosMy->z = PosPair.z + SizePair.z + SizeMy.z - pMoveMy->z + MovePair.z;
			pMoveMy->z = 0.0f;

			return true;
		}

		//-から+の面
		if ((PosOldPair.z - SizePair.z >= pPosOldMy.z + SizeMy.z) &&	// 前回の位置同士が当たってない
			(PosPair.z - SizePair.z < pPosMy->z + SizeMy.z) &&			// 現在の位置同士が当たってる

			(PosPair.x + SizePair.x > pPosMy->x - SizeMy.x) &&			// X軸の判定
			(PosPair.x - SizePair.x < pPosMy->x + SizeMy.x))
		{
			// 位置を更新
			pPosMy->z = PosPair.z - SizePair.z - SizeMy.z - pMoveMy->z + MovePair.z;
			pMoveMy->z = 0.0f;

			return true;
		}

#if 0

		//＋から−の面
		if (PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			(PosOldPair.z + SizePair.z <= pPosMy->z - SizeMy.z ||
				PosPair.z + SizePair.z <= pPosOldMy.z - SizeMy.z ||
				(PosPair.z + SizePair.z >= pPosOldMy.z - SizeMy.z &&
					PosOldPair.z + SizePair.z <= pPosOldMy.z - SizeMy.z &&
					PosOldPair.z + SizePair.z >= pPosMy->z - SizeMy.z)) &&
			PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			PosPair.x - SizePair.x < pPosMy->x + SizeMy.x &&
			PosPair.y + SizePair.y > pPosMy->y &&
			PosPair.y - SizePair.y < pPosMy->y + SizeMy.y
			)
		{
			if (MovePair.z >= 0.0f)
			{
				pPosMy->z = pPosOldMy.z - SizeMy.z + SizeMy.z + MovePair.z;
			}
			else
			{
				pPosMy->z = PosPair.z + SizePair.z + SizeMy.z + MovePair.z;
			}
			pMoveMy->z = 0.0f;
			return true;
		}

		//−から＋の面
		if (PosPair.z - SizePair.z < pPosMy->z + SizeMy.z &&
			(PosOldPair.z - SizePair.z >= pPosMy->z + SizeMy.z ||
				PosPair.z - SizePair.z >= pPosOldMy.z + SizeMy.z ||
				(PosPair.z - SizePair.z <= pPosOldMy.z + SizeMy.z &&
					PosOldPair.z - SizePair.z >= pPosOldMy.z + SizeMy.z &&
					PosOldPair.z - SizePair.z <= pPosMy->z + SizeMy.z)) &&
			PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			PosPair.x - SizePair.x < pPosMy->x + SizeMy.x &&
			PosPair.y + SizePair.y > pPosMy->y &&
			PosPair.y - SizePair.y < pPosMy->y + SizeMy.y
			)
		{
			if (MovePair.z <= 0.0f)
			{
				pPosMy->z = pPosOldMy.z + SizeMy.z - SizeMy.z + MovePair.z;
			}
			else
			{
				pPosMy->z = PosPair.z - SizePair.z - SizeMy.z + MovePair.z;
			}
			pMoveMy->z = 0.0f;
			return true;
		}
#endif

		break;
	}

	return false;
}

//====================================================================
// 角度の正規化（酒井）
//====================================================================
void useful::NormalizeAngle(D3DXVECTOR3* rot)
{
	// 目的の向きの補正
	if (rot->x > D3DX_PI)
	{
		rot->x += -D3DX_PI * 2;
	}
	else if (rot->x < -D3DX_PI)
	{
		rot->x += D3DX_PI * 2;
	}

	// 目的の向きの補正
	if (rot->y > D3DX_PI)
	{
		rot->y += -D3DX_PI * 2;
	}
	else if (rot->y < -D3DX_PI)
	{
		rot->y += D3DX_PI * 2;
	}

	// 目的の向きの補正
	if (rot->z > D3DX_PI)
	{
		rot->z += -D3DX_PI * 2;
	}
	else if (rot->z < -D3DX_PI)
	{
		rot->z += D3DX_PI * 2;
	}
}

//====================================================================
// 角度の正規化（��田）
//====================================================================
void useful::NormalizeRot(float* rot)
{
	// 目的の向きの補正
	if (*rot > D3DX_PI)
	{
		*rot += -D3DX_PI * 2;
	}
	else if (*rot < -D3DX_PI)
	{
		*rot += D3DX_PI * 2;
	}
}

//====================================================================
// 2D座標に変換 CieCv2D(出力先、変換座標)
//====================================================================
void useful::Vec3Project(D3DXVECTOR3 *pOut, D3DXVECTOR3 v)
{
	// 変数宣言
	D3DXMATRIX mtxProjection;
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxWorld;
	D3DVIEWPORT9 viewport;

	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;

	//マトリックスの取得
	mtxView = CManager::GetInstance()->GetCamera(0)->GetViewMatrix();
	mtxProjection = CManager::GetInstance()->GetCamera(0)->GetProjectionMatrix();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 座標変換
	D3DXVec3Project(pOut, &v, &viewport, &mtxProjection, &mtxView, nullptr);
}

//================
// 2点間の単位ベクトル取得
//@param (pos0)主となる位置
//@param (pos1)相手の位置
//@return ベクトル
//================
D3DXVECTOR3 useful::Point2Rot(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3 vecDiff = pos1 - pos0;
	D3DXVec3Normalize(&vecDiff, &vecDiff);		//ベクトルを正規化する(1.0f 〜 -1.0fにする)

	return vecDiff;
}

//====================================================================
//	外積の判定の交点との距離を求める
//====================================================================
D3DXVECTOR3 useful::CrossIntersection(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, float SeatchDistance)
{
	D3DXVECTOR3 vecToPos[2];
	D3DXVECTOR3 vecLine;
	D3DXVECTOR3 vecToMove;
	float fA;
	float fB;
	int nA = 0;
	float fAngle = atan2f((pos0.x - pos1.x), (pos0.z - pos1.z));

	vecToPos[0].x = (pos0.x) - (pos2.x);
	vecToPos[0].z = (pos0.z) - (pos2.z);

	vecToPos[1].x = (pos0.x) - (pos3.x);
	vecToPos[1].z = (pos0.z) - (pos3.z);

	vecToMove.x = (pos3.x) - (pos2.x);
	vecToMove.z = (pos3.z) - (pos2.z);

	vecLine.x = sinf(fAngle + D3DX_PI * 0.0f) * 700.0f;
	vecLine.y = 0.0f;
	vecLine.z = cosf(fAngle + D3DX_PI * 0.0f) * 700.0f;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		fA = (vecLine.z * vecToPos[nCnt].x) - (vecLine.x * vecToPos[nCnt].z);

		if (fA > 0)
		{
			nA++;
		}
		else
		{
			nA--;
		}
	}

	if (nA == 0)
	{
		fA = (vecToPos[0].z * vecToMove.x) - (vecToPos[0].x * vecToMove.z);
		fB = (vecLine.z * vecToMove.x) - (vecLine.x * vecToMove.z);

		float fRate = fA / fB;

		//CManager::GetInstance()->GetDebugProc()->Print("%f\n", fRate);

		if (fRate >= 0.0f && fRate <= 1.0f)
		{
			D3DXVECTOR3 CrossPoint = INITVECTOR3;
			CrossPoint.x = pos0.x - vecLine.x * fRate;
			CrossPoint.z = pos0.z - vecLine.z * fRate;

			return D3DXVECTOR3(CrossPoint.x, 0.0f, CrossPoint.z);
		}
	}

	return D3DXVECTOR3(10000.0f, 0.0f, 10000.0f);
}

//====================================================================
// 絶対値判定[float]（酒井）
//====================================================================
bool useful::DecisionAbsolute(float fDataMy, float fDec)
{
	// 絶対値として値を取得
	fDataMy = std::abs(fDataMy);

	// 比較対象より大きい
	if (fDataMy >= fDec)
	{
		return true;
	}

	return false;
}

//====================================================================
// 絶対値判定[D3DXVECTOR3]（酒井）
//====================================================================
bool useful::DecisionAbsolute(D3DXVECTOR3 dataMy, float fDec)
{
	// 絶対値として値を取得
	dataMy.x = std::abs(dataMy.x);
	dataMy.y = std::abs(dataMy.y);
	dataMy.z = std::abs(dataMy.z);

	// 比較対象より大きい
	if (dataMy.x >= fDec ||
		dataMy.y >= fDec ||
		dataMy.z >= fDec)
	{
		return true;
	}

	return false;
}

//====================================================================
// 引数の位置の方向を向く処理
//====================================================================
float useful::PosDirection(D3DXVECTOR3 MyPos, D3DXVECTOR3 PairPos, float ChageRot)
{
	float fRot = 0.0f;

	fRot = atan2f(PairPos.z - MyPos.z, MyPos.x - PairPos.x) + D3DX_PI * ChageRot;

	return fRot;
}

//====================================================================
// 度からラジアン変換
//====================================================================
float useful::DegreesToRadians(float degrees)
{
	return degrees * (D3DX_PI / 180.0f);
}

//==========================================
//  マトリックスの計算
//==========================================
D3DXVECTOR3 useful::CalcMatrix(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXMATRIX& mtx)
{
	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//算出したマトリクスをかけ合わせる
	D3DXMatrixMultiply(&mtxWorld,
		&mtxWorld,
		&mtx);

	return D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
}

//==========================================
//  回転計算を分離する処理
//==========================================
D3DXVECTOR3 useful::CalcNormalRotation(const D3DXMATRIX& mtx)
{
	D3DXVECTOR3 rot;
	rot.x = atan2f(mtx._31, mtx._33); // X軸回転
	rot.z = atan2f(mtx._12, mtx._22); // Z軸回転
	return rot;
}

//==========================================
//  ジンバルロック状態の回転計算
//==========================================
D3DXVECTOR3 useful::CalcGimbalLockRotation(const D3DXMATRIX& mtx)
{
	D3DXVECTOR3 rot;
	rot.x = atan2f(-mtx._13, mtx._11); // X軸回転（代替計算式）
	rot.z = 0.0f; // Z軸は0に固定
	return rot;
}

//==========================================
//  マトリックスから角度を算出する
//==========================================
D3DXVECTOR3 useful::CalcMatrixToRot(const D3DXMATRIX& mtx)
{
	D3DXVECTOR3 rot;

	// Y軸回転を計算
	rot.y = asinf(-mtx._32);

	// ジンバルロックを判定し、対応する計算方法を適用
	if (fabs(mtx._32) < GIMBAL_LOCK_THRESHOLD)
	{
		D3DXVECTOR3 normalRot = CalcNormalRotation(mtx);
		rot.x = normalRot.x;
		rot.z = normalRot.z;
	}
	else
	{
		D3DXVECTOR3 gimbalRot = CalcGimbalLockRotation(mtx);
		rot.x = gimbalRot.x;
		rot.z = gimbalRot.z;
	}

	return rot;
}
