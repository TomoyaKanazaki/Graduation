//============================================
//
//	�}�b�v�ړ����u�̏��� [ScrollDevice.h]
//	Author:sakai minato
//
//============================================
#ifndef _SCROLL_DEVICE_H_
#define _SCROLL_DEVICE_H_

#include "objectcharacter.h"

#include "scrollarrow.h"

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

	// ���E����̐ݒu�ꏊ
	enum LOCATE_WORLD_TYPE
	{
		LOCATE_WORLD_TYPE_TOP = 0,	// ��
		LOCATE_WORLD_TYPE_BOTTOM,	// ��
		LOCATE_WORLD_TYPE_LEFT,		// ��
		LOCATE_WORLD_TYPE_RIGHT,	// �E
		LOCATE_WORLD_TYPE_MAX
	};

	// ���[�V����
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,	// �ҋ@���
		ACTION_MAX,			// �ő�
	};

	// �p�[�c�\��
	enum SETUP_TYPE
	{
		SETUP_TYPE_FOUNDATION = 0,	// �y��
		SETUP_TYPE_MAWASIGURMA,		// �񂵎�
		SETUP_TYPE_ROLLRE,			// ���[���[
		SETUP_TYPE_HEAD,			// �����
		SETUP_TYPE_MAX,				// �ő�
	};

	static CScrollDevice* Create(const std::string pModelNameSlopeDevice, const std::string pModelNameEnemy);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(STATE state);
	void SetStateArrow(CScrollArrow::Arrow stateArrow);

	void SetLocateWorldType(LOCATE_WORLD_TYPE LocateWorldType) { m_LocateWorldType = LocateWorldType; }


	// �ÓI�����o�֐�
	static CListManager<CScrollDevice>* GetList(void); // ���X�g�擾

private:
	HRESULT InitModel(const std::string pModelNameScrollDevice, const std::string pModelNameEnemy);

	void StateManager(D3DXVECTOR3& rotMy);
	void Rotate(D3DXVECTOR3& rotMy,int nNldxModel);

	void SetRotMedaman(CScrollArrow::Arrow stateArrow = CScrollArrow::Arrow(0));

	STATE m_State;					//���
	int m_nStateCount;				//��ԊǗ��p�ϐ�

	LOCATE_WORLD_TYPE m_LocateWorldType;	// ���E����̐ݒu�ʒu

	D3DXVECTOR3 m_rotMove;					// �����̈ړ���

	CObjectCharacter* m_pObjectCharacter;	// �I�u�W�F�N�g�L�����N�^�[�̃|�C���^

	// �ÓI�����o�ϐ�
	static CListManager<CScrollDevice>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CScrollDevice>::AIterator m_iterator; // �C�e���[�^�[

};
#endif