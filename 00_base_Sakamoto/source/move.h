//============================================
//
//	移動状態の管理処理 [move.h]
//	Author:Satone Shion
//
//============================================
#ifndef _MOVE_H_
#define _MOVE_H_

#include "main.h"

//============================================
// 前方宣言
//============================================
class CMoveStateContext;        // 移動状態管理

//============================================
// 移動状態のインターフェースクラス
//============================================
class CMoveState
{
public:
    CMoveState() {};
    virtual ~CMoveState() {};

    // 状態

    // 操作
    virtual void ControlRandom(CMoveStateContext* pMoveStateContext) {}        // 操作とランダム切り替え
    virtual void ControlAStar(CMoveStateContext* pMoveStateContext) {}         // 操作と追跡切り替え
    virtual void ControlStop(CMoveStateContext* pMoveStateContext) {}          // 操作と停止切り替え

    // ランダム歩行
    virtual void RandomAStar(CMoveStateContext* pMoveStateContext) {}           // ランダムと追跡切り替え
    virtual void RandomStop(CMoveStateContext* pMoveStateContext) {}            // ランダムと停止切り替え

    // 追跡
    virtual void AStarStop(CMoveStateContext* pMoveStateContext) {}            // 追跡と停止切り替え

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

    void ControlRandom(CMoveStateContext* pMoveStateContext) override;      // ランダムに切り替え
    void ControlAStar(CMoveStateContext* pMoveStateContext) override;       // 追跡に切り替え
    void ControlStop(CMoveStateContext* pMoveStateContext) override;        // 停止に切り替え
};

//============================================
// ランダム歩行状態
//============================================
class CStateRandom : public CMoveState
{
public:
    CStateRandom(){}
    ~CStateRandom() {}

    void ControlRandom(CMoveStateContext* pMoveStateContext) override;      // 操作に切り替え
    void RandomAStar(CMoveStateContext* pMoveStateContext) override;        // 追跡に切り替え
    void RandomStop(CMoveStateContext* pMoveStateContext) override;         // 停止に切り替え
};

//============================================
// 追跡状態
//============================================
class CStateAStar : public CMoveState
{
public:
    CStateAStar(){}
    ~CStateAStar() {}

    void ControlAStar(CMoveStateContext* pMoveStateContext) override;       // 操作に切り替え
    void RandomAStar(CMoveStateContext* pMoveStateContext) override;        // ランダム歩行に切り替え
    void AStarStop(CMoveStateContext* pMoveStateContext) override;          // 停止に切り替え

};


//============================================
// 停止状態
//============================================
class CStateStop : public CMoveState
{
public:
    CStateStop(){}
    ~CStateStop() {}

    void ControlStop(CMoveStateContext* pMoveStateContext) override;        // 操作に切り替え
    void RandomStop(CMoveStateContext* pMoveStateContext) override;         // ランダム歩行に切り替え
    void AStarStop(CMoveStateContext* pMoveStateContext) override;          // 追跡に切り替え

};

#endif