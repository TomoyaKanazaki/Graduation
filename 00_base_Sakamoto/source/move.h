//============================================
//
//	移動状態の管理処理 [move.h]
//	Author:Satone Shion
//
//============================================
#ifndef _MOVE_H_
#define _MOVE_H_

#include "main.h"
#include "enemy.h"

//============================================
// 前方宣言
//============================================
class CObjectCharacter;         // キャラクター情報
class CItem;                    // アイテム情報
class CObjectX;                 // オブジェクトXの情報

//============================================
// 移動状態のインターフェースクラス
//============================================
class CMoveState
{
public:

    // デバッグ用移動状態
    enum STATE
    {
        STATE_NONE = 0,     // 何もなし
        STATE_CONTROL,      // 操作
        STATE_ASTAR,        // 追跡
        STATE_RANDOM,       // ランダム
        STATE_STOP,         // 停止
        STATE_ROLL,         // 転がる

        STATE_MAX
    };

    // 移動方向
    enum ROTSTATE
    {
        ROTSTATE_LEFT = 0,	// 左方向
        ROTSTATE_RIGHT,		// 右方向
        ROTSTATE_UP,		// 上方向
        ROTSTATE_DOWN,		// 下方向
        ROTSTATE_MAX,		// 最大
        ROTSTATE_WAIT,	    // 待機
        ROTSTATE_NONE
    };

    // 移動の進行状況を管理する構造体
    struct PROGGRESS
    {
        bool bOKL;		//左への進行が許されるかどうか
        bool bOKR;		//右への進行が許されるかどうか
        bool bOKU;		//上への進行が許されるかどうか
        bool bOKD;		//下への進行が許されるかどうか
    };

    CMoveState();
    virtual ~CMoveState() {}

    virtual void Init() {}         // 初期化
    virtual void Release() {}      // 破棄

    // 操作
    virtual void ControlRandom(CObjectCharacter* pCharacter) {}        // 操作とランダム切り替え
    virtual void ControlAStar(CObjectCharacter* pCharacter) {}         // 操作と追跡切り替え
    virtual void ControlStop(CObjectCharacter* pCharacter) {}          // 操作と停止切り替え

    // ランダム歩行
    virtual void RandomAStar(CObjectCharacter* pCharacter) {}           // ランダムと追跡切り替え
    virtual void RandomStop(CObjectCharacter* pCharacter) {}            // ランダムと停止切り替え

    // 追跡
    virtual void AStarStop(CObjectCharacter* pCharacter) {}             // 追跡と停止切り替え
    virtual void RollStop(CObjectX* pObjectX) {}                        // 転がりと停止切り替え

    // 移動処理
    virtual void Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot) {}           // 移動処理(キャラクター)
    virtual void Move(CItem* pItem, D3DXVECTOR3& pos, D3DXVECTOR3& rot) {}                           // 移動処理(アイテム)
    virtual void Move(CObjectX* pObjectX, D3DXVECTOR3& pos, D3DXVECTOR3& rot) {}                     // 移動処理(オブジェクトX)


    CMoveState* GetMoveState(CObjectCharacter* pCharacter);             // 移動状態の情報取得

    // 設定・取得
    virtual void SetRotState(ROTSTATE RotState) {}      // 移動方向の状態を取得
    virtual ROTSTATE GetRotState() { return ROTSTATE_NONE; }                       // 移動方向の状態を取得

    virtual void SetEnemyType(CEnemy::ENEMY_TYPE Type) {}             // 敵の種類設定

    void Debug(void);       // デバッグ表示用

protected:

    // メンバ関数
    void Rot(CObjectCharacter* pCharacter, D3DXVECTOR3& rotMy);				// 移動方向処理(キャラクター)
    void Rot(CItem* pItem, D3DXVECTOR3& rotMy);						        // 移動方向処理(アイテム)

    void UpdatePos(CObjectCharacter* pCharacter, D3DXVECTOR3& pos);	        // 位置更新処理(キャラクター)
    void UpdatePos(CItem* pItem, D3DXVECTOR3& pos);	                        // 位置更新処理(アイテム)


    // メンバ変数
    STATE m_State;                  // デバッグ用状態
    PROGGRESS m_Progress;			// 移動の進行許可状況
    PROGGRESS m_ProgressOld;		// 前回の移動の進行許可状況

   
};

//============================================
// 操作出来る状態
//============================================
class CStateControl : public CMoveState
{
public:
    CStateControl();
    ~CStateControl(){}

    void Release() override;      // 破棄

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
    D3DXVECTOR3 MoveInputPadStick(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot, D3DXVECTOR3 Move, float fSpeed);

