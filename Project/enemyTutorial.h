//============================================
//
//	チュートリアル敵の処理 [enemyTutorial.h]
//	Author:酒井 南勝
//
//============================================
#ifndef _ENEMY_TUTORIAL_H_
#define _ENEMY_TUTORIAL_H_

#include "enemyAshigaru.h"
#include "tutorial.h"

// チュートリアルの敵クラス
class CEnemyTutorial : public CEnemyAshigaru
{
public:
	CEnemyTutorial(int nPriority = 2);
	~CEnemyTutorial();

	static CEnemyTutorial* Create(const char* pFilename);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	virtual void HitDamage(float fDamage);

	void SetTask(CTutorial::TASK task) { m_info.task = task; }
	CTutorial::TASK GetTask(void) { return m_info.task; }

private:

	struct Info
	{
		CTutorial::TASK task;		// チュートリアル進捗の番号
		bool bTaskOk;				// タスク完了の有無
	};

	Info m_info;	// 情報

};

#endif