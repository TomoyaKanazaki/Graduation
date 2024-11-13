//============================================
//
//	プリコンパイルヘッダ [precompile.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PRECOMPILE_H_
#define _PRECOMPILE_H_

#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <d3dx9.h>

// ビルド時の警告対処用マクロ
#define  DIRECTINPUT_VERSION	(0x0800)
#include <dinput.h>		// 入力処理

#include <Xinput.h>		// パッド
#include <xaudio2.h>	// サウンド

// メモリリーク出力用マクロ
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		// メモリリーク検出に必要

#include <iostream>		// 入出力ストリーム
#include <fstream>		// ファイル入出力ストリーム
#include <sstream>		// 文字列入出力ストリーム
#include <iomanip>		// 小数点出力
#include <filesystem>	// ファイル操作
#include <functional>	// 関数ポインタ
#include <string>		// 文字列
#include <thread>		// スレッド
#include <future>		// スレッド返り値取得
#include <vector>		// 動的配列
#include <list>			// リスト構造
#include <map>			// 連想配列

#include "useful.h"			// 便利関数
#include "listManager.h"	// リスト管理
#include "input.h"			// 入力検知
#include "debugproc.h"		// デバッグ表示
#include "deltatime.h"

#pragma warning(disable : 26812)	// enumclass警告の削除
#pragma warning(disable : 6031)		// 返り値無視警告の削除

// エフェクシア
//#ifndef _DEBUG
//#pragma comment(lib, "Effekseer.lib")
//#else
//#pragma comment(lib, "Effekseerd.lib")
//#endif
//#include <Effekseer.h>
//
//#ifndef _DEBUG
//#pragma comment(lib, "EffekseerRendererDX9.lib")
//#else
//#pragma comment(lib, "EffekseerRendererDX9d.lib")
//#endif
//#include <EffekseerRendererDX9.h>

//************************************************************
//	メモリリークのソース表示 ON/OFF
//************************************************************
#if 1
#ifdef _DEBUG
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif	// _DEBUG
#endif

#endif	//_PRECOMPILE_H_