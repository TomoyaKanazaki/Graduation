//============================================
//
//	キャラクターの管理 [characterManager.h]
//	Author:sakai minato
//
//============================================
#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

//マクロ定義
#define MAX_CHARACTER (1024)

// キャラクター管理クラス
class CCharacterManager
{
public:
	CCharacterManager();
	~CCharacterManager();

	HRESULT Load(void);
	void Unload(void);

	int Regist(const char* pFilename);
	void XModelRegist(int* nTexIdx, int nXModelIdx);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);
	const char* GetTextureName(int Idx) { return &c_apTexturename[Idx][0]; }

private:
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_CHARACTER];
	static int m_NumAll;
	static char c_apTexturename[MAX_CHARACTER][128];
};
#endif