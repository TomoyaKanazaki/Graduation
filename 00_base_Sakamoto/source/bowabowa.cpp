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

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CBowabowa::CBowabowa(int nPriority) : CItem(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = NULL;			//�}�e���A���̐�
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CBowabowa::~CBowabowa()
{

}

//====================================================================
//��������
//====================================================================
CBowabowa* CBowabowa::Create(char* pModelName)
{
	CBowabowa* pSample = NULL;

	if (pSample == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample = new CBowabowa();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample->Init(pModelName)))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pSample;
}

//====================================================================
//����������
//====================================================================
HRESULT CBowabowa::Init(char* pModelName)
{
	SetType(CObject::TYPE_BOWABOWA);

	CItem::Init(pModelName);

	//���[�h���Ƃɏ����l��ݒ�o����
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:
		CGame::AddBowabowa(1);
		break;

	case CScene::MODE_RESULT:
		break;
	}


	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CBowabowa::Uninit(void)
{
	CItem::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CBowabowa::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//�^�C�g���ł̍X�V����
//====================================================================
void CBowabowa::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	//�ʒu�X�V
	pos += m_move;

	SetPos(pos);

	//���_���̍X�V
	CItem::Update();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CBowabowa::GameUpdate(void)
{
	//�X�V�O�̈ʒu���ߋ��̈ʒu�Ƃ���
	m_posOld = m_pos;

	//�ʒu�X�V
	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);

	//��ʊO����
	if (m_pos.y < 0.0f)
	{

	}

	//�傫���̐ݒ�
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//��ԊǗ�
	StateManager();

	//���_���̍X�V
	CItem::Update();
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
void CBowabowa::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ACTION:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
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