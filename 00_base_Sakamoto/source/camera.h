//============================================
//
//	カメラの処理 [camera.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//カメラクラス
class CCamera
{
public:
	CCamera();
	~CCamera();

	//カメラのモード
	enum CAMERAMODE
	{
		CAMERAMODE_CONTROL = 0,		//操作
		CAMERAMODE_FIXEDNOW,		//現在のカメラで固定
		CAMERAMODE_BETWEEN,			//2点の間に位置するカメラ
		CAMERAMODE_DOWNVIEW,		//見下ろし
		CAMERAMODE_SIDEVIEW,		//2D
		CAMERAMODE_EVENTBOSS,		//イベント中のボス注目
		CAMERAMODE_FPS,				//一人称視点
		CAMERAMODE_FPSCOMPLEMENT,	//一人称視点変更時の補完用
		CAMERAMODE_SETPOS,			//指定位置へ移動
		CAMERAMODE_AROUND,			//視点を中心に回転する
		CAMERAMODE_SEAMLESS,		//シームレス
		CAMERAMODE_MAX,
	};

	enum CAMERATYPE
	{
		CAMERATYPE_MAIN = 0,	// メインカメラ
		CAMERATYPE_SIGNAL,		// 矢印カメラ
		CAMERATYPE_DEVIL,		// デビルカメラ
	};
	
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void SetCameraPos(D3DXVECTOR3 Pos) { m_posV = Pos; m_posR = Pos; }
	void SetCameraPosMode(D3DXVECTOR3 PosV, D3DXVECTOR3 PosR) { m_SetposV = PosV; m_SetposR = PosR; }
	D3DXVECTOR3 GetCameraPos(void) { return m_posR;}
	void SetCameraPosY(float PosY) { m_EventPosY = PosY; }
	void ResetCamera(void);
	void SetBib(bool Set) { m_bBib = Set; }
	float GetPosY(void) { return m_posR.y; }
	void SetRot(D3DXVECTOR3 Rot) { m_rot = Rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetCameraMode(CAMERAMODE Mode) { CameraMode = Mode; }
	CAMERAMODE GetCameraMode(void) { return CameraMode; }
	void SetFollowTime(int Time) { m_FollowTime = Time; }
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXVECTOR3 GetVecU(void) { return m_vecU; }
	void SetAttention(bool Set) { m_bAttention = Set; }
	bool GetAttention(void) { return m_bAttention; }
	D3DXMATRIX GetViewMatrix() { return m_mtxView; }
	D3DXMATRIX GetProjectionMatrix() { return m_mtxProjection; }
	void SetHomingSpeed(float Speed) { m_fHomingSpeed = Speed; }
	void SetCameraDistance(float Dis) { m_CameraDistance = Dis; }

	void SetCameraCount(int nCount) { m_nCameraCount = nCount; }
	int GetCameraCount(void) { return m_nCameraCount; }

	void SetAroundRot(float Rot) { m_fAroundRot = Rot; }
	float GetAroundRot(void) { return m_fAroundRot; }
	void SetAroundDistance(float Distance) { m_fAroundDistance = Distance; }
	float GetAroundDistance(void) { return m_fAroundDistance; }

	// 静的メンバ関数
	static CListManager<CCamera>* GetList(void); // リスト取得

private:
	void ControlCamera(void);
	void FixedNow(void);
	void BetWeen(void);
	void DownviewCamera(void);
	void SideviewCamera(void);
	void EventBossCamera(void);
	void FPSCamera(void);
	void SetPosCamera(void);
	void AroundCamera(void);
	void TitleCamera(void);
	void SeamlessModeChangeCamera(void);

	void FPSComplementCamera(void);

	D3DXMATRIX m_mtxView;		//ビューマトリックス
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリックス
	D3DVIEWPORT9 m_viewport;	//ビューポート
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_posV;			//視点
	D3DXVECTOR3 m_posR;			//注視点
	D3DXVECTOR3 m_SetposV;		//指定カメラモード視点
	D3DXVECTOR3 m_SetposR;		//指定カメラモード注視点
	D3DXVECTOR3 m_vecU;			//上方向ベクトル
	D3DXVECTOR3 m_rot;			//現在の向き
	D3DXVECTOR3 m_rotOld;		//過去の向き
	D3DXVECTOR3 m_posVDest;		//目的の視点
	D3DXVECTOR3 m_posRDest;		//目的の注視点
	D3DXVECTOR3 m_PlayerPos;	//プレイヤーの位置
	D3DXVECTOR3 m_DelCameraPos;	//リスポーンカメラ位置
	CAMERAMODE CameraMode;		//カメラの状態
	D3DXVECTOR3 m_fRotMove;		//向きのカメラ移動速度
	float m_CameraDistance;		//注視点とカメラの距離
	float m_EventPosY;			//イベント時のカメラの高さ
	float m_fHomingSpeed;		//カメラ移動のホーミング速度
	float m_fAroundRot;			//視点中心回転カメラの向き
	float m_fAroundDistance;	//視点中心回転カメラとの距離
	bool m_bFollowY;			//カメラの上下追従をするかどうか
	bool m_bBib;				//カメラが振動するかどうか
	float m_fBibPowor;			//カメラ振動の強さ
	int m_FollowTime;			//回り込み時間
	int m_FollowDistance;		//追従時のカメラ距離
	bool m_bAttention;			//注目状態
	int m_nCameraCount;			//どのくらいこのカメラを維持するかというカウント

	// 静的メンバ変数
	static CListManager<CCamera>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CCamera>::AIterator m_iterator; // イテレーター

};

#endif