//=============================================================================
// 
//  ���]���o���� [reverse_direction.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "reverse_direction.h"
#include "manager.h"
#include "texture.h"
#include "calculation.h"
#include "sound.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\reverse.png";	// �e�N�X�`���̃t�@�C��
}

namespace StateTime
{
	const float APPEARANCE = 1.0f;	// �o��
	const float WAIT = 0.5f;		// �ҋ@
	const float FADEOUT = 0.3f;		// �t�F�[�h�A�E�g
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CReverse_Direction::STATE_FUNC CReverse_Direction::m_StateFuncList[] =
{
	&CReverse_Direction::StateNone,			// �Ȃ�
	&CReverse_Direction::StateAppearance,	// �o��
	&CReverse_Direction::StateWait,			// �ҋ@
	&CReverse_Direction::StateFadeout,		// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CReverse_Direction::CReverse_Direction(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CReverse_Direction::~CReverse_Direction()
{

}

//==========================================================================
// ��������
//==========================================================================
CReverse_Direction* CReverse_Direction::Create()
{
	// �������̊m��
	CReverse_Direction* pObj = new CReverse_Direction;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CReverse_Direction::Init()
{

	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 180.0f);
#endif
	SetSize(size.x, size.y);
	SetSizeOrigin(size);


	// �ʒu
	SetPos(MyLib::Vector3(640.0f, 360.0f, 0.0f));
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// ��ԑJ��
	SetState(State::STATE_APPEARANCE);

	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �T�E���h�Đ�
	pSound->PlaySound(CSound::SOUND_LABEL_SE_REVERSE);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CReverse_Direction::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CReverse_Direction::Update()
{
	// ��ԍX�V
	UpdateState();

	if (IsDeath()) return;

	// �X�V����
	CObject2D::Update();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CReverse_Direction::UpdateState()
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ���s
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CReverse_Direction::StateNone()
{

}

//==========================================================================
// �o��
//==========================================================================
void CReverse_Direction::StateAppearance()
{
	// �����x�Ɗ�������
	float ratio = UtilFunc::Correction::EaseInExpo(0.3f, 1.0f, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, ratio));

	// ��]
	MyLib::Vector3 rot = GetRot();
	rot.z = UtilFunc::Correction::EaseInExpo(0.0f, -(D3DX_PI * 2.0f) * 1.0f, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetRot(rot);

	// �T�C�Y
	D3DXVECTOR2 size;
	size.x = UtilFunc::Correction::EaseInBack(GetsizeOrigin().x, GetsizeOrigin().x * 3.0f, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	size.y = UtilFunc::Correction::EaseInBack(GetsizeOrigin().y, GetsizeOrigin().y * 3.0f, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetSize(size.x, size.y);

	if (m_fStateTime >= StateTime::APPEARANCE)
	{
		SetState(State::STATE_WAIT);
	}
}

//==========================================================================
// �ҋ@
//==========================================================================
void CReverse_Direction::StateWait()
{
	if (m_fStateTime >= StateTime::WAIT)
	{
		SetState(State::STATE_FADEOUT);
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CReverse_Direction::StateFadeout()
{
	// �����x�Ɗ�������
	float ratio = UtilFunc::Correction::EaseInExpo(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, ratio));

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		Uninit();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CReverse_Direction::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CReverse_Direction::SetState(const State& state)
{
	m_fStateTime = 0.0f;
	m_state = state;
}
