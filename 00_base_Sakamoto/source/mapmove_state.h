//============================================
//
//	マップ移動状態の管理処理 [mapmove_state.h]
//	Author:Satone Shion
//
//============================================
#ifndef _MAPMOVE_STATE_H_
#define _MAPMOVE_STATE_H_

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

    CMapMoveState() {};
    virtual ~CMapMoveState() {}

    virtual void Init() {}         // 初期化
    virtual void Release() {}      // 破棄
};

//============================================
// 待機状態
//============================================
class CMapStateWait : public CMapMoveState
{
public:
	CMapStateWait(){}
	~CMapStateWait(){}


};

//============================================
// スクロール状態
//============================================
class CMapStateScroll : public CMapMoveState
{
public:
	CMapStateScroll() {}
	~CMapStateScroll() {}


};

//============================================
// 傾き状態
//============================================
class CMapStateSlope : public CMapMoveState
{
public:
	CMapStateSlope() {}
	~CMapStateSlope() {}


};

#endif