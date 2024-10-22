//============================================
//
//	デビルの処理(仮) [devil.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _DEVIL_H_
#define _DEVIL_H_

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
class CPlayer;

//オブジェクトプレイヤークラス
class CDevil : public CObject
{
private:
	static const int ENCOUNT_MAX = 128;

public:
	CDevil(int nPriority = 2);
	~CDevil();

	//プレイヤーのモーション
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
	static CDevil* Create();
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
	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	void SetDevilPos(D3DXVECTOR3 size) { m_DevilPos = size; }
	D3DXVECTOR3 GetDevilPos(void) { return m_DevilPos; }
	void SetDevilSize(D3DXVECTOR3 size) { m_DevilSize = size; }
	D3DXVECTOR3 GetDevilSize(void) { return m_DevilSize; }
	void SetDifference(D3DXVECTOR3 size) { m_MapDifference = size; }
	D3DXVECTOR3 GetDifference(void) { return m_MapDifference; }
	void SetModelDisp(bool Sst);

private:
	void MyObjCreate(void);		//自分が保持するオブジェクトの生成
	void ActionState(void);		//モーションと状態の管理
	void StateManager(void);	//状態管理
	void Move(void);			//移動処理
	void Rot(void);				//移動方向処理
	void ObjectScroll(D3DXVECTOR3 Move);	//オブジェクトのスクロール
	void GritScroll(D3DXVECTOR3 Move);		//グリットのスクロール
	void CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size);	//プレイヤーが潰される時の処理

	void DebugKey(void);		//デバッグキー

	void LoadLevelData(const char* pFilename);

	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;		//攻撃状態記録用変数
	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_posOld;			//過去の位置
	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_Objmove;			//オブジェクトから影響される移動量
	D3DXVECTOR3 m_rot;				//向き
	D3DXVECTOR3 m_rotDest;			//向きの目的地
	D3DXVECTOR3 m_AutoMoveRot;		//自動移動の移動方向
	D3DXVECTOR3 m_size;				//大きさ
	D3DXVECTOR3 m_DevilPos;			//デビルパワーがマップのブロックにデビルスクロールする位置、テスト用
	D3DXVECTOR3 m_DevilSize;		//デビルパワーが及ぶ範囲、通称デビルサイズ、テスト用
	D3DXVECTOR3 m_MapDifference;	//マップを動かした時の差分
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス
	int m_nActionCount;				//行動のカウント
	STATE m_State;					//状態
	int m_nStateCount;				//状態管理用カウント

	float m_CollisionRot;			//当たり判定用の向き

	CSlowManager* m_pSlow;			// スロー

	//階層構造とモーションのポインタ
	CModel* m_apModel[64];
	CMotion* m_pMotion;
	char* m_aModelName[64];
	int m_nNumModel;
};

#endif