//========================================
//
//�I�u�W�F�N�g2D[object2D.h]
//Author�F�X��x��
//
//========================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_
#include "main.h"
#include "object.h"

//========================================
// �萔��`
//========================================
namespace
{
const int MAX_BG = 64;	// �w�i�̍ő吔
}

//========================================
//�I�u�W�F�N�g2D�N���X
//========================================
class CObject2D : public CObject
{
public:
	CObject2D(int nPriority = 3);	//�R���X�g���N�^
	~CObject2D();	//�f�X�g���N�^

	//�����o�֐�
	HRESULT Init(void);		//������
	void Uninit(void);		//�I��
	void Update(void);		//�X�V
	void Draw(void);		//�`��
	static CObject2D *Create();		//�I�u�W�F�N�g�̐���
	void SetSize(float fWidht, float fHeight);	//�傫��
	D3DXVECTOR2 GetSize() { return m_size; }
	void SetSizeOrigin(const D3DXVECTOR2& size) { m_sizeOrigin = size; }
	D3DXVECTOR2 GetsizeOrigin() { return m_sizeOrigin; }	// ���̃T�C�Y

	virtual void SetVertex(void);			//���_���

	void BindTexture(int pTexture);				//�e�N�X�`�����蓖��
	void SetVertexAnim(int Num, int Cnt);		//�e�N�X�`���A�j���[�V����
	void SetPosVertex(void);		//���_���
	void SetVertexBg(void);			//�w�i�̒��_���
	void SetVertexEffect(float m_fRadius, D3DXCOLOR m_col);		//�G�t�F�N�g�̒��_���W
	void SetVertexMoveBg(void);
	void SetScoreVertex(void);		//�X�R�A�̒��_���
	void SetScore(int nNum[8]);
	void SetVertexColor(D3DXCOLOR col);

	void SetCol(D3DXCOLOR col) { m_col = col; }	// �F�ݒ�
	D3DXCOLOR GetCol() { return m_col; }		// �F�擾

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() { return m_pVtxBuff; }		// ���_���̎擾

private:
	//�����o�ϐ�
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		//���_�����i�[
	D3DXCOLOR m_col;			// �F
	float m_fLength;			// �Ίp���̒���
	float m_fAngle;				// �Ίp���̊p�x
	D3DXVECTOR2 m_size;			// �T�C�Y
	D3DXVECTOR2 m_sizeOrigin;	// ���̃T�C�Y
	int m_nCounterAnim;     //�A�j���[�V�����J�E���^�[
	int m_nPatternAnim;     //�A�j���[�V�����p�^�[��No,
	float m_aTexV;			//�e�N�X�`�����W�̊J�n�ʒu
};

#endif
