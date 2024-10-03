//============================================
//
//	マウスUI [mouseUI.h]
//	Author : Takeru Ogasawara
//
//============================================
#ifndef _CURSORUI_H_
#define _CURSORUI_H_

#include "main.h"
#include "object2D.h"

// マウスUIクラス
class CCursorUI : public CObject2D
{
public:
	CCursorUI(int nPriority = 6);
	~CCursorUI();

	static CCursorUI* Create(D3DXVECTOR2 pos = { SCREEN_CENTER.x, SCREEN_CENTER.y });

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// メンバ関数
	bool Collsion(D3DXVECTOR3 pos2D, D3DXVECTOR3 size2D);

private:
	// メンバ関数
	void move();

	// メンバ変数
	D3DXVECTOR3 m_move;
};
#endif