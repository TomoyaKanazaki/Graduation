//============================================
//
//	チュートリアル画面 [tutorial.h]
//	Author:酒井 南勝
//
//============================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "manager.h"

//前方宣言
class CPlayer;
class CMap2D;
class CEdit;
class CPause;

class CAim;

class CCubeBlock;
class CBoss;

class CObject2D;
class CObject3D;
class CObjGauge2D;

//シーンクラス
class CTutorial : public CScene
{
public:

	enum TASK
	{
		TASK_NONE = 0,	// なし
		TASK_SLASH,		// 斬撃
		TASK_MOVE_ROT,	// 移動・視点移動
		TASK_SYURIKENN,	// 手裏剣
		TASK_EVASION,	// 回避
		TASK_ENEMY,		// 敵
		TASK_ULTIMATE,	// 術
		TASK_BRIDGE,	// 橋
		TASK_MAX
	};

	CTutorial();
	~CTutorial();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CPause* GetPause(void) { return m_pPause; }
	static CEdit* GetEdit(void) { return m_pEdit; }
	static void SetEvent(bool Set) { m_bEvent = Set; }
	static bool GetEvent(void) { return m_bEvent; }
	static bool GetEventEnd(void) { return m_bEventEnd; }
	static D3DXVECTOR3 GetEventPos(void) { return m_EventPos; }
	static float GetBGColorA(void) { return m_BGColorA; }
	static void AddBGColorA(float Add) { m_BGColorA += Add; }
	static void SetStageBlock(void);
	static void SetGameEnd(bool Set) { m_bGameEnd = Set; }
	static void DispUI(bool Set);

	static CAim* GetAim(void) { return m_pAim; }
	static bool GetSlow(void) { return m_Slow; }

private:

	struct InfoEnemy
	{
		TASK spwanTask;
		D3DXVECTOR3 pos[32];
		int nPosMax;
		bool bLoop;
		int nType;
		TASK task;
	};

	static void LoadMap(void);

	static void LoadPlayerAutoPos(const char* pFilename);
	static void LoadStageBlock(const char* pFilename);
	static void LoadStageMapModel(const char* pFilename);
	static void LoadEnemy(const char* pFilename);
	static void LoadStair(const char* pFilename);
	static void LoadGimmick(const char* pFilename);

	static void SetTaskEnemyCreate(void);

	static void SkipUICreate(void);

	static int m_nEnemyNum;					//現在の敵の数
	static bool m_bGameEnd;					//ゲーム終了状態かどうか
	static bool m_bEvent;					//イベント状態かどうか
	static bool m_bEventEnd;				//イベントが終わったかどうか
	static int m_nEventCount;				//イベント時間
	static int m_nTutorialWave;				//チュートリアルの段階
	static float m_EventHeight;				//イベント用ポリゴンの高さ
	static float m_NameColorA;				//イベント用の名前表示の不透明度
	static float m_BGColorA;				//ゲーム背景の不透明度
	static D3DXVECTOR3 m_EventPos;			//イベント開始座標
	static D3DXVECTOR3 m_BGRot;				//背景の回転向き

	static CPlayer* m_pPlayer;				//プレイヤーのポインタ
	static CEdit* m_pEdit;					//エディットモードのポインタ
	static CPause* m_pPause;				//ポーズのポインタ

	static CCubeBlock* m_pCubeBlock;		// キューブブロックのポインタ
	static CBoss* m_pBoss;					// ボスクラス

	static bool m_Wireframe;				//ワイヤーフレームのオンオフ
	static bool m_Slow;						//スロー演出のオンオフ

	static CAim* m_pAim;					// 照準

	static CObject2D* m_pSkipUi;			// スキップ説明UI
	static CObject3D* m_pRiver;				// 川UI
	static CObjGauge2D* m_pSkipGage;		// スキップゲージ
	static float m_fCntSkip;				// スキップカウント

	static bool m_abIsTask[TASK_MAX];		// タスクの進捗状況

	static InfoEnemy m_infoEnemy;			// 敵情報
};
#endif