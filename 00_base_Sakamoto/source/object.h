//============================================
//
//	オブジェクト [object.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

class CObject
{
public:

	// オブジェクトの種類
	enum OBJECT_TYPE
	{
		TYPE_NONE = 0,			// 使用禁止
		TYPE_OBJECT2D,			// オブジェクト2D
		TYPE_PLAYER3D,			// 3Dのプレイヤー
		TYPE_ENEMY3D,			// 3Dの敵
		TYPE_FIRE,				// 炎
		TYPE_TIME,				// 時間
		TYPE_NUMBER,			// 数字
		TYPE_TUTORIALUI,		// チュートリアルUI
		TYPE_BLOCK,				// ブロック
		TYPE_PLAYEREFFECT,		// プレイヤーのエフェクト
		TYPE_CUBEBLOCK,			// キューブ
		TYPE_CUBEEFFECT,		// キューブのエフェクト
		TYPE_2DUI,				// 2DのUI
		TYPE_MAPMODEL,			// マップ上のモデル
		TYPE_OBJMESHFIELD,		// フィールド
		TYPE_DEVIL,				// デビル
		TYPE_BOWABOWA,			// ボワボワ
		TYPE_CROSS,				// クロス
		TYPE_SCROLLALLOW,		// 矢印
		TYPE_DEVILHOLE,			// デビルホール
		TYPE_DEVILKEY,			// デビルホールにはめるやつ
		TYPE_BIBLE,				// 聖書
		TYPE_RAILBLOCK,			// 動くブロック
		TYPE_RAIL,				// レール
		TYPE_SOFTCREAM,			// ソフトクリーム
		TYPE_FRIEDEGG,			// 目玉焼き
		TYPE_TILE,				// 床
		TYPE_SIGNAL,			// 矢印
		TYPE_ROLLROCK,			// 岩
		TYPE_MAX,
	};

	//当たり判定の使用軸
	enum COLLISION
	{
		COLLISION_X = 0,
		COLLISION_Y,
		COLLISION_Z,
		COLLISION_MAX,
	};
	
	CObject(int nPriority = 3);
	virtual ~CObject();

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(int nCnt);
	static void MultiTargetDraw(void);
	static void ResetObjectMap(void);
	static void ReleaseEnemy(void);
	static void DeleteBlock(void);
	static void ScrollAll(void);

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static CObject *GetTop(int nPriority) { return m_pTop[nPriority]; }
	CObject *GetNext(void) { return m_pNext; }
	void SetDeathFlag(bool Set) { m_bDeath = Set; }
	bool GetDeathFlag(void) { return m_bDeath; }

	void SetType(OBJECT_TYPE type) { m_type = type; }
	OBJECT_TYPE GetType(void) { return m_type; }
	void SetAppear(bool Set) { m_Appear = Set; }
	bool GetAppear(void) { return m_Appear; }
	void SetDisp(bool Set) { m_bDisp = Set; }
	bool GetDisp(void) { return m_bDisp; }
	void SetMapScroll(bool Set) { m_bMapScroll = Set; }
	bool GetMapScroll(void) { return m_bMapScroll; }

	virtual void SetPos(D3DXVECTOR3 pos) {}
	virtual D3DXVECTOR3 GetPos(void) { return INITVECTOR3; }

	void SetIdxCamera(int nIdx) { m_nIdxCamera = nIdx; }

protected:
	void Release(void);

private:

	static CObject *m_pTop[PRIORITY_MAX];	//先頭のオブジェクトへのポインタ
	static CObject *m_pCur[PRIORITY_MAX];	//最後尾のオブジェクトへのポインタ
	CObject *m_pPrev;						//前のオブジェクトへのポインタ
	CObject *m_pNext;						//次のオブジェクトへのポインタ
	bool m_bDeath;							//死亡フラグ
	static int m_nNumAll;					//オブジェクト総数
	int m_Priority;							//描画の優先順位
	static bool m_bLevelStop;				//レベルアップ時のポーズ
	bool m_bLevelUI;						//レベルアップ時のUI
	bool m_Appear;							//表示状態かどうか
	bool m_bDisp;							//表示状態かどうか
	bool m_bMapScroll;						//スクロール状態かどうか
	OBJECT_TYPE m_type;						//オブジェクトの種類
	int m_nIdxCamera;						//カメラ番号
};

#endif