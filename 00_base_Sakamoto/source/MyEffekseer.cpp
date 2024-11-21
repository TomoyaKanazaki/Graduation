//===========================================================
//
// �G�t�F�N�V�A[MyEffekseer.cpp]
// Author �匴�叫
//
//===========================================================
#include <stdio.h>
#include <string>
#include <windows.h>

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "MyEffekseer.h"
#include "camera.h"

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const float DELTA_TIME = 60.0f;
}

//==========================================
//  �ÓI�����o�ϐ��錾
//==========================================
CMyEffekseer* CMyEffekseer::m_pInstance = nullptr; // ���g�̃|�C���^

namespace MyEffekseer
{
	// �G�t�F�N�g�̖��O
	const char* EFFECT_PATH[] =
	{
		"data\\EFFEKSEER\\Effect\\fireball.efkefc",       // �t�@�C�A�{�[��
		"data\\EFFEKSEER\\Effect\\charge.efkefc",         // �`���[�W
		"data\\EFFEKSEER\\Effect\\smoke.efkefc",          // �X���[�N
		"data\\EFFEKSEER\\Effect\\hit.efkefc",			  // �q�b�g
		"data\\EFFEKSEER\\Effect\\get_items.efkefc",      // �A�C�e���擾
		"data\\EFFEKSEER\\Effect\\eat.efkefc",			  // �ߐH
		"data\\EFFEKSEER\\Effect\\medaman_respawn.efkefc",			  // ���_�}���̃��X�|�[��
		"data\\EFFEKSEER\\Effect\\bonbon_respawn.efkefc",			  // �{���{���̃��X�|�[��
		"data\\EFFEKSEER\\Effect\\smalldevil_respawn.efkefc",		  // �R�f�r���̃��X�|�[��
		"data\\EFFEKSEER\\Effect\\boaboa.efkefc",		  // �{�A�{�A
		"data\\EFFEKSEER\\Effect\\cross.efkefc",		  // �\����
		"data\\EFFEKSEER\\Effect\\bible.efkefc",		  // ����
		"data\\EFFEKSEER\\Effect\\dustcloud.efkefc",		  // ���������̍���
		"data\\EFFEKSEER\\Effect\\fall_smoke.efkefc",		  // �����̓y��
		"data\\EFFEKSEER\\Effect\\born_player00.efkefc",		  // �v���C���[�P�̗�����
		"data\\EFFEKSEER\\Effect\\born_player01.efkefc",		  // �v���C���[2�̗�����
	};

