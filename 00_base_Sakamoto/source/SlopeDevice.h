//============================================
//
//	傾き装置の処理 [SlopeDevice.h]
//	Author:sakai minato
//
//============================================
#ifndef _SLOPE_DEVICE_H_
#define _SLOPE_DEVICE_H_

#include "object.h"

#define MODEL_NUM		(64)	// モデルの数

class CCharacter;

//オブジェクトプレイヤークラス
class CSlopeDevice : public CObject
{
public:

	CSlopeDevice(int nPriority = 3);
	~CSlopeDevice();

	// 状態
	enum STATE
	{
		STATE_NORMAL = 0,	// 通常
		STATE_ROTATE,		// 回転
		STATE_MAX,
	};

	// モーション
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,	// 待機状態
		ACTION_MAX,			// 最大
	};

	// モーション
	enum SETUP_TYPE
	{
		SETUP_TYPE_FOUNDATION = 0,	// 土台
		SETUP_TYPE_MAWASIGURMA,		// 回し車
		SETUP_TYPE_ROLLRE,			// ローラー
		SETUP_TYPE_HEAD,			// 先っぽ
		SETUP_TYPE_MAX,				// 最大
	};

	static CSlopeDevice* Create(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	void SetMultiMatrix(bool Set) { m_bMultiMatrix = Set; }
	bool GetMultiMatrix(void) { return m_bMultiMatrix; }
	void SetUseMultiMatrix(D3DXMATRIX* Set) { m_UseMultiMatrix = Set; }

	// 静的メンバ関数
	static CListManager<CSlopeDevice>* GetList(void); // リスト取得

private:
	HRESULT InitModel(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	void StateManager(void);
	void Rotate(int nNldxModel, D3DXVECTOR3 rotate);

	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_posOld;		//過去の位置
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_size;			//大きさ

	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス

	STATE m_State;					//状態
	int m_nStateCount;				//状態管理用変数

	CCharacter* m_pCharacter;			// キャラクターのクラス

	//マップとのマトリックス情報
	bool m_bMultiMatrix;					// マトリックスの掛け合わせをするかどうか
	D3DXMATRIX* m_UseMultiMatrix;			// 掛け合わせるマトリックス

	// 静的メンバ変数
	static CListManager<CSlopeDevice>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CSlopeDevice>::AIterator m_iterator; // イテレーター

};
#endif