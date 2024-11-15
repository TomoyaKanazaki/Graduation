//============================================
//
//	�����̏��� [bible.cpp]
//	Author:morikawa shunya
//
//============================================
#include "bible.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "effect.h"
#include "game.h"
#include "objmeshField.h"
#include "sound.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
	const char* MODEL_PASS = "data\\MODEL\\02_item\\holybible.x"; // ���f���p�X
	const float MOVE_SCALE = sqrtf(100.0f * 100.0f * 2.0f); // �ړ���
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CBible>* CBible::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
// �R���X�g���N�^
//====================================================================
CBible::CBible(int nPriority) : CItem(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CBible::~CBible()
{

}

//====================================================================
// ������
//====================================================================
HRESULT CBible::Init()
{
	// �e�N���X�̏�����
	if (FAILED(CItem::Init(MODEL_PASS))) { assert(false); return E_FAIL; }

	// �I�u�W�F�N�g�̎�ނ�ݒ�
	SetType(CObject::TYPE_BIBLE);

	// �X�N���[���̑Ώۂ���O��
	SetMapScroll(false);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CBible>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
// �I��
//====================================================================
void CBible::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �p���N���X�̏I��
	CItem::Uninit();
}

//====================================================================
// �X�V
//====================================================================
void CBible::Update(void)
{
	//�e�N���X�̍X�V
	CItem::Update();
}

//====================================================================
// �`��
//====================================================================
void CBible::Draw(void)
{
	// �p���N���X�̕`��
	CItem::Draw();
}

//====================================================================
// �����̐���
//====================================================================
void CBible::Move(D3DXVECTOR3& pos)
{
	// �ړ������擾
	D3DXVECTOR3 base = GetBase();
	float time = GetMoveTime();

	// �t�B�[���h�̍��W���擾
	D3DXVECTOR3 posField = CGame::GetMapField()->GetPos();

	// ���g�̍��W�ƃt�B�[���h�����Ԏ��̌������Z�o
	D3DXVECTOR3 vec = pos - posField;
	float rot = atan2f(vec.z, vec.x);

	// �o�ߎ��Ԃ��擾
	time += DeltaTime::Get();

	// �ړ����Ɍo�ߎ��Ԃ��������킹��
	float fScale = sinf(time) * MOVE_SCALE;

	// �ړ�����xz�����ɕ�������
	float x = fScale * cosf(rot);
	float z = fScale * sinf(rot);

	// ��ʒu�Ɉړ��ʂ����Z����
	pos.x = base.x + x;
	pos.z = base.z + z;
}

//====================================================================
// ��ԊǗ�
//====================================================================
bool CBible::Hit(CPlayer* pPlayer)
{
	// ���ɐ����������Ă���ꍇ�֐��𔲂���
	if (pPlayer->GetItemType() == CPlayer::TYPE_BIBLE) { return false; }

	// �v���C���[�̃A�C�e����ݒ�
	pPlayer->SetItemType(CPlayer::TYPE_BIBLE);

	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_GET_BIBLE);

	// ���g�̍폜
	Uninit();
	return true;
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CBible>* CBible::GetList(void)
{
	return m_pList;
}
