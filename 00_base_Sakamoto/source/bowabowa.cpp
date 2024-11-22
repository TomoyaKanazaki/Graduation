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
#include "sound.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
	const char* MODEL_PASS = "data\\MODEL\\Testbowabowa.x"; // ���f���p�X
	const float MOVE_HEIGHT = 25.0f; // �ړ���
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
	// �e�N���X�̏�����
	if(FAILED(CItem::Init(MODEL_PASS))){ assert(false); return E_FAIL; }

	// �I�u�W�F�N�g�̎�ނ�ݒ�
	SetType(CObject::TYPE_BOWABOWA);

	// �����̎��Ԃ�ݒ肷��
	float time = (float)(rand() / 628) * 0.01f;
	SetMoveTime(time);

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

	// �e�N���X�̏I������
	CItem::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CBowabowa::Update(void)
{
	// �e�N���X�̍X�V����
	CItem::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CBowabowa::Draw(void)
{
	// �e�N���X�̕`�揈��
	CItem::Draw();
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

	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_GET_BOWA);

	// �G�t�F�N�g�𐶐�����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	//MyEffekseer::EffectCreate(CMyEffekseer::TYPE_EAT, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);

	// �폜
	Uninit();
	return true;
}

//========================================== 
//  �ړ�����
//==========================================
void CBowabowa::Move(D3DXVECTOR3& pos)
{
	// �ړ������擾
	D3DXVECTOR3 base = GetBase();
	float time = GetMoveTime();

	// �o�ߎ��Ԃ��擾
	time += DeltaTime::Get();

	// �ړ����Ɍo�ߎ��Ԃ��������킹��
	float fScale = sinf(time) * MOVE_HEIGHT;

	// ��ʒu�Ɉړ��ʂ����Z����
	pos.y = base.y + fScale;
}

//==========================================
//  �G�t�F�N�g�𐶐�
//==========================================
void CBowabowa::SetEffect()
{
	// ���g�̏����擾����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_BOABOA, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot, D3DXVECTOR3(20.0f, 20.0f, 20.0f)));
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CBowabowa>* CBowabowa::GetList(void)
{
	return m_pList;
}
