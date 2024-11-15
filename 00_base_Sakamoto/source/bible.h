//============================================
//
//	聖書の処理 [bible.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _BIBLE_H_
#define _BIBLE_H_

#include "main.h"
#include "item.h"
#include "MapSystem.h"

//前方宣言
class CObjGauge2D;

//オブジェクト聖書クラス
class CBible : public CItem
{
public:

	CBible(int nPriority = 2);
	~CBible();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetGrid(const CMapSystem::GRID& pos) override;
	bool Hit(CPlayer* pPlayer) override;

	// 静的メンバ関数
	static CListManager<CBible>* GetList(void); // リスト取得

private:

	// メンバ関数
	void Move(D3DXVECTOR3& pos) override;

	// メンバ変数
	CListManager<CBible>::AIterator m_iterator; // イテレーター
	D3DXVECTOR3 m_posBase; // 移動の中心座標
	float m_fMoveTime; // 移動時間

	// 静的メンバ変数
	static CListManager<CBible>* m_pList; // オブジェクトリスト

};

#endif
