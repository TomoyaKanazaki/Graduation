//==============================================================
//
//�����L���O����[ranking.h]
//Author:����������
//
//==============================================================
#ifndef _RANKING_H_		//���̃}�N����`������Ă��Ȃ�������
#define _RANKING_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "manager.h"

//�}�N����`
#define MAX_RANK		(5)			//�����L���O��
#define NUM_DIGIT		(8)			//�X�R�A�̌���

//�O���錾
class CNumber;			//�����̏��

//�����L���O��ʃN���X�̒�`
class CRanking : public CScene
{
public:
	//�����L���O�\����
	typedef struct
	{
		D3DXVECTOR3 pos;			//�ʒu
		int nRanking;				//�X�R�A
	} Ranking;

	CRanking();
	~CRanking();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Reset(void);
	void Set(int nNum);
	void SetSound(bool bgm) { m_bBgm = bgm; }

private:
	static CNumber *m_apNumber[NUM_DIGIT * MAX_RANK];
	static bool bReset;				//���Z�b�g������
	static int m_nNum;				//�X�R�A�̒l

	static int m_aTexU[NUM_DIGIT];

	static int m_nRankUpdate;		//�X�V�����NNo,

	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�
	bool m_bBgm;			//BGM������
	bool m_bCol;			//�F�ύX���邩
	int m_nCntTrans;		//�J�ڂ���܂ł̎���
	int m_nCntColor;		//�����L���O�̐F�ύX�Ԋu
	Ranking m_ranking[MAX_RANK];		//�����L���O�̏��

};

#endif

