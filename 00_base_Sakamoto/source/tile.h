//============================================
//
//	�����f���̏��� [tile.h]
//	Author:Satone Shion
//
//============================================
#ifndef _TILE_H_
#define _TILE_H_

#include "main.h"
#include "objectX.h"
#include "MapSystem.h"

//���N���X
class CTile : public CObjectX
{
public:

	CTile(int nPriority = 3);
	CTile(int nPriority, CMapSystem::GRID gridCenter);
	~CTile();

	static CTile* Create(CMapSystem::GRID gridCenter);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CListManager<CTile>* GetList(void); // ���X�g�擾

private:

	CMapSystem::GRID m_Grid;	//�O���b�g�ԍ�

	// �ÓI�����o�ϐ�
	static CListManager<CTile>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CTile>::AIterator m_iterator; // �C�e���[�^�[

};
#endif