	//============================================
	// �G�t�F�N�g�̐���
	// �p�����[�^�[
	// type  : ��
	// bLopp : ���[�v���邩�̃t���O
	// pos   : �G�t�F�N�g�̈ʒu
	// rot   : �G�t�F�N�g�̌���
	// scale : �G�t�F�N�g�̑傫��
	// 
	// �߂�l
	// �G�t�F�N�g�B���s�����Ƃ���nullptr���Ԃ�
	//============================================
	CEffekseer* EffectCreate(CMyEffekseer::TYPE type, bool bLoop, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
	{
		// �G�t�F�N�V�A�擾
		CMyEffekseer* pMyEffekseer = CManager::GetInstance()->GetEffect();

		// �g�p����Ă��Ȃ��ꍇ�����𔲂���
		if (pMyEffekseer == nullptr) { assert(false); return nullptr; }

		// �G�t�F�N�g�𐶐�
		CEffekseer* pEffekseer = pMyEffekseer->CreateEffect(type,
			Effekseer::Vector3D(pos.x, pos.y, pos.z),
			Effekseer::Vector3D(rot.x, rot.y, rot.z),
			Effekseer::Vector3D(scale.x, scale.y, scale.z),
			bLoop);

		return pEffekseer;
	}
}

//==========================================
//  �ÓI�x������
//==========================================
static_assert(NUM_ARRAY(MyEffekseer::EFFECT_PATH) == CMyEffekseer::TYPE_MAX, "ERROR : Type Count Missmatch");

//===========================================================
// �R���X�g���N�^
//===========================================================
CMyEffekseer::CMyEffekseer()
{
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CMyEffekseer::~CMyEffekseer()
{
}

//===========================================================
// ��������
//===========================================================
CEffekseer* CMyEffekseer::CreateEffect(const TYPE eType, ::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale, bool bLoop)
{
	CEffekseer* pEffect = new CEffekseer;

	if (pEffect == nullptr)
		return nullptr;

	// char16_t�ɕϊ�
	std::string file = MyEffekseer::EFFECT_PATH[eType];
	int len = MultiByteToWideChar(CP_UTF8, 0, file.c_str(), -1, nullptr, 0);
	std::u16string string16t(len, 0);
	MultiByteToWideChar(CP_UTF8, 0, file.c_str(), -1, reinterpret_cast<LPWSTR>(&string16t[0]), len);

	// �G�t�F�N�g�̓Ǎ�
	Effekseer::EffectRef effect = Effekseer::Effect::Create(m_EfkManager, string16t.c_str());
	pEffect->SetEffect(effect);

	// �G�t�F�N�g�̍Đ�
	Effekseer::Handle Handle = m_EfkManager->Play(effect, 0, 0, 0);
	pEffect->SetEfkHandle(Handle);

	// ��ސݒ�
	pEffect->SetEfkType(eType);

	// �G�t�F�N�g�l�[���ݒ�
	pEffect->SetEfkName(MyEffekseer::EFFECT_PATH[eType]);

	// �ʒu�ݒ�
	pEffect->SetPosition(pos);

	// �����ݒ�
	pEffect->SetRotation(rot);

	// �傫���ݒ�
	pEffect->SetScale(scale);

	// ����������
	pEffect->Init(pos, rot, scale, bLoop);

	// �ʒu�A�����A�傫���ݒ�
	m_EfkManager->SetLocation(Handle, pos);
	m_EfkManager->SetRotation(Handle, rot.X, rot.Y, rot.Z);
	m_EfkManager->SetScale(Handle, scale.X, scale.Y, scale.Z);

	return pEffect;
}

//===========================================================
// ����������
//===========================================================
void CMyEffekseer::Init(void)
{
	// �G�t�F�N�g�̃}�l�[�W���[�̍쐬
	m_EfkManager = ::Effekseer::Manager::Create(8000);

	// ���_�ʒu���m��
	m_ViewerPosition = ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f);

	// ���W�n��ݒ肷��B�A�v���P�[�V�����ƈ�v������K�v������B
	m_EfkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// Effekseer�̃��W���[�����Z�b�g�A�b�v����
	SetupEffekseerModules(m_EfkManager);

	// ���e�s���ݒ�
	m_ProjectionMatrix.PerspectiveFovLH(0.5f * D3DX_PI, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f);

	// �J�����s���ݒ�
	m_CameraMatrix.LookAtLH(m_ViewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));
}

//===========================================================
// �I������
//===========================================================
void CMyEffekseer::Uninit(void)
{
	// �G�t�F�N�g���X�g���擾
	if (CEffekseer::GetList() == nullptr) { return; }
	std::list<CEffekseer*> list = CEffekseer::GetList()->GetList();    // ���X�g���擾
	
	// ���X�g�̒��g���폜����
	for (CEffekseer* effect : list)
	{
		effect->Uninit();
	}

	// ���g���I������
	delete m_pInstance;
}

//===========================================================
// �X�V����
//===========================================================
void CMyEffekseer::Update(void)
{
	// �^�C�����Z
	m_nTime++;

	// �G�t�F�N�g���X�g���擾
	if (CEffekseer::GetList() == nullptr) { return; }
	std::list<CEffekseer*> list = CEffekseer::GetList()->GetList();    // ���X�g���擾

	for(CEffekseer* effect : list)
	{
		// �G�t�F�N�g�����擾
		Effekseer::Handle Handle = effect->GetHandle();
		Effekseer::Vector3D pos = effect->GetPosition();
		Effekseer::Vector3D rot = effect->GetRotation();
		Effekseer::Vector3D scale = effect->GetScale();

		// �ʒu������A�傫�����Đݒ�
		m_EfkManager->SetLocation(Handle, pos);
		m_EfkManager->SetRotation(Handle, rot.X, rot.Y, rot.Z);
		m_EfkManager->SetScale(Handle, scale.X, scale.Y, scale.Z);

		// �G�t�F�N�g�̍Đ����I�����Ă��Ȃ��ꍇ���ɐi��
		if (m_EfkManager->Exists(Handle) && !effect->IsDeath()) { continue; }
		
		// �Đ��̒�~
		m_EfkManager->StopEffect(Handle);

		// ���[�v�G�t�F�N�g�łȂ��ꍇ�I�����Ď��ɐi��
		if (!effect->IsLoop() || effect->IsDeath())
		{
			// �I������ 
			effect->Uninit();
			continue;
		}

		// �G�t�F�N�g�{�̂̏����擾����
		Effekseer::EffectRef reference = effect->GetEffect();

		// �G�t�F�N�g�̍Đ�
		Handle = m_EfkManager->Play(reference, pos);

		// �n���h���̐ݒ�
		effect->SetEfkHandle(Handle);
	}

	// ���C���[�p�����[�^�̐ݒ�
	Effekseer::Manager::LayerParameter layerParameter;
	layerParameter.ViewerPosition = m_ViewerPosition;
	m_EfkManager->SetLayerParameter(0, layerParameter);

	// �}�l�[�W���[�̍X�V
	Effekseer::Manager::UpdateParameter updateParameter;
	m_EfkManager->Update(updateParameter);
}

