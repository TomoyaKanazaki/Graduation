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

// �O���錾
class CPlayer;

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
		TYPE_CROSS = 0,	// �\����
		TYPE_BIBLE,		// ����
		TYPE_BOWABOWA,	// �ڂ�ڂ�
		TYPE_MAX,
		TYPE_NONE
	};

	// ����ȍ��W�n
	struct GRID
	{
		// �R���X�g���N�^
		GRID() {};
		GRID(int X, int Y) { x = X; y = Y; };

		int x;
		int y;
	};

	HRESULT Init(const char* pModelName);
	HRESULT Init() { assert(false); return E_FAIL; }; // �Ă΂�Ă͂Ȃ�Ȃ�
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CItem* Create(const TYPE eType, const GRID& pos);

	virtual void Move() {}

	// �e�N�X�`���ԍ��擾
	int GetIdx(void) { return m_nIdxTexture; }

	// X���f���ԍ��擾
	int GetIdxXModel(void) { return -1; }

	//�ʒu�ݒ�E�擾
	void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	//�ʒu�ݒ�E�擾
	void SetPosold(const D3DXVECTOR3& posold) { m_posOld = posold; }
	D3DXVECTOR3 GetPosold(void) { return m_posOld; }

	//�ړ��ʐݒ�E�擾
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	//�����ݒ�E�擾
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	//�傫���ݒ�E�擾
	void SetScaling(const float& Scaling) { m_Scaling = Scaling; }
	float GetScaling(void) { return m_Scaling; }

	// ��Ԏ擾
	STATE GetState() { return m_eState; }

	// ��ԊǗ��̎擾
	int GetStateCounter() { return m_nStateCount; }

	// �}�b�v�ԍ��̐ݒ�
	virtual void SetGrid(const GRID& pos);

	// �}�b�v�̉��ԍ��̐ݒ�E�擾
	void SetWightNumber(int Wight) { m_nMapWidth = Wight; }
	int GetWightNumber() { return m_nMapWidth; }

	// �}�b�v�̏c�ԍ��̐ݒ�E�擾
	void SetHeightNumber(int Height) { m_nMapHeight = Height; }
	int GetHeightNumber() { return m_nMapHeight; }

	// �X�N���[�����邩�ǂ����̐ݒ�E�擾
	void SetMapScroll(bool Height) { m_bMapScroll = Height; }
	bool GetMapScroll() { return m_bMapScroll; }

	bool CollisionPlayer();
	virtual bool Hit(CPlayer* pPlayer) = 0;

private:

	STATE m_eState;		// ���
	TYPE m_eType;		// ���

	int m_nIdxXModel;		//X���f���̔ԍ�
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�
	int m_nStateCount;		//��ԊǗ��p�ϐ�

	float m_fColorA;		//�s�����x
	float m_Scaling;		//�傫��

	bool m_UseItem;			//�A�C�e�����g�p�\���ǂ���
	bool m_bCollision;		//�����蔻��p�̍��W
	bool m_bMapScroll;		//�}�b�v�̃X�N���[��

	D3DXVECTOR3 m_CollisionPos;	//�����蔻��p�̍��W
	D3DXVECTOR3 m_pos;			//�ʒu	
	D3DXVECTOR3 m_posOld;		//�ߋ��̈ʒu	
	D3DXVECTOR3 m_move;			//�ړ���	
	D3DXVECTOR3 m_rot;			//����	

	int m_nMapWidth;			//�}�b�v�̉��ԍ�
	int m_nMapHeight;			//�}�b�v�̏c�ԍ�
};

#endif