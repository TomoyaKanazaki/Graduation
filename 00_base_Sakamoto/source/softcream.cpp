//============================================
//
//  �\�t�g�N���[���̏��� [softcream.cpp]
//	Author:morikawa shunya
//
//============================================
#include "softcream.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "MapSystem.h"
#include "game.h"
#include "objmeshField.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
	const char* MODEL_PASS = "data\\MODEL\\02_item\\softcream.x"; // ���f���p�X
	const float MOVE_SCALE_X = 1050.0f; // �ړ���
	const float MOVE_SCALE_Z = 650.0f; // �ړ���

	// TODO : �{�d�l�̎��ɕK������
	float fMove = 0;
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CSoftCream>* CSoftCream::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
// �R���X�g���N�^
//====================================================================
CSoftCream::CSoftCream(int nPriority) : CItem(nPriority)
{
	SetSize(COLLISION_SIZE);
	SetPos(INITVECTOR3);
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CSoftCream::~CSoftCream()
{

}

//====================================================================
// ������
//====================================================================
HRESULT CSoftCream::Init()
{
	// �e�N���X�̏�����
	if (FAILED(CItem::Init(MODEL_PASS))) { assert(false); return E_FAIL; }

	// �I�u�W�F�N�g�̎�ނ�ݒ�
	SetType(CObject::TYPE_SOFTCREAM);

	// �X�N���[���̑Ώۂ���O��
	SetMapScroll(false);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CSoftCream>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
// �I��
//====================================================================
void CSoftCream::Uninit(void)
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
void CSoftCream::Update(void)
{
	//�e�N���X�̍X�V
	CItem::Update();
}

//====================================================================
// �`��
//====================================================================
void CSoftCream::Draw(void)
{
	// �p���N���X�̕`��
	CItem::Draw();
}

//====================================================================
// �����̐���
//====================================================================
void CSoftCream::Move(D3DXVECTOR3& pos)
{
	// �ړ������擾
	D3DXVECTOR3 base = GetBase();
	float time = GetMoveTime();

	// �ړ��ʉ��Z
	float x = MOVE_SCALE_X * sinf(time);
	float z = MOVE_SCALE_Z * cosf(time);

	// ��ʒu�Ɉړ��ʂ����Z����
	pos.x = base.x + x;
	pos.z = base.z + z;

	DebugProc::Print(DebugProc::POINT_CENTER, "�\�t�g�N���[�� : %f, %f\n", pos.x, pos.z);
}

//====================================================================
// ��ԊǗ�
//====================================================================
bool CSoftCream::Hit(CPlayer* pPlayer)
{
	// �G�t�F�N�g�𐶐�
		// �G�t�F�N�g�𐶐�����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_GET_SOFTCREAM, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);

	// ���g�̍폜
	Uninit();
	return true;
}

//==========================================
//  �G�t�F�N�g�𐶐�
//==========================================
void CSoftCream::SetEffect()
{
	// ���g�̏����擾����
	D3DXVECTOR3 pos = GetPos();
	pos.y = 0.0f;
	D3DXVECTOR3 rot = GetRot();

	Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_SOFTCREAM, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot, D3DXVECTOR3(20.0f, 20.0f, 20.0f)));
}

//==========================================
// ���X�g�̎擾
//==========================================
CListManager<CSoftCream>* CSoftCream::GetList(void)
{
	return m_pList;
}
