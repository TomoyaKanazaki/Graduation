//============================================
//
//	�`���[�g���A����� [tutorial.h]
//	Author:���� �쏟
//
//============================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "manager.h"

//�O���錾
class CPlayer;
class CMap2D;
class CEdit;
class CPause;

class CAim;

class CCubeBlock;
class CBoss;

class CObject2D;
class CObject3D;
class CObjGauge2D;

//�V�[���N���X
class CTutorial : public CScene
{
public:

	enum TASK
	{
		TASK_NONE = 0,	// �Ȃ�
		TASK_SLASH,		// �a��
		TASK_MOVE_ROT,	// �ړ��E���_�ړ�
		TASK_SYURIKENN,	// �藠��
		TASK_EVASION,	// ���
		TASK_ENEMY,		// �G
		TASK_ULTIMATE,	// �p
		TASK_BRIDGE,	// ��
		TASK_MAX
	};

	CTutorial();
	~CTutorial();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CPause* GetPause(void) { return m_pPause; }
	static CEdit* GetEdit(void) { return m_pEdit; }
	static void SetEvent(bool Set) { m_bEvent = Set; }
	static bool GetEvent(void) { return m_bEvent; }
	static bool GetEventEnd(void) { return m_bEventEnd; }
	static D3DXVECTOR3 GetEventPos(void) { return m_EventPos; }
	static float GetBGColorA(void) { return m_BGColorA; }
	static void AddBGColorA(float Add) { m_BGColorA += Add; }
	static void SetStageBlock(void);
	static void SetGameEnd(bool Set) { m_bGameEnd = Set; }
	static void DispUI(bool Set);

	static CAim* GetAim(void) { return m_pAim; }
	static bool GetSlow(void) { return m_Slow; }

private:

	struct InfoEnemy
	{
		TASK spwanTask;
		D3DXVECTOR3 pos[32];
		int nPosMax;
		bool bLoop;
		int nType;
		TASK task;
	};

	static void LoadMap(void);

	static void LoadPlayerAutoPos(const char* pFilename);
	static void LoadStageBlock(const char* pFilename);
	static void LoadStageMapModel(const char* pFilename);
	static void LoadEnemy(const char* pFilename);
	static void LoadStair(const char* pFilename);
	static void LoadGimmick(const char* pFilename);

	static void SetTaskEnemyCreate(void);

	static void SkipUICreate(void);

	static int m_nEnemyNum;					//���݂̓G�̐�
	static bool m_bGameEnd;					//�Q�[���I����Ԃ��ǂ���
	static bool m_bEvent;					//�C�x���g��Ԃ��ǂ���
	static bool m_bEventEnd;				//�C�x���g���I��������ǂ���
	static int m_nEventCount;				//�C�x���g����
	static int m_nTutorialWave;				//�`���[�g���A���̒i�K
	static float m_EventHeight;				//�C�x���g�p�|���S���̍���
	static float m_NameColorA;				//�C�x���g�p�̖��O�\���̕s�����x
	static float m_BGColorA;				//�Q�[���w�i�̕s�����x
	static D3DXVECTOR3 m_EventPos;			//�C�x���g�J�n���W
	static D3DXVECTOR3 m_BGRot;				//�w�i�̉�]����

	static CPlayer* m_pPlayer;				//�v���C���[�̃|�C���^
	static CEdit* m_pEdit;					//�G�f�B�b�g���[�h�̃|�C���^
	static CPause* m_pPause;				//�|�[�Y�̃|�C���^

	static CCubeBlock* m_pCubeBlock;		// �L���[�u�u���b�N�̃|�C���^
	static CBoss* m_pBoss;					// �{�X�N���X

	static bool m_Wireframe;				//���C���[�t���[���̃I���I�t
	static bool m_Slow;						//�X���[���o�̃I���I�t

	static CAim* m_pAim;					// �Ə�

	static CObject2D* m_pSkipUi;			// �X�L�b�v����UI
	static CObject3D* m_pRiver;				// ��UI
	static CObjGauge2D* m_pSkipGage;		// �X�L�b�v�Q�[�W
	static float m_fCntSkip;				// �X�L�b�v�J�E���g

	static bool m_abIsTask[TASK_MAX];		// �^�X�N�̐i����

	static InfoEnemy m_infoEnemy;			// �G���
};
#endif