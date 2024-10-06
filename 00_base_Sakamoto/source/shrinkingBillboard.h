//============================================
//
//	収縮するビルボード演出処理 [shrinkingBillboard.h]
//	Author: Kai Takada
//
//============================================
#ifndef _SHRINKING_BILLBOARD_H_
#define _SHRINKING_BILLBOARD_H_

#include "main.h"
#include "object.h"

//========================
//クラス定義
//========================
class CObjectBillboard;

//オブジェクトプレイヤークラス
class CShrinkingBillboard : public CObject
{
public:

	CShrinkingBillboard(int nPriority = 7);
	~CShrinkingBillboard();

	static CShrinkingBillboard* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetRadius(float fRadMin, float fRadMax);
	void SetFrameAll(int nFrame);
	int GetFrame() { return m_nShrinkFrameAll; }
	int GetFrameNow() { return m_nShrinkNow; }
	void SetPos(D3DXVECTOR3 pos);

private:
	int m_nShrinkFrameAll;		//縮む総フレーム数
	int m_nShrinkNow;		//現在のフレーム
	float m_fShrinkRadius;		//フレームで縮ませるサイズ
	float m_fHoldTime;

	CObjectBillboard* m_pMax;
	CObjectBillboard *m_pMin;
};
#endif