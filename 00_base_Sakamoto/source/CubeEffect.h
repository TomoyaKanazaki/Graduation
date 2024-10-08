//============================================
//
//	キューブエフェクト [CubeEffect.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _CUBEEFFECT_H_
#define _CUBEEFFECT_H_

#include "main.h"
#include "objmeshCube.h"

//オブジェクトメッシュフィールドクラス
class CCubeEffect : public CObjmeshCube
{
public:
	CCubeEffect(int nPriority = 4);
	~CCubeEffect();

	static CCubeEffect* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetLife(float Life) { m_nLife = Life; }
	float GetLife(void) { return m_nLife; }
	void SetFall(int Life) { m_bFall = Life; }
	int GetFall(void) { return m_bFall; }

private:

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_move;
	float m_nLife;
	bool m_bFall;

};
#endif