//============================================
//
//	�����f���̏��� [tile.h]
//	Author:sakamoto kai
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
	~CTile();

	static CTile* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	/*int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }*/
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	// �}�b�v�ԍ��̐ݒ�
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	// �ÓI�����o�֐�
	static CListManager<CTile>* GetList(void); // ���X�g�擾

private:

	CMapSystem::GRID m_Grid;	//�O���b�g�ԍ�

	int m_nIdxXModel;				//X���f���̔ԍ�
	//D3DXVECTOR3 m_CollisionPos;		//�����蔻��p�̍��W
	//bool m_bCollision;				//�����蔻��p�̍��W
	int m_nIdxTexture;				//�e�N�X�`���̔ԍ�
	D3DXVECTOR3 m_pos;				//�ʒu	
	D3DXVECTOR3 m_posOld;			//�ߋ��̈ʒu	
	D3DXVECTOR3 m_move;				//�ړ���

	//bool m_bSet[4];					//�㉺���E�̌������܂��Ă��邩�ǂ���
	//CObjectX *m_pHoleKey[4];			//�㉺���E�̌��𖄂߂�|���S��

	// �ÓI�����o�ϐ�
	static CListManager<CTile>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CTile>::AIterator m_iterator; // �C�e���[�^�[

};
#endif