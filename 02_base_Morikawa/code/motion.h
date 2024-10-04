//========================================
//
//モーション[motion.h]
//Author：森川駿弥
//
//========================================
#ifndef _MOTION_H_
#define _MOTION_H_
#include "main.h"
#include "model.h"

//========================================
// 定数定義
//========================================
namespace
{
const int MAX_MOTION = 8;	// モーションの最大数
const int MAX_MODEL = 16;	// パーツの最大数
const int MAX_KEY = 32;		// キーの最大数
}

//========================================
//モーションクラス
//========================================
class CMotion
{
public:
	enum PlayerMotion
	{// プレイヤーモーション種類
		PLAYER_MOTIONTYPE_NEUTRAL = 0,	// 待機
		PLAYER_MOTIONTYPE_WALK,			// 移動
		PLAYER_MOTIONTYPE_JUMP,			// ジャンプ
		PLAYER_MOTIONTYPE_ROLL,			// 転がり
		PLAYER_MOTIONTYPE_MAX
	};

	enum EnemyMotion
	{// エネミーモーション種類
		ENEMY_MOTIONTYPE_WALK = 0,	// 移動
		ENEMY_MOTIONTYPE_ATTACK,	// 攻撃
		ENEMY_MOTIONTYPE_MAX
	};

	struct KEY
	{//キーの構造体
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	};	//位置・向き

	struct KEY_INFO
	{//キー情報の構造体
		int nFrame;				//再生フレーム
		KEY aKey[MAX_MODEL];	//各モデルの情報
	};

	struct INFO
	{//モーション情報の構造体
		bool bLoop;		//ループするかどうか
		int nNumKey;	//キーの総数
		KEY_INFO aKeyInfo[MAX_KEY];	//キー情報
	};

	CMotion();		// コンストラクタ
	~CMotion();		// デストラクタ

	//メンバ関数
	static CMotion *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetInfo(int nType);
	void Set(int nType);
	void SetModel(CModel *ppModel, int nNumModel);
	CModel** GetModel(void) { return &m_ppModel[0]; }	// モデルの情報取得
	CModel* GetModel(int nIdx) { return m_ppModel[nIdx]; }

	int GetMotionKey(int nType) { return m_aInfo[nType].nNumKey; }	// 再生中モーションキーの総数取得
	int GetNowKey() { return m_nKey; }	// 現在のキー
	int GetType() { return m_nType; }	// 種類を返す

	bool IsLoop(int nType) { return m_aInfo[nType].bLoop; }	//	ループするかしないか
	bool IsFinish() { return m_bFinish; }	// 終了したかどうかを返す

	void Load(std::string pfile);

private:
	//メンバ変数
	int m_nNumAll;		// モーション総数
	int m_nType;		// モーション種類
	int m_nKey;			// 現在のキーナンバー
	int m_nCounter;		// モーションカウンター
	int m_nNumMotion;	// 現在のモーション
	int m_nNumParts;	// パーツ数
	int m_nPartsIndex;	// パーツの番号
	int m_nNowKey;		// 現在のキー
	int m_nNumModel;	// モデルの総数
	bool m_bFinish;		// 終了したかどうか

	INFO m_aInfo[MAX_MOTION];		// モーション情報
	CModel *m_ppModel[MAX_PARTS];	// モデルへのダブルポインタ
	CModel *m_pParent;				// 親モデルへのポインタ
	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_rot;				// 向き
};

#endif
