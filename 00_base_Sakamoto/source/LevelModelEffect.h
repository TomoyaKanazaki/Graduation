//============================================
//
//	�v���C���[�̃��x���G�t�F�N�g���� [LevelModelEffect.h]
//	Author:sakamoto kai
//
//============================================

#ifndef _LEVEL_MODEL_EFFECT_H_
#define _LEVEL_MODEL_EFFECT_H_

#include "objectcharacter.h"

// �v���C���[�̃��x���G�t�F�N�g�����N���X
class CLevelModelEffect : public CObjectCharacter
{
public:
	CLevelModelEffect(int nPriority = 5);
	~CLevelModelEffect();

	//�v���C���[�̃��[�V����
	typedef enum
	{
		ACTION_WAIT = 0,
		ACTION_MOVE,
		ACTION_ATTACK,
		ACTION_JAMP,
		ACTION_MAX,

	}ACTION_TYPE;

	static CLevelModelEffect* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetColor(D3DCOLORVALUE Color) { m_Color = Color; }
	void SetDel(float fDel) { m_fDel = fDel; }
	void SetAllPose(int nType, int nKey, float nCounter);
	void SetPose(int nType, int nKey, float nCounter, int nModelNumber);

	// �ÓI�����o�֐�
	static CListManager<CLevelModelEffect>* GetList(void); // ���X�g�擾

private:
	ACTION_TYPE m_Action;		//���[�V����
	D3DCOLORVALUE m_Color;		//�F
	float m_fDel;				//�F�̌���

	// �ÓI�����o�ϐ�
	static CListManager<CLevelModelEffect>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CLevelModelEffect>::AIterator m_iterator; // �C�e���[�^�[
};
#endif