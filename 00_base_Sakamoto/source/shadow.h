//============================================
//
//	影処理 [shadow.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "object3D.h"

//オブジェクトシャドウクラス
class CShadow : public CObject3D
{
public:

	CShadow(int nPriority = 7);
	~CShadow();

	static CShadow* Create(const D3DXVECTOR3& pos, float fWidth, float fHeight);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void SetBaseHeight(float fHeight) { m_fHeight = fHeight; }

	// 静的メンバ関数
	static CListManager<CShadow>* GetList(void);	// リスト取得

private:
	// 静的メンバ変数
	static CListManager<CShadow>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CShadow>::AIterator m_iterator; // イテレーター
	D3DXVECTOR3 m_sizeBase;
	float m_fHeight;
};

#endif