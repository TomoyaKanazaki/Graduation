//============================================
//
//	ゲーム用ポーズ処理 [GamePause.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _GAMEPAUSE_H_
#define _GAMEPAUSE_H_

#include "main.h"
#include "objectAnim2d.h"
//マクロ定義
#define MAX_GAMEPAUSE (1)
#define MAX_GAMEFG (1)

//前方宣言
class CObject2D;

//レベルアップクラス
class CGamePause
{
public:
	CGamePause();
	~CGamePause();

	static CGamePause* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetAppear(bool Set) { m_Appear = Set; m_PauseSelect = 0; }
	bool SetAppear(void) { return m_Appear; }

private:
	int m_PauseSelect;
	bool m_Appear;
	static bool m_bColor;	// 色の変更をするかどうか
	
	static CObject2D* m_pPauseFG[MAX_GAMEFG];

	CObjectAnim2D* pNoise;
	float m_MoveRot;
	D3DXVECTOR3 m_MarkRot;
	D3DXVECTOR3 m_InitPos[3];
};
#endif