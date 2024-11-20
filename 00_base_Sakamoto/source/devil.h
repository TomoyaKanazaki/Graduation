//============================================
//
//	デビルの処理(仮) [devil.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _DEVIL_H_
#define _DEVIL_H_

#include "character.h"
#include "useful.h"
#include "MapSystem.h"

#define SCROLL_ID 1

//前方宣言

class CObject2D;
class CObject3D;
class CObjGauge2D;
class CUltimate;
class CSlowManager;
class CObjectBillboard;
class CNumber;
class CScrollArrow;

class CEnemy;
class CBoss;
class CPlayer;

//オブジェクトプレイヤークラス
class CDevil : public CCharacter
{
private:
	static const int ENCOUNT_MAX = 128;

public:
	CDevil(int nPriority = 2);
	~CDevil();

	// モーション
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,			// 待機
		ACTION_SIGNAL_UP,			// 傾き信号「上」
		ACTION_SIGNAL_DOWN,			// 傾き信号「下」
		ACTION_SIGNAL_LEFT,			// 傾き信号「左」
		ACTION_SIGNAL_RIGHT,		// 傾き信号「右」
		ACTION_MAX,					// 最大
	};

	//デビルの状態
	enum STATE
	{
		STATE_WAIT = 0,		//待機
		STATE_SCROLL,		//スクロール状態
		STATE_SLOPE,		//傾き状態
		STATE_MAX,			//最大
	};

	static CDevil* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	float MoveSlopeX(float Move);		//傾き中の移動量変動
	float MoveSlopeZ(float Move);		//傾き中の移動量変動

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	STATE GetState(void) { return m_State; }
	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	void SetDevilPos(D3DXVECTOR3 size) { m_DevilPos = size; }
	D3DXVECTOR3 GetDevilPos(void) { return m_DevilPos; }
	void SetDifference(D3DXVECTOR3 size) { m_MapDifference = size; }
	D3DXVECTOR3 GetDifference(void) { return m_MapDifference; }
	void SetDevilRot(D3DXVECTOR3 Rot) { m_DevilRot = Rot; }
	D3DXVECTOR3 GetDevilRot(void) { return m_DevilRot; }
	void SetModelDisp(bool Sst);

	// 静的メンバ関数
	static CListManager<CDevil>* GetList(void); // リスト取得

private:
	void ActionState(void);		//モーションと状態の管理
	void StateManager(void);	//状態管理
	void Move(int Arroow);		//移動処理
	void BackSlope(void);		//傾き処理
	void Slope(int Arroow);		//傾き処理
	void CollisionOut();		//ステージ外にいるオブジェクトの処理

	void ObjectScroll(D3DXVECTOR3 Move);	//オブジェクトのスクロール
	void CrossScroll(D3DXVECTOR3 Move, float GritSize);		// 十字架のスクロール
	void BowabowaScroll(D3DXVECTOR3 Move, float GritSize);	// ボワボワのスクロール
	void EnemyScroll(D3DXVECTOR3 Move, float GritSize);		// 敵のスクロール
	void RailBlockScroll(D3DXVECTOR3 Move, float GritSize);	// レールブロックのスクロール
	void RollRockScroll(D3DXVECTOR3 Move, float GritSize);	// 転がる岩のスクロール
	void PlayerScroll(D3DXVECTOR3 Move, float GritSize);	// プレイヤーのスクロール
	void FireScroll(D3DXVECTOR3 Move, float GritSize);	// ファイアボールのスクロール

	void GritScroll(D3DXVECTOR3 Move);		//グリットのスクロール
	void CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size);	//プレイヤーが潰される時の処理

	void DebugKey(void);		//デバッグキー

	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;		//攻撃状態記録用変数
	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_Objmove;			//オブジェクトから影響される移動量
	D3DXVECTOR3 m_rotDest;			//向きの目的地
	D3DXVECTOR3 m_AutoMoveRot;		//自動移動の移動方向
	D3DXVECTOR3 m_DevilPos;			//デビルパワーがマップのブロックにデビルスクロールする位置、テスト用
	D3DXVECTOR3 m_MapDifference;	//マップを動かした時の差分
	float m_fActionCount;			//行動のカウント
	STATE m_State;					//状態
	int m_nStateCount;				//状態管理用カウント
	bool m_bSlope;					//傾き状態かどうか

	CMapSystem::GRID m_MinGrid;		//マップで一番左上にあるブロックの番号
	CMapSystem::GRID m_MaxGrid;		//マップで一番右下にあるブロックの番号

	float m_CollisionRot;			//当たり判定用の向き

	CSlowManager* m_pSlow;			// スロー
	D3DXVECTOR3 m_DevilRot;			// デビルパワーによって傾く値
	int m_DevilArrow;				// デビルパワーの方向
	int m_ScrollArrowOld;			// 過去のスクロールの方向
	int m_SlopwArrowOld;			// 過去の傾きの方向
	CScrollArrow* m_ScrollArrow[2];	// スクロール方向のUI 

	// 静的メンバ変数
	static CListManager<CDevil>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CDevil>::AIterator m_iterator; // イテレーター

};

#endif