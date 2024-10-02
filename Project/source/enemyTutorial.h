//============================================
//
//	�`���[�g���A���G�̏��� [enemyTutorial.h]
//	Author:���� �쏟
//
//============================================
#ifndef _ENEMY_TUTORIAL_H_
#define _ENEMY_TUTORIAL_H_

#include "enemyAshigaru.h"
#include "tutorial.h"

// �`���[�g���A���̓G�N���X
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
		CTutorial::TASK task;		// �`���[�g���A���i���̔ԍ�
		bool bTaskOk;				// �^�X�N�����̗L��
	};

	Info m_info;	// ���

};

#endif