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
class CObjectCharacter;         // キャラクター情報
class CPlayer;                  // プレイヤー情報

//============================================
// 移動状態のインターフェースクラス
//============================================
class CMoveState
{
public:

    // 移動方向
    enum ROTSTATE
    {
        ROTSTATE_WAIT = 0,	// 待機
        ROTSTATE_LEFT,		// 左方向
        ROTSTATE_RIGHT,		// 右方向
        ROTSTATE_UP,		// 上方向
        ROTSTATE_DOWN,		// 下方向
        ROTSTATE_MAX,		// 最大
        ROTSTATE_NONE
    };

    CMoveState();
    virtual ~CMoveState() {};

    // 操作
    virtual void ControlRandom(CObjectCharacter* pCharacter) {}        // 操作とランダム切り替え
    virtual void ControlAStar(CObjectCharacter* pCharacter) {}         // 操作と追跡切り替え
    virtual void ControlStop(CObjectCharacter* pCharacter) {}          // 操作と停止切り替え

    // ランダム歩行
    virtual void RandomAStar(CObjectCharacter* pCharacter) {}           // ランダムと追跡切り替え
    virtual void RandomStop(CObjectCharacter* pCharacter) {}            // ランダムと停止切り替え

    // 追跡
    virtual void AStarStop(CObjectCharacter* pCharacter) {}             // 追跡と停止切り替え

    // 移動処理
    virtual void Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot){}           // 移動処理(キャラクター)

    CMoveState* GetMoveState(CObjectCharacter* pCharacter);             // 移動状態の情報取得

    // 設定・取得
    virtual void SetRotState(ROTSTATE RotState) {}      // 移動方向の状態を取得
    virtual ROTSTATE GetRotState() { return ROTSTATE_NONE; }                       // 移動方向の状態を取得

private:
    

   
};

//============================================
// 操作出来る状態
//============================================
class CStateControl : public CMoveState
{
public:
    CStateControl();
    ~CStateControl(){}

    // 切り替え処理
    void ControlRandom(CObjectCharacter* pCharacter) override;      // ランダムに切り替え
    void ControlAStar(CObjectCharacter* pCharacter) override;       // 追跡に切り替え
    void ControlStop(CObjectCharacter* pCharacter) override;        // 停止に切り替え

    void Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot) override;      // キャラクターの移動処理

    // 設定・取得
    void SetRotState(ROTSTATE RotState) override { m_RotState = RotState; }      // 移動方向の状態を取得
    ROTSTATE GetRotState() override { return m_RotState; }                       // 移動方向の状態を取得

private:

    // メンバ関数
    D3DXVECTOR3 InputKey(CObjectCharacter* pCharacter, D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy, D3DXVECTOR3 Move, float fSpeed);		//移動入力キーボード

    void UpdateMovePlayer(CObjectCharacter* pCharacter, D3DXVECTOR3 NormarizeMove);        // プレイヤーの移動更新処理
    void UpdateMoveEnemy(CObjectCharacter* pCharacter, D3DXVECTOR3 NormarizeMove);         // 敵の移動更新処理

    // メンバ変数
    bool m_bInput;				//入力を行ったかどうか
    ROTSTATE m_RotState;        // 移動方向の状態

};

//============================================
// ランダム歩行状態
//============================================
class CStateRandom : public CMoveState
{
public:
    CStateRandom(){}
    ~CStateRandom() {}

    // 切り替え処理
    void ControlRandom(CObjectCharacter* pCharacter) override;      // 操作に切り替え
    void RandomAStar(CObjectCharacter* pCharacter) override;        // 追跡に切り替え
    void RandomStop(CObjectCharacter* pCharacter) override;         // 停止に切り替え
};

//============================================
// 追跡状態
//============================================
class CStateAStar : public CMoveState
{
public:
    CStateAStar(){}
    ~CStateAStar() {}

    // 切り替え処理
    void ControlAStar(CObjectCharacter* pCharacter) override;       // 操作に切り替え
    void RandomAStar(CObjectCharacter* pCharacter) override;        // ランダム歩行に切り替え
    void AStarStop(CObjectCharacter* pCharacter) override;          // 停止に切り替え

};

//============================================
// 停止状態
//============================================
class CStateStop : public CMoveState
{
public:
    CStateStop(){}
    ~CStateStop() {}

    // 切り替え処理
    void ControlStop(CObjectCharacter* pCharacter) override;        // 操作に切り替え
    void RandomStop(CObjectCharacter* pCharacter) override;         // ランダム歩行に切り替え
    void AStarStop(CObjectCharacter* pCharacter) override;          // 追跡に切り替え

};

#endif