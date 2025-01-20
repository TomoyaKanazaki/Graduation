//===========================================================
//
// エフェクシア[MyEffekseer.h]
// Author 大原怜将
//
//===========================================================
#ifndef _EFFEKSEER_H_     //このマクロが定義されてなかったら
#define _EFFEKSEER_H_     //2重インクルードを防止のマクロを定義する

#include <list>

//===========================================================
// 前方宣言
//===========================================================v
class CEffekseer;

//===========================================================
// エフェクシアのコントロールクラス定義
//===========================================================
class CMyEffekseer
{
public:
	CMyEffekseer();   // コンストラクタ
	~CMyEffekseer();  // デストラクタ

	// 種類
	enum TYPE
	{
		TYPE_FIRE = 0,				// ファイアボール
		TYPE_CHARGE,				// 火炎放射チャージ
		TYPE_SMOKE,					// スモーク
		TYPE_HIT,					// ヒット
		TYPE_GETITEM,				// 聖書、十字架取得
		TYPE_EAT,					// 捕食
		TYPE_RESPAWN_MEDAMAN,		// メダマンのリスポーン
		TYPE_RESPAWN_BONBON,		// ボンボンのリスポーン
		TYPE_RESPAWN_SMALLDEVIL,	// コデビルのリスポーン
		TYPE_BOABOA,				// ボアボアエフェクト
		TYPE_CROSS,					// 十字架エフェクト
		TYPE_BIBLE,					// 聖書
		TYPE_DUSTCLOUD,				// 歩いた時の煙
		TYPE_FALLSMOKE,				// 落下時の土煙
		TYPE_BORN_PLAYER_00,		// 卵が落下00
		TYPE_BORN_PLAYER_01,		// 卵が落下01
		TYPE_DROP,					// タマゴンの汗
		TYPE_ACCELE,				// タマゴンの加速
		TYPE_ROLL,					// 石が転がる
		TYPE_HITTHEWALL,			// 壁に火があたる
		TYPE_HIT_SMALLDEVIL,		// 小デビル当たる
		TYPE_STEAM,					// 卵から出る湯気
		TYPE_BORNGUID1,				// 復活ガイド　プレイヤー1
		TYPE_BORNGUID2,				// 復活ガイド　プレイヤー2
		TYPE_GOT_THEITEM,			// アイテム持っているとき
		TYPE_ACENTION,				// 昇天
		TYPE_REVIVE_MEDAMAN,		// メダマンの復活ガイド
		TYPE_REVIVE_BONBON,			// ボンボンの復活ガイド
		TYPE_REVIVE_SMALLDEVIL,		// 子デビルの復活ガイド
		TYPE_GET_SOFTCREAM,			// ソフトクリーム取得
		TYPE_SOFTCREAM,				// ソフトクリームのオーラ
		TYPE_CRASH_CROSS,			// 十字架を失う時
		TYPE_SPAWN_DEVIL,			// デビルの出現
		TYPE_QUAKE,					// 揺れる時の破片
		TYPE_TRUE,					// デバッグtrue
		TYPE_STAGE_LIMIT,			// ステージ端
		TYPE_UNLOCKING,				// 解錠
		TYPE_MAX,
		TYPE_NONE					// なんもない
	};

	void Init(void);       // 初期化処理
	void Uninit(void);     // 終了処理
	void Update(void);     // 更新処理
	void Draw(void);       // 描画処理

	// 取得
	::EffekseerRendererDX9::RendererRef GetEffekseerRenderer(void) { return m_EfkRenderer; }  // エフェクトレンダラー
	::Effekseer::ManagerRef GetEfkManager(void) { return m_EfkManager; }                      // エフェクトマネージャ

	// モジュール
	void SetupEffekseerModules(::Effekseer::ManagerRef efkManager);

	CEffekseer* CreateEffect(const TYPE eType, ::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale, bool bLoop);

private:

	EffekseerRendererDX9::RendererRef m_EfkRenderer;
	::Effekseer::Matrix44 m_CameraMatrix;
	::Effekseer::Matrix44 m_ProjectionMatrix;
	::Effekseer::ManagerRef m_EfkManager;
	::Effekseer::Vector3D m_ViewerPosition;
	int m_nNum;
	int32_t m_nTime;
};

//===========================================================
// エフェクシアクラス定義
//===========================================================
class CEffekseer
{
public:
	CEffekseer();     // コンストラクタ
	~CEffekseer();    // デストラクタ

	void Init(Effekseer::Vector3D pos, Effekseer::Vector3D rot, Effekseer::Vector3D scale, bool bLoop); // 初期化処理
	void Uninit(void); // 終了処理 

	// 設定
	void SetPosition(Effekseer::Vector3D pos) { m_pos = pos; } // 位置
	void SetRotation(Effekseer::Vector3D rot) { m_rot = rot; } // 向き
	void SetScale(Effekseer::Vector3D scale) { m_scale = scale; } // 大きさ
	void SetEffect(Effekseer::EffectRef effect) { m_effect = effect; } // エフェクト
	void SetEfkHandle(Effekseer::Handle handle) { m_efkHandle = handle; } // ハンドル
	void SetEfkName(const char* pName) { m_EfkName = pName; } // エフェクトの名前
	void SetEfkType(const CMyEffekseer::TYPE eType) { m_eType = eType; } // エフェクトの種類
	void SetDeath(); // 終了設定

	// 取得
	Effekseer::Vector3D GetPosition(void) { return m_pos; }				// 位置
	Effekseer::Vector3D GetRotation(void) { return m_rot; }				// 向き
	Effekseer::Vector3D GetScale(void) { return m_scale; }				// 大きさ
	Effekseer::EffectRef GetEffect(void) { return m_effect; }			// エフェクト
	Effekseer::Handle GetHandle(void) { return m_efkHandle; }			// ハンドル
	const char* GetEfkName(void) { return m_EfkName; }					// エフェクトの名前
	CMyEffekseer::TYPE GetEfkType() { return m_eType; }					// エフェクトの種類
	bool IsLoop(void) { return m_bLoop; }								// ループするフラグ
	bool IsDeath(void) { return m_bDeath; }								// 死亡フラグ

	// 静的メンバ関数
	static CListManager<CEffekseer>* GetList(); // リスト取得

private:

	// メンバ変数
	Effekseer::Vector3D m_pos; // 位置
	Effekseer::Vector3D m_rot; // 向き
	Effekseer::Vector3D m_scale; // 大きさ
	Effekseer::EffectRef m_effect; // エフェクト
	Effekseer::Handle m_efkHandle; // ハンドル
	const char* m_EfkName; // 名前
	CMyEffekseer::TYPE m_eType; // 種類
	bool m_bLoop; // ループするフラグ
	bool m_bDeath; // 死亡フラグ
	CListManager<CEffekseer>::AIterator m_iterator; // イテレーター

	// 静的メンバ変数
	static CListManager<CEffekseer>* m_pList; // オブジェクトリスト
};

namespace MyEffekseer
{
	CEffekseer* EffectCreate(CMyEffekseer::TYPE type, 
		                     bool bLoop = false,
		                     D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f }, 
		                     D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f },
		                     D3DXVECTOR3 scale = { 25.0f, 25.0f, 25.0f } );
}

#endif