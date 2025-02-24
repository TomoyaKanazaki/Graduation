//============================================
//
//	マネージャ [manager.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "object.h"

//前方宣言
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CInputMouse;
class CCamera;
class CMiniMapCamera;
class CLight;
class CTexture;
class CXModel;
class CCharacterManager;
class CLevelUP;
class CFade;
class CSound;
class CTimer;
class CRanking;
class CBlockManager;
class CItemManager;
class CScene;
class CMapSystem;

#define NUM_CAMERA	(3)		//カメラの最大数

//マネージャクラス
class CManager
{
public:
	enum TYPE_INPUT
	{
		TYPE_NONE = 0,
		TYPE_MNK,		// キーマウ
		TYPE_JOYPAD,	// パッド
		TYPE_JOYCON,	// ジョイコン
		TYPE_MAX
	};

	enum GAME_MODE
	{
		MODE_NONE = 0,
		MODE_SINGLE,
		MODE_MULTI,
		MODE_MAX
	};

public:
	CManager();
	~CManager();

	static CManager *GetInstance();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow); 	//返り値がHRESULTなのは頂点バッファの生成などに失敗したとき確認のため
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CScene* GetScene(void) { return m_pScene; }
	void SetScene(CScene* Scene) { m_pScene = Scene; }

	CRenderer *GetRenderer(void) { return m_pRenderer; }
	CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	CInputJoypad* GetInputJoyPad(void) { return m_pInputJoyPad; }
	CInputMouse*GetInputMouse(void) { return m_pInputMouse; }
	CCamera* GetCamera(int nIdx) { return m_pCamera[nIdx]; }
	CMiniMapCamera* GetMiniMapCamera(void) { return m_pMiniMapCamera; }
	CTexture *GetTexture(void) { return m_pTexture; }
	CXModel *GetXModel(void) { return m_pXModel; }
	CCharacterManager* GetCharacterManager(void) { return m_pCharacterManager; }
	CFade *GetFade(void) { return m_pFade; }
	CSound *GetSound(void) { return m_pSound; }
	CRanking* GetRanking(void) { return m_pRanking; }
	CLight* GetLight(void) { return m_pLight; }
	CBlockManager* GetBlockManager(void) { return m_pBlockManager; }
	CMyEffekseer* GetEffect(void) { return m_pEffect; }
	CMapSystem* GetMapSystem(void) { return m_pMapSystem; }
	void SetLevelUP(CLevelUP *LevelUP) { m_LevelUP = LevelUP; }
	void SetEdit(bool bEdit) { m_bEdit = bEdit; }
	bool GetEdit(void) { return m_bEdit; }
	void SetStop(bool Set) { m_bStop = Set; }
	bool GetStop(void) { return m_bStop; }
	void SetPause(bool Set) { m_Pause = Set; }
	bool GetPause(void) { return m_Pause; }
	void SetEndScore(int Set) { m_EndScore = Set; }
	int GetEndScore(void) { return m_EndScore; }
	void SetEnd1PScore(int Set) { m_End1PScore = Set; }
	int GetEnd1PScore(void) { return m_End1PScore; }
	void SetEnd2PScore(int Set) { m_End2PScore = Set; }
	int GetEnd2PScore(void) { return m_End2PScore; }
	void SetEndTime(int Set) { m_EndTime = Set; }
	int GetEndTime(void) { return m_EndTime; }
	void SetTutorialStart(bool Set) { m_SetTutorial = Set; }
	bool GetTutorialStart(void) { return m_SetTutorial; }
	void SetScoreResult(bool Set) { m_SetScoreResult = Set; }
	bool GetScoreResult(void) { return m_SetScoreResult; }
	void SetSetTutorialPad(bool Set) { m_SetJoyPad = Set; }
	bool GetSetTutorialPad(void) { return m_SetJoyPad; }
	void SetPauseOK(bool Set) { m_PauseOK = Set; }
	bool GetPauseOK(void) { return m_PauseOK; }
	void SetUseJoyPad(bool Set) { m_bPad = Set; }
	bool GetUseJoyPad(void) { return m_bPad; }
	void SetGameClear(bool Set) { m_bGameClear = Set; }
	bool GetGameClear(void) { return m_bGameClear; }
	void SetGameSpeed(float Speed) { GameSpeed = Speed; }
	float GetGameSpeed(void) { return GameSpeed; }
	void SetGameViewport(D3DVIEWPORT9 Viewport) { m_GameViewport = Viewport; }
	D3DVIEWPORT9 GetGameViewport(void) { return m_GameViewport; }
	void SetGameMtxView(D3DXMATRIX Viewport) { m_GamemtxView = Viewport; }
	D3DXMATRIX GetGameMtxView(void) { return m_GamemtxView; }
	void SetGamemtxProjection(D3DXMATRIX Viewport) { m_GamemtxProjection = Viewport; }
	D3DXMATRIX GetGamemtxProjection(void) { return m_GamemtxProjection; }
	void SetTypeInput(TYPE_INPUT type) { m_typeInput = type; }
	TYPE_INPUT GetTypeInput(void) { return m_typeInput; }

	void SetScrollType(int type) { m_ScrollType = type; }
	int GetScrollType(void) { return m_ScrollType; }

	void SetGameMode(GAME_MODE type) { m_GameMode = type; }
	GAME_MODE GetGameMode(void) { return m_GameMode; }
	bool Getstart() { return m_bState; }
	int GetStage(void) { return m_nStage; }
	void SetStage(int Stage) { m_nStage = Stage; }

