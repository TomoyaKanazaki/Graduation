//============================================
//
//	キャラクターの管理 [characterManager.h]
//	Author:sakai minato
//
//============================================
#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

// マクロ定義
#define MAX_LENGTH_TXT	(128)	// テキストの最大文字数
#define MAX_CHARACTER	(1024)	// キャラクターの最大数
#define MAX_MODEL_PARTS	(64)	// モデルパーツ数の最大数
#define MAX_MOTION		(64)	// モーション数の最大数
#define MAX_KEY			(16)	// キー数の最大数

#include "objectcharacter.h"

// キャラクター管理クラス
class CCharacterManager
{
public:
	CCharacterManager();
	~CCharacterManager();

	int Regist(const char* pFilename);
	const char* GetCharacterName(int Idx) { return &m_aCharacterInfo[Idx].acFileName[0]; }

private:

	// モデルパーツ情報
	struct ModelParts
	{
		int nIndex;			// インデックス
		int nParent;		// 親の番号

		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き

		char acModelFileName[MAX_LENGTH_TXT];	// ファイル名
	};

	// モデル管理情報
	struct ModelManager
	{
		int nNumModel;								// モデル数
		ModelParts aModelParts[MAX_MODEL_PARTS];	// パーツ情報
	};

	// キー情報
	struct Key
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
	};

	// キー管理情報
	struct KeyManager
	{
		int nFrame;					// 再生フレーム
		Key aKey[MAX_MODEL_PARTS];	// 各モデルのキー要素
	};

	// モーション情報
	struct MotionInfo
	{
		bool bLoop;						// ループの有無
		int nNumKey;					// キーの総数
		KeyManager aKeyInfo[MAX_KEY];	// キー管理情報
	};

	// モーション管理情報
	struct MotionManager
	{
		int nNumMotion;					// モーション数
		MotionInfo aMotion[MAX_MOTION];	// モーション情報
	};

	// キャラクター情報
	struct CharacterInfo
	{
		ModelManager ModelManager;			// モデル管理情報
		MotionManager MotionManager;		// モーション管理情報

		char acFileName[MAX_CHARACTER];		// ファイル名
	};

	void LoadModel(const char* pFilename, int nNumCharacter);

	CharacterInfo m_aCharacterInfo[MAX_CHARACTER];
	int m_nNumAll;						// キャラクターの使用数

};
#endif