//============================================
//
//	破片の処理 [debris.h]
//	Author:hashimoto kenta
//
//============================================
#ifndef _DEBRIS_H_
#define _DEBRIS_H_

#include "main.h"
#include "objectX.h"
#include "useful.h"

//前方宣言
class CObjGauge2D;

//オブジェクトプレイヤークラス
class CDebris : public CObjectX
{
public:

	CDebris(int nPriority = 3);
	~CDebris();

	//サンプルの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_ACTION,		//行動
		STATE_MAX,
	};

	static CDebris* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int ModelIdx);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int ModelIdx);
	void Uninit();
	void Update();
	void TitleUpdate();
	void GameUpdate();
	void Draw();

	int GetIdxXModel() { return -1; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos() { return m_pos; }
	void SetMove(D3DXVECTOR3 Move) { m_move = Move; }
	D3DXVECTOR3 GetMove() { return m_move; }

private:
	char* SetModelName();	//モデルのパスを決める
	void SetRamScaling();	//モデルのサイズをランダムで決める
	void SetRotMove();		//回転の移動量を設定
	void CollisionMapObject(D3DXVECTOR3 Mypos, D3DXVECTOR3 MyposOld, useful::COLLISION XYZ);	//壁との当たり判定

	int m_nIdxXModel;				//Xモデルの番号
	float m_Scaling;				//大きさ
	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_posOld;			//過去の位置
	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_rot;				//向き
	D3DXVECTOR3 m_rotmove;			//回転の移動量
};
#endif