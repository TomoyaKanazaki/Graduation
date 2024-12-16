//============================================
//
//	チュートリアルのプレイヤー [TutorialPlayer.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TUTORIAL_PLAYER_H_
#define _TUTORIAL_PLAYER_H_

#include "player.h"
#include "useful.h"
#include "Model.h"
#include "MapSystem.h"

//前方宣言
class CSlowManager;
class CLifeUi;
class CObjectX;

//オブジェクトプレイヤークラス
class CTutorialPlayer : public CPlayer
{
public:
	CTutorialPlayer(int nPriority = 2);
	~CTutorialPlayer();

	static CTutorialPlayer* Create(int PlayNumber);
	HRESULT Init(int PlayNumber) override;
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Death(void) override;

private:
};

#endif