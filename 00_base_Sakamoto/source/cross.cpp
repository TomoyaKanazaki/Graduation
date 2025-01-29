//============================================
//
//	�I�u�W�F�N�g�w���f���̃T���v�� [SampleObjX.cpp]
//	Author:sakamoto kai
//
//============================================
#include "cross.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "sound.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const int CROSS_DELETTIME = 600;	// �\���ˏ��Ŏ���
	const D3DXVECTOR3 INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 200.0f);		//�����蔻��
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
	const char* MODEL_PASS = "data\\MODEL\\zyuzika.x"; // ���f���p�X
	const D3DXVECTOR3 ADD_ROT = D3DXVECTOR3(0.01f, 0.01f, 0.01f); // ��]��
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CCross>* CCross::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CCross::CCross(int nPriority) : CItem(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_EventPos = INITVECTOR3;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CCross::~CCross()
{

}

//====================================================================
//����������
//====================================================================
HRESULT CCross::Init()
{
	// �e�N���X�̏�����
	if (FAILED(CItem::Init(MODEL_PASS))) { assert(false); return E_FAIL; }

	// �I�u�W�F�N�g�̎�ނ�ݒ�
	SetType(CObject::TYPE_CROSS);

	// �`����I�t�ɂ���
	SetDisp(false);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CCross>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CCross::Uninit(void)
{
	if (m_pList != nullptr)
	{
		// ���X�g���玩�g�̃I�u�W�F�N�g���폜
		m_pList->DelList(m_iterator);

		if (m_pList->GetNumAll() == 0)
		{ // �I�u�W�F�N�g������Ȃ��ꍇ

			// ���X�g�}�l�[�W���[�̔j��
			m_pList->Release(m_pList);
		}
	}

	// �e�N���X�̏I������
	CItem::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CCross::Update(void)
{
	// ��]
	Rotation();

	// �e�N���X�̍X�V����
	CItem::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CCross::Draw(void)
{
	// �e�N���X�̕`�揈��
	CItem::Draw();
}

//====================================================================
//�@�q�b�g����
//====================================================================
bool CCross::Hit(CPlayer* pPlayer)
{
	// ���ɐ����������Ă����ꍇ�֐��𔲂���
	if (pPlayer->GetItemType() == CPlayer::TYPE_BIBLE) { return false; }

	// �\���ˏ������Ԃ̃��Z�b�g
	pPlayer->ResetCrossTimer();

	// ���ɏ\���˂������Ă����ꍇ�֐��𔲂���
	if (pPlayer->GetItemType() == CPlayer::TYPE_CROSS) { return false; }

	// �v���C���[�̃A�C�e����ݒ�
	pPlayer->SetItemType(CPlayer::TYPE_CROSS);

	// �G�t�F�N�g�𐶐�����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_GETITEM, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);

	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_GET_CROSS);
	return true;
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CCross>* CCross::GetList(void)
{
	return m_pList;
}

//==========================================
//  ��]���鏈��
//==========================================
void CCross::Rotation()
{
	// ���݂̌������擾����
	D3DXVECTOR3 rot = GetRot();

	// ���������Z
	rot += ADD_ROT;

	// ������K�p
	SetRot(rot);
}

//==========================================
//  �G�t�F�N�g�𐶐�
//==========================================
void CCross::SetEffect()
{
	// ���g�̏����擾����
	D3DXVECTOR3 pos = GetPos();
	pos.y = 0.0f;
	D3DXVECTOR3 rot = GetRot();

	Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_CROSS, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot, D3DXVECTOR3(20.0f, 20.0f, 20.0f)));
}
