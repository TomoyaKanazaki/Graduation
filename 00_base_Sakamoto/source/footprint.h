#pragma once
//============================================
//
//  ���ՃN���X(footprint.h)
//  Author : Tomoya Kanazaki
//
//============================================
#include "object3D.h"
#include "MapSystem.h"

//==========================================
//  �N���X��`
//==========================================
class CFootPrint : public CObject3D
{
public:

	CFootPrint(int nPriority = 7);
	~CFootPrint();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// �ÓI�����o�֐�
	static CFootPrint* Create(CMapSystem::GRID& grid, const D3DXVECTOR3& rot);
	static CListManager<CFootPrint>* GetList(void);	// ���X�g�擾

private:

	// �����o�֐�
	void SetPos(const D3DXVECTOR3& pos) override;

	// �ÓI�����o�ϐ�
	static CListManager<CFootPrint>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CFootPrint>::AIterator m_iterator; // �C�e���[�^�[

};
