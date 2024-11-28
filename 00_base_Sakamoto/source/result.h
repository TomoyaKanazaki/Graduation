//============================================
//
//	���U���g��� [result.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "manager.h"
#include "Scene.h"

//�O���錾
class CObject2D;
class CRanking;
class CBreak_Block3D;
class CNumber;

//�V�[���N���X
class CResult : public CScene
{
public:
	CResult();
	~CResult();

	static CResult* GetInstance();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetApprear(bool Set) { m_Appear = Set; }
	bool GetApprear(void) { return m_Appear; }

private:

	void AutoFade();			// �����J�ڏ���

	// �V���O���g��
	static CResult* m_pResult;

	bool m_Appear;

	CObject2D* m_pBg;			// �w�i
	CObject2D* m_pScoreTex;		// �X�R�A�e�L�X�g
	CObject2D* m_p1PTex;		// [1P]�e�L�X�g
	CObject2D* m_p2PTex;		// [2P]�e�L�X�g

	CNumber* m_apScore[6];		// ����̃X�R�A(����)
	CNumber* m_ap1P[6];			// 1P�̍���̃X�R�A(����)
	CNumber* m_ap2P[6];			// 2P�̍���̃X�R�A(����)
	CRanking* m_pLifeRanking;	// �����L���O

	int m_AddScoreCount;
	int m_ScoreData;
	int m_1PData;
	int m_2PData;
	int m_RankData;

	float m_fTimer; // �����J�ڃ^�C�}�[
};
#endif