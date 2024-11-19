//===========================================================
//
// エフェクシア[MyEffekseer.h]
// Author 大原怜将
//
//===========================================================
#ifndef _EFFEKSEER_H_     //このマクロが定義されてなかったら
#define _EFFEKSEER_H_     //2重インクルードを防止のマクロを定義する

#define MAX_EFK   (64)

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
		TYPE_FIRE = 0, // ファイアボール

		TYPE_MAX,
		TYPE_NONE  // なんもない
	};

	void Init(void);       // 初期化処理
	void Uninit(void);     // 終了処理
	void Update(void);     // 更新処理
	void Draw(void);       // 描画処理

	// 取得
	::EffekseerRendererDX9::RendererRef GetEffekseerRenderer(void) { return m_EfkRenderer; }  // エフェクトレンダラー
	::Effekseer::ManagerRef GetEfkManager(void) { return m_EfkManager; }                      // エフェクトマネージャ

	static CMyEffekseer* GetInstance(void) { return m_pInstance; }  // 自分自身

	// モジュール
	void SetupEffekseerModules(::Effekseer::ManagerRef efkManager);

	CEffekseer* CreateEffect(const TYPE eType, ::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale, bool bLoop);

	void ListIn(CEffekseer* pEffect);  // リストに挿入

	void Release(CEffekseer* pEffect);  // 指定した要素を削除
	void ReleaseAll(void);              // すべての要素を削除

private:

	std::list<CEffekseer*> m_EffectList;  // エフェクトのリスト

	static CMyEffekseer* m_pInstance;  // 自分自身のポインタ
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

	// 取得
	Effekseer::Vector3D GetPosition(void) { return m_pos; }				// 位置
	Effekseer::Vector3D GetRotation(void) { return m_rot; }				// 向き
	Effekseer::Vector3D GetScale(void) { return m_scale; }				// 大きさ
	Effekseer::EffectRef GetEffect(void) { return m_effect; }			// エフェクト
	Effekseer::Handle GetHandle(void) { return m_efkHandle; }			// ハンドル
	const char* GetEfkName(void) { return m_EfkName; }					// エフェクトの名前
	CMyEffekseer::TYPE GetEfkType() { return m_eType; }					// エフェクトの種類
	bool IsLoop(void) { return m_bLoop; }								// ループするフラグ

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
	
};

namespace MyEffekseer
{
	CEffekseer* EffectCreate(CMyEffekseer::TYPE type, 
		                     bool bLoop = false,
		                     D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f }, 
		                     D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f },
		                     D3DXVECTOR3 scale = { 500.0f, 500.0f, 500.0f } );
}

#endif