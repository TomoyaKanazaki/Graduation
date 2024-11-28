#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

class CSound
{
public:
	CSound();
	~CSound();

	//*****************************************************************************
	// サウンド一覧
	//*****************************************************************************
	typedef enum
	{
		// SE
		SOUND_LABEL_SE_DEATH = 0,	// タマゴンの死亡
		SOUND_LABEL_SE_DEVILHOLE,	// デビルホール
		SOUND_LABEL_SE_BREAKEGG,	// 卵が割れる
		SOUND_LABEL_SE_ENEMROTATION,	// 敵の方向転換	
		SOUND_LABEL_SE_FIRE,	// タマゴンの火炎放射
		SOUND_LABEL_SE_GET_BIBLE,	// 聖書の取得
		SOUND_LABEL_SE_STAGECLEAR,	// ステージクリア
		SOUND_LABEL_SE_WALK,	// タマゴンの徒歩	
		SOUND_LABEL_SE_FLAP,	// 羽ばたき	
		SOUND_LABEL_SE_RESULT,	// リザルトの効果音
		SOUND_LABEL_SE_FAIL,	// 失敗時
		SOUND_LABEL_SE_EXPLOSION,	// 爆発
		SOUND_LABEL_SE_BORN,	// タマゴンが生まれる
		SOUND_LABEL_SE_EAT,	// タマゴンの捕食
		SOUND_LABEL_SE_GET_CROSS,	// 十字架
		SOUND_LABEL_SE_GET_BOWA,	// ぼあぼあ
		SOUND_LABEL_SE_SELECT,		// 選択
		SOUND_LABEL_SE_ENTER,		// 決定

		// BGM
		SOUND_LABEL_BGM_TITLE,	// タイトルbgm
		SOUND_LABEL_BGM_RESULT,	// リザルトbgm
		SOUND_LABEL_BGM_STAGE1,	// ステージ1bgm	
		SOUND_LABEL_BGM_STAGE2,	// ステージ2bgm	

		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//*****************************************************************************
	// メンバ関数宣言
	//*****************************************************************************
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);

private:

	//*****************************************************************************
	// サウンド情報の構造体定義
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDINFO;

	//*****************************************************************************
	// メンバ関数宣言
	//*****************************************************************************
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	//*****************************************************************************
	// メンバ変数宣言
	//*****************************************************************************
	IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	// サウンドの情報
	static SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX];
};
#endif
