//============================================
//
//	ゲーム画面 [game.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _GAME_H_
#define _GAME_H_

#include "Scene.h"

//前方宣言
class CTutorialUI;
class CPlayer;
class CMap2D;
class CTutorialUI;
class CObject2D;
class CObject3D;
class CObjectBillboard;
class CObjectX;
class CObjmeshField;
class CObjmeshWall;
class CObjmeshCylinder;
class CObjmeshDome;
class CCubeBlock;
class CPause;
class CScore;
class CTimer;
class CPlayer2D;
class CGamePause;
class CDevil;
class CMask;
class CEventMovie;
class CGamePlayer;
class CObjectCharacter;

//シーンクラス
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	static CGame* GetInstance();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	CGamePlayer* GetPlayer(int Num) { return m_pPlayer.at(Num); }
	CDevil* GetDevil(void) { return m_pDevil; }
	CPause* GetPause(void) { return m_pPause; }
	CTimer* GetTime(void) { return m_pTime; }
	void SetEvent(bool Set) { m_bEvent = Set; }
	bool GetEvent(void) { return m_bEvent; }
	bool GetEventEnd(void) { return m_bEventEnd; }
	float GetBGColorA(void) { return m_BGColorA; }
	void AddBGColorA(float Add) { m_BGColorA += Add; }
	void SetGameEnd(bool Set) { m_bGameEnd = Set; }
	void SetGameClear(bool Set) { m_bGameClear = Set; }
	int GetBowabowa(void) { return m_nNumBowabowa; }
	void SetBowabowa(int Bowabowa) { m_nNumBowabowa = Bowabowa; }
	void AddBowabowa(int nNum) { m_nNumBowabowa+= nNum; }
	void SetDevilHoleFinish(bool Set) { m_bDevilHoleFinish = Set; }
	bool GetTrans(void) { return m_bTrans; }		// 遷移中かどうか

	void SetMapField(CObjmeshField* map) { m_pMapField = map; }
	CObjmeshField* GetMapField(void) { return m_pMapField; }
	
	void DeleteMap(void);
	void NextStage(void);		//ステージの進行

	bool GetSlow(void) { return m_Slow; }

private:
	void ResetStage(void);		//ステージの初期化
	void DeleteCross(void);
	void CreateBible(void);

	void UpdateLetterBox(void);
	void StageClear(int Stage);
	void LoadStageRailBlock(const std::string pFilename);

	// シングルトン
	static CGame* m_pGame;

	//インゲーム用変数
	std::vector<CGamePlayer*> m_pPlayer;		//プレイヤーのポインタ
	CDevil* m_pDevil;						//デビルのポインタ

	int m_nNumBowabowa;						//ボワボワの数
	bool m_bDevilHoleFinish;				//デビルホールがすべて埋まったかどうか
	bool m_bGameEnd;						//ゲーム終了状態かどうか
	bool m_bGameClear;						//ゲームクリア状態かどうか
	bool m_bTrans;							//遷移に入ったかどうか

	//イベント用変数
	CEventMovie* m_pEventMovie;				//イベント演出
	bool m_bEvent;							//イベント状態かどうか
	bool m_bEventEnd;						//イベントが終わったかどうか

	int m_nTutorialWave;					//チュートリアルの段階
	float m_BGColorA;						//ゲーム背景の不透明度
	D3DXVECTOR3 m_BGRot;					//背景の回転向き

	CPause* m_pPause;						//ポーズのポインタ
	CTimer* m_pTime;						//タイムのポインタ
	CObjmeshField* m_pMapField;				//マップフィールド
	bool m_Wireframe;						//ワイヤーフレームのオンオフ
	bool m_Slow;							//スロー演出のオンオフ

	int m_nTransCounter;					// 遷移するまでのカウンター

	CObject2D* LetterBox[2];				//イベント時の背景表示
};
#endif