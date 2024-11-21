//============================================
//
//	チュートリアル画面 [tutorial.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "manager.h"
#include "Scene.h"

#define NUM_PLAYER (2)

//前方宣言
class CTutorialUI;
class CPlayer;
class CMap2D;
class CEdit;
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
class CBoss;
class CPlayer2D;
class CTutorialPause;
class CDevil;
class CMask;

class CAim;

//シーンクラス
class CTutorial : public CScene
{
public:
	CTutorial();
	~CTutorial();

	static CTutorial* GetInstance();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	CPlayer* GetPlayer(int Num) { return m_pPlayer[Num]; }
	CDevil* GetDevil(void) { return m_pDevil; }
	CBoss* GetBoss(void) { return m_pBoss; }
	CPause* GetPause(void) { return m_pPause; }
	CScore* GetScore(void) { return m_pScore; }
	CEdit* GetEdit(void) { return m_pEdit; }
	CTimer* GetTime(void) { return m_pTime; }
	CObjmeshField* GetMapField(void) { return m_pMapField; }
	CObjmeshDome* GetDomeUp(void) { return m_pMeshDomeUp; }
	void SetCubeBlock(CCubeBlock* pBlock) { m_pCubeBlock = pBlock; }
	CCubeBlock* GetCubeBlock(void) { return m_pCubeBlock; }
	void SetEvent(bool Set) { m_bEvent = Set; }
	bool GetEvent(void) { return m_bEvent; }
	bool GetEventEnd(void) { return m_bEventEnd; }
	D3DXVECTOR3 GetEventPos(void) { return m_EventPos; }
	float GetBGColorA(void) { return m_BGColorA; }
	void AddBGColorA(float Add) { m_BGColorA += Add; }
	void SetStageBlock(void);
	void SetGameEnd(bool Set) { m_bGameEnd = Set; }
	void SetGameClear(bool Set) { m_bGameClear = Set; }
	int GetBowabowa(void) { return m_nNumBowabowa; }
	void SetBowabowa(int Bowabowa) { m_nNumBowabowa = Bowabowa; }
	void AddBowabowa(int nNum) { m_nNumBowabowa += nNum; }
	void SetDevilHoleFinish(bool Set) { m_bDevilHoleFinish = Set; }

	void DeleteMap(void);

	bool GetSlow(void) { return m_Slow; }
private:
	void StageClear(int Stage);
	void LoadStageRailBlock(const char* pFilename);
	void LoadStageMapModel(const char* pFilename);

	void EventUpdate(void);

	void SetBgObjTest(void);

	void Sample(void);								//サンプル系が入ってるヨ

	// シングルトン
	static CTutorial* m_pTutorial;

	//インゲーム用変数
	CPlayer* m_pPlayer[NUM_PLAYER];			//プレイヤーのポインタ
	CDevil* m_pDevil;						//デビルのポインタ

	int m_nNumBowabowa;						//ボワボワの数
	bool m_bDevilHoleFinish;					//デビルホールがすべて埋まったかどうか
	bool m_bGameEnd;							//ゲーム終了状態かどうか
	bool m_bGameClear;						//ゲームクリア状態かどうか

	//イベント用変数
	bool m_bEvent;							//イベント状態かどうか
	bool m_bEventEnd;						//イベントが終わったかどうか
	int m_nEventCount;						//イベント時間
	int m_nEventWave;						//イベント段階
	int m_nEventNumber;						//イベント番号
	float m_fEvectFinish;					//鍔迫り合いのカウント
	float m_fEventAngle;						//イベント用の角度
	float m_EventHeight;						//イベント用ポリゴンの高さ
	float m_NameColorA;						//イベント用の名前表示の不透明度
	D3DXVECTOR3 m_EventPos;					//イベント開始座標

	int m_nTutorialWave;						//チュートリアルの段階
	float m_BGColorA;						//ゲーム背景の不透明度
	D3DXVECTOR3 m_BGRot;						//背景の回転向き

	CBoss* m_pBoss;							//ボス

	CEdit* m_pEdit;							//エディットモードのポインタ
	CPause* m_pPause;						//ポーズのポインタ
	CScore* m_pScore;						//スコアのポインタ
	CTimer* m_pTime;							//タイムのポインタ
	CObjmeshDome* m_pMeshDomeUp;				//メッシュドーム
	CObjmeshField* m_pMapField;				//マップフィールド
	CCubeBlock* m_pCubeBlock;				//キューブブロック
	bool m_Wireframe;						//ワイヤーフレームのオンオフ
	bool m_Slow;								//スロー演出のオンオフ

	CObject2D* m_p2DSample;					//2Dポリゴンのサンプル
	CObject3D* m_p3DSample;					//3Dポリゴンのサンプル
	CObjectBillboard* m_pBillboardSample;	//Billboardポリゴンのサンプル
	CObjectX* m_pXModelSample;				//Xモデルのサンプル
	CObjmeshField* m_pMeshFieldSample;		//メッシュフィールドのサンプル
	CObjmeshWall* m_pMeshWallSample;			//メッシュウォールのサンプル
	CObjmeshCylinder* m_pMeshCylinderSample;	//メッシュシリンダーのサンプル

	CMask* m_pMask;	// 2Dマスク
};
#endif