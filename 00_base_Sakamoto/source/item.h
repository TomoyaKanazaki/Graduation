//============================================
//
//	�A�C�e���̏��� [item.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"
#include "objectX.h"

//�I�u�W�F�N�g�A�C�e���N���X
class CItem : public CObjectX
{
public:

	CItem(int nPriority = 3);
	~CItem();

	//�T���v���̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_ACTION,		//�s��
		STATE_MAX,
	};
	
	enum TYPE
	{// �A�C�e���̎��
		TYPE_NONE = 0,
		TYPE_CROSS,		// �\����
		TYPE_BIBLE,		// ����
		TYPE_BOWABOWA,	// �ڂ�ڂ�
		TYPE_MAX
	};

	static CItem* Create(TYPE eType, const D3DXVECTOR3& pos);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	virtual void CollisionPlayer();

	// �e�N�X�`���ԍ��擾
	int GetIdx(void) { return m_nIdxTexture; }

	// X���f���ԍ��擾
	int GetIdxXModel(void) { return -1; }

	//�ʒu�ݒ�E�擾
	void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

private:
	STATE m_eState;		// ���
	TYPE m_eType;		// ���

	int m_nIdxXModel;		//X���f���̔ԍ�
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�
	int m_nStateCount;		//��ԊǗ��p�ϐ�

	float m_fColorA;		//�s�����x
	float m_Scaling;		//�傫��

	bool m_bCollision;		//�����蔻��p�̍��W

	D3DXVECTOR3 m_CollisionPos;	//�����蔻��p�̍��W
	D3DXVECTOR3 m_pos;			//�ʒu	
	D3DXVECTOR3 m_posOld;		//�ߋ��̈ʒu	
	D3DXVECTOR3 m_move;			//�ړ���	
	D3DXVECTOR3 m_rot;			//����	
};

#endif