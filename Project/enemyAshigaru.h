//============================================
//
//	足軽の処理 [enemyAshigaru.h]
//	Author:酒井 南勝
//
//============================================
#ifndef _ENEMY_ASHIGARU_H_
#define _ENEMY_ASHIGARU_H_

#include "enemy.h"

class CModelEffect;

// 足軽の敵クラス
class CEnemyAshigaru : public CEnemy
{
public:
	CEnemyAshigaru(int nPriority = 2);
	~CEnemyAshigaru();

	// モーション
	enum ACTION_TYPE
	{
		ACTION_BWAIT = 0,		//戦闘待機
		ACTION_SMOVE,			//探索移動
		ACTION_BMOVE,			//戦闘移動
		ACTION_SWAIT,			//探索待機
		ACTION_ATTACK,			//攻撃
		ACTION_BDEATH,			//戦闘中の死亡
		ACTION_SDEATH,			//探索中の死亡
		ACTION_DAMAGE,			//ダメージ
		ACTION_MAX,				//最大
	};

	//敵の戦闘状態
	enum BATTLESTATE
	{
		BATTLESTATE_NORMAL = 0,	//通常
		BATTLESTATE_DEATH,		//死亡
		BATTLESTATE_WAIT,		//待機
		BATTLESTATE_ATTACK,		//待機
		BATTLESTATE_DAMAGE,		//ダメージ
		BATTLESTATE_WALK,		//歩行
		BATTLESTATE_MAX,		//最大
	};

	//敵の探索状態
	enum SEARCHSTATE
	{
		SEARCHSTATE_NORMAL = 0,	//通常
		SEARCHSTATE_WALK,		//歩行
		SEARCHSTATE_TRUN,		//方向転換
		SEARCHSTATE_WAIT,		//待機
		SEARCHSTATE_SEARCH,		//探索
		SEARCHSTATE_MAX,		//最大
	};

	static CEnemyAshigaru* Create(const char* pFilename);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	void HitDamage(float fDamage);

	BATTLESTATE GetBattleState(void) { return m_BState; }
	SEARCHSTATE GetSearchState(void) { return m_SState; }

	void SetSeatchState(SEARCHSTATE State) { m_SState = State; }

private:

	void MyObjCreate(void);								//自分が保持するオブジェクトの生成
	void ActionState(void);								//モーションと状態の管理
	void BattleStateManager(void);						//戦闘状態管理
	void SearchStateManager(void);						//探索状態管理
	void SetDamage(float Damage);						//ダメージの判定

	void WalkPoint(void);								// 目的値に歩く処理
	void TurnPoint(void);								// 目的地から方向転換処理
	void CollisionPush(void);

	ACTION_TYPE m_Action;			// モーション
	ACTION_TYPE m_AtkAction;		// 攻撃状態記録用変数
	BATTLESTATE m_BState;			// 戦闘状態	
	SEARCHSTATE m_SState;			// 探索状態

	int m_nStateCount;			// 状態カウント
};

//// 足軽の敵クラス
//class CEnemyArcher : public CEnemy
//{
//public:
//	CEnemyArcher(int nPriority = 2);
//	~CEnemyArcher();
//
//	// モーション
//	enum ACTION_TYPE
//	{
//		ACTION_WAIT = 0,	//待機
//		ACTION_MOVE,		//移動
//		ACTION_ATTACK1,		//地上攻撃
//		ACTION_JAMP,		//ジャンプ
//		ACTION_MAX,			//最大
//	};
//
//	//敵の戦闘状態
//	enum BATTLESTATE
//	{
//		BATTLESTATE_NORMAL = 0,	//通常
//		BATTLESTATE_DEATH,		//死亡
//		BATTLESTATE_WAIT,		//待機
//		BATTLESTATE_DAMAGE,		//ダメージ
//		BATTLESTATE_WALK,		//歩行
//		BATTLESTATE_MAX,		//最大
//	};
//
//	//敵の探索状態
//	enum SEARCHSTATE
//	{
//		SEARCHSTATE_NORMAL = 0,	//通常
//		SEARCHSTATE_WALK,		//歩行
//		SEARCHSTATE_TRUN,		//方向転換
//		SEARCHSTATE_WAIT,		//待機
//		SEARCHSTATE_SEARCH,		//探索
//		SEARCHSTATE_MAX,		//最大
//	};
//
//	static CEnemyArcher* Create(const char* pFilename);
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void TitleUpdate(void);
//	void GameUpdate(void);
//	void Draw(void);
//
//	BATTLESTATE GetBattleState(void) { return m_BState; }
//	SEARCHSTATE GetSearchState(void) { return m_SState; }
//
//	void SetSeatchState(SEARCHSTATE State) { m_SState = State; }
//
//private:
//
//	void Death(void);
//
//	void MyObjCreate(void);								//自分が保持するオブジェクトの生成
//	void ActionState(void);								//モーションと状態の管理
//	void BattleStateManager(void);						//戦闘状態管理
//	void SearchStateManager(void);						//探索状態管理
//
//	void CollisionPush(void);
//
//	ACTION_TYPE m_Action;		// モーション
//	ACTION_TYPE m_AtkAction;	// 攻撃状態記録用変数
//	BATTLESTATE m_BState;		// 戦闘状態	
//	SEARCHSTATE m_SState;		// 探索状態
//
//	int m_nStateCount;			// 状態カウント
//};

#endif