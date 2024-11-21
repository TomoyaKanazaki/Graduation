//===========================================================
//
// エフェクシア[MyEffekseer.cpp]
// Author 大原怜将
//
//===========================================================
#include <stdio.h>
#include <string>
#include <windows.h>

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "MyEffekseer.h"
#include "camera.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const float DELTA_TIME = 60.0f;
}

//==========================================
//  静的メンバ変数宣言
//==========================================
CMyEffekseer* CMyEffekseer::m_pInstance = nullptr; // 自身のポインタ

namespace MyEffekseer
{
	// エフェクトの名前
	const char* EFFECT_PATH[] =
	{
		"data\\EFFEKSEER\\Effect\\fireball.efkefc",       // ファイアボール
		"data\\EFFEKSEER\\Effect\\charge.efkefc",         // チャージ
		"data\\EFFEKSEER\\Effect\\smoke.efkefc",          // スモーク
		"data\\EFFEKSEER\\Effect\\hit.efkefc",			  // ヒット
		"data\\EFFEKSEER\\Effect\\get_items.efkefc",      // アイテム取得
		"data\\EFFEKSEER\\Effect\\eat.efkefc",			  // 捕食
		"data\\EFFEKSEER\\Effect\\medaman_respawn.efkefc",			  // メダマンのリスポーン
		"data\\EFFEKSEER\\Effect\\bonbon_respawn.efkefc",			  // ボンボンのリスポーン
		"data\\EFFEKSEER\\Effect\\smalldevil_respawn.efkefc",		  // コデビルのリスポーン
		"data\\EFFEKSEER\\Effect\\boaboa.efkefc",		  // ボアボア
		"data\\EFFEKSEER\\Effect\\cross.efkefc",		  // 十字架
		"data\\EFFEKSEER\\Effect\\bible.efkefc",		  // 聖書
		"data\\EFFEKSEER\\Effect\\dustcloud.efkefc",		  // 歩いた時の砂埃
		"data\\EFFEKSEER\\Effect\\fall_smoke.efkefc",		  // 落下の土煙
		"data\\EFFEKSEER\\Effect\\born_player00.efkefc",		  // プレイヤー１の卵落下
		"data\\EFFEKSEER\\Effect\\born_player01.efkefc",		  // プレイヤー2の卵落下
	};

	//============================================
	// エフェクトの生成
	// パラメーター
	// type  : 列挙
	// bLopp : ループするかのフラグ
	// pos   : エフェクトの位置
	// rot   : エフェクトの向き
	// scale : エフェクトの大きさ
	// 
	// 戻り値
	// エフェクト。失敗したときはnullptrが返る
	//============================================
	CEffekseer* EffectCreate(CMyEffekseer::TYPE type, bool bLoop, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
	{
		// エフェクシア取得
		CMyEffekseer* pMyEffekseer = CManager::GetInstance()->GetEffect();

		// 使用されていない場合処理を抜ける
		if (pMyEffekseer == nullptr) { assert(false); return nullptr; }

		// エフェクトを生成
		CEffekseer* pEffekseer = pMyEffekseer->CreateEffect(type,
			Effekseer::Vector3D(pos.x, pos.y, pos.z),
			Effekseer::Vector3D(rot.x, rot.y, rot.z),
			Effekseer::Vector3D(scale.x, scale.y, scale.z),
			bLoop);

		return pEffekseer;
	}
}

//==========================================
//  静的警告処理
//==========================================
static_assert(NUM_ARRAY(MyEffekseer::EFFECT_PATH) == CMyEffekseer::TYPE_MAX, "ERROR : Type Count Missmatch");

//===========================================================
// コンストラクタ
//===========================================================
CMyEffekseer::CMyEffekseer()
{
}

//===========================================================
// デストラクタ
//===========================================================
CMyEffekseer::~CMyEffekseer()
{
}

//===========================================================
// 生成処理
//===========================================================
CEffekseer* CMyEffekseer::CreateEffect(const TYPE eType, ::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale, bool bLoop)
{
	CEffekseer* pEffect = new CEffekseer;

	if (pEffect == nullptr)
		return nullptr;

	// char16_tに変換
	std::string file = MyEffekseer::EFFECT_PATH[eType];
	int len = MultiByteToWideChar(CP_UTF8, 0, file.c_str(), -1, nullptr, 0);
	std::u16string string16t(len, 0);
	MultiByteToWideChar(CP_UTF8, 0, file.c_str(), -1, reinterpret_cast<LPWSTR>(&string16t[0]), len);

	// エフェクトの読込
	Effekseer::EffectRef effect = Effekseer::Effect::Create(m_EfkManager, string16t.c_str());
	pEffect->SetEffect(effect);

	// エフェクトの再生
	Effekseer::Handle Handle = m_EfkManager->Play(effect, 0, 0, 0);
	pEffect->SetEfkHandle(Handle);

	// 種類設定
	pEffect->SetEfkType(eType);

	// エフェクトネーム設定
	pEffect->SetEfkName(MyEffekseer::EFFECT_PATH[eType]);

	// 位置設定
	pEffect->SetPosition(pos);

	// 向き設定
	pEffect->SetRotation(rot);

	// 大きさ設定
	pEffect->SetScale(scale);

	// 初期化処理
	pEffect->Init(pos, rot, scale, bLoop);

	// 位置、向き、大きさ設定
	m_EfkManager->SetLocation(Handle, pos);
	m_EfkManager->SetRotation(Handle, rot.X, rot.Y, rot.Z);
	m_EfkManager->SetScale(Handle, scale.X, scale.Y, scale.Z);

	return pEffect;
}

//===========================================================
// 初期化処理
//===========================================================
void CMyEffekseer::Init(void)
{
	// エフェクトのマネージャーの作成
	m_EfkManager = ::Effekseer::Manager::Create(8000);

	// 視点位置を確定
	m_ViewerPosition = ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f);

