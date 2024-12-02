//============================================
//
//	キャラクターの処理 [character.h]
//	Author:sakai minato
//
//============================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "object.h"

class CModel;
class CMotion;
class CShadow;

#define MODEL_NUM		(64)	// モデルの数
#define FILE_NAME_SIZE	(128)	// ファイル名の最大文字数

// キャラクタークラス
class CObjectCharacter : public CObject
{
public:
	CObjectCharacter(int nPriority = 3);
	~CObjectCharacter();

	virtual HRESULT Init(void) { return S_OK; };
	virtual HRESULT Init(int PlayNumber) { return S_OK; };
	HRESULT Init(const char* pModelName);

	void Uninit(void);
	void Update(void);
	void Draw(void);

	CModel* GetModel(int nCnt);
	CMotion* GetMotion(void);
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

	void SetTxtCharacter(const char* pFilename);

protected:

	CShadow* m_pShadow;

private:

	void LoadModel(const char* pFilename);

	CModel* m_apModel[MODEL_NUM];
	CMotion* m_pMotion;
	char m_aModelName[FILE_NAME_SIZE];
	int m_nNumModel;

	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_rot;				//向き
	D3DXVECTOR3 m_posOld;			//過去の位置
	D3DXVECTOR3 m_size;				//大きさ

	// マトリックス情報
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX* m_UseMultiMatrix;	// 掛け合わせるマトリックス

	bool m_bUseStencil;				// ステンシルバッファの使用の有無
	bool m_bUseShadowMtx;			// シャドウマトリックスの使用の有無
	bool m_bUseShadow;				// 影の使用フラグ
};

#endif
