//==============================================================
//
//�A�C�e������[item.cpp]
//Author:����������
//
//==============================================================
#include "item.h"
#include "game.h"
#include "model.h"
#include "material.h"
#include "player.h"
#include "sound.h"
#include "score.h"
#include "UI_item.h"

//�}�N����`
#define RESPAWN_TIME	(60 * 2)		//�A�C�e�������X�|�[�����鎞��

//�ÓI�����o�ϐ��錾
LPD3DXMESH CItem::m_pMesh = NULL;						//���b�V���i���_���j�ւ̃|�C���^
LPD3DXBUFFER CItem::m_pBuffMat = NULL;					//�}�e���A���ւ̃|�C���^
DWORD CItem::m_dwNumMat = NULL;
const char *CItem::m_pFilename[TYPE_MAX] = 				//�t�@�C���̖��O
{
	NULL,
	"data\\MODEL\\item_rhombus.x",		//�Ђ��`
	"data\\MODEL\\item_star.x",			//��
};

//==============================================================
//�R���X�g���N�^
//==============================================================
CItem::CItem()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_type = TYPEITEM_NONE;	//���
	m_nIdx = -1;				//���f���̔ԍ�
	m_nRespawnCounter = 0;		//���X�|�[���J�E���^�[

	m_bRespawn = false;	//���X�|�[�����邩
}

//==============================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//==============================================================
CItem::CItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPEITEM type)
{
	m_pos = pos;		//�ʒu
	m_rot = rot;		//����
	m_type = type;		//���
	m_nIdx = -1;		//���f���̔ԍ�
	m_nRespawnCounter = 0;		//���X�|�[���J�E���^�[

	m_bRespawn = false;	//���X�|�[�����邩
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CItem::~CItem()
{

}

//==============================================================
//�A�C�e���̐�������
//==============================================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPEITEM type)
{
	CItem *pItemModel = NULL;

	if (pItemModel == NULL)
	{//���������g�p����ĂȂ�������

		//�A�C�e���̐���
		pItemModel = new CItem(pos, rot, type);
	}

	//����������
	pItemModel->Init();

	return pItemModel;
}

//==============================================================
//�A�C�e���̏���������
//==============================================================
HRESULT CItem::Init(void)
{
	CMaterial *pMaterial = CManager::GetInstance()->GetMaterial();

	//���f���̓ǂݍ���
	m_nIdx = pMaterial->Regist(m_pFilename[m_type]);

	//�}�e���A���̊��蓖��
	CObjectX::BindMaterial(m_nIdx);

	//�I�u�W�F�N�gX�̏���������
	CObjectX::Init();

	//�ʒu�̐ݒ�
	CObjectX::SetPosition(m_pos);

	//��ނ̐ݒ�
	CObject::SetType(CObject::TYPE_ITEM);

	return S_OK;
}

//==============================================================
//�A�C�e���̏I������
//==============================================================
void CItem::Uninit(void)
{
	//�I�u�W�F�N�gX�̏I������
	CObjectX::Uninit();
}

//==============================================================
//�A�C�e���̍X�V����
//==============================================================
void CItem::Update(void)
{
	if (m_bRespawn == true)
	{//���X�|�[������Ƃ�

		if (m_nRespawnCounter < RESPAWN_TIME)
		{//���Ԃ��o���ĂȂ��Ƃ�

			m_nRespawnCounter++;
		}
		else if (m_nRespawnCounter >= RESPAWN_TIME)
		{//��莞�Ԍo�߂����Ƃ�

			CObjectX::SetState(CObjectX::STATE_NONE);			//���f���̏�Ԑݒ�

			m_bRespawn = false;		//���X�|�[�����Ȃ���Ԃɂ���

			m_nRespawnCounter = 0;
		}

	}

	//�I�u�W�F�N�gX�̍X�V����
	CObjectX::Update();
}

//==============================================================
//�A�C�e���̕`�揈��
//==============================================================
void CItem::Draw(void)
{
	//�I�u�W�F�N�gX�̕`�揈��
	CObjectX::Draw();
}

//==============================================================
//�A�C�e���̃q�b�g����
//==============================================================
void CItem::Hit(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();
	CSound *pSound = CManager::GetInstance()->GetSound();
	CItemUI *pItemUI = CGame::GetItemUI();

	if (m_type == TYPEITEM_RHOMBUS && m_bRespawn == false)
	{//�Ђ��`�A�C�e���̂Ƃ�

		pPlayer->SetDash(0);		//�_�b�V���񐔃��Z�b�g

		CObjectX::SetColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.3f));		//���f���̐F�ݒ�
		CObjectX::SetState(CObjectX::STATE_DAMAGE);					//���f���̏�Ԑݒ�

		//���X�|�[�������Ԃɂ���
		m_bRespawn = true;

		//SE�Đ�
		pSound->Play(pSound->SOUND_LABEL_SE_ITEM001);
	}
	else if (m_type == TYPEITEM_STAR)
	{//���A�C�e���̂Ƃ�

		//SE�Đ�
		pSound->Play(pSound->SOUND_LABEL_SE_ITEM000);

		//�A�C�e���擾�����Z
		pItemUI->Add(1);

		//�I������
		CItem::Uninit();
	}
}