//============================================
//
//	敵の処理 [enemy.h]
//	Author: sakamoto kai
//
//============================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "objectcharacter.h"
#include "useful.h"
#include "MapSystem.h"
#include "AStar.h"

//前方宣言
class CObject3D;
class CLevelModelEffect;
class CSlowManager;

//オブジェクトエネミークラス
class CEnemy : public CObjectCharacter
{
public:
	CEnemy(int nPriority = 2);
	~CEnemy();

	enum ENEMY_TYPE
	{
		ENEMY_MEDAMAN = 0,
		ENEMY_BONBON,
		ENEMY_LITTLEDEVIL,
		ENEMY_MAX,
		ENEMY_NONE
	};

	//敵の状態
	enum HIT_STATE
	{
		HIT_STATE_NORMAL = 0,	//通常
		HIT_STATE_DAMAGE,		//ダメージ
		HIT_STATE_INVINCIBLE,	//無敵
		E_STATE_EGG,			//たまごやき
		HIT_STATE_MAX,			//最大
	};

	static CEnemy* Create(const ENEMY_TYPE eType, const CMapSystem::GRID& grid);
	HRESULT Init(void);
	virtual void Uninit(void);
	void Update(void);
	void Draw(void);

	virtual bool Hit(void);

	void SetEnemyType(ENEMY_TYPE Type) { m_EnemyType = Type; }
	ENEMY_TYPE GetEnemyType(void) { return m_EnemyType; }

	void SetHitState(HIT_STATE State) { m_HitState = State; }
	HIT_STATE GetHitState(void) { return m_HitState; }
	void SetHitStateCount(int Cnt) { m_nHitStateCount = Cnt; }
	int GetHitStateCount(void) { return m_nHitStateCount; }

	void ChangeMoveState(CMoveState* pMoveState) override;   // 移動状態変更

	// 静的メンバ関数
	static CListManager<CEnemy>* GetList(void); // リスト取得

protected:

	HRESULT InitModel(const char* pFilename);				// モデルの初期化

private:

	void MoveStateManager(D3DXVECTOR3& posMy);				//状態更新
	void HitStateManager(D3DXVECTOR3& posMy);				//状態更新
	void CollisionWall(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& size,useful::COLLISION XYZ);			//壁との当たり判定
	void CollisionDevilHole(useful::COLLISION XYZ);		//デビルホールとの当たり判定
	void CollisionOut(D3DXVECTOR3& posMy);				//ステージ外との当たり判定

	void UpdatePos(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy);	// 位置更新処理
	void Rot(D3DXVECTOR3& rotMy);						//移動方向処理
	virtual void Death(void);							// 死亡処理
	void MoveSelect(void);								// 移動方向の選択
	void SearchWall(D3DXVECTOR3& posMy);				// 壁のサーチ判定
	void Coordinate(); // 最短経路探索
	void Route();	// 最短経路をたどる
	void Effect(); // エフェクトを生成

	CMapSystem::GRID m_SelectGrid;	//方向転換を行った時のグリッド番号

	int m_nActionCount;			//行動のカウント
	ENEMY_TYPE m_EnemyType;		//敵の種類

	float m_ColorA;				//不透明度

	int m_nMoveStateCount;		//状態管理用カウント

	HIT_STATE m_HitState;		//状態
	int m_nHitStateCount;		//状態管理用カウント

	int m_nBugCounter;			//敵がスタックした時間

	CSlowManager* m_pSlow;		// スロー情報

	CEffekseer* m_pEffect; // エフェクト
	CMoveState* m_pMoveState;		// 移動状態

	// 静的メンバ変数
	static CListManager<CEnemy>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CEnemy>::AIterator m_iterator; // イテレーター
	CMapSystem::GRID* m_pPath; // プレイヤーへの最短経路
	float m_fCoordinateTimer; // 経路探索間隔
	int m_nNumCoordinate; // 最短経路の要素数
	int m_nTargetIndex; // 次に向かうべきグリッドのインデックス
};

#endif
