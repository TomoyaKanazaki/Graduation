//==============================================================
//
//チュートリアル画像の処理[tutorial_texture.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _TUTORIAL_TEXTURE_H_		//このマクロ定義がされていなかったら
#define _TUTORIAL_TEXTURE_H_		//2重インクルード防止のマクロを定義する

#include "object3D.h"

class CTest;

//チュートリアルUIクラスの定義
class CTutorialUI : public CObject3D
{
public:

	//チュートリアルのステップ
	enum STEP
	{
		STEP_MOVE = 0,		//移動
		STEP_JUMP,			//ジャンプ
		STEP_DASH,			//ダッシュ
		STEP_ALPHA,			//通り抜け
		STEP_ITEM,			//アイテム
		STEP_ENEMY,			//敵
		STEP_MAX
	};

	CTutorialUI();		//コンストラクタ
	CTutorialUI(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//コンストラクタ(オーバーロード)
	~CTutorialUI();		//デストラクタ

	static CTutorialUI *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//生成処理

	HRESULT Init(void);
	void UnInit(void);
	void Update(void);
	void Draw(void);

	void SetTest2Position(D3DXVECTOR3 pos) { m_posTest2 = pos; }

	D3DXVECTOR3 GetPosition(void) { return m_pos; }

private:
	void MoveKeyboard(void);	//プレイヤーキーボードの移動操作
	void ScreenLimit(void);		// 画面外判定
	void CircleColllllll(void);

	static LPDIRECT3DTEXTURE9 m_pTexture;
	static const char *m_apFilename[STEP_MAX];		//テクスチャの名前
	int m_nIdxTexture;		//テクスチャの番号

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_posTest2;		//位置

	D3DXVECTOR3 m_posOld;	//前回の位置
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_distance;		//移動量

	int m_nType;			//チュートリアルの種類

	float m_fDistance;
	float m_fScale;			// 距離の倍率

	CTest* m_pTest2;
	CObject3D* m_pCircle;

};

#endif