    void UpdateMovePlayer(CObjectCharacter* pCharacter, D3DXVECTOR3& NormarizeMove);        // プレイヤーの移動更新処理
    void UpdateMoveEnemy(CObjectCharacter* pCharacter, D3DXVECTOR3& NormarizeMove);         // 敵の移動更新処理

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

    CStateRandom();
    ~CStateRandom() {}

    void Release() override;      // 破棄

    // 切り替え処理
    void ControlRandom(CObjectCharacter* pCharacter) override;      // 操作に切り替え
    void RandomAStar(CObjectCharacter* pCharacter) override;        // 追跡に切り替え
    void RandomStop(CObjectCharacter* pCharacter) override;         // 停止に切り替え

    void Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot) override;      // キャラクターの移動処理
    void Move(CItem* pItem, D3DXVECTOR3& pos, D3DXVECTOR3& rot) override;                      // 移動処理(アイテム)

private:

    // メンバ関数
    void SearchWall(CObjectCharacter* pCharacter, D3DXVECTOR3& pos);				// 壁のサーチ判定(キャラクター)
    void SearchWall(CItem* pItem, D3DXVECTOR3& pos);				                // 壁のサーチ判定(アイテム)

    void MoveSelect(CObjectCharacter* pCharacter);		                            // 移動方向の選択(キャラクター)
    void MoveSelect(CItem* pItem);		                                            // 移動方向の選択(アイテム)

    void MoveAngle(D3DXVECTOR3& moveSave, std::vector<D3DXVECTOR3>& move, std::vector<ROTSTATE>& rotState);      // 各方向の移動量設定

    // メンバ変数
    CMapSystem::GRID m_GridOld;
    ROTSTATE m_RotState;            // 移動方向の状態
    int m_nSelectCounter;           // 移動方向変更カウンター
    bool m_bSwitchMove;             // 移動方向を変えるか

};

//============================================
// 追跡状態
//============================================
class CStateAStar : public CMoveState
{
public:
    CStateAStar();
    ~CStateAStar() {}

    void Init() override;         // 初期化
    void Release() override;      // 破棄

    // 切り替え処理
    void ControlAStar(CObjectCharacter* pCharacter) override;       // 操作に切り替え
    void RandomAStar(CObjectCharacter* pCharacter) override;        // ランダム歩行に切り替え
    void AStarStop(CObjectCharacter* pCharacter) override;          // 停止に切り替え

    void Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot) override;      // キャラクターの移動処理

    // 設定
    void SetEnemyType(CEnemy::ENEMY_TYPE Type) { m_EnemyType = Type; }             // 敵の種類設定

private:

    // メンバ関数
    void Coordinate(CObjectCharacter* pCharacter); // 最短経路探索
    void Route(CObjectCharacter* pCharacter);	// 最短経路をたどる

    // メンバ変数
    CMapSystem::GRID* m_pPath;          // プレイヤーへの最短経路
    CEnemy::ENEMY_TYPE m_EnemyType;		//敵の種類

    int m_nNumCoordinate;       // 最短経路の要素数
    int m_nTargetIndex;         // 次に向かうべきグリッドのインデックス
    float m_fCoordinateTimer;   // 経路探索間隔

    bool m_bStateFlag;          // ステートフラグ
};

//============================================
// 停止状態
//============================================
class CStateStop : public CMoveState
{
public:

    CStateStop(){ m_State = STATE_STOP; }
    ~CStateStop() {}

    void Release() override;      // 破棄

    // 切り替え処理
    void ControlStop(CObjectCharacter* pCharacter) override;        // 操作に切り替え
    void RandomStop(CObjectCharacter* pCharacter) override;         // ランダム歩行に切り替え
    void AStarStop(CObjectCharacter* pCharacter) override;          // 追跡に切り替え
    void RollStop(CObjectX* pObjectX) override;                     // 転がりと停止切り替え

    void Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot) override;      // キャラクターの移動処理
};

//============================================
// 転がる状態
//============================================
class CStateRoll : public CMoveState
{
public:

    CStateRoll();
    ~CStateRoll() {}

    void Release() override;      // 破棄

    // 切り替え処理
    void RollStop(CObjectX* pObjectX) override;                     // 転がりと停止切り替え

    void Move(CObjectX* pObjectX, D3DXVECTOR3& pos, D3DXVECTOR3& rot) override;      // キャラクターの移動処理

private:
    void SetJudg(int& nGridPosX, int& nGridPosZ, bool& bProgress);                  // 転がるか確認
    void RailCheck(CMapSystem::GRID& grid, bool& bProgress);                        // レールがあるか確認

    void Stop(D3DXVECTOR3& pos, D3DXVECTOR3& GridPos, D3DXVECTOR3& move);           // 転がらないときの位置
};

#endif