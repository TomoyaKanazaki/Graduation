//============================================
//
//	チュートリアル画面 [tutorial.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "manager.h"

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

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CPlayer* GetPlayer(int Num) { return m_pPlayer[Num]; }
	static CDevil* GetDevil(void) { return m_pDevil; }
	static CBoss* GetBoss(void) { return m_pBoss; }
	static CPause* GetPause(void) { return m_pPause; }
	static CScore* GetScore(void) { return m_pScore; }
	static CEdit* GetEdit(void) { return m_pEdit; }
	static CTimer* GetTime(void) { return m_pTime; }
	static CObjmeshField* GetMapField(void) { return m_pMapField; }
	static CObjmeshDome* GetDomeUp(void) { return m_pMeshDomeUp; }
	static void SetCubeBlock(CCubeBlock* pBlock) { m_pCubeBlock = pBlock; }
	static CCubeBlock* GetCubeBlock(void) { return m_pCubeBlock; }
	static void SetEvent(bool Set) { m_bEvent = Set; }
	static bool GetEvent(void) { return m_bEvent; }
	static bool GetEventEnd(void) { return m_bEventEnd; }
	static D3DXVECTOR3 GetEventPos(void) { return m_EventPos; }
	static float GetBGColorA(void) { return m_BGColorA; }
	static void AddBGColorA(float Add) { m_BGColorA += Add; }
	static void SetStageBlock(void);
	static void SetGameEnd(bool Set) { m_bGameEnd = Set; }
	static void SetGameClear(bool Set) { m_bGameClear = Set; }
	static int GetBowabowa(void) { return m_nNumBowabowa; }
	static void SetBowabowa(int Bowabowa) { m_nNumBowabowa = Bowabowa; }
	static void AddBowabowa(int nNum) { m_nNumBowabowa += nNum; }
	static void SetDevilHoleFinish(bool Set) { m_bDevilHoleFinish = Set; }

	void DeleteMap(void);

	static bool GetSlow(void) { return m_Slow; }
private:
	static void StageClear(int Stage);
	static void LoadStageRailBlock(const char* pFilename);
	static void LoadStageMapModel(const char* pFilename);

	static void EventUpdate(void);

	static void SetBgObjTest(void);

	void Sample(void);								//サンプル系が入ってるヨ

	//インゲーム用変数
	static CPlayer* m_pPlayer[NUM_PLAYER];			//プレイヤーのポインタ
	static CDevil* m_pDevil;						//デビルのポインタ

	static int m_nNumBowabowa;						//ボワボワの数
	static bool m_bDevilHoleFinish;					//デビルホールがすべて埋まったかどうか
	static bool m_bGameEnd;							//ゲーム終了状態かどうか
	static bool m_bGameClear;						//ゲームクリア状態かどうか

	//イベント用変数
	static bool m_bEvent;							//イベント状態かどうか
	static bool m_bEventEnd;						//イベントが終わったかどうか
	static int m_nEventCount;						//イベント時間
	static int m_nEventWave;						//イベント段階
	static int m_nEventNumber;						//イベント番号
	static float m_fEvectFinish;					//鍔迫り合いのカウント
	static float m_fEventAngle;						//イベント用の角度
	static float m_EventHeight;						//イベント用ポリゴンの高さ
	static float m_NameColorA;						//イベント用の名前表示の不透明度
	static D3DXVECTOR3 m_EventPos;					//イベント開始座標

	static int m_nTutorialWave;						//チュートリアルの段階
	static float m_BGColorA;						//ゲーム背景の不透明度
	static D3DXVECTOR3 m_BGRot;						//背景の回転向き

	static CBoss* m_pBoss;							//ボス

	static CEdit* m_pEdit;							//エディットモードのポインタ
	static CPause* m_pPause;						//ポーズのポインタ
	static CScore* m_pScore;						//スコアのポインタ
	static CTimer* m_pTime;							//タイムのポインタ
	static CObjmeshDome* m_pMeshDomeUp;				//メッシュドーム
	static CObjmeshField* m_pMapField;				//マップフィールド
	static CCubeBlock* m_pCubeBlock;				//キューブブロック
	static bool m_Wireframe;						//ワイヤーフレームのオンオフ
	static bool m_Slow;								//スロー演出のオンオフ

	static CObject2D* m_p2DSample;					//2Dポリゴンのサンプル
	static CObject3D* m_p3DSample;					//3Dポリゴンのサンプル
	static CObjectBillboard* m_pBillboardSample;	//Billboardポリゴンのサンプル
	static CObjectX* m_pXModelSample;				//Xモデルのサンプル
	static CObjmeshField* m_pMeshFieldSample;		//メッシュフィールドのサンプル
	static CObjmeshWall* m_pMeshWallSample;			//メッシュウォールのサンプル
	static CObjmeshCylinder* m_pMeshCylinderSample;	//メッシュシリンダーのサンプル

	static CMask* m_pMask;	// 2Dマスク
};
#endif