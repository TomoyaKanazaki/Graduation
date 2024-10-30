//============================================
//
//	�G�̏��� [enemy.cpp]
//	Author:sakamoto kai
//
//============================================
#include "modelEffect.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "Particle.h"
#include "objGauge2D.h"
#include "number.h"
#include "input.h"
#include "CubeBlock.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
	const float MOVE_SPEED = 3.0f;		//�ړ����x
}

//==========================================
// �ÓI�����o�ϐ��錾
//==========================================
CListManager<CModelEffect>* CModelEffect::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CModelEffect::CModelEffect(int nPriority) : CObjectX(nPriority)
{
	SetSize(COLLISION_SIZE);
	m_nIdxXModel = 0;
	m_nIdxTexture = -1;
	m_pos = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CModelEffect::~CModelEffect()
{

}

//====================================================================
//��������
//====================================================================
CModelEffect* CModelEffect::Create(char* pModelName)
{
	CModelEffect* pBlock = nullptr;

	if (pBlock == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pBlock = new CModelEffect();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pBlock->Init(pModelName)))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pBlock;
}

//====================================================================
//����������
//====================================================================
HRESULT CModelEffect::Init(char* pModelName)
{
	SetType(CObject::TYPE_EFFECT);

	CObjectX::Init(pModelName);

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CModelEffect>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CModelEffect::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	CObjectX::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CModelEffect::Update(void)
{
	if (m_color.a > 0.0f)
	{
		SetMatColor(m_color);
		m_color.a -= 0.05f;
	}
	else
	{
		Uninit();
	}

	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);
}

//====================================================================
//�`�揈��
//====================================================================
void CModelEffect::Draw(void)
{
	CObjectX::Draw();
}