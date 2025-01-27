//============================================
//
//	�`���[�g���A����� [tutorial.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "Scene.h"
#include "MapSystem.h"
#include "tutorialUi.h"

//�O���錾
class CTutorialUI;
class CPlayer;
class CMap2D;
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
class CPlayer2D;
class CTutorialPause;
class CDevil;
class CMask;
class CEventMovie;
class CTutorialCheck;
class CTutorialUi;

class CAim;

//�V�[���N���X
class CTutorial : public CScene
{
public:
	enum CHECKTYPE
	{// �`���[�g���A���̍���
		TYPE_MOVE = 0,	// �ړ�
		TYPE_CROSS,		// �\����
		TYPE_BOWABOWA,	// �{���{��
		TYPE_ATTACK,	// �U��
		TYPE_BIBLE,		// ����
		TYPE_DEVILHOLE,	// �f�r���z�[��
		TYPE_MAX
	};

	CTutorial();
	~CTutorial();

	static CTutorial* GetInstance();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	CPlayer* GetPlayer(int Num) { return m_pPlayer.at(Num); }
	CDevil* GetDevil(void) { return m_pDevil; }
	CPause* GetPause(void) { return m_pPause; }
	CTimer* GetTime(void) { return m_pTime; }
	float GetBGColorA(void) { return m_BGColorA; }
	void AddBGColorA(float Add) { m_BGColorA += Add; }
	void SetGameEnd(bool Set) { m_bTutorialEnd = Set; }
	void SetGameClear(bool Set) { m_bTutorialClear = Set; }
	int GetBowabowa(void) { return m_nNumBowabowa; }
	void SetBowabowa(int Bowabowa) { m_nNumBowabowa = Bowabowa; }
	void AddBowabowa(int nNum) { m_nNumBowabowa += nNum; }
	void SetDevilHoleFinish(bool Set) { m_bDevilHoleFinish = Set; }

	void SetMapField(CObjmeshField* map) { m_pMapField = map; }
	CObjmeshField* GetMapField(void) { return m_pMapField; }

	void DeleteMap(void);

	bool GetSlow(void) { return m_Slow; }

private:
	void NextStage(void);
	void DeleteCross(void);
	void CreateBible(void);

	void LoadStageRailBlock(const std::string pFilename);

	void SetBgObjTest(void);

	void SetUIAlpha(const CHECKTYPE type, const float fAlpha = 1.0f);

	// �V���O���g��
	static CTutorial* m_pTutorial;

	//�C���Q�[���p�ϐ�
	std::vector<CPlayer*> m_pPlayer;		//�v���C���[�̃|�C���^
	CDevil* m_pDevil;						//�f�r���̃|�C���^

	int m_nNumCross;						// �\���˂̑���
	int m_nNumBowabowa;						// �{���{���̑���
	int m_nNumEnemy;						// �G�̑���
	int m_nNumBible;						// �����̑���
	std::vector<CMapSystem::GRID> m_gridPlayer;	// �v���C���[�̏����ʒu
	int m_nTutorialWave;					// �`���[�g���A���̒i�K

	float m_BGColorA;						// �Q�[���w�i�̕s�����x

	bool m_bDevilHoleFinish;				// �f�r���z�[�������ׂĖ��܂������ǂ���
	bool m_bTutorialEnd;					// �`���[�g���A���I����Ԃ��ǂ���
	bool m_bTutorialClear;					// �`���[�g���A���N���A��Ԃ��ǂ���
	bool m_Wireframe;						// ���C���[�t���[���̃I���I�t
	bool m_Slow;							// �X���[���o�̃I���I�t
	bool m_bCheck[TYPE_MAX];				// �`�F�b�N�}�[�J�[�\��

	D3DXVECTOR3 m_BGRot;					// �w�i�̉�]����
	D3DXVECTOR3 InitPlayerPos;				// �v���C���[�̏����ʒu

	CPause* m_pPause;						// �|�[�Y�̃|�C���^
	CTimer* m_pTime;						// �^�C���̃|�C���^
	CObjmeshField* m_pMapField;				// �}�b�v�t�B�[���h
	CTutorialUi* m_pTutorialUI;
	std::vector<CTutorialUi*> m_pText;		// �`���[�g���A���e�L�X�g�̃|�C���^
	bool m_bSound[TYPE_MAX];
};

#endif
