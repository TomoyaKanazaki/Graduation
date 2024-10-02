//============================================
//
//	移動方向のヘルプUI [MoveHelpUI.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MOVEHELPUI_H_
#define _MOVEHELPUI_H_

#include "main.h"
#include "object3D.h"

//オブジェクトプレイヤークラス
class CMoveHelpUI : public CObject3D
{
public:

	CMoveHelpUI(int nPriority = 4);
	~CMoveHelpUI();

	static CMoveHelpUI* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVerTex(void);

	//取得処理・設定処理
	int GetSample(void) { return m_nSample; }
	void SetSample(int Sample) { m_nSample = Sample; }

private:
	int m_nSample;							//サンプル変数
};
#endif