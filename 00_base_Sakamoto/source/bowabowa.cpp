//============================================
//
//	�{���{���̏��� [SampleObjX.cpp]
//	Author:sakamoto kai
//
//============================================
#include "bowabowa.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "game.h"
#include "score.h"
#include "player.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
	const char* MODEL_PASS = "data\\MODEL\\Testbowabowa.x"; // ���f���p�X
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CBowabowa>* CBowabowa::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CBowabowa::CBowabowa(int nPriority) : CItem(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CBowabowa::~CBowabowa()
{

}

//====================================================================
//����������
//====================================================================
HRESULT CBowabowa::Init()
{
	SetType(CObject::TYPE_BOWABOWA);

	CItem::Init(MODEL_PASS);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CBowabowa>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CBowabowa::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	CItem::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CBowabowa::Update(void)
{

}

//====================================================================
//�`�揈��
//====================================================================
void CBowabowa::Draw(void)
{
	CItem::Draw();
}

//====================================================================
//��ԊǗ�
//====================================================================
void CBowabowa::Take(void)
{
	CGame::AddBowabowa(-1);
	CScore *pScore = CGame::GetScore();
	pScore->AddScore(100);
	Uninit();
}

//==========================================
// �q�b�g����
//==========================================
bool CBowabowa::Hit(CPlayer* pPlayer)
{
	// �\���˂������ĂȂ��ꍇ�֐��𔲂���
	if (pPlayer->GetItemType() != CPlayer::TYPE_CROSS)
	{
		return false;
	}

	// �폜
	Uninit();
	return true;
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CBowabowa>* CBowabowa::GetList(void)
{
	return m_pList;
}
