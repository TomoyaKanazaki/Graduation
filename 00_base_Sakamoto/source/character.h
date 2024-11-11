//============================================
//
//	キャラクターの処理 [character.h]
//	Author:sakai minato
//
//============================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "object.h"

//オブジェクトプレイヤークラス
class CCharacter
{
public:
	CCharacter();
	~CCharacter();

	static CCharacter* Create(const char* pModelName);
	HRESULT Init(const char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:


};

#endif