//=============================================================================
// 
//  �X�e�[�W�G�f�B�^�[�w�b�_�[ [stageedittor.h]
//  Author : ��������
// 
//=============================================================================

#ifndef _STAGE_EDITTOR_H_
#define _STAGE_EDITTOR_H_		// ��d�C���N���[�h�h�~

#include "object.h"

class CStageObj;

//==========================================================================
// �N���X��`
//==========================================================================
// �X�e�[�W�̔z�u���N���X
class CStageEdittor : public CObject
{
public:

	CStageEdittor(void);
	~CStageEdittor();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	CStageObj* GetObj() { return m_pStageObj; }

	// �ÓI�����o�֐�
	static CStageEdittor* Create();

private:

	// �����o�֐�
	void SetType();
	void Control();

	CStageObj* m_pStageObj;
	int m_nType;
};


#endif