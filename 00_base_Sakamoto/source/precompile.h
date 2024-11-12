//============================================
//
//	プリコンパイルヘッダ [precompile.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PRECOMPILE_H_
#define _PRECOMPILE_H_

#include <windows.h>
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対処用マクロ
#include "dinput.h"						//入力処理に必要
#include "xaudio2.h"					//サウンド処理
#include <cassert>
#include <math.h>
#include <stdio.h>
#include <cmath>
#include <list>
#include <vector>

#include <string>			// 文字列
#include <string.h>

#include "useful.h"
#include "debugproc.h"
#include "listManager.h"
#include "deltatime.h"
#include <algorithm>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		// メモリリーク検出に必要

#ifdef _DEBUG
//#define CHECK_MEM_LEAK
#endif // _DEBUG

//普段使用禁止
#ifdef CHECK_MEM_LEAK
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif // CHECK_MEM_LEAK

#endif	//_PRECOMPILE_H_