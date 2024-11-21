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
class CInputKeyboard;
class CInputJoypad;
class CInputMouse;
class CCamera;
class CMiniMapCamera;
class CLight;
class CTexture;
class CXModel;
class CLevelUP;
class CFade;
class CSound;
class CTimer;
class CRanking;
class CBlockManager;
class CItemManager;

//シーンクラス
class CScene
{
public:
	CScene();
	~CScene();

	static CScene* GetInstance();

	enum MODE
	{
		MODE_LOGO = 0,
		MODE_TITLE,
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

private:
	static CScene* pScene;
	static MODE m_mode;
};

#endif