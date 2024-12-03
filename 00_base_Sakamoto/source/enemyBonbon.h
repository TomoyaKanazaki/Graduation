//============================================
//
//	�{���{���̏��� [enemyBonbon.h]
//	Author: sakamoto kai
//
//============================================
#ifndef _ENEMY_BONBON_H_
#define _ENEMY_BONBON_H_

#include "enemy.h"

class CModelEffect;

// ���y�̓G�N���X
class CEnemyBonbon : public CEnemy
{
public:
	CEnemyBonbon(int nPriority = 2);
	~CEnemyBonbon();

	// ���[�V����
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,	// �ҋ@���
		ACTION_MOVE,		// �ړ����
		ACTION_TEST,		// �e�X�g�p���
		ACTION_MAX,			// �ő�
	};

	HRESULT Init(void) override;
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Hit(void);

	// �ÓI�����o�֐�
	static CListManager<CEnemyBonbon>* GetList(void); // ���X�g�擾

private:

	void ActionState(void);								//���[�V�����Ə�Ԃ̊Ǘ�
	void StateManager(void);							//��ԊǗ�
	void SetDamage(float Damage);						//�_���[�W�̔���

	ACTION_TYPE m_Action;			// ���[�V����
	ACTION_TYPE m_AtkAction;		// �U����ԋL�^�p�ϐ�

	int m_nStateCount;			// ��ԃJ�E���g

	// �ÓI�����o�ϐ�
	static CListManager<CEnemyBonbon>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CEnemyBonbon>::AIterator m_iterator; // �C�e���[�^�[

};

#endif