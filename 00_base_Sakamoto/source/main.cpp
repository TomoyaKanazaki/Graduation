//============================================
//
//	ソリューション[main.cpp]
//	Author:sakamoto kai
//
//============================================
#include "main.h"
#include "manager.h"
#include "game.h"
#include "timer.h"

//ライブラリのリンク
#pragma comment (lib,"d3d9.lib")		//描画処理に必要
#pragma comment (lib,"d3dx9.lib")		//[d3d9.lib]の拡張ライブラリ
#pragma comment (lib,"dxguid.lib")		//DirectXコンポーネント(部品)使用に必要
#pragma comment (lib,"winmm.lib")		//システム時刻所得に必要
#pragma comment (lib,"dinput8.lib")		//入力処理に必要

//==========================================
//  定数定義
//==========================================
namespace
{
	const char* CLASS_NAME = "Pause.";				//ウインドウクラスの名前
	const char* WINDOW_NAME = "　Pause.　";			//ウインドウの名前(キャプションに表示)
}

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//グローバル変数
int g_nCountFPS = 0;						//FPSカウンタ
bool g_bDispDebug = false;					//デバッグ表示のON/OFF

//============================================
//メイン関数
//============================================
int	WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hinstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	//終了時にメモリリークを出力
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CManager *pManager = nullptr;	//マネージャのインスタンス

	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							//ウインドウのスタイル
		WindowProc,							//ウインドウプロシージャ
		0,									//0にする(通常は使用しない)
		0,									//0にする(通常は使用しない)
		hInstance,							//インスタンスハンドル
		LoadIcon(nullptr,IDI_APPLICATION),		//タスクバーのアイコン
		LoadCursor(nullptr,IDC_ARROW),			//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			//クライアント領域の背景色
		nullptr,								//メニューバー
		CLASS_NAME,							//ウインドウクラスの名前
		LoadIcon(nullptr,IDI_APPLICATION),		//ファイルのアイコン
	};
	HWND hWnd;
	MSG msg;
	RECT rect = { 0,0,SCREEN_WIDTH ,SCREEN_HEIGHT };

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(0,	//拡張ウインドウスタイル
		CLASS_NAME,				//ウインドウクラスの名前
		WINDOW_NAME,			//ウインドウの名前
		WS_OVERLAPPEDWINDOW,	//ウインドウスタイル
		CW_USEDEFAULT,			//ウインドウの左上X座標
		CW_USEDEFAULT,			//ウインドウの左上Y座標
		SCREEN_WIDTH,			//ウインドウの幅
		SCREEN_HEIGHT,			//ウインドウの高さ
		nullptr,					//親ウインドウのハンドル
		nullptr,					//メニューハンドルまたは子ウインドウID
		hInstance,				//インスタンスハンドル
		nullptr);					//ウインドウ作成データ
								//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;				//初期化する
	dwExecLastTime = timeGetTime();	//現在時刻を所得(保存)

	srand(dwExecLastTime);

	//マネージャの生成
	if (pManager == nullptr)
	{
		pManager = CManager::GetInstance();
	}

	//初期化
	if (pManager != nullptr)
	{
		if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
		{//初期化処理が失敗した場合
			return -1;
		}
	}

	CTimer* pTime = nullptr;
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		pTime = CGame::GetInstance()->GetTime();
	}

	// システム時間の初期化
	DeltaTime::Init();

	//メッセージループ
	while (1)
	{//Windowsの処理
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);	//ウインドウの表示状態を設定
				DispatchMessage(&msg);	//クライアント領域を更新
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();		//現在時刻を所得
			pTime->FloatSetTime(timeGetTime());

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
			 //FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	//FPSを計測した時刻を保存
				dwFrameCount = 0;				//フレームカウントをクリア
			}

			if (dwCurrentTime - dwExecLastTime >= (1000 / 60))
			{//６０分の１秒経過
				dwExecLastTime = dwCurrentTime;	//処理開始の時刻[現在時刻]を保存

				if (pManager != nullptr)
				{
					// 時間を更新
					DeltaTime::Update();

					//更新処理
					pManager->Update();

					//描画処理
					pManager->Draw();
				}

				dwFrameCount++;					//フレームカウントを加算
			}
		}
	}

	if (pManager != nullptr)
	{
		//終了処理
		pManager->Uninit();

		//メモリ解放
		delete pManager;
		pManager = nullptr;
	}

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//====================================================================
//ウインドウプロシージャ
//====================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char aString[256] = "test";		//描画するテキストを格納
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//ウインドウの領域（矩形）

	int nID;						//返り値を格納
	HWND hWndButtonFinish = 0;			//終了ボタンのウインドウハンドル(識別子)

	switch (uMsg)
	{
	case WM_DESTROY:	//ウインドウ破棄のメッセージ
						//WM_QUITメッセージを送る。
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:	//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE: //[ESC]キーが押された
			nID = MessageBox(nullptr, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				//ウインドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
				break;
			}
			break;
		}

		//終了ボタンに表示する文字列を設定
		SetWindowText(hWndButtonFinish, "終了");

		break;

	case WM_CLOSE:	//閉じる押下のメッセージ
		nID = MessageBox(nullptr, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			//ウインドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;	//０を返さないと終了してしまう
		}

		break;

	case WM_LBUTTONDOWN: //マウス左クリックのメッセージ
						 //対象ウインドウにフォーカスを合わせる
		SetFocus(hWnd);
		break;

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//既定の処理を返す
}

int GetFps(void)
{
	return g_nCountFPS;
}