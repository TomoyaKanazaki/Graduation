//==========================================
//
//	矢印(signal.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include "objectX.h"
#include "main.h"

class CDevil;

//==========================================
//  クラス定義
//==========================================
class CSignal : public CObjectX
{
public:
	// モーション
	enum ACTION_TYPE
	{
		ACTION_NONE = 0,			// 待機
		ACTION_SIGNAL_UP,			// 傾き信号「上」
		ACTION_SIGNAL_DOWN,			// 傾き信号「下」
		ACTION_SIGNAL_LEFT,			// 傾き信号「左」
		ACTION_SIGNAL_RIGHT,		// 傾き信号「右」
		ACTION_MAX,					// 最大
	};

	// メンバ変数
	CSignal(int nPriority = 2);
	~CSignal();

	HRESULT Init(const char* pModelName);
	HRESULT Init() { assert(false); return E_FAIL; }; // 呼ばれてはならない
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CListManager<CSignal>* GetList(void); // リスト取得
	static CSignal* Create(const char* pModelNameSignal,D3DXVECTOR3 pos);

	//大きさ設定・取得
	void SetScaling(const float& Scaling) { m_Scaling = Scaling; }
	float GetScaling(void) { return m_Scaling; }

private:
	int m_nIdxXModel;		//Xモデルの番号
	int m_nIdxTexture;		//テクスチャの番号
	int m_nStateCount;		//状態管理用変数
	int m_nStateNum;

	float m_fColorA;		//不透明度
	float m_Scaling;		//大きさ

	bool m_UseItem;			//アイテムが使用可能かどうか

	D3DXVECTOR3 m_rot;		//回転量

	CDevil* m_pDevil;

	// 静的メンバ変数
	static CListManager<CSignal>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CSignal>::AIterator m_iterator; // イテレーター
};

#endif