	// 座標系を設定する。アプリケーションと一致させる必要がある。
	m_EfkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// Effekseerのモジュールをセットアップする
	SetupEffekseerModules(m_EfkManager);

	// 投影行列を設定
	m_ProjectionMatrix.PerspectiveFovLH(0.5f * D3DX_PI, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f);

	// カメラ行列を設定
	m_CameraMatrix.LookAtLH(m_ViewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));
}

//===========================================================
// 終了処理
//===========================================================
void CMyEffekseer::Uninit(void)
{
	// エフェクトリストを取得
	if (CEffekseer::GetList() == nullptr) { return; }
	std::list<CEffekseer*> list = CEffekseer::GetList()->GetList();    // リストを取得
	
	// リストの中身を削除する
	for (CEffekseer* effect : list)
	{
		effect->Uninit();
	}

	// 自身を終了する
	delete m_pInstance;
}

//===========================================================
// 更新処理
//===========================================================
void CMyEffekseer::Update(void)
{
	// タイム加算
	m_nTime++;

	// エフェクトリストを取得
	if (CEffekseer::GetList() == nullptr) { return; }
	std::list<CEffekseer*> list = CEffekseer::GetList()->GetList();    // リストを取得

	for(CEffekseer* effect : list)
	{
		// エフェクト情報を取得
		Effekseer::Handle Handle = effect->GetHandle();
		Effekseer::Vector3D pos = effect->GetPosition();
		Effekseer::Vector3D rot = effect->GetRotation();
		Effekseer::Vector3D scale = effect->GetScale();

		// 位置や向き、大きさを再設定
		m_EfkManager->SetLocation(Handle, pos);
		m_EfkManager->SetRotation(Handle, rot.X, rot.Y, rot.Z);
		m_EfkManager->SetScale(Handle, scale.X, scale.Y, scale.Z);

		// エフェクトの再生が終了していない場合次に進む
		if (m_EfkManager->Exists(Handle) && !effect->IsDeath()) { continue; }
		
		// 再生の停止
		m_EfkManager->StopEffect(Handle);

		// ループエフェクトでない場合終了して次に進む
		if (!effect->IsLoop() || effect->IsDeath())
		{
			// 終了処理 
			effect->Uninit();
			continue;
		}

		// エフェクト本体の情報を取得する
		Effekseer::EffectRef reference = effect->GetEffect();

		// エフェクトの再生
		Handle = m_EfkManager->Play(reference, pos);

		// ハンドルの設定
		effect->SetEfkHandle(Handle);
	}

	// レイヤーパラメータの設定
	Effekseer::Manager::LayerParameter layerParameter;
	layerParameter.ViewerPosition = m_ViewerPosition;
	m_EfkManager->SetLayerParameter(0, layerParameter);

	// マネージャーの更新
	Effekseer::Manager::UpdateParameter updateParameter;
	m_EfkManager->Update(updateParameter);
}

