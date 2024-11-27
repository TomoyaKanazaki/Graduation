//============================================
//
//	�`���[�g���A���̃v���C���[ [TutorialPlayer.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TUTORIAL_PLAYER_H_
#define _TUTORIAL_PLAYER_H_

#include "player.h"
#include "useful.h"
#include "Model.h"
#include "MapSystem.h"

//�O���錾
class CSlowManager;
class CLifeUi;
class CObjectX;
class CScore;

//�I�u�W�F�N�g�v���C���[�N���X
class CTutorialPlayer : public CPlayer
{
private:
	static const int ENCOUNT_MAX = 128;

public:
	CTutorialPlayer(int nPriority = 2);
	~CTutorialPlayer();

	static CTutorialPlayer* Create(int PlayNumber);
	HRESULT Init(int PlayNumber) override;
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CListManager<CTutorialPlayer>* GetList(void);	// ���X�g�擾

private:
	// �ÓI�����o�ϐ�
	static CListManager<CTutorialPlayer>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CTutorialPlayer>::AIterator m_iterator; // �C�e���[�^�[
};

#endif