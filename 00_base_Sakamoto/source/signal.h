//==========================================
//
//	���(signal.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include "objectX.h"
#include "main.h"

class CDevil;

//==========================================
//  �N���X��`
//==========================================
class CSignal : public CObjectX
{
public:
	// ���[�V����
	enum ACTION_TYPE
	{
		ACTION_NONE = 0,			// �ҋ@
		ACTION_SIGNAL_UP,			// �X���M���u��v
		ACTION_SIGNAL_DOWN,			// �X���M���u���v
		ACTION_SIGNAL_LEFT,			// �X���M���u���v
		ACTION_SIGNAL_RIGHT,		// �X���M���u�E�v
		ACTION_MAX,					// �ő�
	};

	// �����o�ϐ�
	CSignal(int nPriority = 2);
	~CSignal();

	HRESULT Init(const char* pModelName);
	HRESULT Init() { assert(false); return E_FAIL; }; // �Ă΂�Ă͂Ȃ�Ȃ�
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CListManager<CSignal>* GetList(void); // ���X�g�擾
	static CSignal* Create(const char* pModelNameSignal,D3DXVECTOR3 pos);

	//�傫���ݒ�E�擾
	void SetScaling(const float& Scaling) { m_Scaling = Scaling; }
	float GetScaling(void) { return m_Scaling; }

private:
	int m_nIdxXModel;		//X���f���̔ԍ�
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�
	int m_nStateCount;		//��ԊǗ��p�ϐ�
	int m_nStateNum;

	float m_fColorA;		//�s�����x
	float m_Scaling;		//�傫��

	bool m_UseItem;			//�A�C�e�����g�p�\���ǂ���

	D3DXVECTOR3 m_rot;		//��]��

	CDevil* m_pDevil;

	// �ÓI�����o�ϐ�
	static CListManager<CSignal>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CSignal>::AIterator m_iterator; // �C�e���[�^�[
};

#endif