//===========================================================
// 描画処理
//===========================================================
void CMyEffekseer::Draw(void)
{
	// レンダラーが使用されていない場合処理を抜ける
	if (m_EfkRenderer == nullptr)
		return;

	// タイム設定
	m_EfkRenderer->SetTime(m_nTime / DELTA_TIME);

	// 投影行列を設定
	m_EfkRenderer->SetProjectionMatrix(m_ProjectionMatrix);

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// カメラが使用されていない場合処理抜ける
	if (pCamera == nullptr)
		return;

	auto ViewMatrix = pCamera->GetViewMatrix();
	auto Projection = pCamera->GetProjectionMatrix();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_ProjectionMatrix.Values[i][j] = Projection.m[i][j];
			m_CameraMatrix.Values[i][j] = ViewMatrix.m[i][j];
		}
	}

	// カメラ行列を設定
	m_EfkRenderer->SetCameraMatrix(m_CameraMatrix);

	// エフェクトの描画開始処理
	m_EfkRenderer->BeginRendering();

	// エフェクトの描画
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix = m_EfkRenderer->GetCameraProjectionMatrix();
	m_EfkManager->Draw(drawParameter);

	// エフェクトの描画終了処理
	m_EfkRenderer->EndRendering();
}

//==========================================
//  自身の取得
//==========================================
CMyEffekseer* CMyEffekseer::GetInstance(void)
{
	// 自身が存在していない場合生成する
	if (m_pInstance == nullptr)
	{
		assert(false);
		return nullptr;
	}

	return m_pInstance;
}

//===========================================================
// モジュール設定
//===========================================================
void CMyEffekseer::SetupEffekseerModules(::Effekseer::ManagerRef efkManager)
{
	// 描画デバイスの作成
	Effekseer::Backend::GraphicsDeviceRef graphicsDevice;
	graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(CManager::GetInstance()->GetRenderer()->GetDevice());

	// エフェクトのレンダラーの作成
	m_EfkRenderer = ::EffekseerRendererDX9::Renderer::Create(graphicsDevice, 8000);

	// 描画モジュールの設定
	efkManager->SetSpriteRenderer(m_EfkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(m_EfkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(m_EfkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(m_EfkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(m_EfkRenderer->CreateModelRenderer());

	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる
	efkManager->SetTextureLoader(m_EfkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(m_EfkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(m_EfkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

//===========================================================================
// エフェクトクラス
//===========================================================================

//==========================================
//  静的メンバ変数宣言
//==========================================
CListManager<CEffekseer>* CEffekseer::m_pList = nullptr; // オブジェクトリスト]

//===========================================================
// コンストラクタ
//===========================================================
CEffekseer::CEffekseer() :
	m_eType(CMyEffekseer::TYPE_NONE),
	m_bDeath(false)
{

}

//===========================================================
// デストラクタ
//===========================================================
CEffekseer::~CEffekseer()
{

}

//===========================================================
// 初期化処理
//===========================================================
void CEffekseer::Init(Effekseer::Vector3D pos, Effekseer::Vector3D rot, Effekseer::Vector3D scale, bool bLoop)
{
	m_bLoop = bLoop;

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CEffekseer>::Create();
		if (m_pList == nullptr) { assert(false); }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);
}

//===========================================================
// 終了処理
//===========================================================
void CEffekseer::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 自身の終了
	delete this;
}

//==========================================
//  死亡設定
//==========================================
void CEffekseer::SetDeath()
{
	// 再生の停止
	CManager::GetInstance()->GetEffect()->GetEfkManager()->StopEffect(m_efkHandle);

	// フラグの設定
	m_bDeath = true;
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CEffekseer>* CEffekseer::GetList(void)
{
	return m_pList;
}
