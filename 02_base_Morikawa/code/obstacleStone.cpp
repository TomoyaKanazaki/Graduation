//========================================
//
// ��Q��(��)�̏���[obstacleStone.cpp]
// Author�F��������
//
//========================================
#include "calculation.h"
#include "obstacleStone.h"
#include "player.h"
#include "sound.h"

#include "model.h"

//========================================
//���O���
//========================================
namespace
{
	const D3DXVECTOR3 SIZEMAX = D3DXVECTOR3(50.0f, 500.0f, 50.0f);		// �T�C�Y�̍ő�l


	const D3DXVECTOR3 SIZEMIN = D3DXVECTOR3(-50.0f, 0.0f, -50.0f);		// �T�C�Y�̍ŏ��l
}

//========================================
//�R���X�g���N�^
//========================================
CObstacleStone::CObstacleStone() : CObstacle()
{//�l���N���A
}

//========================================
//�f�X�g���N�^
//========================================
CObstacleStone::~CObstacleStone()
{
}

//========================================
// ������
//========================================
HRESULT CObstacleStone::Init(const EType type)
{
	if (FAILED(CObstacle::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	return S_OK;
}

//========================================
// �I��
//========================================
void CObstacleStone::Uninit(void)
{
	// �I��
	CObstacle::Uninit();
}

//========================================
// �X�V
//========================================
void CObstacleStone::Update(void)
{
	// �X�V
	CObstacle::Update();
}

//========================================
// �`��
//========================================
void CObstacleStone::Draw(void)
{
	// �`��
	CObstacle::Draw();
}

//========================================
// �v���C���[�Ƃ̓����蔻��
//========================================
bool CObstacleStone::Collision(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rSize, const CPlayer::PLAYERSTATE state)
{
	D3DXVECTOR3 max = rSize;
	D3DXVECTOR3 min = D3DXVECTOR3(-rSize.x, 0.0f, -rSize.z);

	// ���f���� NULL ����Ȃ��ꍇ
	if (m_pModel == nullptr) { assert(false); return false; }

	if (UtilFunc::Collision::IsAABBCollidingWithBox
	(
		MyLib::AABB(min, max),
		MyLib::Matrix(rMtx),
		MyLib::AABB(SIZEMIN, SIZEMAX),
		m_pModel->GetMtxWorld())
		)
	{
 		if (state == CPlayer::STATE_ROLL)
		{ // �]���蒆�̏ꍇ�A������

			// �T�E���h���擾
			CSound* pSound = CManager::GetInstance()->GetSound();

			// �T�E���h�Đ�
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE_CLASH);

			// �I������
			Uninit();

			// false ��Ԃ�
			return false;
		}
		else
		{ // ��L�ȊO

			// true ��Ԃ�
			return true;
		}
	}

	// �����蔻���Ԃ�
	return false;
}
