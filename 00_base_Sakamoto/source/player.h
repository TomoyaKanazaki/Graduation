//============================================
//
//	プレイヤーの処理 [Player.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object.h"
#include "useful.h"
#include "MapSystem.h"

//前方宣言
class CModel;
class CMotion;
class CObject2D;
class CObject3D;
class CObjGauge2D;
class CUltimate;
class CSlowManager;
class CObjectBillboard;
class CNumber;
class CMoveHelpUI;
class CParameterUI;

class CEnemy;
class CBoss;
class CLifeUi;
class CFire;

//オブジェクトプレイヤークラス
class CPlayer : public CObject
{
private:
	static const int ENCOUNT_MAX = 128;

public:
	CPlayer(int nPriority = 2);
	~CPlayer();

	//プレイヤーのモーション
	enum ACTION_TYPE
	{
		ACTION_WAIT = 0,		// 待機
		ACTION_MOVE,			// 移動
		ACTION_EGG,				// 卵状態(未完成００
		//ACTION_ATTACK,			// 攻撃状態(未完成０１
		ACTION_DEATH,			// 死亡状態(未完成０２
		ACTION_MAX,				// 最大
		ACTION_NONE
	};

	//プレイヤーの状態
	enum STATE
	{
		STATE_WAIT = 0,		//待機
		STATE_WALK,			//歩き
		STATE_DEATH,		//死亡
		STATE_EGG,			//卵
		STATE_ATTACK,		//攻撃
		STATE_MAX,			//最大
		STATE_NONE
	};

	// プレイヤーの移動方向
	enum MOVE_STATE
	{
		MOVE_STATE_WAIT = 0,	// 待機
		MOVE_STATE_LEFT,		// 左方向
		MOVE_STATE_RIGHT,		// 右方向
		MOVE_STATE_UP,			// 上方向
		MOVE_STATE_DOWN,		// 下方向
		MOVE_STATE_MAX,			// 最大
		MOVE_STATE_NONE
	};

