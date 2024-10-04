//========================================
//
//チュートリアル[tutorial.h]
//Author：森川駿弥
//
//========================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
#include "main.h"
#include "manager.h"

//========================================
//前方宣言
//========================================
class CObjectX;
class CObject3D;
class CLight;
class CPlayer;
class CIdxMeshField;
class CField;
class CFade;
class CGauge;
class CObject2D;
class CMapObject;

//========================================
//チュートリアルクラス
//========================================
class CTutorial : public CScene
{
public:
	CTutorial();	//コンストラクタ
	~CTutorial();	//デストラクタ

	//メンバ関数
	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CObjectX* m_pObjectX;		// Xファイルオブジェクトのポインタ
	CPlayer* m_pPlayer;			// プレイヤーのポインタ
	CIdxMeshField* m_pIdxMesh;	// メッシュフィールドのポインタ
	CField* m_pField;			// ポリゴンのポインタ
	CFade* m_pFade;				// フェードのポインタ
	CMapObject* m_pMobj;		// マップオブジェクトのポインタ
	CObject2D* m_pObj2D;		// オブジェクト2Dのポインタ
	bool m_bEnd;
};

#endif
