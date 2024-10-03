//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 佐藤根詩音
//
//=============================================================================
#ifndef _SOUND_H_		//このマクロ定義がされていなかったら
#define _SOUND_H_		//2重インクルード防止のマクロを定義する

#include "main.h"

//サウンドクラスの定義
class CSound
{
public:
	//*****************************************************************************
	// サウンド一覧
	//*****************************************************************************
	enum SOUND_LABEL
	{
		SOUND_LABEL_BGM000 = 0,			// タイトル
		SOUND_LABEL_BGM001,				// あらすじBGM
		SOUND_LABEL_BGM002,				// ゲームBGM
		SOUND_LABEL_BGM003,				// リザルトBGM(クリア)
		SOUND_LABEL_BGMBOSS,			// ボス戦BGM
		SOUND_LABEL_SE_DAMAGE000,		// ダメージ音0
		SOUND_LABEL_SE_DAMAGE001,		// ダメージ音1
		SOUND_LABEL_SE_SHOT000,			// ショット音0
		SOUND_LABEL_SE_ITEM000,			// アイテム音
		SOUND_LABEL_SE_ITEM001,			// 回復音
		SOUND_LABEL_SE_ENTER000,		// 決定音000
		SOUND_LABEL_SE_ENTER001,		// 決定音001
		SOUND_LABEL_SE_ENTER002,		// 決定音002
		SOUND_LABEL_SE_SWITCH,			// スイッチ音
		SOUND_LABEL_SE_STEP,			// 歩く音
		SOUND_LABEL_SE_JUMP000,			// ジャンプ音
		SOUND_LABEL_SE_JUMP001,			// ジャンプ音
		SOUND_LABEL_SE_DASH,			// ダッシュ音
		SOUND_LABEL_SE_DASH_AUTO,		// オートダッシュ音
		SOUND_LABEL_SE_COLLAPSE00,		// 崩れる音
		SOUND_LABEL_SE_COLLAPSE01,		// 崩れた音
		SOUND_LABEL_SE_APPEAR,			// 出現する音

		SOUND_LABEL_MAX,
	};

	CSound();		//コンストラクタ
	~CSound();		//デストラクタ

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	static HRESULT Play(SOUND_LABEL label);
	static void Stop(SOUND_LABEL label);
	static void Stop(void);

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	//*****************************************************************************
	// サウンド情報の構造体定義
	//*****************************************************************************
	struct SOUNDINFO
	{
		char *m_pFilename;	// ファイル名
		int m_nCntLoop;		// ループカウント
	};

	IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
	static SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX];
};


#endif