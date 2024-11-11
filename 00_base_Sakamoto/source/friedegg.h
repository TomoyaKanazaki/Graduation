//==========================================
//
//  目玉焼きクラス(friedegg.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#pragma once
#include "item.h"
#include "enemy.h"

//==========================================
//  クラス定義
//==========================================
class CFriedEgg : public CItem
{
public:

	// メンバ関数
	CFriedEgg(int nPriority = 3);
	~CFriedEgg();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Hit(CPlayer* pPlayer) override;

	void SetEnemy(const CEnemy::ENEMY_TYPE eType) { m_eCreateType = eType; };

	// 静的メンバ関数
	static CListManager<CFriedEgg>* GetList(void); // リスト取得
	static CFriedEgg* Create(const CEnemy::ENEMY_TYPE eType, const CMapSystem::GRID& pos);

private:

	// メンバ関数
	void Move(D3DXVECTOR3& pos) override;

	// 静的メンバ変数
	static CListManager<CFriedEgg>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CFriedEgg>::AIterator m_iterator; // イテレーター
	float m_fDeleteTimer; // 自動消滅タイマー
	bool m_bMove; // 移動可能フラグ
	CEnemy::ENEMY_TYPE m_eCreateType; // 消滅した時に生成する敵の種類

};
