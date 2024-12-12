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

//�O���錾
class CObjGauge2D;

//���[���}�l�[�W���[�N���X
class CRailManager
{
public:

	CRailManager();
	~CRailManager();

	HRESULT Init(CMapSystem::GRID grid);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

	// �ÓI�����o�ϐ�
	static CListManager<CRailManager>* m_pList;			// �I�u�W�F�N�g���X�g
	static std::vector<CMapSystem::GRID> m_GridPos;		// ���[���̈ʒu��ێ�

};
#endif