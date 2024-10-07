//==========================================================
//
// デバッグ表示処理 [debugproc.cpp]
// Author : Ibuki Okusada
// Aded by Tomoya Kanazaki
//
//==========================================================
#include "debugproc.h"
#include <string.h>
#include <stdio.h>
#include "input.h"
#include "manager.h"
#include "renderer.h"

//==========================================
//  静的変数宣言
//==========================================
namespace
{
	// 定数定義
	const int MAX_FLOATNUM = 4; // 小数点以下の表示桁数
	const int MAX_DEBUGSTRING = 2048; // デバッグ表示の最大文字数
	const char* FONT_PASS = "Terminal"; // 使用するフォント
	const D3DXCOLOR FONT_COL = D3DCOLOR_RGBA(250, 250, 250, 255);	// フォント色

	// 静的変数
	LPD3DXFONT m_pFont = nullptr; // デバッグフォントへのポインタ
	char m_aStr[DebugProc::POINT_MAX][MAX_DEBUGSTRING] = {}; // デバッグ表示用の文字列
	bool m_bDisp = true; // 表示フラグ
}

//==========================================================
//デバッグ表示の初期化処理
//==========================================================
void DebugProc::Init()
{
	// デバッグ表示情報の初期化
	m_bDisp = false;
	m_pFont = nullptr;
	memset(&m_aStr[0][0], 0, sizeof(m_aStr));

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//デバッグ表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FONT_PASS, &m_pFont);

	//初期表示設定
#if _DEBUG
	m_bDisp = true;
#else NDEBUG
	m_bDisp = false;
#endif
}

//==========================================================
//デバッグ表示の終了処理
//==========================================================
void DebugProc::Uninit(void)
{
	//デバッグ表示用フォントの廃棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//==========================================================
//デバッグ表示の更新処理
//==========================================================
void DebugProc::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();	// キーボードのポインタ

#ifdef _DEBUG


	if(pKeyboard->GetTrigger(DIK_F1) == true)
	{//F1キーが押されたとき
		m_bDisp = m_bDisp ? false : true;
	}

#endif // _DEBUG
}

//==========================================================
//デバッグ表示の描画処理
//==========================================================
void DebugProc::Draw(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	if (m_bDisp == true)
	{//デバックモードがオンの時
	 //テキストの描画
		m_pFont->DrawText(nullptr, &m_aStr[POINT_CENTER][0], -1, &rect, DT_CENTER, FONT_COL);
		m_pFont->DrawText(nullptr, &m_aStr[POINT_LEFT][0], -1, &rect, DT_LEFT, FONT_COL);
		m_pFont->DrawText(nullptr, &m_aStr[POINT_RIGHT][0], -1, &rect, DT_RIGHT, FONT_COL);
	}

	//デバッグ表示情報のクリア
	memset(&m_aStr[0][0], 0, sizeof(m_aStr));
}

//==========================================================
//デバッグ表示の設定処理
//==========================================================
void DebugProc::Print(const EPoint point, const char *fmt, ...)
{
	va_list args;
	char aString[MAX_DEBUGSTRING] = {};		// 指定文字格納用
	char aSaveString[MAX_DEBUGSTRING] = {};	// 可変引数中身格納用
	int nLength = 0;	// 可変引数内の文字の長さ
	int nStopLength = 0;	// 可変引数挿入場所より

	//文字列の代入
	strcpy(&aString[0], fmt);

	va_start(args, fmt);

	for (char *p = &aString[0]; *p != '\0'; p++)
	{
		if (*p == '%')
		{//確認文字がある場合

			p++;	//確認文字まで進める

			nLength = 0;	// 文字の長さリセット

			// 指定された種類ごとに変数の値を抜き出す(va_arg)
			switch (*p)
			{
			case 'd':	//数字

				sprintf(&aSaveString[0], "%d", va_arg(args, int));

				break;
			case 'f':	//小数

				sprintf(&aSaveString[0], "%f", va_arg(args, double));

				//文字列の長さを取得
				nLength = (int)strlen(&aSaveString[0]);

				//小数点以下の1文字目まで確認
				for (int nCntlength = 0; aSaveString[nCntlength] != '\0'; nCntlength++)
				{
					if (aSaveString[nCntlength] == '.')
					{//小数点があった場合

						//小数点以下の桁数を求める
						int nMin = (int)strlen(&aSaveString[nCntlength + 1]);

						//小数点より先の文字数と指定した値までの差分を求める
						nMin -= MAX_FLOATNUM;

						//文字列の長さを小数点以下第二位までに補正
						nLength -= nMin;
					}
				}

				break;
			case 'c':	//文字

				sprintf(&aSaveString[0], "%c", va_arg(args, char));

				break;
			case 's':	//文字列

				sprintf(&aSaveString[0], "%s", va_arg(args, const char*));

				break;
			}

			if (nLength == 0)
			{// 文字の長さを取得していない場合

				//文字列の長さを取得
				nLength = (int)strlen(&aSaveString[0]);
			}

			nStopLength = (int)strlen(p) + 1;

			//メモリ内の確認文字(%～)より後ろの文字列をずらす
			memmove(p + nLength - 2, p, nStopLength);

			p--;	//ポインタを%まで戻す

			//可変引数を指定された場所に挿入
			memcpy(p, &aSaveString[0], nLength);
		}
	}

	va_end(args);

	//文字列を連結する
	strcat(&m_aStr[point][0], &aString[0]);
}
