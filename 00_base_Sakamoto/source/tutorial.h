//============================================
//
//	�Q�[����� [game.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "Scene.h"

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

	CPlayer* GetPlayer(int Num) { return m_pPlayer.at(Num); }
	CDevil* GetDevil(void) { return m_pDevil; }
	CPause* GetPause(void) { return m_pPause; }
	CTimer* GetTime(void) { return m_pTime; }
	CObjmeshDome* GetDomeUp(void) { return m_pMeshDomeUp; }
	void SetCubeBlock(CCubeBlock* pBlock) { m_pCubeBlock = pBlock; }
	CCubeBlock* GetCubeBlock(void) { return m_pCubeBlock; }
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

	void LoadStageRailBlock(const char* pFilename);
	void LoadStageMapModel(const char* pFilename);

	void SetBgObjTest(void);

	// �V���O���g��
	static CTutorial* m_pTutorial;

	//�C���Q�[���p�ϐ�
	std::vector<CPlayer*> m_pPlayer;		//�v���C���[�̃|�C���^
	CDevil* m_pDevil;						//�f�r���̃|�C���^

	int m_nNumBowabowa;						//�{���{���̐�
	bool m_bDevilHoleFinish;				//�f�r���z�[�������ׂĖ��܂������ǂ���
	bool m_bTutorialEnd;					//�`���[�g���A���I����Ԃ��ǂ���
	bool m_bTutorialClear;					//�`���[�g���A���N���A��Ԃ��ǂ���

	int m_nTutorialWave;					//�`���[�g���A���̒i�K
	float m_BGColorA;						//�Q�[���w�i�̕s�����x
	D3DXVECTOR3 m_BGRot;					//�w�i�̉�]����

	CPause* m_pPause;						//�|�[�Y�̃|�C���^
	CTimer* m_pTime;						//�^�C���̃|�C���^
	CObjmeshDome* m_pMeshDomeUp;			//���b�V���h�[��
	CObjmeshField* m_pMapField;				//�}�b�v�t�B�[���h
	CCubeBlock* m_pCubeBlock;				//�L���[�u�u���b�N
	bool m_Wireframe;						//���C���[�t���[���̃I���I�t
	bool m_Slow;							//�X���[���o�̃I���I�t

	CObject2D* m_pTutorialGuide;	// �`���[�g���A���K�C�h

	CMask* m_pPlayerMask;	// �v���C���[�}�X�N
	CMask* m_pItemMask;		// �A�C�e���}�X�N
	CMask* m_pMedamanMask;	// �G�}�X�N

};
#endif