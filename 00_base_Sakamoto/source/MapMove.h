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
class CSignal;
class CPlayer;

//マップムーブクラス
class CMapMove
{
public:
	CMapMove();
	~CMapMove();

	// スクロールタイプ
	enum SCROLL_TYPE
	{
		SCROLL_TYPE_NORMAL = 0,	// スムーズにスクロールする
		SCROLL_TYPE_RETRO,		// カクカクとスクロールする
		SCROLL_TYPE_MAX,		// 最大
	};

	// 向き
	enum ROTTYPE
	{
		ROTTYPE_UP = 0,		// 上
		ROTTYPE_DOWN,		// 下
		ROTTYPE_LEFT,		// 左
		ROTTYPE_RIGHT,		// 右
		ROTTYPE_MAX
	};

	// 移動モード
	enum MOVEMODE
	{
		MOVEMODE_SCROLL = 0,	// スクロール
		MOVEMODE_SLOPE,			// 傾き
		MOVEMODE_WAIT,			// 待機
		MOVEMODE_MAX
	};

	//加減速状態
	enum SPEED
	{
		SPEED_NONE = 0,		//待機
		SPEED_UP,			//加速
		SPEED_DOWN,			//減速
		SPEED_MAX,			//最大
	};



	//移動の状態----------------------------------------------------------お前も消えろ全ての元凶
	//enum MOVE
	//{
	//	MOVE_WAIT = 0,		//待機
	//	MOVE_SCROLL_UP,		//スクロール[上]
	//	MOVE_SCROLL_DOWN,	//スクロール[下]
	//	MOVE_SCROLL_LEFT,	//スクロール[左]
	//	MOVE_SCROLL_RIGHT,	//スクロール[右]

	//	MOVE_SLOPE_UP,		//傾き状態[上]
	//	MOVE_SLOPE_DOWN,	//傾き状態[下]
	//	MOVE_SLOPE_LEFT,	//傾き状態[左]
	//	MOVE_SLOPE_RIGHT,	//傾き状態[右]

	//	MOVE_MAX,			//最大
	//};

	static CMapMove* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	float MoveSlopeX(float Move, SPEED& Speed);		//傾き中の移動量変動
	float MoveSlopeZ(float Move, SPEED& Speed);		//傾き中の移動量変動

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
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

	void SetStateCount(int Set) { m_nStateCount = Set; }
	int GetStateCount(void) { return m_nStateCount; }

	ROTTYPE GetRotState(void) { return m_RotType; }			// 向き状態
	MOVEMODE GetMoveMode(void) { return m_MoveMode; }		// 移動状態

	void FollowScroll(D3DXVECTOR3& pos) { pos += m_move; }

private:

	// 移動用関数
	void StateManager(void);	//状態管理
	void Move();				//移動処理
	void BackSlope(void);		//傾き処理
	void Slope();				//傾き処理
	void CollisionOut();		//ステージ外にいるオブジェクトの処理

	// 移動関係の設定
	void SetScroll(void);		// スクロールの設定
	void SetSlope(void);		// 傾きの設定
	void SetBackSlope(void);	// 元に戻す傾きの設定
	void SetSlopeRot(ROTTYPE& RotType);		// 傾きの向きを設定
	void SetDeviceMap(void);	// マップ装置の設定
	void MoveScroll(void);		// 傾きの回転処理

	// 矢印の音
	void PlaySound();	// 喋らせる
	void StopSound();	// 黙らせる

	void ScrollReset();		//スクロールのずれ修正
	void GritScroll();		//グリットのスクロール

	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_Objmove;			//オブジェクトから影響される移動量
	D3DXVECTOR3 m_rotDest;			//向きの目的地
	D3DXVECTOR3 m_AutoMoveRot;		//自動移動の移動方向
	D3DXVECTOR3 m_DevilPos;			//デビルパワーがマップのブロックにデビルスクロールする位置、テスト用
	D3DXVECTOR3 m_MapDifference;	//マップを動かした時の差分
	float m_fActionCount;			//行動のカウント
	int m_nStateCount;				//状態管理用カウント
	bool m_bSlope;					//傾き状態かどうか

	float m_fScrollMove;			//スクロールが移動した量
	float m_fScrollEndLine;			//スクロール終了位置
	bool m_bScrollOK;				//スクロールが完了したかどうか

	CMapSystem::GRID m_MinGrid;		//マップで一番左上にあるブロックの番号
	CMapSystem::GRID m_MaxGrid;		//マップで一番右下にあるブロックの番号

	float m_CollisionRot;			//当たり判定用の向き

	D3DXVECTOR3 m_DevilRot;			// デビルパワーによって傾く値
	SCROLL_TYPE m_ScrollType;		// スクロールの種類
	int m_SlopeType;				// スロープの種類
	float m_fEffectTime;			// エフェクト生成タイマー


	// 新しく追加した変数(佐藤根)
	MOVEMODE m_MoveMode;			// 移動モード
	ROTTYPE m_RotType;				// 移動向きの種類
	ROTTYPE m_OldScrollRotType;		// 前回の移動向きの種類(スクロール)
	ROTTYPE m_OldSlopeRotType;		// 前回の移動向きの種類(傾き)

};
#endif