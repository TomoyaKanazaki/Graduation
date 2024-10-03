//============================================
//
//	プレイヤー2Dのサンプル [Player2D.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PLAYER2D_H_
#define _PLAYER2D_H_

#include "main.h"
#include "object2D.h"
namespace
{
	D3DXVECTOR2 Size = D3DXVECTOR2(30.0f, 40.0f);//プレイヤーの大きさ
	float move_player = 3.0f; //プレイヤーの移動速度
	float jump_player = 5.0f; //プレイヤーのジャンプ強度		
	float gravity = 0.1f;	//重力

	D3DXVECTOR3 move_space = D3DXVECTOR3(300.0f, 300.0f, 0.0f);//中心からの移動範囲
	D3DXVECTOR3 pos_max = SCREEN_CENTER + move_space;
	D3DXVECTOR3 pos_min = SCREEN_CENTER - move_space;
}
//オブジェクトプレイヤークラス
class CPlayer2D : public CObject2D
{
public:

	CPlayer2D(int nPriority = 6);
	~CPlayer2D();

	static CPlayer2D* Create(int nPriority = 6);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得処理・設定処理
	int GetSample(void) { return m_nSample; }
	void SetSample(int Sample) { m_nSample = Sample; }
	D3DXVECTOR3 GetMove(void) { return m_Move; }
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }
	D3DXVECTOR3 GetPosOld(void) { return m_PosOld; }
	bool GetLanding(void) { return m_bLanding; }
	void SetLanding(bool bLanding) { m_bLanding = bLanding; }
	void Death();
private:
	void Move(D3DXVECTOR3* pos);
	void Jump();
	int m_nSample;							//サンプル変数
	D3DXVECTOR3 m_Move;						//移動速度
	D3DXVECTOR3 m_PosOld;						//移動速度
	bool m_bLanding;						//接地状態
};
#endif