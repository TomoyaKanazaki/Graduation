//============================================
//
//	���[���}�l�[�W���[ [RailManager.h]
//	Author:Satone Shion
//
//============================================
#ifndef _RAILMANAGER_H_
#define _RAILMANAGER_H_

#include "main.h"
#include "MapSystem.h"
#include "Rail.h"

//�O���錾
class CObjGauge2D;

//���[���}�l�[�W���[�N���X
class CRailManager
{
public:

	CRailManager();
	~CRailManager();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(CMapSystem::GRID& grid);		// ���[���̐ݒ�

private:
	// �����o�֐�
	void SetRot(CMapSystem::GRID& grid, CRail::POSTYPE& PosType0, CRail::POSTYPE& PosType1);		// ���f���̌����̐ݒ�

	// �����o�֐�
	bool m_bRail[CRail::POSTYPE_MAX];					// ���[���̒u����ʒu�̔���


	// �ÓI�����o�ϐ�
	static CListManager<CRailManager>* m_pList;			// �I�u�W�F�N�g���X�g
	static std::vector<CMapSystem::GRID> m_GridPos;		// ���[���̈ʒu��ێ�

};
#endif