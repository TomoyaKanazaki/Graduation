//========================================
//
//�Q�[�W[gauge.h]
//Author�F�X��x��
//
//========================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"
#include "object2D.h"

//========================================
//�Q�[�W�N���X
//========================================
class CGauge : public CObject
{
public:
	enum Gauge
	{//	�Q�[�W��
		TYPE_BASE = 0,	// ���n
		TYPE_GAUGE,		// �Q�[�W
		TYPE_FRAME,		// �g��
		TYPE_MAX
	};

	CGauge(int nPriority = 7);		//�R���X�g���N�^
	~CGauge();	//�f�X�g���N�^

	//�����o�֐�
	static CGauge* Create(int nMaxLife);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVertex();
	void SetSize(float fWidht, float fHeight);
	void SetLife(int nLife) { m_nLife = nLife; }

private:
	//�����o�ϐ�
	int m_nMaxLife;		// �̗͂̍ő�l
	int m_nLife;		// ���݂̗̑�

	float m_fLength;		//�Ίp���̒���
	float m_fAngle;			//�Ίp���̊p�x
	float m_aTexU;			//�e�N�X�`����U�l

	CObject2D* m_p2D[TYPE_MAX];		// �I�u�W�F�N�g2D�̃|�C���^
	static 	LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���̃|�C���^
};

#endif
