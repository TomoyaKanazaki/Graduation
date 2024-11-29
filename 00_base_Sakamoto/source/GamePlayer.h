//============================================
//
//	ゲームのプレイヤー [GamePlayer.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _GAME_PLAYER_H_
#define _GAME_PLAYER_H_

#include "player.h"
#include "useful.h"
#include "Model.h"
#include "MapSystem.h"

//前方宣言
class CSlowManager;
class CLifeUi;
class CObjectX;
class CScore;

//オブジェクトプレイヤークラス
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

private:

	CMapSystem::GRID m_Grid;		// グリッドの情報
	

};

#endif