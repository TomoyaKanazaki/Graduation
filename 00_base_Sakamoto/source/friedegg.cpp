//============================================
//
//	�ڋʏĂ��N���X(friedegg.cpp)
//	Author : Tomoya Kanazaki
//
//============================================
#include "friedegg.h"
#include "XModel.h"
#include "player.h"
#include "manager.h"
#include "sound.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float DELET_TIME = 10.0f;	// ���Ŏ���
	const char* MODEL_PASS = "data\\MODEL\\02_Item\\friedegg.x"; // ���f���p�X
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CFriedEgg>* CFriedEgg::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CFriedEgg::CFriedEgg(int nPriority) : CItem(nPriority),
m_fDeleteTimer(0.0f),
m_bMove(true),
m_eCreateType(CEnemy::ENEMY_NONE)
{
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CFriedEgg::~CFriedEgg()
{

}

//====================================================================
//����������
//====================================================================
HRESULT CFriedEgg::Init()
{
	// �e�N���X�̏�����
	if (FAILED(CItem::Init(MODEL_PASS))) { assert(false); return E_FAIL; }

	// �I�u�W�F�N�g�̎�ނ�ݒ�
	SetType(CObject::TYPE_FRIEDEGG);

	// �^�C�v�̐ݒ�
	SetItem(CItem::TYPE_FRIEDEGG);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CFriedEgg>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CFriedEgg::Uninit(void)
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
void CFriedEgg::Update(void)
{
	// ���Ń^�C�}�[�����Z����
	m_fDeleteTimer += DeltaTime::Get();

	// ���ŏ����𖞂������ꍇ
	if (m_fDeleteTimer >= DELET_TIME)
	{
		// �G�𐶐�����
		CEnemy::Create(m_eCreateType, GetGrid());

		// ���g���I�����֐��𔲂���
		Uninit();
		return;
	}

	// �e�N���X�̍X�V����
	CItem::Update();

	DebugProc::Print(DebugProc::POINT_CENTER, "�ڋʏĂ� : %f, %f\n", GetPos().x, GetPos().z);
}

//====================================================================
//�`�揈��
//====================================================================
void CFriedEgg::Draw(void)
{
	// �e�N���X�̕`�揈��
	CItem::Draw();
}

//====================================================================
//�@�q�b�g����
//====================================================================
bool CFriedEgg::Hit(CPlayer* pPlayer)
{
	// �ړ��t���O��false�̏ꍇfalse��Ԃ�
	if (!m_bMove) { return false; }

	// �ړ��t���O��false�ɂ���
	m_bMove = false;

	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_EAT);

	// �G�t�F�N�g�𐶐�����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	//MyEffekseer::EffectCreate(CMyEffekseer::TAYPE_EAT, false, useful::CalcMatrix(pos, rot,GetUseMultiMatrix()), rot);

	return true;
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CFriedEgg>* CFriedEgg::GetList(void)
{
	return m_pList;
}

//==========================================
//  ��������
//==========================================
CFriedEgg* CFriedEgg::Create(const CEnemy::ENEMY_TYPE eType, const CMapSystem::GRID& pos)
{
	// �C���X�^���X����
	CFriedEgg* pItem = new CFriedEgg;

	// ����������
	if (FAILED(pItem->Init())) { assert(false); return nullptr; }

	// �ʒu�̐ݒ�
	pItem->SetGrid(pos);

	// �^�C�v�̐ݒ�
	pItem->SetEnemy(eType);

	return pItem;
}

//==========================================
//  �ړ�����
//==========================================
void CFriedEgg::Move(D3DXVECTOR3& pos)
{
	// �ړ��t���O��false�̏ꍇ�֐��𔲂���
	if (!m_bMove) { return; }

	// TODO : �����_�����s�ł����ł��d�l��p�ӂ���
}
