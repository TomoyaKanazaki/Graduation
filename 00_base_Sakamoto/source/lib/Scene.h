//============================================
//
//	シーン [Scene.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h"

//前方宣言
class CRenderer;
class CFade;
class CObjmeshField;
class CItemManager;
class CPlayer;
class CDevil;

//シーンクラス
class CScene
{
public:
	CScene();
	~CScene();

	enum MODE
	{
		MODE_LOGO = 0,
		MODE_TITLE,
		MODE_SELECT,
		MODE_GAME,
		MODE_RESULT,
		MODE_TUTORIAL,
		MODE_MAX
	};

	static CScene* Create(MODE mode);
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static void SetMode(MODE mode);
	static MODE GetMode(void) { return m_mode; }

	virtual CPlayer* GetPlayer(int Num) { return nullptr; }
	virtual CDevil* GetDevil(void) { return nullptr; }
	virtual CObjmeshField* GetMapField(void) { return nullptr; }

private:
	static MODE m_mode;
};

#endif