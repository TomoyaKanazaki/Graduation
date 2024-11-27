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
class CScore;

//オブジェクトプレイヤークラス
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

	// 静的メンバ関数
	static CListManager<CTutorialPlayer>* GetList(void);	// リスト取得

private:
	// 静的メンバ変数
	static CListManager<CTutorialPlayer>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CTutorialPlayer>::AIterator m_iterator; // イテレーター
};

#endif