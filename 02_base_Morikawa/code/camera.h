//=======================================
//
//カメラ[camera.h]
//Author　:　MORIKAWA SHUNYA
//
//=======================================

#ifndef _CAMERA_H_     //このマクロ定義が定義されていなかったら
#define _CAMERA_H_     //2連インクルード防止のマクロを定義する

#include "main.h"

//=======================================
//カメラクラス
//=======================================
class CCamera
{
public:
	CCamera();		//コンストラクタ
	~CCamera();		//デストラクタ

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void CameraMoveV(void);
	float RotNor(float RotN);
	void Following(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void TitleCamera();
	void CollisionArena();

	D3DXVECTOR3 GetRot(void);				//位置取得
	D3DXVECTOR3 GetPosV(void);				//視点の取得
	D3DXVECTOR3 GetPosR(void);				//注視点の取得
	D3DXVECTOR3 GetVecU() { return m_vecU; }

private:

	//メンバ変数
	D3DXMATRIX m_mtxProjection;		//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;			//ビューマトリックス

	D3DXVECTOR3 m_posV;		//視点
	D3DXVECTOR3 m_posR;		//注視点
	D3DXVECTOR3 m_vecU;		//上方向ベクトル
	D3DXVECTOR3 m_rot;		//向き
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_VDest;	//目的の視点
	D3DXVECTOR3 m_RDest;	//目的の注視点
	float m_fDistance;		//距離
};

#endif
