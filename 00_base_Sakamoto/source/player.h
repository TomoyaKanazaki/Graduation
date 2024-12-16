//============================================
//
//	プレイヤーの処理 [Player.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "objectcharacter.h"
#include "useful.h"
#include "MapSystem.h"

//前方宣言
class CSlowManager;
class CLifeUi;
class CObjectX;
class CScore;
class CObjmeshField;
class CDevil;
class CObjectBillboard;

//オブジェクトプレイヤークラス
class CPlayer : public CObjectCharacter
{
public:
	CPlayer(int nPriority = 2);
	~CPlayer();

	//プレイヤーのモーション
	enum ACTION_TYPE
	{
		ACTION_WAIT = 0,		// 待機
		ACTION_MOVE,			// 移動
		ACTION_ENEMYDEATH,		// 倒れる死亡状態
		ACTION_JUMPEAT,			// ジャンプして食べる
		ACTION_EAT,				// ジャンプせず食べる
		ACTION_ATTACK,			// 炎で攻撃
		ACTION_GETITEM,			// アイテムを持ち上げる
		ACTION_PRESSDEATH,		// バタバタ死亡モーション
		ACTION_MAX,				// 最大
	};

	// アイテムの種類
	enum ITEM_TYPE
	{
		TYPE_NONE = 0,		// 何もない
		TYPE_CROSS,			// 十字架
		TYPE_BIBLE,			// 聖書
		TYPE_MAX
	};

	HRESULT Init(int PlayNumber) override;
	void Uninit(void);
	void Update(void);
	void Draw(void);

	virtual void Death(void);	//死亡処理
	void Reivel(D3DXVECTOR3& posThis);	//復活処理
	void PlayerNumberDisp(bool Set);	//プレイヤー番号UIの表示状態処理

	void SetPlayNumber(int Number) { m_nPlayNumber = Number; }
	int GetPlayNumber(void) { return m_nPlayNumber; }

	bool GetJump(void) { return m_bJump; }
	void SetCameraPos(D3DXVECTOR3 pos) { m_CameraPos = pos; }
	D3DXVECTOR3 GetCameraPos(void) { return m_CameraPos; }
	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	void SetModelDisp(bool Sst);
	void SetPartsDisp(int nParts, bool Set);
	void SetPressObj(bool Set) { m_bPressObj = Set; }
	bool GetPressObj(void) { return m_bPressObj; }
	CScore* GetScore(void) { return m_pScore; }

	void SetLife(int Set) { m_nLife = Set; }
	int GetLife(void) { return m_nLife; }

	void SetStateCount(int Set) { m_nStateCount = Set; }
	int GetStateCount(void) { return m_nStateCount; }

	void SetUseItem(bool bUse) { m_UseItem = bUse; }
	bool GetbUseItem() { return m_UseItem; }

	bool GetGritCenter() { return m_bGritCenter; }
	
	void ChangeMoveState(CMoveState* pMoveState) override;		// 移動状態変更

	void SetItemType(ITEM_TYPE eType);
	ITEM_TYPE GetItemType() { return m_eItemType; }		// アイテムの種類取得

	void ResetCrossTimer() { m_fCrossTimer = 0.0f; } // 十字架所持時間の初期化

	// 静的メンバ関数
	static CListManager<CPlayer>* GetList(void);	// リスト取得

private:

