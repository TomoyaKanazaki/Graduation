//============================================
//
//	モーション [motion.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MOTION_H_
#define _MOTION_H_

//マクロ定義
#define MAX_INFO (64)	//情報の最大数
#define MAX_PARTS (64)	//パーツの総数
#define MAX_KEY (32)	//キーの総数

//前方宣言
class CModel;

//マップクラス
class CMotion
{
private:

	// 再生方法列挙型
	enum PLAYTYPE
	{
		PLAYTYPE_NORMAL = 0,
		PLAYTYPE_BLEND,
	};

	//キーの構造体
	struct KEY
	{
		float fPosX;		//位置X
		float fPosY;		//位置Y
		float fPosZ;		//位置Z
		float fRotX;		//向きX
		float fRotY;		//向きY
		float fRotZ;		//向きZ

		// コンストラクタ
		KEY() : fPosX(0.0f), fPosY(0.0f), fPosZ(0.0f),
			fRotX(0.0f), fRotY(0.0f), fRotZ(0.0f) {}
	};

	//キー情報の構造体
	struct KEY_INFO
	{
		int nFrame;					// 再生総フレーム
		KEY aKey[MAX_PARTS];		// 各モデルのキー要素[パーツの総数]

		// コンストラクタ
		KEY_INFO() : nFrame(0), aKey() {}
	};

	//モーション情報の構造体
	struct INFO
	{
		bool bLoop;						// ループするかどうか
		int NumKey;						// キー使用総数
		KEY_INFO aKeyInfo[MAX_KEY];		// キー情報[キーの総数]

		// コンストラクタ
		INFO() : bLoop(false), NumKey(0), aKeyInfo() {}
	};

	// 使用階層構造モデル管理用の構造体
	struct SBodyData
	{
		CModel** ppModel;		//モデルへのポインタ
		int nNumModel;			//モデルの総数

		// コンストラクタ
		SBodyData() : ppModel(nullptr), nNumModel(0) {}
	};

	// モーションブレンド用の構造体
	struct SBlendInfo
	{
		int nOldId;				// ブレンド前の前回のモーション
		int nOldKey;			// ブレンド前の前回のキー
		float fCounter;			// 経過カウント
		float fTime;			// 残りカウント
		float fTimeMax;			// 総カウント

		// コンストラクタ
		SBlendInfo() : nOldId(0), nOldKey(0),
			fCounter(0.0f), fTime(0.0f), fTimeMax(0.0f) {}
	};

	// モーション再生用の構造体
	struct SPlayInfo
	{
		int nId;		// 現在のモーション番号
		int nKey;		// 現在のキー
		int nKeyNext;	// 次のキー
		float fCounter;	// 再生カウント
		bool bFinish;	// 終了フラグ
		SBlendInfo blendInfo;	// ブレンド用情報
		PLAYTYPE type;		// 再生方法の種類

		// コンストラクタ
		SPlayInfo() : nId(0), nKey(0), nKeyNext(1),
			fCounter(0.0f), bFinish(false), blendInfo(SBlendInfo()), type(PLAYTYPE::PLAYTYPE_NORMAL) {}
	};

public:

	CMotion();
	~CMotion();

	// メンバ関数
	void Update(void);
	void Set(int nType, float BlendTime);
	void SetModel(CModel** ppModel, int nNumModel);
	void SetSlowVaule(float fValue) { m_fSlowVaule = fValue; }
	void LoadData(const std::string pFilename);

	// メンバ関数(取得系)
	int GetType(void) { return m_PlayInfo.nId; }
	int GetKey(void) { return m_PlayInfo.nKey; }
	float GetCounter(void) { return m_PlayInfo.fCounter; }
	bool GetFinish(void) { return m_PlayInfo.bFinish; }
	bool GetInfoLoop(int nMotion);
	int GetInfoNumKey(int nMotion);
	int GetInfoKeyFrame(int nMotion, int nKey);

	// メンバ関数(設定系)
	void SetInfoLoop(bool bLoop, int nMotion) { m_aInfo[nMotion].bLoop = bLoop; }
	void SetInfoNumKey(int nNumKey, int nMotion) { m_aInfo[nMotion].NumKey = nNumKey; }
	void SetInfoKeyFrame(int nFrame, int nMotion, int nKey) { m_aInfo[nMotion].aKeyInfo[nKey].nFrame = nFrame; }

	// キー情報設定関数 ( パラメータ, モーション番号, キー番号, パーツ番号)
	void SetInfoPartsPosX(float fPosX, int nMotion, int nKey, int nParts) { m_aInfo[nMotion].aKeyInfo[nKey].aKey[nParts].fPosX = fPosX; }
	void SetInfoPartsPosY(float fPosY, int nMotion, int nKey, int nParts) { m_aInfo[nMotion].aKeyInfo[nKey].aKey[nParts].fPosY = fPosY; }
	void SetInfoPartsPosZ(float fPosZ, int nMotion, int nKey, int nParts) { m_aInfo[nMotion].aKeyInfo[nKey].aKey[nParts].fPosZ = fPosZ; }
	void SetInfoPartsRotX(float fRotX, int nMotion, int nKey, int nParts) { m_aInfo[nMotion].aKeyInfo[nKey].aKey[nParts].fRotX = fRotX; }
	void SetInfoPartsRotY(float fRotY, int nMotion, int nKey, int nParts) { m_aInfo[nMotion].aKeyInfo[nKey].aKey[nParts].fRotY = fRotY; }
	void SetInfoPartsRotZ(float fRotZ, int nMotion, int nKey, int nParts) { m_aInfo[nMotion].aKeyInfo[nKey].aKey[nParts].fRotZ = fRotZ; }

	int GetFullFrame(int nIdx);

private:
	//=============================
	// 関数リスト
	//=============================
	typedef void(CMotion::* PLAYTYPE_FUNC)();
	static PLAYTYPE_FUNC m_PlayTypeFunc[];

	// 状態設定関数
	void PlayTypeNormal();
	void PlayTypeBlend();

	// メンバ変数
	INFO m_aInfo[MAX_INFO];		// モーションの情報[モーションの総数]
	SPlayInfo m_PlayInfo;		// モーション再生用の情報
	SBodyData m_BodyData;		// 階層モデルの情報
	float m_fSlowVaule;			//スロー(カウントに掛ける用)
};
#endif