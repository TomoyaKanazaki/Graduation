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
		TYPE_MAX,
		TYPE_NONE
	};

	// メンバ変数
	CItem(int nPriority = 3);
	~CItem();

	HRESULT Init(const char* pModelName);
	HRESULT Init() { assert(false); return E_FAIL; }; // 呼ばれてはならない
	void Uninit(void);
	void Update(void);
	void Draw(void);

	virtual void Move(D3DXVECTOR3& pos) {} // 移動処理

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
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	//// マップの横番号の設定・取得
	//void SetWightNumber(int Wight) { m_nMapWidth = Wight; }
	//int GetWightNumber() { return m_nMapWidth; }

	//// マップの縦番号の設定・取得
	//void SetHeightNumber(int Height) { m_nMapHeight = Height; }
	//int GetHeightNumber() { return m_nMapHeight; }

	// スクロールするかどうかの設定・取得
	void SetMapScroll(bool Height) { m_bMapScroll = Height; }
	bool GetMapScroll() { return m_bMapScroll; }

	bool CollisionPlayer();
	virtual bool Hit(CPlayer* pPlayer) = 0;

	// 静的メンバ関数
	static CItem* Create(const TYPE eType, const CMapSystem::GRID& pos);

private:

	TYPE m_eType;		// 種類

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

	CMapSystem::GRID m_Grid;	// マップ番号
	//int m_nMapWidth;			//マップの横番号
	//int m_nMapHeight;			//マップの縦番号
};

#endif