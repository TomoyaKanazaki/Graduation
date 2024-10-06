//==============================================================
//
//プレイヤーのモデル処理[player.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _PLAYER_H_		//このマクロ定義がされていなかったら
#define _PLAYER_H_		//2重インクルード防止のマクロを定義する

#include "object.h"

//前方宣言
class CModelHier;	//モデルの階層構造
class CMotion;		//モーションの階層構造
class CModel;
class CObject3D;

//プレイヤークラスの定義
class CPlayer : public CObject
{
public:

	//セーブポイント
	enum POINT
	{
		POINT_DEFAULT = 0,		//チュートリアルの初期値
		POINT_ARTBRIDGE,		//チュートリアルの1番目
		POINT_ENEMYBRIDGE,		//チュートリアルの2番目

		POINT_START,			//ゲームの初期値
		POINT_DOWN_FLOOR,		//ゲームの1番目
		POINT_PIER,				//ゲームの2番目
		POINT_MAZE,				//ゲームの3番目
		POINT_BIGINEND,			//ゲームの4番目
		POINT_FALLING,			//ゲームの5番目
		POINT_MAX
	};

	CPlayer();		//コンストラクタ
	CPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//コンストラクタ(オーバーロード)
	~CPlayer();		//デストラクタ

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//生成処理

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Hit(void);

	//設定処理
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }		//位置設定
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }		//向き設定
	void SetState(STATE state) { m_state = state; }			//状態設定

	void SetDash(int nCntDash) { m_nDashCounter = nCntDash; }	//ダッシュ回数設定
	void SetDashAuto(bool bDashAuto);		//自動ダッシュの設定

	//取得処理
	D3DXVECTOR3 GetPosition(void) { return m_pos; }			//位置取得
	D3DXVECTOR3 GetRotation(void) { return m_rot; }			//向き取得
	D3DXVECTOR3 GetMove(void) { return m_move; }			//移動量取得

	int GetPressFrame(void) { return m_nPressCounter; }		//キーを押したフレーム数
	int GetNumSavePoint(void) { return m_nNumPosSave; }		//何番目のセーブポイントか取得

	bool GetIsJump(void) { return m_bJump; }				//ジャンプの取得
	bool GetIsMove(void) { return m_bMove; }				//移動の取得
	bool GetIsDash(void) { return m_bDash; }				//ダッシュの取得
	bool GetIsLand(void) { return m_bLand; }				//着地の取得
	bool GetIsDashAuto(void) { return m_bDashAuto; }		//自動ダッシュの判定取得

	D3DXVECTOR3 GetSizeMin(void) { return m_min; }			//大きさの最大値取得
	D3DXVECTOR3 GetSizeMax(void) { return m_max; }			//大きさの最小値取得

private:
	//プレイヤーのパーツ
	enum PARTS
	{
		PARTS_BODY = 0,		//[0]体
		PARTS_MAX
	};

	void UpdateFront(void);			//手前側の更新処理
	void UpdateState(void);			//状態の更新処理

	void MotionManager(void);				//モーション管理
	void ControlFrontKeyboard(void);		//プレイヤーキーボード操作(手前側)
	void ControlFrontKeyboardMove(void);	//プレイヤーキーボードの移動操作(手前側)
	void ControlFrontKeyboardJump(void);	//プレイヤーキーボードのジャンプ操作(手前側)
	void ControlFrontKeyboardDash(void);	//プレイヤーキーボードのダッシュ操作(手前側)

	void ControlFrontJoyPad(void);			//プレイヤーパッド操作(手前側)
	void ControlFrontJoyPadMove(void);		//プレイヤーパッドの移動操作(手前側)
	void ControlFrontJoyPadJump(void);		//プレイヤーパッドのジャンプ操作(手前側)
	void ControlFrontJoyPadDash(void);		//プレイヤーパッドのダッシュ操作(手前側)

	void Screen(void);						//画面外判定
	void SavePoint(void);					//セーブポイント処理
	void LoadFile(void);					//モデルファイル読み込み
	void RotCorrection(void);				//向きの補正処理
	void SEStep(void);				//足音鳴らす処理
	void CollisionAction(void);		//当たった時の処理

	static char *m_apFileName[PARTS_MAX];	//ファイル名

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_posOld;	//前回の位置
	D3DXVECTOR3 m_posSavePoint[POINT_MAX];	//全ての復活用の位置保存用
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_moveSave;	//移動量保存用
	D3DXVECTOR3 m_rot;		//向き
	D3DXVECTOR3 m_max;		//人間の最大値
	D3DXVECTOR3 m_min;		//人間の最小値
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	CModelHier *m_apModel[PARTS_MAX];		//モデル(パーツ)へのポインタ
	CModel* m_pModel[2];		// モデル
	CObject3D* m_apObject3D[2];	// プレイヤー


	int m_nNumModel;		//モデル(パーツ)の総数
	int m_nCntDamage;		//ダメージカウンター

	int m_nPressCounter;	//キーを押したフレーム数

	bool m_bMove;			//歩いてるかの判定
	bool m_bJump;			//ジャンプしたかの判定
	bool m_bLand;			//着地したか
	bool m_bDash;			//ダッシュしたか
	bool m_bDashAuto;		//自動ダッシュするか
	bool m_bCollisionAlpha;		//透明ブロックに当たったか

	float m_fRotDest;		//目標
	float m_fRotDiff;		//差分

	STATE m_state;			//プレイヤーの状態

	int m_nDashCounter;		//ダッシュした回数

	int m_particleType;		//パーティクルの種類
	int m_nType;			//何番目のパーティクルか
	int m_nParticleLife;	//パーティクルの寿命
	int m_nCntHit;			//ヒットできるまでのカウンター
	int m_nCntMove;			//足音のカウンター
	int m_nCntSand;			//砂のパーティクルカウンター

	int m_nNumPosSave;		//何番目のセーブポイントか

	bool m_bPad;			//パッドのスティックを倒してるか

	//CMotion *m_pMotion;		//モーション情報
};

#endif
