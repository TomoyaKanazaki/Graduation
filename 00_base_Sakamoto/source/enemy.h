//============================================
//
//	敵の処理 [enemy.h]
//	Author: sakamoto kai
//
//============================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "object.h"
#include "useful.h"
#include "MapSystem.h"

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

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CEnemy* Create(char* aModelName);
	HRESULT Init(void);
	virtual void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	virtual bool Hit(int nLife);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	const char* GetTextModelName(void) { return &m_cFileName[0]; }

	CModel** GetLevelModel(void) { return &m_apModel[0]; }
	CModel* GetLevelModel(int nIdx) { return m_apModel[nIdx]; }

	CMotion* GetMotion(void){
		if (m_pMotion != nullptr) { return m_pMotion; }
		else { return nullptr; }}

	int GetNumModel(void) { return m_nNumModel; }
	void SetEnemyType(ENEMY_TYPE Type) { m_EnemyType = Type; }
	ENEMY_TYPE GetEnemyType(void) { return m_EnemyType; }

	void SetMultiMatrix(bool Set) { m_bMultiMatrix = Set; }
	bool GetMultiMatrix(void) { return m_bMultiMatrix; }
	void SetUseMultiMatrix(D3DXMATRIX Set) { m_UseMultiMatrix = Set; }
	D3DXMATRIX GetUseMultiMatrix(void) { return m_UseMultiMatrix; }

	// マップ番号の設定
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	// 静的メンバ関数
	static CListManager<CEnemy>* GetList(void); // リスト取得

protected:

	void InitModel(const char* pFilename);				// モデルの初期化

private:

	void StateManager();								//状態更新
	void CollisionWall(useful::COLLISION XYZ);			//壁との当たり判定
	void CollisionDevilHole(useful::COLLISION XYZ);		//デビルホールとの当たり判定
	void CollisionOut();								//ステージ外との当たり判定

	void UpdatePos(void);								// 位置更新処理
	void Rot(void);										//移動方向処理
	virtual void Death(void);							// 死亡処理
	void MyObjCreate(void);								// 自分が保持するオブジェクトの生成
	void MoveSelect(void);								// 移動方向の選択
	void SearchWall(void);								// 壁のサーチ判定
	void MapSystemNumber(void);							// どのマスに存在しているか設定する

	CMapSystem::GRID m_Grid;	//グリット番号

	void LoadLevelData(const char* pFilename);

	int m_nLife;				// 体力

	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_posOld;		//過去の位置
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_Objmove;		//オブジェクトから影響される移動量
	D3DXVECTOR3 m_rot;			//向き
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	int m_nActionCount;			//行動のカウント
	D3DXVECTOR3 m_size;			//大きさ
	ENEMY_TYPE m_EnemyType;		//敵の種類

	float m_ColorA;				//不透明度
	char m_cFileName[128];		//ファイルの名前

	E_STATE m_State;			//状態
	int m_nStateCount;			//状態管理用カウント
	SELECT_MOVE m_SelectMove;	//移動方向

	bool m_OKL;					//左への進行が許されるかどうか
	bool m_OKR;					//右への進行が許されるかどうか
	bool m_OKU;					//上への進行が許されるかどうか
	bool m_OKD;					//下への進行が許されるかどうか

	int m_nBugCounter;			//敵がスタックした時間

	CSlowManager* m_pSlow;		// スロー情報

	//階層構造とモーションのポインタ
	CModel* m_apModel[MODEL_NUM];
	CMotion* m_pMotion;
	char* m_aModelName[MODEL_NUM];
	int m_nNumModel;

	//マップとのマトリックス情報
	bool m_bMultiMatrix;					//マトリックスの掛け合わせをするかどうか
	D3DXMATRIX m_UseMultiMatrix;			//掛け合わせるマトリックス

	// 静的メンバ変数
	static CListManager<CEnemy>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CEnemy>::AIterator m_iterator; // イテレーター

};

#endif