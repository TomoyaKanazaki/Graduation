//============================================
//
//	�u���b�N�S�ʂ̃}�l�[�W���[ [blockmanager.h]
//	Author:nakamura riku
//
//============================================
#ifndef _BLOCKMANAGER_H_
#define _BLOCKMANAGER_H_

#include "main.h"
#include <list>

// �O���錾
class CBlock;
class CPlayer2D;

// �u���b�N�}�l�[�W���[�N���X
class CBlockManager
{
public:

	CBlockManager();
	~CBlockManager();

	static CBlockManager* GetInstance();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Collision(CPlayer2D* pPlayer2D);

	// �擾�����E�ݒ菈��
	void SetTutorial(void);

private:
	void TitleUpdate(void);
	void GameUpdate(void);
	void TutorialUpdate(void);
	void SetStage(void);

	static CBlockManager* pBlockManager;
	std::list<CBlock*> listBlock;

	int m_nBlockCounter;
	int m_nBlockRandom = 1450;
	int m_nBlockRandomPos;
	int m_nBlockLevel;
	int m_nBlockLevelCount;
};
#endif