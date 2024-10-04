//========================================
//
// ��Q���̏���[reverse.h]
// Author�F���n�Ή�
//
//========================================
#ifndef _REVERSE_H_
#define _REVERSE_H_

#include "stageobj.h"
class CBillboard;

//========================================
// ��Q���N���X
//========================================
class CReverse : public CStageObj
{
public:

	CReverse(int nPriority = 6);		//�R���X�g���N�^
	~CReverse();	//�f�X�g���N�^

	// �����o�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Kill() override;

	void SetPos(D3DXVECTOR3 pos) override;
	virtual bool Collision(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rSize, const CPlayer::PLAYERSTATE state) override;	// �v���C���[�Ƃ̓����蔻��
	static CListManager<CReverse> GetList() { return m_List; }	// ���X�g�擾

private:

	float m_fRotationTime;	// ��]����
	CBillboard* m_pBillboard;		// �r���{�[�h�I�u�W�F�N�g
	static CListManager<CReverse> m_List;	// ���X�g
};

#endif
