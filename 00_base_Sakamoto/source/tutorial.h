//============================================
//
//	チュートリアル画面 [tutorial.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "Scene.h"
#include "MapSystem.h"

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
class CTutorialPause;
class CDevil;
class CMask;
class CEventMovie;
class CTutorialCheck;

class CAim;

//シーンクラス
class CTutorial : public CScene
{
public:
	enum CHECKTYPE
	{// チュートリアルの項目
		TYPE_NONE = 0,	// 種類無し
		TYPE_MOVE,		// 移動
		TYPE_CROSS,		// 十字架
		TYPE_ATTACK,	// 攻撃
		TYPE_BOWABOWA,	// ボワボワ
		TYPE_BIBLE,		// 聖書
		TYPE_DEVILHOLE,	// デビルホール
		TYPE_MAX
	};

	CTutorial();
	~CTutorial();

	static CTutorial* GetInstance();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	CPlayer* GetPlayer(int Num) { return m_pPlayer.at(Num); }
	CDevil* GetDevil(void) { return m_pDevil; }
	CPause* GetPause(void) { return m_pPause; }
	CTimer* GetTime(void) { return m_pTime; }
	CObjmeshDome* GetDomeUp(void) { return m_pMeshDomeUp; }
	void SetCubeBlock(CCubeBlock* pBlock) { m_pCubeBlock = pBlock; }
	CCubeBlock* GetCubeBlock(void) { return m_pCubeBlock; }
	float GetBGColorA(void) { return m_BGColorA; }
	void AddBGColorA(float Add) { m_BGColorA += Add; }
	void SetGameEnd(bool Set) { m_bTutorialEnd = Set; }
	void SetGameClear(bool Set) { m_bTutorialClear = Set; }
	int GetBowabowa(void) { return m_nNumBowabowa; }
	void SetBowabowa(int Bowabowa) { m_nNumBowabowa = Bowabowa; }
	void AddBowabowa(int nNum) { m_nNumBowabowa += nNum; }
	void SetDevilHoleFinish(bool Set) { m_bDevilHoleFinish = Set; }

	void SetMapField(CObjmeshField* map) { m_pMapField = map; }
	CObjmeshField* GetMapField(void) { return m_pMapField; }

	void DeleteMap(void);

	bool GetSlow(void) { return m_Slow; }

private:
	void NextStage(void);
	void DeleteCross(void);
	void CreateBible(void);

	void LoadStageRailBlock(const char* pFilename);
	void LoadStageMapModel(const char* pFilename);

	void SetBgObjTest(void);

	// シングルトン
	static CTutorial* m_pTutorial;

	//インゲーム用変数
	std::vector<CPlayer*> m_pPlayer;		//プレイヤーのポインタ
	CDevil* m_pDevil;						//デビルのポインタ

	int m_nNumCross;						// 十字架の総数
	int m_nNumBowabowa;						// ボワボワの総数
	int m_nNumEnemy;						// 敵の総数
	int m_nNumBible;						// 聖書の総数
	std::vector<CMapSystem::GRID> m_gridPlayer;	// プレイヤーの初期位置
	int m_nTutorialWave;					// チュートリアルの段階

	float m_BGColorA;						// ゲーム背景の不透明度

	bool m_bDevilHoleFinish;				// デビルホールがすべて埋まったかどうか
	bool m_bTutorialEnd;					// チュートリアル終了状態かどうか
	bool m_bTutorialClear;					// チュートリアルクリア状態かどうか
	bool m_Wireframe;						// ワイヤーフレームのオンオフ
	bool m_Slow;							// スロー演出のオンオフ
	bool m_bCheck[TYPE_MAX];

	D3DXVECTOR3 m_BGRot;					// 背景の回転向き
	D3DXVECTOR3 InitPlayerPos;				// プレイヤーの初期位置

	CPause* m_pPause;						// ポーズのポインタ
	CTimer* m_pTime;						// タイムのポインタ
	CObjmeshDome* m_pMeshDomeUp;			// メッシュドーム
	CObjmeshField* m_pMapField;				// マップフィールド
	CCubeBlock* m_pCubeBlock;				// キューブブロック
	CObject2D* m_pTutorialGuide;			// チュートリアルガイド
};

#endif