//===========================================================
// �`�揈��
//===========================================================
void CMyEffekseer::Draw(void)
{
	// �����_���[���g�p����Ă��Ȃ��ꍇ�����𔲂���
	if (m_EfkRenderer == nullptr)
		return;

	// �^�C���ݒ�
	m_EfkRenderer->SetTime(m_nTime / DELTA_TIME);

	// ���e�s���ݒ�
	m_EfkRenderer->SetProjectionMatrix(m_ProjectionMatrix);

	// �J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �J�������g�p����Ă��Ȃ��ꍇ����������
	if (pCamera == nullptr)
		return;

	auto ViewMatrix = pCamera->GetViewMatrix();
	auto Projection = pCamera->GetProjectionMatrix();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_ProjectionMatrix.Values[i][j] = Projection.m[i][j];
			m_CameraMatrix.Values[i][j] = ViewMatrix.m[i][j];
		}
	}

	// �J�����s���ݒ�
	m_EfkRenderer->SetCameraMatrix(m_CameraMatrix);

	// �G�t�F�N�g�̕`��J�n����
	m_EfkRenderer->BeginRendering();

	// �G�t�F�N�g�̕`��
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix = m_EfkRenderer->GetCameraProjectionMatrix();
	m_EfkManager->Draw(drawParameter);

	// �G�t�F�N�g�̕`��I������
	m_EfkRenderer->EndRendering();
}

//==========================================
//  ���g�̎擾
//==========================================
CMyEffekseer* CMyEffekseer::GetInstance(void)
{
	// ���g�����݂��Ă��Ȃ��ꍇ��������
	if (m_pInstance == nullptr)
	{
		assert(false);
		return nullptr;
	}

	return m_pInstance;
}

//===========================================================
// ���W���[���ݒ�
//===========================================================
void CMyEffekseer::SetupEffekseerModules(::Effekseer::ManagerRef efkManager)
{
	// �`��f�o�C�X�̍쐬
	Effekseer::Backend::GraphicsDeviceRef graphicsDevice;
	graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(CManager::GetInstance()->GetRenderer()->GetDevice());

	// �G�t�F�N�g�̃����_���[�̍쐬
	m_EfkRenderer = ::EffekseerRendererDX9::Renderer::Create(graphicsDevice, 8000);

	// �`�惂�W���[���̐ݒ�
	efkManager->SetSpriteRenderer(m_EfkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(m_EfkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(m_EfkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(m_EfkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(m_EfkRenderer->CreateModelRenderer());

	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���
	efkManager->SetTextureLoader(m_EfkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(m_EfkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(m_EfkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

//===========================================================================
// �G�t�F�N�g�N���X
//===========================================================================

//==========================================
//  �ÓI�����o�ϐ��錾
//==========================================
CListManager<CEffekseer>* CEffekseer::m_pList = nullptr; // �I�u�W�F�N�g���X�g]

//===========================================================
// �R���X�g���N�^
//===========================================================
CEffekseer::CEffekseer() :
	m_eType(CMyEffekseer::TYPE_NONE),
	m_bDeath(false)
{

}

//===========================================================
// �f�X�g���N�^
//===========================================================
CEffekseer::~CEffekseer()
{

}

//===========================================================
// ����������
//===========================================================
void CEffekseer::Init(Effekseer::Vector3D pos, Effekseer::Vector3D rot, Effekseer::Vector3D scale, bool bLoop)
{
	m_bLoop = bLoop;

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CEffekseer>::Create();
		if (m_pList == nullptr) { assert(false); }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);
}

//===========================================================
// �I������
//===========================================================
void CEffekseer::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// ���g�̏I��
	delete this;
}

//==========================================
//  ���S�ݒ�
//==========================================
void CEffekseer::SetDeath()
{
	// �Đ��̒�~
	CManager::GetInstance()->GetEffect()->GetEfkManager()->StopEffect(m_efkHandle);

	// �t���O�̐ݒ�
	m_bDeath = true;
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CEffekseer>* CEffekseer::GetList(void)
{
	return m_pList;
}
