//============================================
//
//	�X�����u�̏��� [SlopeDevice.h]
//	Author:sakai minato
//
//============================================

#ifndef _SLOPE_DEVICE_H_
#define _SLOPE_DEVICE_H_

#include "objectcharacter.h"

// �X�����u�N���X
class CSlopeDevice : public CObjectCharacter
{
public:

	CSlopeDevice(int nPriority = 3);
	~CSlopeDevice();

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

	static CSlopeDevice* Create(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(STATE state) { m_State = state; }

	// �ÓI�����o�֐�
	static CListManager<CSlopeDevice>* GetList(void); // ���X�g�擾

private:
	HRESULT InitModel(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	void StateManager(D3DXVECTOR3& rotMy);

	STATE m_State;					//���
	int m_nStateCount;				//��ԊǗ��p�ϐ�

	// �ÓI�����o�ϐ�
	static CListManager<CSlopeDevice>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CSlopeDevice>::AIterator m_iterator; // �C�e���[�^�[

};
#endif