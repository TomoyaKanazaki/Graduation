//============================================
//
//	デビルの処理(仮) [devil.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _DEVIL_H_
#define _DEVIL_H_

#include "objectcharacter.h"
#include "useful.h"
#include "MapSystem.h"

//前方宣言

class CObject2D;
class CObject3D;
class CObjGauge2D;
class CUltimate;
class CSlowManager;
class CObjectBillboard;
class CNumber;
class CScrollArrow;
class CSignal;

class CEnemy;
class CBoss;
class CPlayer;
class CSignalManager;
class CMapMove;

//オブジェクトプレイヤークラス
class CDevil : public CObjectCharacter
{
public:
	CDevil(int nPriority = 2);
	~CDevil();

	// モーション
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,			// 待機
		ACTION_GOMI00,				// ゴミデータ00
		ACTION_SIGNAL_DOWN,			// 傾き信号「下」
		ACTION_SIGNAL_LEFT,			// 傾き信号「左」
		ACTION_SIGNAL_RIGHT,		// 傾き信号「右」
		ACTION_SIGNAL_UP,			// 傾き信号「上」
		ACTION_RUN,					// 逃げる
		ACTION_KING,				// 玉座
		ACTION_MAX,					// 最大
	};

	enum SLOPE_TYPE
	{
		SLOPE_TYPE_FRONT = 0,
		SLOPE_TYPE_BACK,
		SLOPE_TYPE_RIGHT,
		SLOPE_TYPE_LEFT,
		SLOPE_TYPE_MAX,
	};

	static CDevil* Create();
	HRESULT Init(void) override;
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//お引越し開始

	void SetDevilPos(D3DXVECTOR3 size) { m_DevilPos = size; }
	D3DXVECTOR3 GetDevilPos(void) { return m_DevilPos; }
	void SetDifference(D3DXVECTOR3 size) { m_MapDifference = size; }
	D3DXVECTOR3 GetDifference(void) { return m_MapDifference; }
	void SetDevilRot(D3DXVECTOR3 Rot) { m_DevilRot = Rot; }
	D3DXVECTOR3 GetDevilRot(void) { return m_DevilRot; }

	//お引越し終了

	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	CMapMove* GetMove(void) { return m_pMapMove; }

	void SetModelDisp(bool Sst);

	// 静的メンバ関数
	static CListManager<CDevil>* GetList(void); // リスト取得
	static CDevil* GetListTop(void); // リスト取得

private:
	void ActionState(void);		//モーションと状態の管理

	//お引越し開始

	void CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size);	//プレイヤーが潰される時の処理

	//お引越し終了

	ACTION_TYPE m_AtkAction;		//攻撃状態記録用変数
	CMapMove* m_pMapMove;

	//お引越し開始
	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_Objmove;			//オブジェクトから影響される移動量
	D3DXVECTOR3 m_rotDest;			//向きの目的地
	D3DXVECTOR3 m_AutoMoveRot;		//自動移動の移動方向
	D3DXVECTOR3 m_DevilPos;			//デビルパワーがマップのブロックにデビルスクロールする位置、テスト用
	D3DXVECTOR3 m_MapDifference;	//マップを動かした時の差分
	float m_fActionCount;			//行動のカウント
	bool m_bSlope;					//傾き状態かどうか

	CMapSystem::GRID m_MinGrid;		//マップで一番左上にあるブロックの番号
	CMapSystem::GRID m_MaxGrid;		//マップで一番右下にあるブロックの番号

	CSignal* m_pSignal[4];			//矢印モデルのポインタ

	float m_CollisionRot;			//当たり判定用の向き

	CSignalManager* m_pSignalManager;	// シグナルマネージャー
	D3DXVECTOR3 m_DevilRot;			// デビルパワーによって傾く値
	int m_DevilArrow;				// デビルパワーの方向
	int m_ScrollArrowOld;			// 過去のスクロールの方向
	int m_SlopwArrowOld;			// 過去の傾きの方向
	int m_SlopeType;				// スロープの種類
	//お引越し終了

	ACTION_TYPE m_Action;
	int m_nStateCount;				//状態管理用カウント
	int m_nStateNum;				//状態カウント

	CSlowManager* m_pSlow;			// スロー

	// 静的メンバ変数
	static CListManager<CDevil>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CDevil>::AIterator m_iterator; // イテレーター

};

#endif