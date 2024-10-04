//========================================
//
//�L�����N�^�[�S�ʏ���[character.cpp]
//Author�F�X��x��
//
//========================================
#include "character.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugproc.h"
#include "game.h"
#include "camera.h"
#include "particle.h"

//========================================
// �萔��`
//========================================
namespace
{
const float CHARACTER_INERTIA = 0.3f;	// �L�����N�^�[�̊���
}

//========================================
//�R���X�g���N�^
//========================================
CCharacter::CCharacter(int nPriority) :
	m_pTexture(nullptr),
	m_nIdxTexture(0),				// �e�N�X�`���̔ԍ�
	m_pMesh(nullptr),				// ���b�V��(���_���)�ւ̃|�C���^
	m_pBuffMat(nullptr),			// �}�e���A���ւ̃|�C���^
	m_dwNumMat(0),					// �}�e���A���̐�
	m_apNumModel(0), 				// ���f��(�p�[�c)�̑���
	m_RotDest(0.0f, 0.0f, 0.0f),	// �ړI�̌���
	m_nState(STATE_NONE),			// �L�����̏��
	m_bJump(false),					// �W�����v��
	m_bMove(false),					// �ړ����[�V����
	m_bWait(false),					// �ҋ@���[�V����
	m_pMotion(nullptr)				// ���[�V�����|�C���^
{//�l���N���A
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//���f���̃|�C���^
}

//========================================
//�f�X�g���N�^
//========================================
CCharacter::~CCharacter()
{
}

//========================================
// ������
//========================================
HRESULT CCharacter::Init(std::string pfile)
{
	D3DXVECTOR3 pos = GetPos();		// �ʒu�擾
	D3DXVECTOR3 move = GetMove();	// �ړ��ʎ擾

	// �����̐ݒ�
	m_RotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �ŏ��l
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �ő�l
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �ʒu�ݒ�
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �����ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	// ��ԏ�����
	m_nState = STATE_NONE;

	//���[�V�����̃|�C���^
	m_pMotion = nullptr;

	if (m_pMotion == nullptr)
	{
		// ���[�V��������
		m_pMotion = CMotion::Create();

		// ���[�V�����ǂݍ���
		m_pMotion->Load(pfile);

		// �ҋ@
		m_bWait = true;
	}

	return S_OK;
}

//========================================
// �I��
//========================================
void CCharacter::Uninit(void)
{
	if (m_pMotion != nullptr)
	{//���[�V�����j��
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	//�I�u�W�F�N�g(�������g)�̔j��
	Release();
}

//========================================
// �X�V
//========================================
void CCharacter::Update(void)
{
	// ���[�V�����X�V
	m_pMotion->Update();
}

//========================================
// �`��
//========================================
void CCharacter::Draw(void)
{
	//�`��
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//CRenderer�^�̃|�C���^
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�ʒu�擾
	D3DXVECTOR3 pos = GetPos();

	// �����擾
	D3DXVECTOR3 rot = GetRot();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���[�V�����`��
	m_pMotion->Draw();
}
