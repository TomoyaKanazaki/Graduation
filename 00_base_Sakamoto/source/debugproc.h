//==========================================================
//
//デバッグ表示処理 [debugproc.h]
//Author Ibuki Okusada
// Added by Tomoya Kanazaki
//
//==========================================================
#ifndef _DEBUGPROC_H_		//このマクロが定義されていない場合
#define _DEBUGPROC_H_

//**********************************************************
//ネームスペースの定義
//**********************************************************
namespace DebugProc
{
	// デバッグ位置列挙
	enum EPoint
	{
		POINT_CENTER = 0,	// 中央
		POINT_LEFT,			// 左
		POINT_RIGHT,		// 右
		POINT_MAX			// この列挙型の総数
	};

	// 変数宣言
	static LPD3DXFONT m_pFont;	// フォントへのポインタ

	// 関数宣言
	void Init(void); // 初期化処理
	void Uninit(void); // 終了処理
	void Update(void); // 更新処理
	void Draw(void); // 描画処理
	void Print(const EPoint point, const char *fmt, ...); // 文字列入力
};

#endif