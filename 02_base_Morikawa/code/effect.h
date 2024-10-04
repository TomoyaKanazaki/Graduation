//=======================================
//
//�G�t�F�N�g����[effect.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _EFFECT_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _EFFECT_H_     //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "billboard.h"

//========================================
// �萔��`
//========================================
namespace
{
const int MAX_EFFECT = 30;	// �G�t�F�N�g�̍ő吔
}

//===========================================
//�G�t�F�N�g�N���X
//===========================================
class CEffect : public CBillboard
{
public:
	// �\���̂̒�`
	struct Effect
	{
		int nLife;			// ����(�\������)
		int nFirstLife;		// ��������
		float fFirstAlpha;	// �����s�����x
		bool balpha;			//���Z����
	};

	//=============================
	// �񋓌^��`
	//=============================
	enum TYPE
	{
		TYPE_NORMAL = 0,	// �ʏ�
		TYPE_POINT,			// �_
		TYPE_SMOKE,			// ��
		TYPE_SMOKEBLACK,	// ����
		TYPE_BLACK,			// ��
		TYPE_JUJI,			// �\��
		TYPE_JUJI2,			// �\��
		TYPE_THUNDER,		// ��
		TYPE_NULL,			// NULL
		TYPE_MAX
	};

	CEffect(int nPriority = 5);		//�R���X�g���N�^
	~CEffect();		//�f�X�g���N�^

	//�����o�֐�
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, bool balpha, TYPE type);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ���ݒ�E�擾
	void SetAddSizeValue(float value) { m_fAddSizeValue = value; }	// �T�C�Y���Z�ʐݒ�
	float GetAddSizeValue() { return m_fAddSizeValue; }				// �T�C�Y���Z�ʎ擾

private:

	// �\���̂̒�`
	struct TexInfo
	{
		std::string textureFile;	// �e�N�X�`���t�@�C����
		int texID;					// �e�N�X�`��ID
	};


	//=============================
	// �����o�֐�
	//=============================
	// �����n
	void Resize();	// ���T�C�Y

	// �ÓI�֐�
	static void TexLoad();	// �e�N�X�`���ǂݍ���

	//�����o�ϐ�
	Effect m_aEffect;	// �\���̂̏��
	float m_fAddSizeValue;		// �T�C�Y���Z��


	static int m_TexIdx[TYPE::TYPE_MAX];	// �e�N�X�`���C���f�b�N�X
	static bool m_bTexLoad;	// �e�N�X�`���̓ǂݍ��ݔ���
};

#endif