	void ActionState(void);							//モーションと状態の管理
	void StateManager(D3DXVECTOR3& posMy, D3DXVECTOR3& rotThis);			//状態管理
	void Move(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy);								//移動処理
	D3DXVECTOR3 MoveInputKey(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy,D3DXVECTOR3 Move);		//移動入力キーボード
	D3DXVECTOR3 MoveInputPadStick(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy, D3DXVECTOR3 Move);//移動入力パッドスティック
	D3DXVECTOR3 MoveInputPadKey(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy, D3DXVECTOR3 Move);	//移動入力パッドキー
	void Rot(D3DXVECTOR3& rotMy);									//移動方向処理
	void Attack(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy);			//攻撃処理
	void CollisionWall(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ);		//壁との当たり判定
	void CollisionWaitRailBlock(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ);	//止まっているレールブロックとの当たり判定
	void CollisionMoveRailBlock(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ);	//動いているレールブロックとの当たり判定
	void CollisionWaitRock(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ);	//止まっている岩との当たり判定
	void CollisionMoveRock(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ);	//動いている岩との当たり判定
	void SearchWall(D3DXVECTOR3& posMy);							//壁のサーチ判定
	void CollisionDevilHole(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ);	//デビルホールとの当たり判定
	void CollisionEnemy(D3DXVECTOR3& posMy);					// 敵との当たり判定
	void CollisionStageOut(D3DXVECTOR3& posMy);					// ステージ外の当たり判定
	bool CollisionStageIn(D3DXVECTOR3& posMy);					// ステージ内にいるかどうか
	void CollisionPressStageOut(D3DXVECTOR3& posMy);			// ステージ外の圧死判定
	void CollisionPressWall(D3DXVECTOR3& posThis, int Rot);		//壁との圧死判定

	void CameraPosUpdate(D3DXVECTOR3& posMy);	//カメラ位置更新処理
	void PosUpdate(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy);		//位置更新処理
	void ObjPosUpdate(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy);	//オブジェクトによる位置更新処理
	void RotUpdate(D3DXVECTOR3& rotMy);		//向き更新処理
	void EggMove(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy);			//卵の動き
	void PrintFoot(const D3DXVECTOR3& rotThis);			// 足跡の設置

	void UI_Create(void);			//所持するUIの生成

	void ControlEffect(CEffekseer* pTarget, const D3DXVECTOR3* pPos = nullptr); // エフェクトの操作
	void RotationEffect(CEffekseer* pTarget); // エフェクトの操作
	void MoveEffect(CEffekseer* pTarget, const D3DXVECTOR3* pPos = nullptr); // エフェクトの操作

	int m_nPlayNumber;				//プレイ用番号
	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;		//攻撃状態記録用変数
	D3DXVECTOR3 m_rotDest;			//向きの目的地
	D3DXVECTOR3 m_AutoMoveRot;		//自動移動の移動方向
	D3DXVECTOR3 m_AtkPos;			//攻撃位置
	D3DXVECTOR3 m_CameraPos;		//カメラ位置位置
	bool m_bJump;					//ジャンプをしたかどうか
	int m_nActionCount;				//行動のカウント
	int m_nStateCount;				//状態管理用カウント

	float m_CollisionRot;			//当たり判定用の向き

	CMapSystem::GRID m_OldGrid;		//グリット番号
	bool m_bGritCenter;				//グリットの中心位置にいるかどうか

	int m_nLife;					//ライフ
	int m_nTime;
	
	bool m_bInput;					//入力を行ったかどうか
	bool m_bPressObj;				//オブジェクトに押されているかどうか
	bool m_bInvincible;				//無敵かどうか
	int m_nInvincibleCount;			//無敵時間

	bool m_UseItem;					//アイテムが使用可能かどうか

	CSlowManager* m_pSlow;			// スロー

	ITEM_TYPE m_eItemType;			// 持ってるアイテムの種類
	CScore* m_pScore;				// スコアのポインタ

	CObjectX* m_pUpEgg;				//卵モデルの上
	CObjectX* m_pDownEgg;			//卵モデルの下

	CObjectBillboard* m_pP_NumUI;	//プレイヤー番号UI		

	CMoveState* m_pMoveState;		// 移動状態

	CEffekseer* m_pEffectEgg;		// 卵のエフェクト
	CEffekseer* m_pEffectSpeed;		// 加減速のエフェクト
	CEffekseer* m_pEffectGuide; // 復活位置のエフェクト
	CEffekseer* m_pEffectItem; // アイテム所持エフェクト

	// 静的メンバ変数
	static CListManager<CPlayer>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CPlayer>::AIterator m_iterator; // イテレーター
	float m_fCrossTimer; // 十字架を持っていられる時間
};

#endif