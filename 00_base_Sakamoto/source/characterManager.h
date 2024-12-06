//============================================
//
//	キャラクターの管理 [characterManager.h]
//	Author:sakai minato
//
//============================================
#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "objectcharacter.h"

// キャラクター管理クラス
class CCharacterManager
{
public:

	CCharacterManager();
	~CCharacterManager();

	int Regist(CObjectCharacter* pObjCharacter, const char* pFilename);
	const char* GetCharacterName(int Idx) { return &m_aCharacterInfo[Idx].acFileName[0]; }

private:

	// マクロ定義
#define MAX_LENGTH_TXT	(128)	// テキストの最大文字数
#define MAX_CHARACTER	(128)	// キャラクターの最大数
#define MAX_MODEL_PARTS	(32)	// モデルパーツ数の最大数
#define MAX_MOTION		(32)	// モーション数の最大数
#define MAX_KEY			(16)	// キー数の最大数

	// モデルパーツ情報
	struct ModelParts
	{
		char acModelFileName[MAX_LENGTH_TXT];	// ファイル名

		int nIndex;			// インデックス
		int nParent;		// 親の番号

		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
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
		bool bLoop;							// ループの有無
		int nNumKey;						// キーの総数
		KeyManager aKeyManager[MAX_KEY];	// キー管理情報
	};

	// モーション管理情報
	struct MotionManager
	{
		int nNumMotion;						// モーション数
		MotionInfo aMotionInfo[MAX_MOTION];	// モーション情報
	};

	// キャラクター情報
	struct CharacterInfo
	{
		ModelManager ModelManager;			// モデル管理情報
		MotionManager MotionManager;		// モーション管理情報

		char acFileName[MAX_CHARACTER];		// ファイル名
	};

	void SetModelData(CObjectCharacter* pObjCharacter, int nNumCharacter);
	void SetMotionData(CObjectCharacter* pObjCharacter, int nNumCharacter);

	bool LoadModel(const char* pFileName, int nNumCharacter);
	bool LoadMotion(const char* pFileName, int nNumModel, int nNumCharacter);

	CharacterInfo m_aCharacterInfo[MAX_CHARACTER];
	int m_nNumAll;						// キャラクターの使用数

};
#endif