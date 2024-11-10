//============================================
//
//	子デビル処理 [enemyMedaman.h]
//	Author: sakamoto kai
//
//============================================
#ifndef _ENEMY_YOUNGDEVIL_H_
#define _ENEMY_YOUNGDEVIL_H_

#include "enemy.h"

class CModelEffect;

// 足軽の敵クラス
class CEnemyYoungDevil : public CEnemy
{
public:
	CEnemyYoungDevil(int nPriority = 2);
	~CEnemyYoungDevil();

	// モーション
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,	// 待機状態
		ACTION_MOVE,		// 移動状態
		ACTION_TEST,		// テスト用状態
		ACTION_MAX,			// 最大
	};

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	void HitDamage(float fDamage);

	// 静的メンバ関数
	static CListManager<CEnemyYoungDevil>* GetList(void); // リスト取得

private:

	void ActionState(void);								//モーションと状態の管理
	void StateManager(void);							//状態管理
	void SetDamage(float Damage);						//ダメージの判定

	ACTION_TYPE m_Action;			// モーション
	ACTION_TYPE m_AtkAction;		// 攻撃状態記録用変数

	int m_nStateCount;			// 状態カウント

	// 静的メンバ変数
	static CListManager<CEnemyYoungDevil>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CEnemyYoungDevil>::AIterator m_iterator; // イテレーター

};

#endif