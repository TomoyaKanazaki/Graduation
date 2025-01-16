//============================================
//
//	�\���˃A�C�e����UI [crossUI.h]
//	Author:���������Y
//
//============================================
#ifndef _CROSSUI_H_
#define _CROSSUI_H_

#include "main.h"
#include "object2D.h"

//�I�u�W�F�N�g�v���C���[�N���X
class CCrossUi : public CObject2D
{
public:

	CCrossUi(int nPriority = 6);
	~CCrossUi();

	static CCrossUi* Create(int nPriority = 6);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�擾�����E�ݒ菈��
	float GetStateCount(void) { return m_fStateCountParcent; }
	void SetStateCount(int StateCount) { m_fStateCountParcent = StateCount; }

	D3DXVECTOR2 GetSizeDefault(void);	//UI�̃f�t�H���g�̃T�C�Y�擾

private:
	float m_fStateCountParcent;		//�\���˕ێ���Ԃ̃J�E���g�̊���
	D3DXVECTOR2 m_SizeDefault;		//UI�̃f�t�H���g�̃T�C�Y
};
#endif