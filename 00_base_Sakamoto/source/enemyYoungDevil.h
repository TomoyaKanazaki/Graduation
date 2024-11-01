//============================================
//
//	�q�f�r������ [enemyMedaman.h]
//	Author: sakamoto kai
//
//============================================
#ifndef _ENEMY_YOUNGDEVIL_H_
#define _ENEMY_YOUNGDEVIL_H_

#include "enemy.h"

class CModelEffect;

// ���y�̓G�N���X
class CEnemyYoungDevil : public CEnemy
{
public:
	CEnemyYoungDevil(int nPriority = 2);
	~CEnemyYoungDevil();

	// ���[�V����
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,	// �ҋ@���
		ACTION_MOVE,		// �ړ����
		ACTION_TEST,		// �e�X�g�p���
		ACTION_MAX,			// �ő�
	};

	static CEnemyYoungDevil* Create(const char* pFilename);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	void HitDamage(float fDamage);
private:

	void MyObjCreate(void);								//�������ێ�����I�u�W�F�N�g�̐���
	void ActionState(void);								//���[�V�����Ə�Ԃ̊Ǘ�
	void StateManager(void);							//��ԊǗ�
	void SetDamage(float Damage);						//�_���[�W�̔���

	ACTION_TYPE m_Action;			// ���[�V����
	ACTION_TYPE m_AtkAction;		// �U����ԋL�^�p�ϐ�

	int m_nStateCount;			// ��ԃJ�E���g
};

#endif