//=============================================================================
// 
//  反転演出ヘッダー [reverse_direction.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _REVERSE_DIRECTION_H_
#define _REVERSE_DIRECTION_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// 反転演出クラス
class CReverse_Direction : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_NONE = 0,		// なにもない
		STATE_APPEARANCE,	// 登場
		STATE_WAIT,			// 待機
		STATE_FADEOUT,		// フェードアウト
		STATE_MAX
	};

	CReverse_Direction(int nPriority = 7);
	~CReverse_Direction();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetState(const State& state);	// 状態設定

	//=============================
	// 静的関数
	//=============================
	static CReverse_Direction* Create();

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CReverse_Direction::* STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();		// 状態更新
	void StateNone();		// なし
	void StateAppearance();	// 登場
	void StateWait();		// 待機
	void StateFadeout();	// フェードアウト

	// その他関数

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fStateTime;		// 状態カウンター
	State m_state;			// 状態

	// その他変数

};


#endif