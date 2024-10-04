//=============================================================================
// 
//  ステージエディターヘッダー [stageedittor.h]
//  Author : 小原立暉
// 
//=============================================================================

#ifndef _STAGE_EDITTOR_H_
#define _STAGE_EDITTOR_H_		// 二重インクルード防止

#include "object.h"

class CStageObj;

//==========================================================================
// クラス定義
//==========================================================================
// ステージの配置物クラス
class CStageEdittor : public CObject
{
public:

	CStageEdittor(void);
	~CStageEdittor();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	CStageObj* GetObj() { return m_pStageObj; }

	// 静的メンバ関数
	static CStageEdittor* Create();

private:

	// メンバ関数
	void SetType();
	void Control();

	CStageObj* m_pStageObj;
	int m_nType;
};


#endif