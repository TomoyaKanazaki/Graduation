//============================================
//
//	�|���̏��� [enemyBowman.h]
//	Author:��{�@�ėB
//
//============================================
#ifndef _ENEMY_BOWMAN_H_
#define _ENEMY_BOWMAN_H_

#include "enemy.h"

class CModelEffect;

// �|���̓G�N���X
class CEnemyBowman : public CEnemy
{
public:
	CEnemyBowman(int nPriority = 2);
	~CEnemyBowman();

	// ���[�V����
	enum ACTION_TYPE
	{
		ACTION_BWAIT = 0,		//�퓬�ҋ@
		ACTION_SMOVE,			//�T���ړ�
		ACTION_BMOVE,			//�퓬�ړ�
		ACTION_SWAIT,			//�T���ҋ@
		ACTION_ATTACK,			//�U��
		ACTION_BDEATH,			//�퓬���̎��S
		ACTION_SDEATH,			//�T�����̎��S
		ACTION_DAMAGE,			//�_���[�W
		ACTION_MAX,				//�ő�
	};

	//�G�̐퓬���
	enum BATTLESTATE
	{
		BATTLESTATE_NORMAL = 0,	//�ʏ�
		BATTLESTATE_DEATH,		//���S
		BATTLESTATE_WAIT,		//�ҋ@
		BATTLESTATE_ATTACK,		//�ҋ@
		BATTLESTATE_DAMAGE,		//�_���[�W
		BATTLESTATE_MAX,		//�ő�
	};

	//�G�̒T�����
	enum SEARCHSTATE
	{
		SEARCHSTATE_NORMAL = 0,	//�ʏ�
		SEARCHSTATE_WALK,		//���s
		SEARCHSTATE_TRUN,		//�����]��
		SEARCHSTATE_WAIT,		//�ҋ@
		SEARCHSTATE_SEARCH,		//�T��
		SEARCHSTATE_MAX,		//�ő�
	};

	static CEnemyBowman* Create(const char* pFilename);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	BATTLESTATE GetBattleState(void) { return m_BState; }
	SEARCHSTATE GetSearchState(void) { return m_SState; }

	void SetSeatchState(SEARCHSTATE State) { m_SState = State; }

private:
	void HitDamage(float fDamage);

	void MyObjCreate(void);								//�������ێ�����I�u�W�F�N�g�̐���
	void ActionState(void);								//���[�V�����Ə�Ԃ̊Ǘ�
	void BattleStateManager(void);						//�퓬��ԊǗ�
	void SearchStateManager(void);						//�T����ԊǗ�
	void SetDamage(float Damage);						//�_���[�W�̔���

	void WalkPoint(void);								// �ړI�l�ɕ�������
	void TurnPoint(void);								// �ړI�n��������]������
	void CollisionPush(void);

	ACTION_TYPE m_Action;			// ���[�V����
	ACTION_TYPE m_AtkAction;		// �U����ԋL�^�p�ϐ�
	BATTLESTATE m_BState;			// �퓬���	
	SEARCHSTATE m_SState;			// �T�����

	int m_nStateCount;			// ��ԃJ�E���g
};

#endif