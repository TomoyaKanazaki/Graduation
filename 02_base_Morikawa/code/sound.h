//=============================================================================
//
// サウンド処理 [sound.h]
// Author : MORIKAWA SHUNYA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドクラス
//*****************************************************************************
class CSound
{
public:
	//*****************************************************************************
	// サウンド一覧
	//*****************************************************************************
	typedef enum
	{
		SOUND_LABEL_BGM_GAME = 0,	// ゲームBGM
		SOUND_LABEL_BGM_TUTORIAL,	// チュートリアルBGM
		SOUND_LABEL_BGM_TITLE,		// タイトルBGM
		SOUND_LABEL_BGM_WIN,		// 勝利BGM
		SOUND_LABEL_BGM_LOSE,		// 敗北BGM
		SOUND_LABEL_SE_WALK,		// プレイヤーの歩行時SE
		SOUND_LABEL_SE_HIT,			// プレイヤーヒット
		SOUND_LABEL_SE_ENEMYHIT,	// 敵ヒット
		SOUND_LABEL_SE_ENTER,	// 敵ヒット
		SOUND_LABEL_SE_JUMP,	// ジャンプ
		SOUND_LABEL_SE_ROWLING,	// 回転
		SOUND_LABEL_SE_CHANGE,	// 回転
		SOUND_LABEL_SE_CLASH,	// 破壊音
		SOUND_LABEL_SE_REVERSE,	// 破壊音
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//*****************************************************************************
	// サウンド情報の構造体定義
	//*****************************************************************************
	typedef struct
	{
		const char *m_pFilename;	// ファイル名
		int m_nCntLoop;		// ループカウント
	} SOUNDINFO;

	CSound();	//コンストラクタ
	~CSound();	//デストラクタ

	//メンバ関数
	HRESULT Init(HWND hWnd);		//初期化
	void Uninit(void);				//終了
	HRESULT PlaySound(SOUND_LABEL label);	//再生
	void Stop(SOUND_LABEL label);			//停止
	void Stop(void);	//すべて停止

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	//メンバ変数
	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	// サウンドの情報
	SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
	{
		{ "data\\BGM\\game.wav", -1 },		//ゲームBGM
		{ "data\\BGM\\tutorial.wav", -1 },	//チュートリアルBGM
		{ "data\\BGM\\title.wav", -1 },		//タイトルBGM
		{ "data\\BGM\\win.wav", 0 },		//勝利BGM
		{ "data\\BGM\\lose.wav", 0 },		//敗北BGM
		{ "data\\SE\\walk02.wav", 0 },		//歩行時
		{ "data\\SE\\dmg_01.wav", 0 },		//プレイヤーのヒット
		{ "data\\SE\\EnemyDamage.wav", 0 },		//敵ヒット
		{ "data\\SE\\Enter.wav", 0 },		//敵ヒット
		{ "data\\SE\\se_jump.wav", 0 },		//敵ヒット
		{ "data\\SE\\se_drift.wav", 0 },		//敵ヒット
		{ "data\\SE\\change.wav", 0 },		//敵ヒット
		{ "data\\SE\\clash.wav", 0 },		//敵ヒット
		{ "data\\SE\\reverse.wav", 0 },		//敵ヒット
	};
};

#endif
