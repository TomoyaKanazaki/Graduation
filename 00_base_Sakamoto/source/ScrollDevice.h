//============================================
//
//	�}�b�v�ړ����u�̏��� [ScrollDevice.h]
//	Author:sakai minato
//
//============================================
#ifndef _SCROLL_DEVICE_H_
#define _SCROLL_DEVICE_H_

#include "objectcharacter.h"

// �}�b�v�ړ����u�N���X
class CScrollDevice : public CObjectCharacter
{
public:

	CScrollDevice(int nPriority = 3);
	~CScrollDevice();

	// ���
	enum STATE
	{
		STATE_NORMAL = 0,	// �ʏ�
		STATE_ROTATE,		// ��]
		STATE_MAX,
	};

	// ���[�V����
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,	// �ҋ@���
		ACTION_MAX,			// �ő�
	};

	// ���[�V����
	enum SETUP_TYPE
	{
		SETUP_TYPE_FOUNDATION = 0,	// �y��
		SETUP_TYPE_MAWASIGURMA,		// �񂵎�
		SETUP_TYPE_ROLLRE,			// ���[���[
		SETUP_TYPE_HEAD,			// �����
		SETUP_TYPE_MAX,				// �ő�
	};

	static CScrollDevice* Create(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(STATE state) { m_State = state; }

	// �ÓI�����o�֐�
	static CListManager<CScrollDevice>* GetList(void); // ���X�g�擾

private:
	HRESULT InitModel(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	void StateManager(D3DXVECTOR3& rotMy);
	void Rotate(D3DXVECTOR3& rotMy,int nNldxModel, D3DXVECTOR3 rotate);

	STATE m_State;					//���
	int m_nStateCount;				//��ԊǗ��p�ϐ�

	// �ÓI�����o�ϐ�
	static CListManager<CScrollDevice>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CScrollDevice>::AIterator m_iterator; // �C�e���[�^�[

};
#endif