//============================================
//
//	�I�u�W�F�N�g [object.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

class CObject
{
public:

	// �I�u�W�F�N�g�̎��
	enum OBJECT_TYPE
	{
		TYPE_NONE = 0,			// �g�p�֎~
		TYPE_OBJECT2D,			// �I�u�W�F�N�g2D
		TYPE_PLAYER3D,			// 3D�̃v���C���[
		TYPE_ENEMY3D,			// 3D�̓G
		TYPE_FIRE,				// ��
		TYPE_TIME,				// ����
		TYPE_NUMBER,			// ����
		TYPE_TUTORIALUI,		// �`���[�g���A��UI
		TYPE_BLOCK,				// �u���b�N
		TYPE_PLAYEREFFECT,		// �v���C���[�̃G�t�F�N�g
		TYPE_CUBEBLOCK,			// �L���[�u
		TYPE_CUBEEFFECT,		// �L���[�u�̃G�t�F�N�g
		TYPE_2DUI,				// 2D��UI
		TYPE_MAPMODEL,			// �}�b�v��̃��f��
		TYPE_OBJMESHFIELD,		// �t�B�[���h
		TYPE_DEVIL,				// �f�r��
		TYPE_BOWABOWA,			// �{���{��
		TYPE_CROSS,				// �N���X
		TYPE_SCROLLALLOW,		// ���
		TYPE_DEVILHOLE,			// �f�r���z�[��
		TYPE_DEVILKEY,			// �f�r���z�[���ɂ͂߂���
		TYPE_BIBLE,				// ����
		TYPE_RAILBLOCK,			// �����u���b�N
		TYPE_RAIL,				// ���[��
		TYPE_SOFTCREAM,			// �\�t�g�N���[��
		TYPE_FRIEDEGG,			// �ڋʏĂ�
		TYPE_MAX,
	};

	//�����蔻��̎g�p��
	enum COLLISION
	{
		COLLISION_X = 0,
		COLLISION_Y,
		COLLISION_Z,
		COLLISION_MAX,
	};
	
	CObject(int nPriority = 3);
	virtual ~CObject();

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(int nCnt);
	static void MultiTargetDraw(void);
	static void ResetObjectMap(void);
	static void ReleaseEnemy(void);
	static void DeleteBlock(void);

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static CObject *GetTop(int nPriority) { return m_pTop[nPriority]; }
	CObject *GetNext(void) { return m_pNext; }
	void SetDeathFlag(bool Set) { m_bDeath = Set; }
	bool GetDeathFlag(void) { return m_bDeath; }

	void SetType(OBJECT_TYPE type) { m_type = type; }
	OBJECT_TYPE GetType(void) { return m_type; }
	virtual D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	virtual D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	virtual void HitDamage(float Damage) {}
	virtual void Hit(D3DXVECTOR3 pos, int Damage) {}
	virtual float GetLife(void) { return 0; }
	virtual float GetWidth(void) { return 0.0f; }
	virtual float GetHeight(void) { return 0.0f; }
	virtual D3DXVECTOR3 GetSize(void) { return INITVECTOR3; }
	static void SetLevelStop(bool Set) { m_bLevelStop = Set; }
	virtual void SetLevelUI(bool Set) { m_bLevelUI = Set; }
	void SetAppear(bool Set) { m_Appear = Set; }
	bool GetAppear(void) { return m_Appear; }
	virtual bool CollisionPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, bool* bJumpMove, bool* bHit, bool b_Jamp) { return false; }
	virtual bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ) { return false; }
	virtual bool CollisionBlockUp(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ) { return false; }
	virtual bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fWidth, bool *bJumpMove, bool *bHit, bool b_Jamp) { return false; }
	virtual bool CollisionDamageBlock(D3DXVECTOR3 pPos, D3DXVECTOR3 Size, float* Damage) { return false; }
	virtual void SetNULL(void) {}
	virtual D3DXVECTOR3* GetScreenPos(void) { return nullptr; }
	static void DebugKey();

protected:
	void Release(void);

private:

	static CObject *m_pTop[PRIORITY_MAX];	//�擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CObject *m_pCur[PRIORITY_MAX];	//�Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	CObject *m_pPrev;						//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject *m_pNext;						//���̃I�u�W�F�N�g�ւ̃|�C���^
	bool m_bDeath;							//���S�t���O
	static int m_nNumAll;					//�I�u�W�F�N�g����
	int m_Priority;							//�`��̗D�揇��
	static bool m_bLevelStop;				//���x���A�b�v���̃|�[�Y
	bool m_bLevelUI;						//���x���A�b�v����UI
	bool m_Appear;							//�\����Ԃ��ǂ���
	OBJECT_TYPE m_type;							//�I�u�W�F�N�g�̎��
};

#endif