//============================================
//
//	敵の処理 [enemy.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "object.h"
#include "useful.h"

//前方宣言
class CModel;
class CMotion;
class CObject3D;
class CLevelModelEffect;
class CSlowManager;

#define SCREEN_POS_NUM	(8)		// スクリーン座標の数
#define SEARCH_POS_NUM	(32)	// 探索位置の数
#define MODEL_NUM		(64)	// モデルの数

//オブジェクトプレイヤークラス
class CEnemy : public CObject
{
public:
	CEnemy(int nPriority = 2);
	~CEnemy();

	//敵の状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_BATTLE,		//戦闘
		STATE_SEATCH,		//探索
		STATE_MAX,			//最大
	};

	enum ENEMY_TYPE
	{
		ENEMY_ASHIGARU = 0,
		ENEMY_BOWMAN,
		ENEMY_SAMURAI,
		ENEMY_BOSS,
		ENEMY_MAX,
	};

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CEnemy* Create(char* aModelName);
	HRESULT Init(void);
	virtual void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	virtual void HitDamage(float fDamage);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3* GetScreenPos(void) { return &m_ScreenPos[0]; }
	int GetSeatchCount(void) { return m_nSeatchCount; }
	void SetSeatchCount(int nCnt) { m_nSeatchCount = nCnt; }
	D3DXVECTOR3* GetSeatchPos(void) { return &m_SeatchPos[0]; }
	void SetSeatchPos(D3DXVECTOR3 pos, int nCnt) { m_SeatchPos[nCnt] = pos; }
	bool GetSeatchLoop(void) { return m_bSeatchLoop; }
	void SetSeatchLoop(bool Loop) { m_bSeatchLoop = Loop; }
	void SetState(STATE State) { m_State = State; }
	STATE GetState(void) { return m_State; }
	void SetDeath(bool Set) { m_bDeath = Set; }
	bool GetDeath(void) { return m_bDeath; }
	void SetLife(float Set) { m_fLife = Set; }
	float GetLife(void) { return m_fLife; }
	void SetAttack(bool Set) { m_bAttack = Set; }
	bool GetAttack(void) { return m_bAttack; }
	void SetSearchDistance(float Distance) { m_SearchDistance = Distance; }
	const char* GetTextModelName(void) { return &m_cFileName[0]; }

	CModel** GetLevelModel(void) { return &m_apModel[0]; }
	CModel* GetLevelModel(int nIdx) { return m_apModel[nIdx]; }

	CMotion* GetMotion(void){
		if (m_pMotion != nullptr) { return m_pMotion; }
		else { return nullptr; }}

	int GetNumModel(void) { return m_nNumModel; }
	void SetEnemyType(ENEMY_TYPE Type) { m_EnemyType = Type; }
	ENEMY_TYPE GetEnemyType(void) { return m_EnemyType; }

protected:

	void InitModel(const char* pFilename);				// モデルの初期化

	bool DistancePlayer(void);							// プレイヤーと間合いを取る処理
	void CollisionPush(void);							// 自分以外の敵を押し出す処理
	void SearchPlayer(void);							// プレイヤーを索敵する処理
	void CreateWeaponEffect(int ModelNumber);							// 武器の残像エフェクトを生成する
	bool SortObject(D3DXVECTOR3 pos);					// オブジェクトとのソート処理

private:

	virtual void BattleStateManager(void){;};			// 戦闘状態管理
	virtual void SearchStateManager(void){;};			// 探索状態管理
	virtual void UpdateState();							// 状態更新
	void CollisionWall(useful::COLLISION XYZ);	//壁との当たり判定

	void UpdatePos(void);								// 位置更新処理
	virtual void Death(void);							// 死亡処理
	void ScreenCollision(void);							// スクリーン位置の当たり判定
	void MyObjCreate(void);								// 自分が保持するオブジェクトの生成


	void LoadLevelData(const char* pFilename);

	D3DXVECTOR3 m_pos;							//位置
	D3DXVECTOR3 m_posOld;						//過去の位置
	D3DXVECTOR3 m_move;							//移動量
	D3DXVECTOR3 m_Objmove;						//オブジェクトから影響される移動量
	D3DXVECTOR3 m_rot;							//向き
	D3DXVECTOR3 m_AtkPos;						//攻撃位置
	D3DXVECTOR3 m_SeatchPos[SEARCH_POS_NUM];	//探索位置
	int m_nSeatchCount;							//探索段階のカウント
	bool m_bSeatchLoop;							//探索がループするかどうか
	D3DXMATRIX m_mtxWorld;						//ワールドマトリックス
	int m_nActionCount;							//行動のカウント
	bool m_nAttackHit;							//攻撃が当たったかどうか
	D3DXVECTOR3 m_size;							//大きさ
	STATE m_State;								//状態
	int m_nStateCount;							//状態管理用カウント
	bool m_bJump;								//ジャンプ状態かどうか
	bool m_bDeath;								//死亡状態かどうか
	bool m_bAttack;								//攻撃状態かどうか
	ENEMY_TYPE m_EnemyType;						//敵の種類
	float m_SearchDistance;						//索敵距離
	float m_fLife;								//体力

	D3DXVECTOR3 m_ScreenPos[SCREEN_POS_NUM];	//スクリーン座標
	float m_CollisionRot;						//当たり判定用の向き

	float m_ColorA;								//不透明度
	char m_cFileName[128];						//ファイルの名前

	CSlowManager* m_pSlow;						// スロー情報

	//階層構造とモーションのポインタ
	CModel* m_apModel[MODEL_NUM];
	CMotion* m_pMotion;
	char* m_aModelName[MODEL_NUM];
	int m_nNumModel;
};

#endif