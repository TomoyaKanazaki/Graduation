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
		SOUND_LABEL_BGM_TITLE = 0,			// BGM0						
		SOUND_LABEL_BGM_GAME,				// BGM1
		SOUND_LABEL_BGM_BOSS,				// BGM1
		SOUND_LABEL_BGM_RESULT,				// BGM2
		SOUND_LABEL_SE_SELECT_PUSH,			// 選択音
		SOUND_LABEL_SE_ENTER_PUSH,			// 決定音
		SOUND_LABEL_SE_WARNING,				// 警告音
		SOUND_LABEL_SE_BOSS_BULLET,			// ボスの射撃音
		SOUND_LABEL_SE_BOSS_RAIN,			// ボスの雨攻撃発動音
		SOUND_LABEL_SE_BOSS_DASH,			// ボスの突進音
		SOUND_LABEL_SE_BOSS_SMOKE,			// ボスの煙玉
		SOUND_LABEL_SE_BOSS_SLASH,			// ボスの斬撃
		SOUND_LABEL_SE_BOSS_BLOCK_SYURIKEN,	// ボスの手裏剣防御
		SOUND_LABEL_SE_BOSS_SYURIKEN,		// ボスの手裏剣
		SOUND_LABEL_SE_BOSS_BACKJUMP,		// ボスの後ろ飛び
		SOUND_LABEL_SE_BOSS_KABUTO,			// ボスの兜割
		SOUND_LABEL_SE_BOSS_MOW,			// ボスの薙ぎ払い
		SOUND_LABEL_SE_DAMAGE_BOSS,			// ボスのダメージ
		SOUND_LABEL_SE_DAMAGE_PLAYER,		// プレイヤーダメージ音
		SOUND_LABEL_SE_ATTACK_NORMAL,		// プレイヤー通常攻撃音
		SOUND_LABEL_SE_ATTACK_SKY,			// プレイヤー空中攻撃音
		SOUND_LABEL_SE_DODGE,				// プレイヤー回避音
		SOUND_LABEL_SE_JUMP,				// プレイヤージャンプ音
		SOUND_LABEL_SE_ATTENTION,			// 注目切り替え音
		SOUND_LABEL_SE_DEATHDAMAGE,			// 死亡ダメージ音
		SOUND_LABEL_SE_DEATHEXPLOSION,		// 死亡爆発音
		SOUND_LABEL_SE_GOGOGOGO,			// ゴゴゴゴゴゴゴ音
		SOUND_LABEL_SE_HOWL,				// 咆哮音
		SOUND_LABEL_BGM_TUTORIAL,			// チュートリアルBGM
		SOUND_LABEL_SE_SWORD_SWING_00,		// 忍刀空振り音_00
		SOUND_LABEL_SE_SWORD_SWING_01,		// 忍刀空振り音_01
		SOUND_LABEL_SE_SWORD_HIT,			// 忍刀ヒット音
		SOUND_LABEL_SE_SHURIKEN_SHOT,		// 手裏剣発射音
		SOUND_LABEL_SE_SHURIKEN_HIT,		// 手裏剣ヒット音
		SOUND_LABEL_SE_TARGET_SWITCHING,	// ターゲット切り替え音
		SOUND_LABEL_SE_SWORD_GUARD,			// 刀のガード音
		SOUND_LABEL_SE_WRITE_BRUSH,			// 筆で書く音
		SOUND_LABEL_SE_SCROLL_OPEN,			// 巻物を開く音
		SOUND_LABEL_SE_FIRE_ACTIVE,			// 火遁発動音
		SOUND_LABEL_SE_FIRE_0,				// 炎の音
		SOUND_LABEL_SE_BOTLE_BREAK,			// 瓶の壊れる音
		SOUND_LABEL_SE_BOX_BREAK,			// 木箱の壊れる音
		SOUND_LABEL_SE_BOW,					// 弓音
		SOUND_LABEL_SE_BATTLE_ENTRY,		// 戦闘突入音
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//*****************************************************************************
	// メンバ関数宣言
	//*****************************************************************************
	HRESULT InitSound(HWND hWnd);
	void UninitSound(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

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
