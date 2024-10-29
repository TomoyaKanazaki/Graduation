//============================================
//
//	�L���[�u�G�t�F�N�g [CubeEffect.cpp]
//	Author:sakamoto kai
//
//============================================
#include "CubeEffect.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const int WAIGHT_SIZE = 11;		//���̒��_��
	const int HEIGHT_SIZE = 11;		//�c�̒��_��
	const float WAIGHT_CENTER = 0.5f;	//���̌��_(0.0f�`1.0f)
	const float HEIGHT_CENTER = 0.5f;	//�c�̌��_(0.0f�`1.0f)
	const float FIELD_SIZE = 200.0f;		//���ꖇ�̑傫��
}

//====================================================================
//�ÓI�����o�ϐ��錾
//====================================================================
CListManager<CCubeEffect>* CCubeEffect::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CCubeEffect::CCubeEffect(int nPriority) :CObjmeshCube(nPriority)
{
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_nLife = -1;
	m_bFall = false;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CCubeEffect::~CCubeEffect()
{

}

//====================================================================
//��������
//====================================================================
CCubeEffect* CCubeEffect::Create(void)
{
	CCubeEffect* pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		//�I�u�W�F�N�g3D�̐���
		pObject3D = new CCubeEffect();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pObject3D->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pObject3D;
}

//====================================================================
//����������
//====================================================================
HRESULT CCubeEffect::Init(void)
{
	CObjmeshCube::Init();

	SetTexture("data\\TEXTURE\\Test.jpg");
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	SetType(TYPE_CUBEEFFECT);

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CCubeEffect>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CCubeEffect::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	CObjmeshCube::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CCubeEffect::Update(void)
{
	m_pos = GetPos();

	if (m_bFall == true)
	{
		m_move.y -= 0.98f;
	}

	m_pos += m_move;

	CObjmeshCube::SetPos(m_pos);
	CObjmeshCube::Update();

	if (m_nLife > 0 && m_nLife != -1)
	{
		m_nLife--;
	}
	else if(m_nLife == 0)
	{
		Uninit();
		return;
	}

	if (m_bFall == true)
	{
		if (m_pos.y < 0.0f)
		{
			Uninit();
			return;
		}
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CCubeEffect::Draw(void)
{
	CObjmeshCube::Draw();
}