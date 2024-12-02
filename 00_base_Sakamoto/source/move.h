//============================================
//
//	移動状態の管理処理 [move.h]
//	Author:Satone Shion
//
//============================================
#ifndef _MOVE_H_
#define _MOVE_H_

#include "main.h"

////移動クラス
//class CMove
//{
//public:
//
//	CMove();
//	~CMove();
//
//	static CMove* Create();
//
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//
//	// 静的メンバ関数
//	static CListManager<CMove>* GetList(void); // リスト取得
//
//private:
//
//	// 静的メンバ変数
//	static CListManager<CMove>* m_pList; // オブジェクトリスト
//
//	// メンバ変数
//	CListManager<CMove>::AIterator m_iterator; // イテレーター
//
//};

//============================================
// 移動状態のインターフェースクラス
//============================================
class CMoveState
{
public:
    CMoveState() {};
    virtual ~CMoveState() {};

    virtual void ChangeWalk() = 0;      // 歩行方法切り替え(ランダムor追跡)
    virtual void Stop() = 0;
};

//============================================
// 移動状態管理クラス
//============================================
class CMoveStateContext
{
public:

    CMoveStateContext() { m_pMoveState = nullptr; }
    ~CMoveStateContext() {}

    void ChangeState(CMoveState* pMoveState);   // 状態変更

private:

    CMoveState* m_pMoveState;       // 移動状態の情報

};

//============================================
// 操作出来る状態
//============================================
class CStateControl : public CMoveState
{
public:
    CStateControl(){}
    ~CStateControl(){}


};

//============================================
// ランダム歩行状態
//============================================
class CStateRandom : public CMoveState
{
public:
    CStateRandom(){}
    ~CStateRandom() {}

};

//============================================
// 追跡状態
//============================================
class CStateChase : public CMoveState
{
public:
    CStateChase(){}
    ~CStateChase() {}

};


//============================================
// 停止状態
//============================================
class CStateStop : public CMoveState
{
public:
    CStateStop(){}
    ~CStateStop() {}

};

#endif