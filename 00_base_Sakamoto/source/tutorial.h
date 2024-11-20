//============================================
//
//	�`���[�g���A����� [tutorial.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "manager.h"

#define NUM_PLAYER (2)

//�O���錾
class CTutorialUI;
class CPlayer;
class CMap2D;
class CEdit;
class CTutorialUI;
class CObject2D;
class CObject3D;
class CObjectBillboard;
class CObjectX;
class CObjmeshField;
class CObjmeshWall;
class CObjmeshCylinder;
class CObjmeshDome;
class CCubeBlock;
class CPause;
class CScore;
class CTimer;
class CBoss;
class CPlayer2D;
class CTutorialPause;
class CDevil;
class CMask;

class CAim;

//�V�[���N���X
class CTutorial : public CScene
{
public:
	CTutorial();
	~CTutorial();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CPlayer* GetPlayer(int Num) { return m_pPlayer[Num]; }
	static CDevil* GetDevil(void) { return m_pDevil; }
	static CBoss* GetBoss(void) { return m_pBoss; }
	static CPause* GetPause(void) { return m_pPause; }
	static CScore* GetScore(void) { return m_pScore; }
	static CEdit* GetEdit(void) { return m_pEdit; }
	static CTimer* GetTime(void) { return m_pTime; }
	static CObjmeshField* GetMapField(void) { return m_pMapField; }
	static CObjmeshDome* GetDomeUp(void) { return m_pMeshDomeUp; }
	static void SetCubeBlock(CCubeBlock* pBlock) { m_pCubeBlock = pBlock; }
	static CCubeBlock* GetCubeBlock(void) { return m_pCubeBlock; }
	static void SetEvent(bool Set) { m_bEvent = Set; }
	static bool GetEvent(void) { return m_bEvent; }
	static bool GetEventEnd(void) { return m_bEventEnd; }
	static D3DXVECTOR3 GetEventPos(void) { return m_EventPos; }
	static float GetBGColorA(void) { return m_BGColorA; }
	static void AddBGColorA(float Add) { m_BGColorA += Add; }
	static void SetStageBlock(void);
	static void SetGameEnd(bool Set) { m_bGameEnd = Set; }
	static void SetGameClear(bool Set) { m_bGameClear = Set; }
	static int GetBowabowa(void) { return m_nNumBowabowa; }
	static void SetBowabowa(int Bowabowa) { m_nNumBowabowa = Bowabowa; }
	static void AddBowabowa(int nNum) { m_nNumBowabowa += nNum; }
	static void SetDevilHoleFinish(bool Set) { m_bDevilHoleFinish = Set; }

	void DeleteMap(void);

	static bool GetSlow(void) { return m_Slow; }
private:
	static void StageClear(int Stage);
	static void LoadStageRailBlock(const char* pFilename);
	static void LoadStageMapModel(const char* pFilename);

	static void EventUpdate(void);

	static void SetBgObjTest(void);

	void Sample(void);								//�T���v���n�������Ă郈

	//�C���Q�[���p�ϐ�
	static CPlayer* m_pPlayer[NUM_PLAYER];			//�v���C���[�̃|�C���^
	static CDevil* m_pDevil;						//�f�r���̃|�C���^

	static int m_nNumBowabowa;						//�{���{���̐�
	static bool m_bDevilHoleFinish;					//�f�r���z�[�������ׂĖ��܂������ǂ���
	static bool m_bGameEnd;							//�Q�[���I����Ԃ��ǂ���
	static bool m_bGameClear;						//�Q�[���N���A��Ԃ��ǂ���

	//�C�x���g�p�ϐ�
	static bool m_bEvent;							//�C�x���g��Ԃ��ǂ���
	static bool m_bEventEnd;						//�C�x���g���I��������ǂ���
	static int m_nEventCount;						//�C�x���g����
	static int m_nEventWave;						//�C�x���g�i�K
	static int m_nEventNumber;						//�C�x���g�ԍ�
	static float m_fEvectFinish;					//�Ք��荇���̃J�E���g
	static float m_fEventAngle;						//�C�x���g�p�̊p�x
	static float m_EventHeight;						//�C�x���g�p�|���S���̍���
	static float m_NameColorA;						//�C�x���g�p�̖��O�\���̕s�����x
	static D3DXVECTOR3 m_EventPos;					//�C�x���g�J�n���W

	static int m_nTutorialWave;						//�`���[�g���A���̒i�K
	static float m_BGColorA;						//�Q�[���w�i�̕s�����x
	static D3DXVECTOR3 m_BGRot;						//�w�i�̉�]����

	static CBoss* m_pBoss;							//�{�X

	static CEdit* m_pEdit;							//�G�f�B�b�g���[�h�̃|�C���^
	static CPause* m_pPause;						//�|�[�Y�̃|�C���^
	static CScore* m_pScore;						//�X�R�A�̃|�C���^
	static CTimer* m_pTime;							//�^�C���̃|�C���^
	static CObjmeshDome* m_pMeshDomeUp;				//���b�V���h�[��
	static CObjmeshField* m_pMapField;				//�}�b�v�t�B�[���h
	static CCubeBlock* m_pCubeBlock;				//�L���[�u�u���b�N
	static bool m_Wireframe;						//���C���[�t���[���̃I���I�t
	static bool m_Slow;								//�X���[���o�̃I���I�t

	static CObject2D* m_p2DSample;					//2D�|���S���̃T���v��
	static CObject3D* m_p3DSample;					//3D�|���S���̃T���v��
	static CObjectBillboard* m_pBillboardSample;	//Billboard�|���S���̃T���v��
	static CObjectX* m_pXModelSample;				//X���f���̃T���v��
	static CObjmeshField* m_pMeshFieldSample;		//���b�V���t�B�[���h�̃T���v��
	static CObjmeshWall* m_pMeshWallSample;			//���b�V���E�H�[���̃T���v��
	static CObjmeshCylinder* m_pMeshCylinderSample;	//���b�V���V�����_�[�̃T���v��

	static CMask* m_pMask;	// 2D�}�X�N
};
#endif