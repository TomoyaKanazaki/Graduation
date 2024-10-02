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
		ACTION_WAIT = 0,	//待機
		ACTION_MOVE,		//移動
		ACTION_READY,		//術準備
		ACTION_ULTIMATE,	//術発動
		ACTION_GOATTACK,	//突進
		ACTION_DEATHEVENT,	//ボス死亡時イベント
		ACTION_MAX,			//最大
	};

	//プレイヤーの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_DEATH,		//死亡
		STATE_WAIT,			//待機
		STATE_DAMAGE,		//ダメージ
		STATE_MAX,			//最大
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
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	STATE GetState(void) { return m_State; }
	bool GetJump(void) { return m_bJump; }
	void SetCameraPos(D3DXVECTOR3 pos) { m_CameraPos = pos; }
	D3DXVECTOR3 GetCameraPos(void) { return m_CameraPos; }
	float GetCameraHeightRot(void) { return m_HeightCameraRot; }
	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	void SetModelDisp(bool Sst);

	void HitDamage(float Damage);
	bool SortObject(D3DXVECTOR3 pos);					// オブジェクトとのソート処理

private:
	void MyObjCreate(void);		//自分が保持するオブジェクトの生成
	void ActionState(void);		//モーションと状態の管理
	void StateManager(void);	//状態管理
	void Move(void);			//移動処理
	void Rot(void);				//移動方向処理
	void CollisionWall(useful::COLLISION XYZ);	//壁との当たり判定
	void CollisionMapModel(useful::COLLISION XYZ);	//壁との当たり判定

	void CameraPosUpdate(void);	//カメラ位置更新処理
	void PosUpdate(void);		//位置更新処理
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
	float m_HeightCameraRot;	//カメラの高さの角度
	D3DXVECTOR3 m_AtkPos;		//攻撃位置
	D3DXVECTOR3 m_CameraPos;	//カメラ位置位置
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	bool m_bJump;				//ジャンプをしたかどうか
	int m_nActionCount;			//行動のカウント
	STATE m_State;				//状態
	int m_nStateCount;			//状態管理用カウント

	float m_CollisionRot;		//当たり判定用の向き

	float m_fLife;					//ライフ

	CSlowManager* m_pSlow;		// スロー

	//階層構造とモーションのポインタ
	CModel* m_apModel[64];
	CMotion* m_pMotion;
	char* m_aModelName[64];
	int m_nNumModel;
};

#endif