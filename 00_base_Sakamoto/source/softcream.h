//============================================
//
//	�\�t�g�N���[���̏��� [softcream.h]
//	Author:morikawa shunya
//
//============================================
#pragma once
#include "item.h"
#include "MapSystem.h"

//==========================================
//  �N���X��`
//==========================================
class CSoftCream : public CItem
{
public:

	CSoftCream(int nPriority = 2);
	~CSoftCream();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Move(D3DXVECTOR3& pos) override;

	bool Hit(CPlayer* pPlayer) override;

	// �ÓI�����o�֐�
	static CListManager<CSoftCream>* GetList(void); // ���X�g�擾

private:

	// �����o�ϐ�
	CListManager<CSoftCream>::AIterator m_iterator; // �C�e���[�^�[

	// �ÓI�����o�ϐ�
	static CListManager<CSoftCream>* m_pList; // �I�u�W�F�N�g���X�g

};
