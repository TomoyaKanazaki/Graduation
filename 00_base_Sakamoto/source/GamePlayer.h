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
class CCrossUi;
class CPopUi;
class CPopUiBg;

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

	void InitUI();
	void DebugKey(void);

	CScore* GetScore(void) override { return m_pScore; }
	CLifeUi* GetLifeUI(void) { return m_pLifeUi; }

private:

	CMapSystem::GRID m_Grid;		// グリッドの情報
	
	CScore* m_pScore;				// スコアのポインタ
	CLifeUi* m_pLifeUi;				//体力UI
	CObjectBillboard* m_pP_NumUI;	//プレイヤー番号UI		

	CCrossUi* m_pCrossUI;
	CCrossUi* m_pCrossUIBg;

	CPopUiBg* m_pPopUIBg;			//吹き出しUI背景
	CPopUi* m_pPopUI;				//吹き出しUI
	float m_fSizePopUI;		//吹き出しUIのサイズ
	float m_fVariableSizePopUI;		//吹き出しUIのサイズ可変値
	int m_nUIFlashCount;		//UIの点滅タイミング

	D3DXVECTOR2 m_posDefCrossUI;
};

#endif