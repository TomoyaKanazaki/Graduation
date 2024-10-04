//========================================
//
//プリコンパイル[precompile.h]
//Author：森川駿弥
//
//========================================

//========================================
// 事前インクルード
//========================================
#ifndef PRECOMPILE_H__
#define PRECOMPILE_H__

#include <windows.h>

#define  DIRECTINPUT_VERSION (0x0800)   //ビルド時の警告対処用マクロ
#include <crtdbg.h>		// メモリリーク検出に必要

#include "d3dx9.h"		//描画処理に必要
#include "xaudio2.h"	//サウンド処理に必要
#include "dinput.h"		//入力処理に必要
#include "Xinput.h"
#include "time.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <vector>
#include "debugproc.h"


#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <DirectXMath.h>
#include <D3DX9Math.h>
#include <assert.h>

// C++のstd用
#include <vector>
#include <algorithm>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iterator>
#include <codecvt>
#include <fstream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <stack>
#include <filesystem>

// マイライブラリ
#include "mylib.h"

#endif // _PRECOMPILE_H_