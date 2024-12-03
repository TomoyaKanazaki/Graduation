//============================================
//
//	アイテムの処理 [item.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"
#include "objectX.h"
#include "MapSystem.h"

// 前方宣言
class CPlayer;
class CShadow;

//オブジェクトアイテムクラス
class CItem : public CObjectX
{
public:

	enum TYPE
	{// アイテムの種類
		TYPE_CROSS = 0,	// 十字架
		TYPE_BIBLE,		// 聖書
		TYPE_BOWABOWA,	// ぼわぼわ
		TYPE_SOFTCREAM,	// ソフトクリーム
		TYPE_FRIEDEGG,	// 目玉焼き
		TYPE_MAX,
		TYPE_NONE
	};

	// メンバ変数
	CItem(int nPriority = 2);
	~CItem();

	HRESULT Init(const char* pModelName);
	HRESULT Init() { assert(false); return E_FAIL; }; // 呼ばれてはならない
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// テクスチャ番号取得
	int GetIdx(void) { return m_nIdxTexture; }

	// Xモデル番号取得
	int GetIdxXModel(void) { return -1; }

	//移動量設定・取得
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	//大きさ設定・取得
	void SetScaling(const float& Scaling) { m_Scaling = Scaling; }
	float GetScaling(void) { return m_Scaling; }

	// 状態管理の取得
	int GetStateCounter() { return m_nStateCount; }

	// マップ番号の設定
	void SetGrid(const CMapSystem::GRID& pos);
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	// スクロールするかどうかの設定・取得
	void SetMapScroll(bool frag) { m_bMapScroll = frag; }
	bool GetMapScroll() { return m_bMapScroll; }

	bool CollisionPlayer();
	virtual bool Hit(CPlayer* pPlayer) = 0;
	void SetItem(const TYPE eType) { m_eType = eType; }

	D3DXVECTOR3 GetBase() { return m_posBase; } // 移動の中心位置を取得
	float GetMoveTime() { return m_fMoveTime; } // 移動時間の取得
	void SetMoveTime(const float time) { m_fMoveTime = time; } // 移動時間の取得
	void Effect(CEffekseer* pEffect) { m_pEffect = pEffect; }
	CEffekseer* GetEffect() { return m_pEffect; }

	// 静的メンバ関数
	static CListManager<CItem>* GetList(void); // リスト取得
	static CItem* Create(const TYPE eType, const CMapSystem::GRID& pos);

private:

	// メンバ関数
	virtual void Move(D3DXVECTOR3& pos); // 移動処理
	virtual void SetEffect() {};

	// メンバ変数
	CListManager<CItem>::AIterator m_iterator; // イテレーター

	TYPE m_eType;			// 種類

	int m_nIdxXModel;		//Xモデルの番号
	int m_nIdxTexture;		//テクスチャの番号
	int m_nStateCount;		//状態管理用変数

	float m_fColorA;		//不透明度
	float m_Scaling;		//大きさ

	bool m_UseItem;			//アイテムが使用可能かどうか
	bool m_bCollision;		//当たり判定用の座標
	bool m_bMapScroll;		//マップのスクロール

	D3DXVECTOR3 m_CollisionPos;	//当たり判定用の座標
	D3DXVECTOR3 m_move;			//移動量	

	D3DXVECTOR3 m_posBase; // 移動の中心座標
	float m_fMoveTime; // 移動時間

	CMapSystem::GRID m_Grid;	// マップ番号
	CShadow* m_pShadow;		// シャドウのポインタ
	CEffekseer* m_pEffect; // エフェクト

	// 静的メンバ変数
	static CListManager<CItem>* m_pList; // オブジェクトリスト
};

#endif