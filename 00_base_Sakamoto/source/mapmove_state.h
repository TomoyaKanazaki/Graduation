//============================================
//
//	マップ移動状態の管理処理 [mapmove_state.h]
//	Author:Satone Shion
//
//============================================
#ifndef _MAPMOVE_STATE_H_
#define _MAPMOVE_STATE_H_

//============================================
// 前方宣言
//============================================
class CMapMove;

//============================================
// マップ移動状態のインターフェースクラス
//============================================
class CMapMoveState
{
public:

	// 向き
	enum ROTTYPE
	{
		ROTTYPE_UP = 0,		// 上
		ROTTYPE_DOWN,		// 下
		ROTTYPE_LEFT,		// 左
		ROTTYPE_RIGHT,		// 右
		ROTTYPE_MAX
	};

    CMapMoveState();
    virtual ~CMapMoveState() {}

    virtual void Init() {}			// 初期化
	virtual void Update(){}			// 更新処理

    virtual void Release() {}		// 破棄
	virtual void Set(){}			// 設定

	virtual void WaitScroll(CMapMove* pMapMove){}		// 待機とスクロール切り替え
	virtual void WaitSlope(CMapMove* pMapMove) {}		// 待機と傾き切り替え

	// 設定・取得
	void SetRotState(ROTTYPE RotType) { m_RotType = RotType; }		// 向き
	ROTTYPE GetRotState() { return m_RotType; }						// 向き

protected:
	// メンバ関数
	void SetMoveRot();		// 移動方向を設定

private:

	ROTTYPE m_RotType;				// 移動向きの種類

};

//============================================
// 待機状態
//============================================
class CMapStateWait : public CMapMoveState
{
public:
	CMapStateWait();
	~CMapStateWait(){}

	void Init() override;		// 初期化
	void Update() override;		// 更新処理

};

//============================================
// スクロール状態
//============================================
class CMapStateScroll : public CMapMoveState
{
public:
	CMapStateScroll() {}
	~CMapStateScroll() {}

	void Update() override {}			// 更新処理
};

//============================================
// 傾き状態
//============================================
class CMapStateSlope : public CMapMoveState
{
public:
	CMapStateSlope();
	~CMapStateSlope() {}

	void Init() override;		// 初期化
	void Update() override;		// 更新処理

	void Set() override;		// 設定

	void WaitSlope(CMapMove* pMapMove) override;			// 待機と傾き切り替え

private:
	bool m_bSlope;					//傾き状態かどうか
};

#endif