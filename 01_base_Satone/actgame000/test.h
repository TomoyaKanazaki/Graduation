//==============================================================
//
//プレイヤーのテスト処理[tutorial_texture.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _TEST_H_		//このマクロ定義がされていなかったら
#define _TEST_H_		//2重インクルード防止のマクロを定義する

#include "object3D.h"

class CTutorialUI;
class CTestLine;

//チュートリアルUIクラスの定義
class CTest : public CObject3D
{
public:

	//チュートリアルのステップ
	enum STEP
	{
		STEP_MOVE = 0,		//移動
		STEP_JUMP,			//ジャンプ
		STEP_MAX
	};

	CTest();		//コンストラクタ
	CTest(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//コンストラクタ(オーバーロード)
	~CTest();		//デストラクタ

	static CTest *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//生成処理

	HRESULT Init(void);
	void UnInit(void);
	void Update(void);
	void Draw(void);

private:

	void MoveKeyboard(void);	//プレイヤーキーボードの移動操作
	void ScreenLimit(void);		// 画面外判定
	void CircleColllllll(void);

	static LPDIRECT3DTEXTURE9 m_pTexture;
	static const char *m_apFilename[STEP_MAX];		//テクスチャの名前
	int m_nIdxTexture;		//テクスチャの番号

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_posOld;	//前回の位置
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_distance;		//移動量

	int m_nType;			//チュートリアルの種類

	float m_fDistance;
	float m_fScale;			// 距離の倍率

	CTutorialUI* m_pTest2;
	CTestLine* m_pTestLine;
	CObject3D* m_pCircle;
};

#endif
