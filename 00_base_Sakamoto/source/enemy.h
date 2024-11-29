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
	enum E_STATE
	{
		E_STATE_WAIT = 0,	//�ҋ@
		E_STATE_TRUN,		//�����]��
		E_STATE_WALK,		//���s
		E_STATE_EGG,		//�ڋʏĂ�
		E_STATE_DEATH,		//���S
		E_STATE_MAX,		//�ő�
	};

	//���݂̈ړ�����
	enum SELECT_MOVE
	{
		SELECT_MOVE_RIGHT = 0,	//�E
		SELECT_MOVE_LEFT,		//��
		SELECT_MOVE_UP,			//��
		SELECT_MOVE_DOWN,		//��
		SELECT_MOVE_MAX,		//�ő�
	};

	static CEnemy* Create(const ENEMY_TYPE eType, const CMapSystem::GRID& grid);
	HRESULT Init(void);
	virtual void Uninit(void);
	void Update(void);
	void Draw(void);

	virtual bool Hit(int nLife);

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	void SetEnemyType(ENEMY_TYPE Type) { m_EnemyType = Type; }
	ENEMY_TYPE GetEnemyType(void) { return m_EnemyType; }

	// �}�b�v�ԍ��̐ݒ�
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	void HitDamage(float fDamage) {};

	// �ÓI�����o�֐�
	static CListManager<CEnemy>* GetList(void); // ���X�g�擾

protected:

	HRESULT InitModel(const char* pFilename);				// ���f���̏�����

private:

	void StateManager(D3DXVECTOR3& posMy);				//��ԍX�V
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

	CMapSystem::GRID m_Grid;		//�O���b�g�ԍ�
	CMapSystem::GRID m_SelectGrid;	//�����]�����s�������̃O���b�h�ԍ�

	int m_nLife;				// �̗�

	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_Objmove;		//�I�u�W�F�N�g����e�������ړ���
	int m_nActionCount;			//�s���̃J�E���g
	ENEMY_TYPE m_EnemyType;		//�G�̎��

	float m_ColorA;				//�s�����x

	E_STATE m_State;			//���
	int m_nStateCount;			//��ԊǗ��p�J�E���g
	SELECT_MOVE m_SelectMove;	//�ړ�����

	bool m_OKL;					//���ւ̐i�s��������邩�ǂ���
	bool m_OKR;					//�E�ւ̐i�s��������邩�ǂ���
	bool m_OKU;					//��ւ̐i�s��������邩�ǂ���
	bool m_OKD;					//���ւ̐i�s��������邩�ǂ���

	int m_nBugCounter;			//�G���X�^�b�N��������

	CSlowManager* m_pSlow;		// �X���[���

	CEffekseer* m_pEffect; // �G�t�F�N�g

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
