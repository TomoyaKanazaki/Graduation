//=============================================================================
// 
// 便利関数 [calculation.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _CALCULATION_H_
#define _CALCULATION_H_		// 二重インクルード防止のマクロを定義する

#include "main.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define MAX_COMMENT	(512)	// コメントの最大数
#define MOVE_LOG	(64)	// ロゴの最大数

// UI拡縮のモード
typedef enum
{
	MOVELOG_TYPE_ADD = 0,	// 拡大
	MOVELOG_TYPE_REMOVE,	// 縮小
	MOVELOG_TYPE_SAME,		// 揃える
	MOVELOG_TYPE_MAX
}MOVELOG_TYPE;

// ロゴ拡縮の構造体
typedef struct
{
	int nCnt;		// ロゴ拡縮のカウント
	int nPattern;	// ロゴ拡縮のパターン
	float Length;	// ロゴ拡縮の長さ
	bool bUse;		// 使用しているか
}MOVELOG;

//==========================================================================
// プロトタイプ宣言
//==========================================================================
void InitCalculastion(void);
void RotNormalize(float &fRot);												// 角度の正規化(-3.14 || 3.14超えたら補正)
template<class T> void ValueNormalize(T &Value, T MaxValue, T MinValue);	// 値の正規化処理
void InertiaCorrection(float &fMain, float fTarget, float fCntCorrection);	// 慣性補正
int Random(int nMinNum, int nMaxNum);										// ランダム生成
bool CircleRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2);						// 1と2の当たり判定(円)
bool SphereRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2);						// 1と2の当たり判定(球)
bool CylinderRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2, float fHeight1, float fHeight2);	// 1と2の当たり判定(筒)
void CheckVtx(float RotY, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin, LPD3DXMESH pMesh, BYTE *pVtxBuff);	// 全頂点チェック
void CollisionCharacter(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 &TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin);		// ぶつかったら補正
bool bHitCharacter(D3DXVECTOR3 &MainPos, D3DXVECTOR3 &MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 &TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin);			// ぶつかったかチェック
void CollisionLimitLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 &pPos, D3DXVECTOR3 posOld);								// 線分の当たり判定
bool CollisionTriangle(D3DXVECTOR3 posCenter, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld);	// 三角の当たり判定
bool CollisionFan(D3DXVECTOR3 posChar, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld);	// 扇の当たり判定
bool CollisionLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld);							// 線分の右にいるかどうか
int SetMoveLog(void);																											// ロゴ拡縮する時のインデックス取得
float MoveLog(int nIdxMoveLog, float pos, int nCntElapsed);																		// pVtx[?].posの長さになるやつ
void ResetMoveLog(int nIdxMoveLog);																								// もう一回拡縮したいとき
template<class T> void CuadricCurveComp(T& Value, const int nRange, T RangeOrigin, T Max, int& nAlphaCnt);	// 二次曲線補正

std::string extractFileName(const std::string& filePath);	// パスの最後の部分だけ取得


//==========================================================================
// 値の正規化処理
//==========================================================================
template<class T> void ValueNormalize(T &Value, T MaxValue, T MinValue)
{
	if (Value >= MaxValue)
	{// 最大値を超えたら

		// 最大値固定
		Value = MaxValue;
	}
	else if (Value <= MinValue)
	{// 最小値を下回ったら

		// 最小値固定
		Value = MinValue;
	}
}

//==========================================================================
// 二次曲線補正
//==========================================================================
template<class T> void CuadricCurveComp(T& Value, const int nRange, T RangeOrigin, T Max, int& nAlphaCnt)
{
	// 補完の値割り出し
	T Comp = ((T)Max / (T)(nRange * nRange));

	if (nAlphaCnt > nRange)
	{// 範囲超えたら戻す
		nAlphaCnt = -nRange;
	}
	else if (nAlphaCnt < -nRange)
	{// 範囲超えたら戻す
		nAlphaCnt = nRange;
	}

	// 不透明のカウント加算
	nAlphaCnt++;

	// 曲線補正
	Value = (T)((Comp * (nAlphaCnt * nAlphaCnt)) - (2 * (Comp * nAlphaCnt * RangeOrigin)) + (Comp * (RangeOrigin * RangeOrigin)));
}


#endif