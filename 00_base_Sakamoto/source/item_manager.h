#pragma once
//==========================================
//
//  アイテム管理クラス(item_manager.h)
//  Author : Tomoya Kanazaki
//
//==========================================

//==========================================
//  前方宣言
//==========================================
class CItem;

//==========================================
//  クラス定義
//==========================================
class CItemManager
{
public:

	// 配置タイプ列挙
	enum EType
	{
		TYPE_BOWABOWA = 0, // ボワボワ配置状態
		TYPE_BIBLE, // 聖書配置状態
		TYPE_MAX,
		TYPE_NONE
	};

	// メンバ関数
	CItemManager();
	~CItemManager();

	void Init(const int nMap, const EType type);
	void Uninit();
	void Update();
	void Draw();

	void SetType(const EType type) { m_eType = type; ItemSeting(); } // 配置タイプの変更

	// 静的メンバ関数
	static CItemManager* Create(const int nMap, const EType type = TYPE_BOWABOWA);
	static HRESULT Load();
	static void UnLoad();

private:

	// アイテム管理構造体
	struct SItems
	{
		int nNum; // アイテム配置総数
		D3DXVECTOR3* pPos; // アイテムの配置座標
		CItem::TYPE* pType; // アイテムの種類
	};

	// メンバ関数
	void ItemSeting(); // アイテムの設置処理

	// メンバ変数
	EType m_eType; // 配置タイプ
	SItems* m_pItems; //現在使われているアイテムのデータ

	// 静的メンバ変数
	static int ms_nNumMap; // 配置されるデータの数
	static SItems* ms_pItems; // アイテム管理用変数
};
