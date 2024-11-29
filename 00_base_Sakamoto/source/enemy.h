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
	enum E_STATE
	{
		E_STATE_WAIT = 0,	//待機
		E_STATE_TRUN,		//方向転換
		E_STATE_WALK,		//歩行
		E_STATE_EGG,		//目玉焼き
		E_STATE_DEATH,		//死亡
		E_STATE_MAX,		//最大
	};

	//現在の移動方向
	enum SELECT_MOVE
	{
		SELECT_MOVE_RIGHT = 0,	//右
		SELECT_MOVE_LEFT,		//左
		SELECT_MOVE_UP,			//上
		SELECT_MOVE_DOWN,		//下
		SELECT_MOVE_MAX,		//最大
	};

	static CEnemy* Create(const ENEMY_TYPE eType, const CMapSystem::GRID& grid);
	HRESULT Init(void);
	virtual void Uninit(void);
	void Update(void);
	void Draw(void);

	virtual bool Hit(int nLife);

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	void SetEnemyType(ENEMY_TYPE Type) { m_EnemyType = Type; }
	ENEMY_TYPE GetEnemyType(void) { return m_EnemyType; }

	// マップ番号の設定
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	void HitDamage(float fDamage) {};

	// 静的メンバ関数
	static CListManager<CEnemy>* GetList(void); // リスト取得

protected:

	HRESULT InitModel(const char* pFilename);				// モデルの初期化

private:

	void StateManager(D3DXVECTOR3& posMy);				//状態更新
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

	CMapSystem::GRID m_Grid;		//グリット番号
	CMapSystem::GRID m_SelectGrid;	//方向転換を行った時のグリッド番号

	int m_nLife;				// 体力

	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_Objmove;		//オブジェクトから影響される移動量
	int m_nActionCount;			//行動のカウント
	ENEMY_TYPE m_EnemyType;		//敵の種類

	float m_ColorA;				//不透明度

	E_STATE m_State;			//状態
	int m_nStateCount;			//状態管理用カウント
	SELECT_MOVE m_SelectMove;	//移動方向

	bool m_OKL;					//左への進行が許されるかどうか
	bool m_OKR;					//右への進行が許されるかどうか
	bool m_OKU;					//上への進行が許されるかどうか
	bool m_OKD;					//下への進行が許されるかどうか

	int m_nBugCounter;			//敵がスタックした時間

	CSlowManager* m_pSlow;		// スロー情報

	CEffekseer* m_pEffect; // エフェクト

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
