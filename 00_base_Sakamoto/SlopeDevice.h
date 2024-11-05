//============================================
//
//	傾き装置の処理 [SlopeDevice.h]
//	Author:sakai minato
//
//============================================
#ifndef _SLOPE_DEVICE_H_
#define _SLOPE_DEVICE_H_

#include "objectX.h"

//オブジェクトプレイヤークラス
class CSlopeDevice : public CObjectX
{
public:

	CSlopeDevice(int nPriority = 3);
	~CSlopeDevice();

	// 状態
	enum STATE
	{
		STATE_NORMAL = 0,	// 通常
		STATE_ACTION,		// 行動
		STATE_MAX,
	};

	static CSlopeDevice* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	// 静的メンバ関数
	static CListManager<CSlopeDevice>* GetList(void); // リスト取得

private:
	void StateManager(void);		// 状態管理

	int m_nIdxXModel;				// Xモデルの番号
	D3DXVECTOR3 m_CollisionPos;		// 当たり判定用の座標
	bool m_bCollision;				// 当たり判定用の座標
	int m_nIdxTexture;				// テクスチャの番号
	STATE m_State;					// 状態
	int m_nStateCount;				// 状態管理用変数
	float m_Scaling;				// 大きさ
	D3DXVECTOR3 m_pos;				// 位置	
	D3DXVECTOR3 m_posOld;			// 過去の位置	
	D3DXVECTOR3 m_move;				// 移動量	
	D3DXVECTOR3 m_rot;				// 向き	
	float m_fColorA;				// 不透明度

	// 静的メンバ変数
	static CListManager<CSlopeDevice>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CSlopeDevice>::AIterator m_iterator; // イテレーター

};
#endif