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
		ACTION_BWAIT_RIGFT = 0,	//�퓬�ҋ@�E
		ACTION_SMOVE,			//�T���ړ�
		ACTION_BMOVE,			//�퓬�ړ�
		ACTION_SWAIT,			//�T���ҋ@
		ACTION_ATTACK,			//�U��
		ACTION_BDEATH,			//�퓬���̎��S
		ACTION_SDEATH,			//�T�����̎��S
		ACTION_DAMAGE,			//�_���[�W
		ACTION_BWAIT_UPDOWN,	//�퓬�ҋ@�㉺
		ACTION_BWAIT_LEFT,		//�퓬�ҋ@��
		ACTION_DASHATTACK,		//�_�b�V���U��
		ACTION_MAX,				//�ő�
	};

	static CEnemyMedaman* Create(const char* pFilename);
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