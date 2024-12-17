//============================================
//
//	キャラクターの処理 [character.h]
//	Author:sakai minato
//
//============================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "object.h"
#include "Model.h"
#include "MapSystem.h"
#include "MapMove.h"

class CModel;
class CMotion;
class CShadow;
class CMoveState;		// 移動の状態

#define MODEL_NUM		(64)	// モデルの数
#define FILE_NAME_SIZE	(128)	// ファイル名の最大文字数

// キャラクタークラス
class CObjectCharacter : public CObject
{
public:
	//状態
	enum STATE
	{
		STATE_WAIT = 0,		//待機
		STATE_WALK,			//歩き
		STATE_DEATH,		//死亡
		STATE_EGG,			//卵
		STATE_ATTACK,		//攻撃
		STATE_TRUN,			//方向転換

		STATE_MAX,			//最大
		STATE_NONE
	};

	// 移動の進行状況を管理する構造体
	struct PROGGRESS
	{
		bool bOKL;		//左への進行が許されるかどうか
		bool bOKR;		//右への進行が許されるかどうか
		bool bOKU;		//上への進行が許されるかどうか
		bool bOKD;		//下への進行が許されるかどうか
	};

	CObjectCharacter(int nPriority = 3);
	~CObjectCharacter();

	static CObjectCharacter* Create(bool bShadow);

	virtual HRESULT Init(void);
	virtual HRESULT Init(int PlayNumber) { return S_OK; };

	void Uninit(void);
	void Update(void);
	void Draw(void);

	virtual void PlayerNumberDisp(bool Set) {}

	void SetTxtCharacter(const std::string pFilename, int nRef);

	void SetModelColor(CModel::COLORTYPE Type, D3DXCOLOR Col);

	void SetModel(CModel* pModel, int nCnt);
	CModel* GetModel(int nCnt);
	CModel** GetModel(void);
	void SetMotion(CMotion* pMotion);
	CMotion* GetMotion(void);

	void SetNumModel(int nNumModel) { m_nNumModel = nNumModel; }
	int GetNumModel(void) { return m_nNumModel; }

	void SetUseMultiMatrix(D3DXMATRIX* Set) { m_UseMultiMatrix = Set; }
	D3DXMATRIX* GetUseMultiMatrix(void) { return m_UseMultiMatrix; }
	void SetUseStencil(bool bUse) { m_bUseStencil = bUse; }
	void SetUseShadowMtx(bool bUse) { m_bUseShadowMtx = bUse; }

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	void SetShadow(bool frag) { m_bUseShadow = frag; }
	bool GetShadow() { return m_bUseShadow; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	// 移動の設定
	void SetMoveX(float moveX) { m_move.x = moveX; }	// 移動の設定X
	void SetMoveZ(float moveZ) { m_move.z = moveZ; }	// 移動の設定Y
	D3DXVECTOR3 GetMove(void) { return m_move; }		// 移動の取得
	D3DXVECTOR3 GetObjMove(void) { return m_Objmove; }
	void SetObjMoveX(float move) { m_Objmove.x = move; }
	void SetObjMoveZ(float move) { m_Objmove.z = move; }
	void SetState(STATE State) { m_State = State; }
	STATE GetState(void) { return m_State; }
	void SetOldState(STATE State) { m_OldState = State; }
	STATE GetOldState(void) { return m_OldState; }

	// 移動状態クラス用
	void SetEggMove(D3DXVECTOR3 EggMove) { m_EggMove = EggMove; }	// 卵の移動量設定
	virtual D3DXVECTOR3 GetEggMove() { return m_EggMove; }			// 卵の移動量取得
	void SetInvincible(bool bInvincible) { m_bInvincible = bInvincible; }		// 無敵かどうか
	void SetInvincibleCount(int nInvincibleCount) { m_nInvincibleCount = nInvincibleCount; }		// 無敵時間

	// マップ番号の設定
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }
	virtual void SetGridOld(const CMapSystem::GRID& posOld) { m_GridOld = posOld; }
	CMapSystem::GRID GetGridOld(void) { return m_GridOld; }

	// 移動状態
	virtual void ChangeMoveState(CMoveState* pMoveState);   // 移動状態変更
	CMoveState* GetMoveState() { return m_pMoveState; }	// 移動状態の情報取得

	void SetProgress(PROGGRESS progress) { m_Progress = progress; }		// 移動の進行許可状況設定
	PROGGRESS GetProgress() { return m_Progress; }		// 移動の進行許可状況取得
	bool GetGritCenter() { return m_bGritCenter; }		// グリッドの中心にいるか取得

	void SetRefIdx(int nRefIdx) { m_nRefIdx = nRefIdx; }
	int GetRefIdx() { return m_nRefIdx; }

	void SetSpeedState(CMapMove::SPEED State) { m_SpeedState = State; }	//速度
	CMapMove::SPEED GetSpeedState(void) { return m_SpeedState; }	//速度
	CMapMove::SPEED GetOldSpeedState(void) { return m_OldSpeedState; }	//速度

protected:

	CShadow* m_pShadow;
	PROGGRESS m_Progress;			// 移動の進行許可状況
	CMapSystem::GRID m_Grid;		//グリット番号
	CMapSystem::GRID m_GridOld;		// 前回のグリット番号

	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_Objmove;			//オブジェクトから影響される移動量

	bool m_bGritCenter;				//グリットの中心位置にいるかどうか

private:

	void SpeedStateManager(void);
	void LoadModel(const std::string pFilename);

	CModel* m_apModel[MODEL_NUM];
	CMotion* m_pMotion;
	char m_aModelName[FILE_NAME_SIZE];
	int m_nNumModel;
	int m_nCharacterNum;

	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_rot;				//向き
	D3DXVECTOR3 m_posOld;			//過去の位置
	D3DXVECTOR3 m_size;				//大きさ

	CMapMove::SPEED m_SpeedState;	//速度
	CMapMove::SPEED m_OldSpeedState;	//速度

	// マトリックス情報
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX* m_UseMultiMatrix;	// 掛け合わせるマトリックス

	bool m_bUseStencil;				// ステンシルバッファの使用の有無
	bool m_bUseShadowMtx;			// シャドウマトリックスの使用の有無
	bool m_bUseShadow;				// 影の使用フラグ

	STATE m_State;					//状態
	STATE m_OldState;				// 前回の状態
	D3DXVECTOR3 m_EggMove;			//卵の動き
	bool m_bInvincible;				//無敵かどうか
	int m_nInvincibleCount;			//無敵時間

	CMoveState* m_pMoveState;		// 移動状態

	int m_nRefIdx;					// ステンシル参照値
};

#endif
