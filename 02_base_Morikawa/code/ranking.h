//========================================
//
//  ���U���g [ranking.h]
//  Author�F���n�Ή�
//
//========================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "manager.h"

//========================================
// �O���錾
//========================================
class CObject2D;
class CRankingScore;
class CResultScore;

//========================================
// �����L���O�N���X
//========================================
class CRanking : public CScene
{
public:
	CRanking();	//�R���X�g���N�^
	~CRanking();	//�f�X�g���N�^

	// �����o�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CRanking* Create(float nowScore);

private:

	//=============================
	// �����o�֐�
	//=============================
	void Save();	// �Z�[�u
	void Load();	// ���[�h
	void Sort();	// �\�[�g

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fNowScore;		// ����̃X�R�A
	float m_fRanking[3];	// �����L���O
	bool m_bNewRecord;		// �j���[���R�[�h
	CObject2D* m_pRankNum[3];
	CResultScore* m_pScore[3];
};

#endif