	// アイテムの種類
	enum ITEM_TYPE
	{
		TYPE_NONE = 0,		// 何もない
		TYPE_CROSS,			// 十字架
		TYPE_BIBLE,			// 聖書
		TYPE_MAX
	};

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CPlayer* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void TutorialUpdate(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetMoveX(float moveX) { m_move.x = moveX; }
	void SetMoveZ(float moveZ) { m_move.z = moveZ; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetObjMoveX(float move) { m_Objmove.x = move; }
	void SetObjMoveZ(float move) { m_Objmove.z = move; }
	D3DXVECTOR3 GetObjMove(void) { return m_Objmove; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	void SetState(STATE State) { m_State = State; }
	STATE GetState(void) { return m_State; }
	bool GetJump(void) { return m_bJump; }
	void SetCameraPos(D3DXVECTOR3 pos) { m_CameraPos = pos; }
	D3DXVECTOR3 GetCameraPos(void) { return m_CameraPos; }
	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	void SetModelDisp(bool Sst);
	void SetPartsDisp(int nParts, bool Set);
	void SetMultiMatrix(bool Set) { m_bMultiMatrix = Set; }
	bool GetMultiMatrix(void) { return m_bMultiMatrix; }
	void SetUseMultiMatrix(D3DXMATRIX Set) { m_UseMultiMatrix = Set; }
	D3DXMATRIX GetUseMultiMatrix(void) { return m_UseMultiMatrix; }
	void SetPressObj(bool Set) { m_bPressObj = Set; }
	bool GetPressObj(void) { return m_bPressObj; }

	void SetUseItem(bool bUse) { m_UseItem = bUse; }
	bool GetbUseItem() { return m_UseItem; }

	bool GetGritCenter() { return m_bGritCenter; }

	// マップ番号の設定
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	void Death(void);
	bool SortObject(D3DXVECTOR3 pos);					// オブジェクトとのソート処理

	void SetItemType(ITEM_TYPE eType);
	ITEM_TYPE GetItemType() { return m_eItemType; }		// アイテムの種類取得

	// 静的メンバ関数
	static CListManager<CPlayer>* GetList(void);	// リスト取得

private:

	void ActionState(void);							//モーションと状態の管理
	void StateManager(void);						//状態管理
	void Move(void);								//移動処理
	void Rot(void);									//移動方向処理
	void Attack(void);								//攻撃処理
	void CollisionWall(useful::COLLISION XYZ);		//壁との当たり判定
	void CollisionPressWall(useful::COLLISION XYZ);	//壁との圧死判定
	void CollisionWaitRailBlock(useful::COLLISION XYZ);	//止まっているレールブロックとの当たり判定
	void CollisionMoveRailBlock(useful::COLLISION XYZ);	//動いているレールブロックとの当たり判定
	void CollisionWaitRock(useful::COLLISION XYZ);	//止まっている岩との当たり判定
	void CollisionMoveRock(useful::COLLISION XYZ);	//動いている岩との当たり判定
	void SearchWall(void);							//壁のサーチ判定
	void CollisionDevilHole(useful::COLLISION XYZ);	//デビルホールとの当たり判定
	void CollisionEnemy(void);						// 敵との当たり判定
	void CollisionStageOut(void);					// ステージ外の当たり判定
	void CollisionPressStageOut(void);				// ステージ外の圧死判定

	void CameraPosUpdate(void);	//カメラ位置更新処理
	void PosUpdate(void);		//位置更新処理
	void ObjPosUpdate(void);	//オブジェクトによる位置更新処理
	void RotUpdate(void);		//向き更新処理

	void DebugKey(void);		//デバッグキー

	void LoadLevelData(const char* pFilename);

	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;	//攻撃状態記録用変数
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_posOld;		//過去の位置
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_Objmove;		//オブジェクトから影響される移動量
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_rotDest;		//向きの目的地
	D3DXVECTOR3 m_AutoMoveRot;	//自動移動の移動方向
	D3DXVECTOR3 m_size;			//大きさ
	D3DXVECTOR3 m_AtkPos;		//攻撃位置
	D3DXVECTOR3 m_CameraPos;	//カメラ位置位置
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	bool m_bJump;				//ジャンプをしたかどうか
	int m_nActionCount;			//行動のカウント
	STATE m_State;				//状態
	MOVE_STATE m_MoveState;		//移動方向
	int m_nStateCount;			//状態管理用カウント

	float m_CollisionRot;		//当たり判定用の向き

	CMapSystem::GRID m_Grid;	//グリット番号
	bool m_bGritCenter;			//グリットの中心位置にいるかどうか

	int m_nLife;				//ライフ
	bool m_OKL;					//左への進行が許されるかどうか
	bool m_OKR;					//右への進行が許されるかどうか
	bool m_OKU;					//上への進行が許されるかどうか
	bool m_OKD;					//下への進行が許されるかどうか
	bool m_bInput;				//入力を行ったかどうか
	bool m_bPressObj;			//オブジェクトに押されているかどうか

	bool m_UseItem;				//アイテムが使用可能かどうか

	CSlowManager* m_pSlow;		// スロー

	ITEM_TYPE m_eItemType;		// 持ってるアイテムの種類

	//階層構造とモーションのポインタ
	CModel* m_apModel[64];
	CMotion* m_pMotion;
	char* m_aModelName[64];
	int m_nNumModel;
	CLifeUi* m_pLifeUi;

	//マップとのマトリックス情報
	bool m_bMultiMatrix;					//マトリックスの掛け合わせをするかどうか
	D3DXMATRIX m_UseMultiMatrix;			//掛け合わせるマトリックス

	// 静的メンバ変数
	static CListManager<CPlayer>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CPlayer>::AIterator m_iterator; // イテレーター
};

#endif