//============================================
//
//	�I�u�W�F�N�g�Q�[�W2D [objGauge2D.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJGAUGE2D_H_
#define _OBJGAUGE2D_H_

#include "main.h"
#include "object.h"

//�I�u�W�F�N�g2D�N���X
class CObjGauge2D : public CObject
{
public:
	enum TYPE_VERTEX
	{
		TYPE_NONE = 0,
		TYPE_LEFT,
		TYPE_MAX
	};

public:
	CObjGauge2D(int nPriority);
	~CObjGauge2D();

	static CObjGauge2D* Create(int nPriority = 6);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetVerTex(void);							//���_���W�̐ݒ�
	void SetVerTex1(void);
	void SetAnim(D3DXVECTOR2 Tex);					//�e�N�X�`�����W�̐ݒ�
	void SetAnim1(D3DXVECTOR2 Tex);
	void SetScroll(D3DXVECTOR2 Tex);				//�e�N�X�`�����W�̐ݒ�
	void SetColor(D3DXCOLOR col);					//���_�J���[�̐ݒ�
	void SetTexture(const char* name);
	void SetTypeVertex(TYPE_VERTEX type) { m_type = type; }

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetWidth(float Width) { m_Width = Width; }
	float GetWidth(void) { return m_Width; }
	void SetHeight(float Height) { m_Height = Height; }
	float GetHeight(void) { return m_Height; }
	void SetRatio(float Ratio) { m_Ratio = Ratio; }
	float GetRatio(void) { return m_Ratio; }
	virtual void SetIdx(int Idx) { m_nIdxTexture = Idx; }
	virtual int GetIdx(void) { return m_nIdxTexture; }
	void SetAddDraw(bool Set) { m_AddDarw = Set; }
	void SetGaugeWidth(float nCMax, float nCMeter);
	void SetNULL(void);

	void SetAppear(bool bAppear) { m_bAppear = bAppear; }
	bool GetAppear(void) { return m_bAppear; }

protected:
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 Height) { m_pVtxBuff = Height; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }

private:
	int m_nIdxTexture;
	float m_Width;							//��
	float m_Ratio;							//����
	float m_Height;							//����
	bool m_AddDarw;							//�`��̎��ɉ��Z���������邩�ǂ���
	D3DXVECTOR3 m_pos;						//���_�̈ʒu
	D3DXVECTOR3 m_rot;						//����
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���ւ̃|�C���^
	TYPE_VERTEX m_type;
	bool m_bAppear;							//�`�悷�邩�ǂ���
};
#endif