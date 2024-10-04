//========================================
//
//プレイヤー処理[player.h]
//Author：森川駿弥
//
//========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "character.h"

//========================================
// 前方宣言
//========================================
class CModel;
class CMotion;
class CEffect;
class CGauge;
class CLockonMarker;

//========================================
//プレイヤークラス
//========================================
class CPlayer : public CCharacter
{
public:

	// 状態
	enum PLAYERSTATE
	{// プレイヤーの状態
		STATE_NONE = 0,
		STATE_NORMAL,		// 通常
		STATE_JUMP,			// ジャンプ
		STATE_ROLL,			// 転がり
		STATE_MAX
	};

	enum PLAYERTYPE
	{// プレイヤーのタイプ
		TYPE_NONE = 0, // 何もない
		TYPE_RABBIT, // 兎
		TYPE_TURTLE, // 亀
		TYPE_MAX
	};

	CPlayer(int nPriority = 4);		//コンストラクタ
	~CPlayer();		//デストラクタ

	// メンバ関数
	static CPlayer* Create(std::string pfile);

	HRESULT Init() { return S_OK; }		//純粋仮想
	HRESULT Init(std::string pfile);
	void Uninit();
	void Update();
	void Draw();

	float GetRadius() { return m_fRadius; }		// 半径取得
	int GetLife() { return m_nLife; }			// 体力取得
	int GetState() { return m_State; }			// 状態取得
	void SetDefaultType(const PLAYERTYPE type) { m_typeDefault = type; } // デフォルトタイプの変更
	PLAYERTYPE GetDefaultType() { return m_typeDefault; } // デフォルトタイプの取得
	PLAYERTYPE GetType() { return m_type; } // タイプの取得

	// 静的メンバ関数
	static CPlayer* GetInstance() { return m_pPlayer; }

private:

	// メンバ関数
	void DamageCounter();
	void Motion();
	void Move(D3DXVECTOR3& pos, D3DXVECTOR3& move, const float fDeltaTime);
	void Jump(D3DXVECTOR3& move, CInputPad* pPad, CInputKeyboard* pKeyboard);
	void Roll(D3DXVECTOR3& move, CInputPad* pPad, CInputKeyboard* pKeyboard, const float fDeltaTime);
	void SwitchType(CInputPad* pPad, CInputKeyboard* pKeyboard);
	void Land(D3DXVECTOR3& pos, D3DXVECTOR3& move);
	void Damage();				// ダメージ関数
	void CheckStageObjRange();	// ステージオブジェの範囲チェック
	void CollisionReverseObj();	// 反転オブジェの当たり判定
	void Collision();	// 当たり判定
	void SpeedUp(D3DXVECTOR3& move);		// 加速処理

	// メンバ変数
	int m_apNumModel;		// モデル(パーツ)の総数
	int m_nLife;			// 体力
	int m_nOldMotion;		// 前回のモーション
	int m_WalkCounter;		// 歩行時のカウンター
	int m_State;			// 状態
	int m_nDamageCount;		// ダメージカウント
	float m_fRadius;		// 半径
	float m_fHeight;		// 身長
	float m_fRollTime;		// 転がり時間
	float m_fSpeedScale;	// 加速度
	float m_fWalkSound;		// 歩行音
	bool m_bJump;			// ジャンプフラグ
	bool m_bRoll;			// 転がりフラグ
	CEffect* m_pEffect;				// エフェクトのポインタ
	CGauge* m_pGauge;				// ゲージのポインタ
	CModel* m_apModel[MAX_PARTS];	// モデルのダブルポインタ
	CLockonMarker* m_pMarker;		// ロックオンマーカーのポインタ
	PLAYERTYPE m_type;				// タイプ
	PLAYERTYPE m_typeDefault;		// デフォルトタイプ

	// 静的メンバ変数
	static CPlayer* m_pPlayer;		// 自身のポインタ
};

#endif
