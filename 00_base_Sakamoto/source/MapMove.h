//============================================
//
//	マップの動き [MapMove.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MAPMOVE_H_
#define _MAPMOVE_H_

#include "useful.h"
#include "MapSystem.h"

//前方宣言
class CObject2D;
class CSignalManager;
class CSignal;
class CPlayer;

//マップムーブクラス
class CMapMove
{
public:
	CMapMove();
	~CMapMove();

	// モーション
	enum SCROLL_TYPE
	{
		SCROLL_TYPE_NORMAL = 0,	// スムーズにスクロールする
		SCROLL_TYPE_RETRO,		// カクカクとスクロールする
		SCROLL_TYPE_MAX,		// 最大
	};

	//デビルの状態
	enum MOVE
	{
		MOVE_WAIT = 0,		//待機
		MOVE_SCROLL_UP,		//スクロール[上]
		MOVE_SCROLL_DOWN,	//スクロール[下]
		MOVE_SCROLL_RIGHT,	//スクロール[右]
		MOVE_SCROLL_LEFT,	//スクロール[左]
		MOVE_SLOPE_UP,		//傾き状態[上]
		MOVE_SLOPE_DOWN,	//傾き状態[下]
		MOVE_SLOPE_RIGHT,	//傾き状態[右]
		MOVE_SLOPE_LEFT,	//傾き状態[左]
		MOVE_MAX,			//最大
	};

	static CMapMove* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	float MoveSlopeX(float Move);		//傾き中の移動量変動
	float MoveSlopeZ(float Move);		//傾き中の移動量変動

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	MOVE GetState(void) { return m_State; }
	void SetDevilPos(D3DXVECTOR3 size) { m_DevilPos = size; }
	D3DXVECTOR3 GetDevilPos(void) { return m_DevilPos; }
	void SetDifference(D3DXVECTOR3 size) { m_MapDifference = size; }
	D3DXVECTOR3 GetDifference(void) { return m_MapDifference; }
	void SetDevilRot(D3DXVECTOR3 Rot) { m_DevilRot = Rot; }
	D3DXVECTOR3 GetDevilRot(void) { return m_DevilRot; }

	void SetScrollType(SCROLL_TYPE Rot) { m_ScrollType = Rot; }
	SCROLL_TYPE GetScrollType(void) { return m_ScrollType; }

	CMapSystem::GRID GetMinGrid(void) { return m_MinGrid; }	//マップで一番左上にあるブロックの番号
	CMapSystem::GRID GetMaxGrid(void) { return m_MaxGrid; }	//マップで一番右下にあるブロックの番号

	void FollowScroll(D3DXVECTOR3& pos) { pos += m_move; }

	// 静的メンバ関数
	static CListManager<CMapMove>* GetList(void); // リスト取得
	static CMapMove* GetListTop(void); // リスト取得

private:

	void StateManager(void);	//状態管理
	void Move(int Arroow);		//移動処理
	void BackSlope(void);		//傾き処理
	void Slope(int Arroow);		//傾き処理
	void CollisionOut();		//ステージ外にいるオブジェクトの処理

	void SignalCreate(void);
	void SignalManager(void);

	void GritScroll();		//グリットのスクロール
	void CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size);	//プレイヤーが潰される時の処理

	void DebugKey(void);		//デバッグキー

	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_Objmove;			//オブジェクトから影響される移動量
	D3DXVECTOR3 m_rotDest;			//向きの目的地
	D3DXVECTOR3 m_AutoMoveRot;		//自動移動の移動方向
	D3DXVECTOR3 m_DevilPos;			//デビルパワーがマップのブロックにデビルスクロールする位置、テスト用
	D3DXVECTOR3 m_MapDifference;	//マップを動かした時の差分
	float m_fActionCount;			//行動のカウント
	MOVE m_State;					//状態
	MOVE m_SlopeOld;				//一個前の傾き状態
	int m_nStateCount;				//状態管理用カウント
	int m_nStateNum;				//状態カウント
	bool m_bSlope;					//傾き状態かどうか

	CMapSystem::GRID m_MinGrid;		//マップで一番左上にあるブロックの番号
	CMapSystem::GRID m_MaxGrid;		//マップで一番右下にあるブロックの番号

	CSignal* m_pSignal[4];			//矢印モデルのポインタ

	float m_CollisionRot;			//当たり判定用の向き

	CSignalManager* m_pSignalManager;	// シグナルマネージャー
	D3DXVECTOR3 m_DevilRot;			// デビルパワーによって傾く値
	int m_DevilArrow;				// 方向[0:上][1:下][2:左][3:右]
	int m_ScrollArrowOld;			// 過去のスクロールの方向
	int m_SlopwArrowOld;			// 過去の傾きの方向
	SCROLL_TYPE m_ScrollType;		// スクロールの種類
	int m_SlopeType;				// スロープの種類
	float m_fEffectTime;			// エフェクト生成タイマー

	// 静的メンバ変数
	static CListManager<CMapMove>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CMapMove>::AIterator m_iterator; // イテレーター
};
#endif