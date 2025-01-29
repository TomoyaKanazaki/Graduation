//============================================
//
//	獲得スコアのUI [getscoreUi.h]
//	Author:澗口将太郎
//
//============================================
#ifndef _GETSCOREUI_H_
#define _GETSCOREUI_H_

#include "main.h"
#include "object.h"

//マクロ定義
#define MAX_NUM (4)
//前方宣言
class CNumBillboard;

//オブジェクトプレイヤークラス
class CGetscoreUi : public CObject
{
public:

	CGetscoreUi(int nPriority = 6);
	~CGetscoreUi();

	static CGetscoreUi* Create(int nGetScore,D3DXVECTOR3 pos,int nPriority = 6);

	HRESULT Init(void) ;
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CNumBillboard* m_pNum[MAX_NUM];
	int m_nLife;		//オブジェクトの寿命

};
#endif