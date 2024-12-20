//============================================
//
//	�w�i�I�u�W�F�N�g�Ǘ� [bjObjManager.h]
//	Author:sakai minato
//
//============================================

#ifndef _BG_OBJ_MANAGER_H_
#define _BG_OBJ_MANAGER_H_

#include "MapSystem.h"

// �O���錾
class CObjectCharacter;

// �w�i�I�u�W�F�N�g�Ǘ��N���X
class CBgObjManager
{
public:

	CBgObjManager();
	~CBgObjManager();

	static CBgObjManager* GetInstance();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetGame(CMapSystem::GRID& grid);

private:

	// �}�N����`
#define MOUNTAIN_OBJ_NUM	(4)	// �R�w�i���f���̎g�p��

	void SetFieldBotton(CMapSystem::GRID& grid);
	void SetScrollDevice(void);
	void SetSlopeDevice(void);
	void SetMountain(void);

	CObjectCharacter* m_pBGCharacter[MOUNTAIN_OBJ_NUM];	// �w�i�L�����N�^�[�̃|�C���^
	int m_nCount;										// �J�E���g

	static CBgObjManager* m_pBgObj;

};

#endif