//============================================
//
//	���_�}���̏��� [enemyMedaman.h]
//	Author: sakamoto kai
//
//============================================
#ifndef _ENEMY_MEDAMAN_H_
#define _ENEMY_MEDAMAN_H_

#include "enemy.h"

class CModelEffect;

// ���y�̓G�N���X
class CEnemyMedaman : public CEnemy
{
public:
	CEnemyMedaman(int nPriority = 2);
	~CEnemyMedaman();

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
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	void HitDamage(float fDamage);

	// �ÓI�����o�֐�
	static CListManager<CEnemyMedaman>* GetList(void); // ���X�g�擾

private:

	void ActionState(void);								//���[�V�����Ə�Ԃ̊Ǘ�
	void StateManager(void);							//��ԊǗ�
	void SetDamage(float Damage);						//�_���[�W�̔���

	ACTION_TYPE m_Action;			// ���[�V����
	ACTION_TYPE m_AtkAction;		// �U����ԋL�^�p�ϐ�

	int m_nStateCount;			// ��ԃJ�E���g

	// �ÓI�����o�ϐ�
	static CListManager<CEnemyMedaman>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CEnemyMedaman>::AIterator m_iterator; // �C�e���[�^�[

};

#endif