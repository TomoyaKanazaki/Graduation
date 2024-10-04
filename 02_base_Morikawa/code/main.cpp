//========================================
//
// メイン[main.h]
// Author：森川駿弥
//
//========================================
#include "main.h"
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include "manager.h"

//=======================================
// 定数定義
//=======================================
namespace
{
const char* CLASS_NAME = "WindowClass";	// ライトの最大数
const char* WINDOW_NAME = "うさぎとかめと森と川";	// ライトの最大数
}

//========================================
//プロトタイプ宣言
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=======================================
//グローバル変数
//=======================================
int g_nCountFPS = 0;                        //FPSカウンタ
LPD3DXFONT g_pFont = NULL;                  //フォントへのポインタ

//========================================
//メイン関数
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	//画面サイズの構造体
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//現在時刻
	DWORD dwCurrentTime;
	//最後に処理した時刻
	DWORD dwExecLastTime;
	//フレームカウント
	DWORD dwFrameCount;
	//最後にFPSを計測した時刻
	DWORD dwFPSLastTime;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							//ウィンドウのスタイル
		WindowProc,							//ウィンドウプロシージャ
		0,									//0にする
		0,									//0にする
		hInstance,							//インスタンスハンドル
		LoadIcon(nullptr,IDI_APPLICATION),		//タスクバーのアイコン
		LoadCursor(nullptr,IDC_ARROW),			//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			//クライアント領域の背景色
		nullptr,								//メニューバー
		CLASS_NAME,							//ウィンドウクラスの名前
		LoadIcon(nullptr,IDI_APPLICATION)		//ファイルアイコン
	};

	//ウィンドウハンドル(識別子)
	HWND hWnd;

	//メッセージを格納する
	MSG msg;

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定サイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

	//ウィンドウ生成
	hWnd = CreateWindowEx
	(0,									//拡張ウィンドウスタイル
		CLASS_NAME,						//ウィンドウクラスの名前
		WINDOW_NAME,					//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,			//ウィンドウスタイル
		CW_USEDEFAULT,					//ウィンドウの左上X座標
		CW_USEDEFAULT,					//ウィンドウの左上Y座標
		(rect.right - rect.left),		//ウィンドウの幅
		(rect.bottom - rect.top),		//ウィンドウの高さ
		nullptr,						//親ウィンドウのハンドル
		nullptr,						//メニューハンドルまたは子ウィンドウID
		hInstance,						//インスタンスハンドル
		nullptr);						//ウィンドウ作成データ

	//マネージャのポインタ
	CManager *pManager = CManager::GetInstance();

	{//マネージャ初期化
		pManager->Init(hInstance, hWnd, TRUE);
	}

	//現在時刻を取得(保存)
	dwExecLastTime = timeGetTime();

	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				TranslateMessage(&msg);

				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
		 //現在の時刻を取得
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				//FPSを測定した時刻を保存
				dwFPSLastTime = dwCurrentTime;
				//フレームレートをクリア
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				//処理開始の時刻[現在時刻]を保存
				dwExecLastTime = dwCurrentTime;

				//マネージャの更新処理
				pManager->Update();

				//マネージャの描画処理
				pManager->Draw();

				//フレームレートを計算
				dwFrameCount++;
			}
		}

	}
	//マネージャの破棄
	if (pManager != nullptr)
	{
		pManager->Uninit();

		delete pManager;

		pManager = nullptr;
	}

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//========================================
//ウィンドウプロシージャ
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID; //返り値を格納

	switch (uMsg)
	{
	case WM_DESTROY://ウィンドウ破棄メッセージ
		//WM_QUITを送る
		PostQuitMessage(0);
		break;

		//キー押下のメッセージ
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE://[ESC]キーが押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
		}
		break;

	case  WM_CLOSE://閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES)
		{
			//ウィンドウを破棄する
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;   //0を返さないと終了してしまう
		}
		break;
	}
	//既定の処理を繰り返す
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
