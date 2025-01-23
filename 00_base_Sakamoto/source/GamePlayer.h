//============================================
//
//	�Q�[���̃v���C���[ [GamePlayer.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _GAME_PLAYER_H_
#define _GAME_PLAYER_H_

#include "player.h"
#include "useful.h"
#include "Model.h"
#include "MapSystem.h"

//�O���錾
class CSlowManager;
class CLifeUi;
class CObjectX;
class CScore;
class CCrossUi;
class CPopUi;
class CPopUiBg;

//�I�u�W�F�N�g�v���C���[�N���X
class CGamePlayer : public CPlayer
{
public:
	CGamePlayer(int nPriority = 2);
	~CGamePlayer();

	static CGamePlayer* Create(int PlayNumber);
	HRESULT Init(int PlayNumber) override;

	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Death(void) override;

	void InitUI();
	void DebugKey(void);

	CScore* GetScore(void) override { return m_pScore; }
	CLifeUi* GetLifeUI(void) { return m_pLifeUi; }

private:

	CMapSystem::GRID m_Grid;		// �O���b�h�̏��
	
	CScore* m_pScore;				// �X�R�A�̃|�C���^
	CLifeUi* m_pLifeUi;				//�̗�UI
	CObjectBillboard* m_pP_NumUI;	//�v���C���[�ԍ�UI		

	CCrossUi* m_pCrossUI;
	CCrossUi* m_pCrossUIBg;

	CPopUiBg* m_pPopUIBg;			//�����o��UI�w�i
	CPopUi* m_pPopUI;				//�����o��UI
	float m_fSizePopUI;		//�����o��UI�̃T�C�Y
	float m_fVariableSizePopUI;		//�����o��UI�̃T�C�Y�ϒl
	int m_nUIFlashCount;		//UI�̓_�Ń^�C�~���O

	D3DXVECTOR2 m_posDefCrossUI;
};

#endif