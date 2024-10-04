//========================================
//
//  ���U���g [result.h]
//  Author�F���n�Ή�
//
//========================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "manager.h"

//========================================
// �O���錾
//========================================
class CObject2D;
class CResultScore;

//========================================
// ���U���g�N���X
//========================================
class CResult : public CScene
{
public:
	CResult();	//�R���X�g���N�^
	~CResult();	//�f�X�g���N�^

	// �����o�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CResult* Create();

private:

	CObject2D* m_pObj2D;	// �w�i
	CResultScore* m_pScore;	// ����̃X�R�A
};

#endif
