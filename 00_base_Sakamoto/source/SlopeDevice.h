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

class CModel;
class CMotion;

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
		STATE_ACTION,		// 行動
		STATE_MAX,
	};

	// モーション
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,	// 待機状態
		ACTION_MAX,			// 最大
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

	void SetMultiMatrix(bool Set) { m_bMultiMatrix = Set; }
	bool GetMultiMatrix(void) { return m_bMultiMatrix; }
	void SetUseMultiMatrix(D3DXMATRIX Set) { m_UseMultiMatrix = Set; }

	// 静的メンバ関数
	static CListManager<CSlopeDevice>* GetList(void); // リスト取得

private:
	void InitModel(const char* pModelNameSlopeDevice, const char* pModelNameEnemy);

	void StateManager(void);		// 状態管理

	void LoadModel(const char* pFilename);

	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_posOld;		//過去の位置
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_size;			//大きさ

	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス

	STATE m_State;					//状態
	int m_nStateCount;				//状態管理用変数

	//階層構造とモーションのポインタ
	CModel* m_apModel[MODEL_NUM];
	CMotion* m_pMotion;
	char* m_aModelName[MODEL_NUM];
	int m_nNumModel;
	char m_cFileName[128];		//ファイルの名前

	//マップとのマトリックス情報
	bool m_bMultiMatrix;					// マトリックスの掛け合わせをするかどうか
	D3DXMATRIX m_UseMultiMatrix;			// 掛け合わせるマトリックス

	// 静的メンバ変数
	static CListManager<CSlopeDevice>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CSlopeDevice>::AIterator m_iterator; // イテレーター

};
#endif