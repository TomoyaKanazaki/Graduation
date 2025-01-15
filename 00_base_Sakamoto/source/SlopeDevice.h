//============================================
//
//	�X�����u�̏��� [SlopeDevice.h]
//	Author:sakai minato
//
//============================================

#ifndef _SLOPE_DEVICE_H_
#define _SLOPE_DEVICE_H_

#include "objectcharacter.h"

#include "scrollarrow.h"

// �X�����u�N���X
class CSlopeDevice : public CObjectCharacter
{
public:

	CSlopeDevice(int nPriority = 3);
	~CSlopeDevice();

	// ���E����̐ݒu�ꏊ
	enum LOCATE_WORLD_TYPE
	{
		LOCATE_WORLD_TYPE_TOP_LEFT = 0,		// ����
		LOCATE_WORLD_TYPE_TOP_RIGHT,		// �E��
		LOCATE_WORLD_TYPE_BOTTOM_LEFT,		// ����
		LOCATE_WORLD_TYPE_BOTTOM_RIGHT,		// �E��
		LOCATE_WORLD_TYPE_MAX
	};

	// ���
	enum STATE
	{
		STATE_NORMAL = 0,	// �ʏ�
		STATE_ASCENT,		// �㏸
		STATE_DESCENT,		// ���~
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
		SETUP_TYPE_EDGE,			// �O��
		SETUP_TYPE_ELEVATING_PART,	// ���~����
		SETUP_TYPE_JACK,			// �W���b�L�{��
		SETUP_TYPE_INJECTION_PART,	// ��������
		SETUP_TYPE_LIVER,			// ���o�[
		SETUP_TYPE_MAX,				// �ő�
	};

	static CSlopeDevice* Create(const std::string pModelNameSlopeDevice, const std::string pModelNameEnemy);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetStateArrow(CScrollArrow::Arrow stateArrow);
	void SetStateArrowBack(CScrollArrow::Arrow stateArrow);

	void ReSet(void);
	
	void SetAppearAll(bool bAppear);

	void SetLocateWorldType(LOCATE_WORLD_TYPE LocateWorldType) { m_LocateWorldType = LocateWorldType ;}
	void SetUseRetroMove(bool bRetroMove) { m_bUseRetroMove = bRetroMove; }

	// �ÓI�����o�֐�
	static CListManager<CSlopeDevice>* GetList(void); // ���X�g�擾

private:
	HRESULT InitModel(const std::string pModelNameSlopeDevice, const std::string pModelNameEnemy);

	void StateManager(void);

	void Ascent(int nNldxModel);
	void Descent(int nNldxModel);
	void ActiveLever(int nNldxModel);

	void SetState(STATE state);

	D3DXVECTOR3 m_posTarget;				// �ړI�ʒu
	D3DXVECTOR3 m_posTargetDef;				// �f�t�H���g�̖ړI�ʒu
	D3DXVECTOR3 m_move;						// �ړ���

	D3DXVECTOR3 m_rotleverDef;				// ���o�[�̌����̏����ʒu
	bool m_bleverMove;						// ���o�[�ړ�����[True = ���Z:False = ���Z]
	
	STATE m_State;							// ���
	LOCATE_WORLD_TYPE m_LocateWorldType;	// ���E����̐ݒu�ʒu

	bool m_bUseRetroMove;					// ���g���^�C�v�̈ړ��̗L��

	CObjectCharacter* m_pObjectCharacter;	// �I�u�W�F�N�g�L�����N�^�[�̃|�C���^

	// �ÓI�����o�ϐ�
	static CListManager<CSlopeDevice>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CSlopeDevice>::AIterator m_iterator; // �C�e���[�^�[

};
#endif