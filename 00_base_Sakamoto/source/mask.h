//=======================================
//
// �X�e���V���o�b�t�@�ɕK�v��2D�}�X�N[mask.h]
// Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _MASK_H_	 //���̃}�N����`����`����Ă��Ȃ�������
#define _MASK_H_	 //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "object2d.h"
#include "enemy.h"
#include "item.h"

//=======================================
// �}�X�N�N���X
//=======================================
class CMask : public CObject2D
{
public:
	CMask(int nPriority = 5);	//�R���X�g���N�^
	~CMask();	//�f�X�g���N�^

	static CMask* Create(int nRefidx, D3DXCOLOR col);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void SetRefidx(int nRef) { m_nRefidx = nRef; }

private:
	//�����o�ϐ�
	int m_nRefidx;	// �X�e���V���̎Q�ƒl
};

#endif
