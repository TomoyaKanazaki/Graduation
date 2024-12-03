//============================================
//
//	ボンボンの処理 [enemyBonbon.h]
//	Author: sakamoto kai
//
//============================================
#ifndef _ENEMY_BONBON_H_
#define _ENEMY_BONBON_H_

#include "enemy.h"

class CModelEffect;

// 足軽の敵クラス
class CEnemyBonbon : public CEnemy
{
public:
	CEnemyBonbon(int nPriority = 2);
	~CEnemyBonbon();

	// モーション
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,	// 待機状態
		ACTION_MOVE,		// 移動状態
		ACTION_TEST,		// テスト用状態
		ACTION_MAX,			// 最大
	};

	HRESULT Init(void) override;
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Hit(void);

	// 静的メンバ関数
	static CListManager<CEnemyBonbon>* GetList(void); // リスト取得

private:

	void ActionState(void);								//モーションと状態の管理
	void StateManager(void);							//状態管理
	void SetDamage(float Damage);						//ダメージの判定

	ACTION_TYPE m_Action;			// モーション
	ACTION_TYPE m_AtkAction;		// 攻撃状態記録用変数

	int m_nStateCount;			// 状態カウント

	// 静的メンバ変数
	static CListManager<CEnemyBonbon>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CEnemyBonbon>::AIterator m_iterator; // イテレーター

};

#endif