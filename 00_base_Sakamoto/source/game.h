//============================================
//
//	ゲーム画面 [game.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "manager.h"

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
class CTime;
class CBoss;
class CPlayer2D;
class CGamePause;

class CAim;

//シーンクラス
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static int GetFloor(void) { return m_nFloor; }
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CPlayer2D* GetPlayer2D(void) { return m_pPlayer2D; }
	static CBoss* GetBoss(void) { return m_pBoss; }
	static CPause* GetPause(void) { return m_pPause; }
	static CGamePause* GetGamePause(void) { return m_pGamePause; }
	static CScore* GetScore(void) { return m_pScore; }
	static CEdit* GetEdit(void) { return m_pEdit; }
	static CTime* GetTime(void) { return m_pTime; }
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
	void DeleteMap(void);

	static CAim* GetAim(void) { return m_pAim; }
	static bool GetSlow(void) { return m_Slow; }
private:
	static void EventUpdate(void);

	bool EnemyCheck(void);					//敵がいるかどうかの判断

	void Sample(void);						//サンプル系が入ってるヨ

	static int m_nFloor;					//現在の階層
	static int m_nEnemyNum;					//現在の敵の数
	static bool m_bGameEnd;					//ゲーム終了状態かどうか

	//イベント用変数
	static bool m_bEvent;					//イベント状態かどうか
	static bool m_bEventEnd;				//イベントが終わったかどうか
	static int m_nEventCount;				//イベント時間
	static int m_nEventWave;				//イベント段階
	static int m_nEventNumber;				//イベント番号
	static float m_fEvectFinish;			//鍔迫り合いのカウント
	static float m_fEventAngle;					//イベント用の角度
	static float m_EventHeight;				//イベント用ポリゴンの高さ
	static float m_NameColorA;				//イベント用の名前表示の不透明度
	static D3DXVECTOR3 m_EventPos;			//イベント開始座標

	static int m_nTutorialWave;						//チュートリアルの段階
	static float m_BGColorA;						//ゲーム背景の不透明度
	static D3DXVECTOR3 m_BGRot;						//背景の回転向き

	static CBoss* m_pBoss;							//ボス

	static CEdit* m_pEdit;							//エディットモードのポインタ
	static CPause* m_pPause;						//ポーズのポインタ
	static CGamePause* m_pGamePause;				//ゲーム用ポーズのポインタ
	static CScore* m_pScore;						//スコアのポインタ
	static CTime* m_pTime;							//タイムのポインタ
	static CObject2D* m_p2DSample;					//2Dポリゴンのサンプル
	static CObject3D* m_p3DSample;					//3Dポリゴンのサンプル
	static CObjectBillboard* m_pBillboardSample;	//Billboardポリゴンのサンプル
	static CObjectX* m_pXModelSample;				//Xモデルのサンプル
	static CObjmeshField* m_pMeshFieldSample;		//メッシュフィールドのサンプル
	static CObjmeshWall* m_pMeshWallSample;			//メッシュウォールのサンプル
	static CObjmeshCylinder* m_pMeshCylinderSample;	//メッシュシリンダーのサンプル
	static CObjmeshDome* m_pMeshDomeUp;				//メッシュドーム
	static CObjmeshField* m_pMeshField;				//メッシュフィールド
	static CCubeBlock* m_pCubeBlock;				//キューブブロック
	static bool m_Wireframe;						//ワイヤーフレームのオンオフ
	static bool m_Slow;								//スロー演出のオンオフ

	static CPlayer* m_pPlayer;						//プレイヤーのポインタ
	static CPlayer2D* m_pPlayer2D;					//プレイヤーのポインタ

	static CAim* m_pAim;							// 照準
};
#endif