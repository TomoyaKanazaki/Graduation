//========================================
//
// ��Q���̏���[reverse.cpp]
// Author�F���n�Ή�
//
//========================================
#include "reverse.h"
#include "model.h"
#include "calculation.h"
#include "billboard.h"
#include "manager.h"
#include "texture.h"
#include "reverse_direction.h"
#include "player.h"

//========================================
//���O���
//========================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\reverse.png";	// ���f���̃p�X
	const float ITEMSIZE = 150.0f;	// �T�C�Y
	const float COLLISIONRANGE = 200.0f;	// �L���͈�
	const float ROTATION_RANGE = D3DX_PI * 0.2f;
}
CListManager<CReverse> CReverse::m_List = {};	// ���X�g

//========================================
//�R���X�g���N�^
//========================================
CReverse::CReverse(int nPriority) : CStageObj(nPriority),
m_fRotationTime(0.0f),	// ��]����
m_pBillboard(nullptr)
{

}

//========================================
//�f�X�g���N�^
//========================================
CReverse::~CReverse()
{

}

//========================================
// ������
//========================================
HRESULT CReverse::Init(void)
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// �r���{�[�h����
	m_pBillboard = CBillboard::Create(GetPos(), ITEMSIZE, ITEMSIZE);
	m_pBillboard->SetEnableFront(false);

	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	int texIdx = pTexture->Regist(TEXTURE);
	m_pBillboard->BindTexture(texIdx);

	if (FAILED(CStageObj::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================================
// �I��
//========================================
void CReverse::Uninit(void)
{
	// ���X�g����폜
	m_List.Delete(this);

	// �I��
	CStageObj::Uninit();
}

//========================================
// �폜
//========================================
void CReverse::Kill()
{
	if (m_pBillboard != nullptr)
	{
		m_pBillboard->Uninit();
		m_pBillboard = nullptr;
	}

	// �I��
	CReverse::Uninit();
}

//==========================================================================
// �ʒu
//==========================================================================
void CReverse::SetPos(D3DXVECTOR3 pos)
{
	CObject::SetPos(pos);

	if (m_pBillboard != nullptr)
	{
		m_pBillboard->SetPos(pos);
	}
}

//========================================
// �X�V
//========================================
void CReverse::Update(void)
{
	// �X�V
	CStageObj::Update();

	if (m_pBillboard != nullptr)
	{
		m_pBillboard->SetPos(GetPos() + MyLib::Vector3(0.0f, ITEMSIZE, 0.0f));

		// ���邭��
		MyLib::Vector3 rot = m_pBillboard->GetRot();
		m_fRotationTime += CManager::GetInstance()->GetDeltaTime();
		rot.y = cosf(m_fRotationTime / 1.0f) * ROTATION_RANGE;
		m_pBillboard->SetRot(rot);

		m_pBillboard->Update();
	}
}

//========================================
// �`��
//========================================
void CReverse::Draw(void)
{
	if (m_pBillboard != nullptr)
	{
		m_pBillboard->Draw();
	}

	// �`��
	CStageObj::Draw();
}

//========================================
// �v���C���[�Ƃ̓����蔻��
//========================================
bool CReverse::Collision(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rSize, const CPlayer::PLAYERSTATE /*state*/)
{
	bool bHit = false;

	// �ʒu�擾
	MyLib::Vector3 pos = GetPos();
	MyLib::Vector3 posTarget = UtilFunc::Transformation::WorldMtxChangeToPosition(rMtx);

	if (UtilFunc::Collision::CircleRange3D(pos, posTarget, COLLISIONRANGE, rSize.x))
	{
		Kill();
		CReverse_Direction::Create();
		bHit = true;
	}

	return bHit;

}
