//============================================
//
//	�e���� [shadow.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "object3D.h"

//�I�u�W�F�N�g�V���h�E�N���X
class CShadow : public CObject3D
{
public:

	CShadow(int nPriority = 7);
	~CShadow();

	static CShadow* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fLimit = -1.0f);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void SetBaseHeight(float fHeight) { m_fHeight = fHeight; }

	// �ÓI�����o�֐�
	static CListManager<CShadow>* GetList(void);	// ���X�g�擾

private:
	// �ÓI�����o�ϐ�
	static CListManager<CShadow>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CShadow>::AIterator m_iterator; // �C�e���[�^�[
	D3DXVECTOR3 m_sizeBase;
	float m_fHeight;
	float m_fLimit;
};

#endif