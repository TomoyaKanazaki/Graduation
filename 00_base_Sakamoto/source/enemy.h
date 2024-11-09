//============================================
//
//	�G�̏��� [enemy.h]
//	Author: sakamoto kai
//
//============================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "object.h"
#include "useful.h"
#include "MapSystem.h"

//�O���錾
class CModel;
class CMotion;
class CObject3D;
class CLevelModelEffect;
class CSlowManager;

#define SCREEN_POS_NUM	(8)		// �X�N���[�����W�̐�
#define SEARCH_POS_NUM	(32)	// �T���ʒu�̐�
#define MODEL_NUM		(64)	// ���f���̐�

//�I�u�W�F�N�g�v���C���[�N���X
class CEnemy : public CObject
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

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CEnemy* Create(char* aModelName);
	HRESULT Init(void);
	virtual void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	virtual bool Hit(int nLife);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	const char* GetTextModelName(void) { return &m_cFileName[0]; }

	CModel** GetLevelModel(void) { return &m_apModel[0]; }
	CModel* GetLevelModel(int nIdx) { return m_apModel[nIdx]; }

	CMotion* GetMotion(void){
		if (m_pMotion != nullptr) { return m_pMotion; }
		else { return nullptr; }}

	int GetNumModel(void) { return m_nNumModel; }
	void SetEnemyType(ENEMY_TYPE Type) { m_EnemyType = Type; }
	ENEMY_TYPE GetEnemyType(void) { return m_EnemyType; }

	void SetMultiMatrix(bool Set) { m_bMultiMatrix = Set; }
	bool GetMultiMatrix(void) { return m_bMultiMatrix; }
	void SetUseMultiMatrix(D3DXMATRIX Set) { m_UseMultiMatrix = Set; }
	D3DXMATRIX GetUseMultiMatrix(void) { return m_UseMultiMatrix; }

	// �}�b�v�ԍ��̐ݒ�
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	// �ÓI�����o�֐�
	static CListManager<CEnemy>* GetList(void); // ���X�g�擾

protected:

	void InitModel(const char* pFilename);				// ���f���̏�����

private:

	void StateManager();								//��ԍX�V
	void CollisionWall(useful::COLLISION XYZ);			//�ǂƂ̓����蔻��
	void CollisionDevilHole(useful::COLLISION XYZ);		//�f�r���z�[���Ƃ̓����蔻��
	void CollisionOut();								//�X�e�[�W�O�Ƃ̓����蔻��

	void UpdatePos(void);								// �ʒu�X�V����
	void Rot(void);										//�ړ���������
	virtual void Death(void);							// ���S����
	void MyObjCreate(void);								// �������ێ�����I�u�W�F�N�g�̐���
	void MoveSelect(void);								// �ړ������̑I��
	void SearchWall(void);								// �ǂ̃T�[�`����
	void MapSystemNumber(void);							// �ǂ̃}�X�ɑ��݂��Ă��邩�ݒ肷��

	CMapSystem::GRID m_Grid;	//�O���b�g�ԍ�

	void LoadLevelData(const char* pFilename);

	int m_nLife;				// �̗�

	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_posOld;		//�ߋ��̈ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_Objmove;		//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rot;			//����
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	int m_nActionCount;			//�s���̃J�E���g
	D3DXVECTOR3 m_size;			//�傫��
	ENEMY_TYPE m_EnemyType;		//�G�̎��

	float m_ColorA;				//�s�����x
	char m_cFileName[128];		//�t�@�C���̖��O

	E_STATE m_State;			//���
	int m_nStateCount;			//��ԊǗ��p�J�E���g
	SELECT_MOVE m_SelectMove;	//�ړ�����

	bool m_OKL;					//���ւ̐i�s��������邩�ǂ���
	bool m_OKR;					//�E�ւ̐i�s��������邩�ǂ���
	bool m_OKU;					//��ւ̐i�s��������邩�ǂ���
	bool m_OKD;					//���ւ̐i�s��������邩�ǂ���

	int m_nBugCounter;			//�G���X�^�b�N��������

	CSlowManager* m_pSlow;		// �X���[���

	//�K�w�\���ƃ��[�V�����̃|�C���^
	CModel* m_apModel[MODEL_NUM];
	CMotion* m_pMotion;
	char* m_aModelName[MODEL_NUM];
	int m_nNumModel;

	//�}�b�v�Ƃ̃}�g���b�N�X���
	bool m_bMultiMatrix;					//�}�g���b�N�X�̊|�����킹�����邩�ǂ���
	D3DXMATRIX m_UseMultiMatrix;			//�|�����킹��}�g���b�N�X

	// �ÓI�����o�ϐ�
	static CListManager<CEnemy>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CEnemy>::AIterator m_iterator; // �C�e���[�^�[

};

#endif