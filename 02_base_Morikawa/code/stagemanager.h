//=============================================================================
// 
//  ステージマネージャーヘッダー [stagemanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _STAGE_MANAGER_H_
#define _STAGE_MANAGER_H_		// 二重インクルード防止

//==========================================================================
// クラス定義
//==========================================================================
// ステージマネージャークラス
class CStageManager
{
public:

	CStageManager();
	~CStageManager();

	// オーバーライド関数
	HRESULT Init();
	void Uninit();
	void Update();

	// 静的メンバ関数
	static CStageManager* Create();

private:

	// ステージロード
	HRESULT StageLoad();
};


#endif
