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
#include "MapSystem.h"

// �O���錾
class CPlayer;
class CShadow;

//�I�u�W�F�N�g�A�C�e���N���X
class CItem : public CObjectX
{
public:

	enum TYPE
	{// �A�C�e���̎��
		TYPE_CROSS = 0,	// �\����
		TYPE_BIBLE,		// ����
		TYPE_BOWABOWA,	// �ڂ�ڂ�
		TYPE_SOFTCREAM,	// �\�t�g�N���[��
		TYPE_FRIEDEGG,	// �ڋʏĂ�
		TYPE_MAX,
		TYPE_NONE
	};

	// �����o�ϐ�
	CItem(int nPriority = 2);
	~CItem();

	HRESULT Init(const char* pModelName);
	HRESULT Init() { assert(false); return E_FAIL; }; // �Ă΂�Ă͂Ȃ�Ȃ�
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �e�N�X�`���ԍ��擾
	int GetIdx(void) { return m_nIdxTexture; }

	// X���f���ԍ��擾
	int GetIdxXModel(void) { return -1; }

	//�ړ��ʐݒ�E�擾
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	//�傫���ݒ�E�擾
	void SetScaling(const float& Scaling) { m_Scaling = Scaling; }
	float GetScaling(void) { return m_Scaling; }

	// ��ԊǗ��̎擾
	int GetStateCounter() { return m_nStateCount; }

	// �}�b�v�ԍ��̐ݒ�
	void SetGrid(const CMapSystem::GRID& pos);
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	// �X�N���[�����邩�ǂ����̐ݒ�E�擾
	void SetMapScroll(bool frag) { m_bMapScroll = frag; }
	bool GetMapScroll() { return m_bMapScroll; }

	bool CollisionPlayer();
	virtual bool Hit(CPlayer* pPlayer) = 0;
	void SetItem(const TYPE eType) { m_eType = eType; }

	D3DXVECTOR3 GetBase() { return m_posBase; } // �ړ��̒��S�ʒu���擾
	float GetMoveTime() { return m_fMoveTime; } // �ړ����Ԃ̎擾
	void SetMoveTime(const float time) { m_fMoveTime = time; } // �ړ����Ԃ̎擾
	void Effect(CEffekseer* pEffect) { m_pEffect = pEffect; }
	CEffekseer* GetEffect() { return m_pEffect; }

	// �ÓI�����o�֐�
	static CListManager<CItem>* GetList(void); // ���X�g�擾
	static CItem* Create(const TYPE eType, const CMapSystem::GRID& pos);

private:

	// �����o�֐�
	virtual void Move(D3DXVECTOR3& pos); // �ړ�����
	virtual void SetEffect() {};

	// �����o�ϐ�
	CListManager<CItem>::AIterator m_iterator; // �C�e���[�^�[

	TYPE m_eType;			// ���

	int m_nIdxXModel;		//X���f���̔ԍ�
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�
	int m_nStateCount;		//��ԊǗ��p�ϐ�

	float m_fColorA;		//�s�����x
	float m_Scaling;		//�傫��

	bool m_UseItem;			//�A�C�e�����g�p�\���ǂ���
	bool m_bCollision;		//�����蔻��p�̍��W
	bool m_bMapScroll;		//�}�b�v�̃X�N���[��

	D3DXVECTOR3 m_CollisionPos;	//�����蔻��p�̍��W
	D3DXVECTOR3 m_move;			//�ړ���	

	D3DXVECTOR3 m_posBase; // �ړ��̒��S���W
	float m_fMoveTime; // �ړ�����

	CMapSystem::GRID m_Grid;	// �}�b�v�ԍ�
	CShadow* m_pShadow;		// �V���h�E�̃|�C���^
	CEffekseer* m_pEffect; // �G�t�F�N�g

	// �ÓI�����o�ϐ�
	static CListManager<CItem>* m_pList; // �I�u�W�F�N�g���X�g
};

#endif