private:
	static CManager *pManager;

	D3DXMATRIX m_GamemtxView;			//ビューマトリックス
	D3DXMATRIX m_GamemtxProjection;		//プロジェクションマトリックス
	D3DVIEWPORT9 m_GameViewport;		//ゲーム画面のビューポート

	int m_nStage;						//現在のステージ
	float GameSpeed;					//ゲームスピード
	int m_EndScore;						//最終的なスコア
	int m_End1PScore;					//最終的なスコア
	int m_End2PScore;					//最終的なスコア
	int m_EndTime;						//最終的なタイム
	bool m_bGameClear;					//ゲームクリア状態か
	bool m_SetTutorial;					//エディットモードかどうか
	bool m_bEdit;						//エディットモードかどうか
	bool m_bStop;						//画面停止状態かどうか
	bool m_Pause;						//ポーズ状態かどうか
	bool m_SetScoreResult;				//リザルトにスコア情報を入れるかどうか
	bool m_SetJoyPad;					//ジョイパッドで操作しているかどうか
	bool m_PauseOK;						//ポーズを押しても大丈夫か
	bool m_bPad;						//ジョイパッドを使用しているかどうか
	bool m_bState;
	CRenderer *m_pRenderer;					//レンダラーのポインタ
	CInputKeyboard *m_pInputKeyboard;		//キーボードのポインタ
	CInputJoypad* m_pInputJoyPad;			//ジョイパッドのポインタ
	CInputMouse*m_pInputMouse;				//マウスのポインタ
	CCamera *m_pCamera[NUM_CAMERA];			//カメラのポインタ
	CMiniMapCamera* m_pMiniMapCamera;		//ミニマップカメラのポインタ
	CLight *m_pLight;						//ライトのポインタ
	CTexture *m_pTexture;					//テクスチャのポインタ
	CXModel *m_pXModel;						//Xモデルのポインタ
	CCharacterManager* m_pCharacterManager; //キャラクター管理のポインタ
	CLevelUP *m_LevelUP;					//レベルアップのポインタ
	CFade *m_pFade;							//フェードのポインタ
	CSound *m_pSound;						//サウンドのポインタ
	CRanking* m_pRanking;
	CBlockManager* m_pBlockManager;
	CMyEffekseer* m_pEffect;
	TYPE_INPUT m_typeInput;				//インプット種類
	GAME_MODE m_GameMode;				//ゲームのプレイモード
	CScene* m_pScene;					//シーンのポインタ
	CMapSystem* m_pMapSystem;			//マップシステム
	int m_ScrollType;	//スクロールの種類
};												

#endif