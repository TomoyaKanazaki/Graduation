//========================================
//
//キャラクター全般処理[character.h]
//Author：森川駿弥
//
//========================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "main.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "motion.h"

//========================================
//キャラクタークラス
//========================================
class CCharacter : public CObject
{
public:
	enum STATE
	{// キャラクターの状態
		STATE_NONE = 0,
		STATE_NORMAL,		// 通常
		STATE_DAMAGE,		// ダメージ
		STATE_DEATH,		// 死亡
		STATE_MAX
	};

	CCharacter(int nPriority = 4);		//コンストラクタ
	~CCharacter();		//デストラクタ

	// メンバ関数
	HRESULT Init(void) { return S_OK; }		//純粋仮想
	HRESULT Init(std::string pfile);		//初期化
	void Uninit(void);		//終了
	void Update(void);		//更新
	void Draw(void);		//描画

	int GetMotionType() { return m_pMotion->GetType(); }				// モーションの種類取得
	bool GetMotionLoop(int nType) { return m_pMotion->IsLoop(nType); }	// モーションのループ取得

	CMotion* GetMotion() { return m_pMotion; }	// モーション情報取得

	// 目的の向きの設定と取得
	void SetRotDest(D3DXVECTOR3 RotDest) { m_RotDest = RotDest; }
	D3DXVECTOR3 GetRotDest() { return m_RotDest; }

	// 状態取得
	int GetState() { return m_nState; }

private:
	//メンバ変数
	LPDIRECT3DTEXTURE9 m_pTexture;	// 共有テクスチャ
	int m_nIdxTexture;				// テクスチャの番号
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	LPD3DXMESH m_pMesh;				// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;		// マテリアルへのポインタ
	DWORD m_dwNumMat;				// マテリアルの数

	D3DXVECTOR3 m_vtxMin, m_vtxMax;	// モデルの最大、最小値

	CModel* m_apModel[MAX_PARTS];	// モデルへのポインタ
	CMotion* m_pMotion;		// モーションのポインタ
	D3DXVECTOR3 m_RotDest;	// 目的の向き

	int m_apNumModel;		// モデル(パーツ)の総数
	int m_nOldMotion;		// 前回のモーション
	int m_nState;			// 現在の状態

	bool m_bJump;			//ジャンプ
	bool m_bMove;			//移動
	bool m_bWait;			//待機
	bool m_bMowingdown;		// 攻撃
	bool m_bCutdown;		// 切り下ろし
	bool m_bStrongAttack;	// 強攻撃
};

#endif
