//========================================
//
// マップオブジェクト[mapobject.h]
// Author：森川駿弥
//
//========================================
#ifndef _MAPOBJECT_H_
#define _MAPOBJECT_H_

#include "main.h"
#include "model.h"

//========================================
// マップオブジェクトクラス
//========================================
class CMapObject : public CObject
{
public:
	CMapObject(int nPriority = 7);		//コンストラクタ
	~CMapObject();	//デストラクタ

	//メンバ関数
	static CMapObject* Create();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
	//メンバ変数
	CModel* m_pModel;		// モデルのポインタ
};

#endif
