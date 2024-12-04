//============================================
//
//	�G�̏��� [enemy.h]
//	Author: sakamoto kai
//
//============================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "objectcharacter.h"
#include "useful.h"
#include "MapSystem.h"
#include "AStar.h"

//�O���錾
class CObject3D;
class CLevelModelEffect;
class CSlowManager;

//�I�u�W�F�N�g�G�l�~�[�N���X
class CEnemy : public CObjectCharacter
{
public:
	CEnemy(int nPriority = 2);
	~CEnemy();

	enum ENEMY_TYPE
	{
		ENEMY_MEDAMAN = 0,
		ENEMY_BONBON,
		ENEMY_LITTLEDEVIL,
		ENEMY_MAX,
		ENEMY_NONE
	};

	//�G�̏��
	enum HIT_STATE
	{
		HIT_STATE_NORMAL = 0,	//�ʏ�
		HIT_STATE_DAMAGE,		//�_���[�W
		HIT_STATE_INVINCIBLE,	//���G
		E_STATE_EGG,			//���܂��₫
		HIT_STATE_MAX,			//�ő�
	};

	static CEnemy* Create(const ENEMY_TYPE eType, const CMapSystem::GRID& grid);
	HRESULT Init(void);
	virtual void Uninit(void);
	void Update(void);
	void Draw(void);

	virtual bool Hit(void);

	void SetEnemyType(ENEMY_TYPE Type) { m_EnemyType = Type; }
	ENEMY_TYPE GetEnemyType(void) { return m_EnemyType; }

	void SetHitState(HIT_STATE State) { m_HitState = State; }
	HIT_STATE GetHitState(void) { return m_HitState; }
	void SetHitStateCount(int Cnt) { m_nHitStateCount = Cnt; }
	int GetHitStateCount(void) { return m_nHitStateCount; }

	void ChangeMoveState(CMoveState* pMoveState) override;   // �ړ���ԕύX

	// �ÓI�����o�֐�
	static CListManager<CEnemy>* GetList(void); // ���X�g�擾

protected:

	HRESULT InitModel(const char* pFilename);				// ���f���̏�����

private:

	void MoveStateManager(D3DXVECTOR3& posMy);				//��ԍX�V
	void HitStateManager(D3DXVECTOR3& posMy);				//��ԍX�V
	void CollisionWall(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& size,useful::COLLISION XYZ);			//�ǂƂ̓����蔻��
	void CollisionDevilHole(useful::COLLISION XYZ);		//�f�r���z�[���Ƃ̓����蔻��
	void CollisionOut(D3DXVECTOR3& posMy);				//�X�e�[�W�O�Ƃ̓����蔻��

	void UpdatePos(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy);	// �ʒu�X�V����
	void Rot(D3DXVECTOR3& rotMy);						//�ړ���������
	virtual void Death(void);							// ���S����
	void MoveSelect(void);								// �ړ������̑I��
	void SearchWall(D3DXVECTOR3& posMy);				// �ǂ̃T�[�`����
	void Coordinate(); // �ŒZ�o�H�T��
	void Route();	// �ŒZ�o�H�����ǂ�
	void Effect(); // �G�t�F�N�g�𐶐�

	CMapSystem::GRID m_SelectGrid;	//�����]�����s�������̃O���b�h�ԍ�

	int m_nActionCount;			//�s���̃J�E���g
	ENEMY_TYPE m_EnemyType;		//�G�̎��

	float m_ColorA;				//�s�����x

	int m_nMoveStateCount;		//��ԊǗ��p�J�E���g

	HIT_STATE m_HitState;		//���
	int m_nHitStateCount;		//��ԊǗ��p�J�E���g

	int m_nBugCounter;			//�G���X�^�b�N��������

	CSlowManager* m_pSlow;		// �X���[���

	CEffekseer* m_pEffect; // �G�t�F�N�g
	CMoveState* m_pMoveState;		// �ړ����

	// �ÓI�����o�ϐ�
	static CListManager<CEnemy>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CEnemy>::AIterator m_iterator; // �C�e���[�^�[
	CMapSystem::GRID* m_pPath; // �v���C���[�ւ̍ŒZ�o�H
	float m_fCoordinateTimer; // �o�H�T���Ԋu
	int m_nNumCoordinate; // �ŒZ�o�H�̗v�f��
	int m_nTargetIndex; // ���Ɍ������ׂ��O���b�h�̃C���f�b�N�X
};

#endif
