//============================================
//
//	ブロック全般のマネージャー [blockmanager.h]
//	Author:nakamura riku
//
//============================================
#ifndef _BLOCKMANAGER_H_
#define _BLOCKMANAGER_H_

#include "main.h"
#include <list>

// 前方宣言
class CBlock;
class CPlayer2D;

// ブロックマネージャークラス
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

	// 取得処理・設定処理
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