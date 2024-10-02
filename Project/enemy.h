//============================================
//
//	�G�̏��� [enemy.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "object.h"
#include "useful.h"

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

	//�G�̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_BATTLE,		//�퓬
		STATE_SEATCH,		//�T��
		STATE_MAX,			//�ő�
	};

	enum ENEMY_TYPE
	{
		ENEMY_ASHIGARU = 0,
		ENEMY_BOWMAN,
		ENEMY_SAMURAI,
		ENEMY_BOSS,
		ENEMY_MAX,
	};

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CEnemy* Create(char* aModelName);
	HRESULT Init(void);
	virtual void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	virtual void HitDamage(float fDamage);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3* GetScreenPos(void) { return &m_ScreenPos[0]; }
	int GetSeatchCount(void) { return m_nSeatchCount; }
	void SetSeatchCount(int nCnt) { m_nSeatchCount = nCnt; }
	D3DXVECTOR3* GetSeatchPos(void) { return &m_SeatchPos[0]; }
	void SetSeatchPos(D3DXVECTOR3 pos, int nCnt) { m_SeatchPos[nCnt] = pos; }
	bool GetSeatchLoop(void) { return m_bSeatchLoop; }
	void SetSeatchLoop(bool Loop) { m_bSeatchLoop = Loop; }
	void SetState(STATE State) { m_State = State; }
	STATE GetState(void) { return m_State; }
	void SetDeath(bool Set) { m_bDeath = Set; }
	bool GetDeath(void) { return m_bDeath; }
	void SetLife(float Set) { m_fLife = Set; }
	float GetLife(void) { return m_fLife; }
	void SetAttack(bool Set) { m_bAttack = Set; }
	bool GetAttack(void) { return m_bAttack; }
	void SetSearchDistance(float Distance) { m_SearchDistance = Distance; }
	const char* GetTextModelName(void) { return &m_cFileName[0]; }

	CModel** GetLevelModel(void) { return &m_apModel[0]; }
	CModel* GetLevelModel(int nIdx) { return m_apModel[nIdx]; }

	CMotion* GetMotion(void){
		if (m_pMotion != nullptr) { return m_pMotion; }
		else { return nullptr; }}

	int GetNumModel(void) { return m_nNumModel; }
	void SetEnemyType(ENEMY_TYPE Type) { m_EnemyType = Type; }
	ENEMY_TYPE GetEnemyType(void) { return m_EnemyType; }

protected:

	void InitModel(const char* pFilename);				// ���f���̏�����

	bool DistancePlayer(void);							// �v���C���[�Ɗԍ�������鏈��
	void CollisionPush(void);							// �����ȊO�̓G�������o������
	void SearchPlayer(void);							// �v���C���[�����G���鏈��
	void CreateWeaponEffect(int ModelNumber);							// ����̎c���G�t�F�N�g�𐶐�����
	bool SortObject(D3DXVECTOR3 pos);					// �I�u�W�F�N�g�Ƃ̃\�[�g����

private:

	virtual void BattleStateManager(void){;};			// �퓬��ԊǗ�
	virtual void SearchStateManager(void){;};			// �T����ԊǗ�
	virtual void UpdateState();							// ��ԍX�V
	void CollisionWall(useful::COLLISION XYZ);	//�ǂƂ̓����蔻��

	void UpdatePos(void);								// �ʒu�X�V����
	virtual void Death(void);							// ���S����
	void ScreenCollision(void);							// �X�N���[���ʒu�̓����蔻��
	void MyObjCreate(void);								// �������ێ�����I�u�W�F�N�g�̐���


	void LoadLevelData(const char* pFilename);

	D3DXVECTOR3 m_pos;							//�ʒu
	D3DXVECTOR3 m_posOld;						//�ߋ��̈ʒu
	D3DXVECTOR3 m_move;							//�ړ���
	D3DXVECTOR3 m_Objmove;						//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rot;							//����
	D3DXVECTOR3 m_AtkPos;						//�U���ʒu
	D3DXVECTOR3 m_SeatchPos[SEARCH_POS_NUM];	//�T���ʒu
	int m_nSeatchCount;							//�T���i�K�̃J�E���g
	bool m_bSeatchLoop;							//�T�������[�v���邩�ǂ���
	D3DXMATRIX m_mtxWorld;						//���[���h�}�g���b�N�X
	int m_nActionCount;							//�s���̃J�E���g
	bool m_nAttackHit;							//�U���������������ǂ���
	D3DXVECTOR3 m_size;							//�傫��
	STATE m_State;								//���
	int m_nStateCount;							//��ԊǗ��p�J�E���g
	bool m_bJump;								//�W�����v��Ԃ��ǂ���
	bool m_bDeath;								//���S��Ԃ��ǂ���
	bool m_bAttack;								//�U����Ԃ��ǂ���
	ENEMY_TYPE m_EnemyType;						//�G�̎��
	float m_SearchDistance;						//���G����
	float m_fLife;								//�̗�

	D3DXVECTOR3 m_ScreenPos[SCREEN_POS_NUM];	//�X�N���[�����W
	float m_CollisionRot;						//�����蔻��p�̌���

	float m_ColorA;								//�s�����x
	char m_cFileName[128];						//�t�@�C���̖��O

	CSlowManager* m_pSlow;						// �X���[���

	//�K�w�\���ƃ��[�V�����̃|�C���^
	CModel* m_apModel[MODEL_NUM];
	CMotion* m_pMotion;
	char* m_aModelName[MODEL_NUM];
	int m_nNumModel;
};

#endif