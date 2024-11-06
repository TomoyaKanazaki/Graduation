//==========================================
//
//  経過時間計測
//  Author : Tomoya Kanazaki
//
//==========================================
#include "deltatime.h"

//==========================================
//  無名名前空間
//==========================================
namespace
{
	DWORD m_dwOldTime; // 前回フレームの時間
	float m_fDeltaTime; // 瞬間の経過時間(秒)
}

//==========================================
//  初期化処理
//==========================================
void DeltaTime::Init()
{
	m_dwOldTime = timeGetTime();
	m_fDeltaTime = 0.0f;
}

//==========================================
//  更新
//==========================================
void DeltaTime::Update()
{
	// 現在の時間を取得
	DWORD Time = timeGetTime();

	// 前回からの差分を求める
	float Delta = (float)(Time - m_dwOldTime);

	// 差分を経過時間として使える形に変換
	m_fDeltaTime = Delta * 0.001f;

	// 現在時間を保存する
	m_dwOldTime = Time;

	// 経過時間を表示する
	DebugProc::Print(DebugProc::POINT_CENTER, "ΔTime : %f\n", m_fDeltaTime);
}

//==========================================
//  取得
//==========================================
float DeltaTime::Get()
{
	return m_fDeltaTime;
}
