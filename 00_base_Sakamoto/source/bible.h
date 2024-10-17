//============================================
//
//	�����̏��� [bible.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _BIBLE_H_
#define _BIBLE_H_

#include "main.h"
#include "item.h"

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�v���C���[�N���X
class CBible : public CItem
{
public:

	CBible(int nPriority = 3);
	~CBible();

	static CBible* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

private:
	int m_nIdxXModel;				//X���f���̔ԍ�
	int m_nIdxTexture;				//�e�N�X�`���̔ԍ�
	int m_nStateCount;				//��ԊǗ��p�ϐ�

	float m_Scaling;				//�傫��
	float m_fColorA;				//�s�����x

	bool m_bCollision;				//�����蔻��p�̍��W

	D3DXVECTOR3 m_CollisionPos;		//�����蔻��p�̍��W
	D3DXVECTOR3 m_pos;				//�ʒu	
	D3DXVECTOR3 m_posOld;			//�ߋ��̈ʒu	
	D3DXVECTOR3 m_move;				//�ړ���	
	D3DXVECTOR3 m_rot;				//����	
};

#endif
