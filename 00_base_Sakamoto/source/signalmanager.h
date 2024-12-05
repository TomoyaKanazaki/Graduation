//==========================================
//
//	���(signalmanager.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#pragma once 
#include "main.h"

class CDevil;

//==========================================
//  �N���X��`
//==========================================
class CSignalManager
{
public:
	// ���[�V����
	enum TYPE_SIGNAL
	{
		// �X�N���[��
		TYPE_SCROLL_UP,				// �X���M���u��v
		TYPE_SCROLL_DOWN,			// �X���M���u���v
		TYPE_SCROLL_LEFT,			// �X���M���u���v
		TYPE_SCROLL_RIGHT,			// �X���M���u�E�v
		TYPE_SCROLL_MAX,

		// �X���[�v
		TYPE_SLOPE_FRONT,			// ��O��
		TYPE_SLOPE_BACK,			// ����
		TYPE_SLOPE_RIGHT,			// �E��
		TYPE_SLOPE_LEFT,			// ����
		TYPE_SLOPE_MAX,				// �ő�
	};

	// �����o�ϐ�
	CSignalManager(int nPriority = 2);
	~CSignalManager();

	static CSignalManager* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetMove(int nType);

	// �ÓI�����o�֐�
	static CListManager<CSignalManager>* GetList(void); // ���X�g�擾

private:
	int m_nIdxXModel;		//X���f���̔ԍ�
	int m_nStateCount;		//��ԊǗ��p�ϐ�
	int m_nStateNum;
	float m_fRot;		
	CDevil* m_pDevil;

	D3DXVECTOR3 m_rot;		//��]��

	// �ÓI�����o�ϐ�
	static CListManager<CSignalManager>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CSignalManager>::AIterator m_iterator; // �C�e���[�^�[
};
