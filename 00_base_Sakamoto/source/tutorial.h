//============================================
//
//	�`���[�g���A����� [tutorial.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "manager.h"
#include "Scene.h"

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

	static CTutorial* GetInstance();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	CPlayer* GetPlayer(int Num) { return m_pPlayer[Num]; }
	CDevil* GetDevil(void) { return m_pDevil; }
	CBoss* GetBoss(void) { return m_pBoss; }
	CPause* GetPause(void) { return m_pPause; }
	CScore* GetScore(void) { return m_pScore; }
	CEdit* GetEdit(void) { return m_pEdit; }
	CTimer* GetTime(void) { return m_pTime; }
	CObjmeshField* GetMapField(void) { return m_pMapField; }
	CObjmeshDome* GetDomeUp(void) { return m_pMeshDomeUp; }
	void SetCubeBlock(CCubeBlock* pBlock) { m_pCubeBlock = pBlock; }
	CCubeBlock* GetCubeBlock(void) { return m_pCubeBlock; }
	void SetEvent(bool Set) { m_bEvent = Set; }
	bool GetEvent(void) { return m_bEvent; }
	bool GetEventEnd(void) { return m_bEventEnd; }
	D3DXVECTOR3 GetEventPos(void) { return m_EventPos; }
	float GetBGColorA(void) { return m_BGColorA; }
	void AddBGColorA(float Add) { m_BGColorA += Add; }
	void SetStageBlock(void);
	void SetGameEnd(bool Set) { m_bGameEnd = Set; }
	void SetGameClear(bool Set) { m_bGameClear = Set; }
	int GetBowabowa(void) { return m_nNumBowabowa; }
	void SetBowabowa(int Bowabowa) { m_nNumBowabowa = Bowabowa; }
	void AddBowabowa(int nNum) { m_nNumBowabowa += nNum; }
	void SetDevilHoleFinish(bool Set) { m_bDevilHoleFinish = Set; }

	void DeleteMap(void);

	bool GetSlow(void) { return m_Slow; }
private:
	void StageClear(int Stage);
	void LoadStageRailBlock(const char* pFilename);
	void LoadStageMapModel(const char* pFilename);

	void EventUpdate(void);

	void SetBgObjTest(void);

	void Sample(void);								//�T���v���n�������Ă郈

	// �V���O���g��
	static CTutorial* m_pTutorial;

	//�C���Q�[���p�ϐ�
	CPlayer* m_pPlayer[NUM_PLAYER];			//�v���C���[�̃|�C���^
	CDevil* m_pDevil;						//�f�r���̃|�C���^

	int m_nNumBowabowa;						//�{���{���̐�
	bool m_bDevilHoleFinish;					//�f�r���z�[�������ׂĖ��܂������ǂ���
	bool m_bGameEnd;							//�Q�[���I����Ԃ��ǂ���
	bool m_bGameClear;						//�Q�[���N���A��Ԃ��ǂ���

	//�C�x���g�p�ϐ�
	bool m_bEvent;							//�C�x���g��Ԃ��ǂ���
	bool m_bEventEnd;						//�C�x���g���I��������ǂ���
	int m_nEventCount;						//�C�x���g����
	int m_nEventWave;						//�C�x���g�i�K
	int m_nEventNumber;						//�C�x���g�ԍ�
	float m_fEvectFinish;					//�Ք��荇���̃J�E���g
	float m_fEventAngle;						//�C�x���g�p�̊p�x
	float m_EventHeight;						//�C�x���g�p�|���S���̍���
	float m_NameColorA;						//�C�x���g�p�̖��O�\���̕s�����x
	D3DXVECTOR3 m_EventPos;					//�C�x���g�J�n���W

	int m_nTutorialWave;						//�`���[�g���A���̒i�K
	float m_BGColorA;						//�Q�[���w�i�̕s�����x
	D3DXVECTOR3 m_BGRot;						//�w�i�̉�]����

	CBoss* m_pBoss;							//�{�X

	CEdit* m_pEdit;							//�G�f�B�b�g���[�h�̃|�C���^
	CPause* m_pPause;						//�|�[�Y�̃|�C���^
	CScore* m_pScore;						//�X�R�A�̃|�C���^
	CTimer* m_pTime;							//�^�C���̃|�C���^
	CObjmeshDome* m_pMeshDomeUp;				//���b�V���h�[��
	CObjmeshField* m_pMapField;				//�}�b�v�t�B�[���h
	CCubeBlock* m_pCubeBlock;				//�L���[�u�u���b�N
	bool m_Wireframe;						//���C���[�t���[���̃I���I�t
	bool m_Slow;								//�X���[���o�̃I���I�t

	CObject2D* m_p2DSample;					//2D�|���S���̃T���v��
	CObject3D* m_p3DSample;					//3D�|���S���̃T���v��
	CObjectBillboard* m_pBillboardSample;	//Billboard�|���S���̃T���v��
	CObjectX* m_pXModelSample;				//X���f���̃T���v��
	CObjmeshField* m_pMeshFieldSample;		//���b�V���t�B�[���h�̃T���v��
	CObjmeshWall* m_pMeshWallSample;			//���b�V���E�H�[���̃T���v��
	CObjmeshCylinder* m_pMeshCylinderSample;	//���b�V���V�����_�[�̃T���v��

	CMask* m_pMask;	// 2D�}�X�N
};
#endif