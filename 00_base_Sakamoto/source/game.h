//============================================
//
//	�Q�[����� [game.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _GAME_H_
#define _GAME_H_

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
class CGamePause;
class CDevil;
class CMask;
class CEventMovie;

class CAim;

//�V�[���N���X
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	static CGame* GetInstance();

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
	void SetEvent(bool Set) { m_bEvent = Set; }
	bool GetEvent(void) { return m_bEvent; }
	bool GetEventEnd(void) { return m_bEventEnd; }
	float GetBGColorA(void) { return m_BGColorA; }
	void AddBGColorA(float Add) { m_BGColorA += Add; }
	void SetGameEnd(bool Set) { m_bGameEnd = Set; }
	void SetGameClear(bool Set) { m_bGameClear = Set; }
	int GetBowabowa(void) { return m_nNumBowabowa; }
	void SetBowabowa(int Bowabowa) { m_nNumBowabowa = Bowabowa; }
	void AddBowabowa(int nNum) { m_nNumBowabowa+= nNum; }
	void SetDevilHoleFinish(bool Set) { m_bDevilHoleFinish = Set; }

	void SetMapField(CObjmeshField* map) { m_pMapField = map; }
	CObjmeshField* GetMapField(void) { return m_pMapField; }
	
	void DeleteMap(void);

	bool GetSlow(void) { return m_Slow; }

private:
	void UpdateLetterBox(void);
	void StageClear(int Stage);
	void LoadStageRailBlock(const char* pFilename);
	void LoadStageMapModel(const char* pFilename);

	void SetBgObjTest(void);

	// �V���O���g��
	static CGame* m_pGame;

	//�C���Q�[���p�ϐ�
	std::vector<CPlayer*> m_pPlayer;		//�v���C���[�̃|�C���^
	CDevil* m_pDevil;						//�f�r���̃|�C���^

	int m_nNumBowabowa;						//�{���{���̐�
	bool m_bDevilHoleFinish;				//�f�r���z�[�������ׂĖ��܂������ǂ���
	bool m_bGameEnd;						//�Q�[���I����Ԃ��ǂ���
	bool m_bGameClear;						//�Q�[���N���A��Ԃ��ǂ���

	//�C�x���g�p�ϐ�
	CEventMovie* m_pEventMovie;				//�C�x���g���o
	bool m_bEvent;							//�C�x���g��Ԃ��ǂ���
	bool m_bEventEnd;						//�C�x���g���I��������ǂ���

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

	CObject2D* LetterBox[2];				//�C�x���g���̔w�i�\��

	CObject2D* m_p2DSample;					//2D�|���S���̃T���v��
	CObject3D* m_p3DSample;					//3D�|���S���̃T���v��
	CObjectBillboard* m_pBillboardSample;	//Billboard�|���S���̃T���v��
	CObjectX* m_pXModelSample;				//X���f���̃T���v��
	CObjmeshField* m_pMeshFieldSample;		//���b�V���t�B�[���h�̃T���v��
	CObjmeshWall* m_pMeshWallSample;		//���b�V���E�H�[���̃T���v��
	CObjmeshCylinder* m_pMeshCylinderSample;//���b�V���V�����_�[�̃T���v��

	CMask* m_pMask;	// 2D�}�X�N
};
#endif