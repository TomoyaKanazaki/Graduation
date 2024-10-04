//=======================================
//
//���C�g[light.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================
#include "light.h"
#include "renderer.h"
#include "manager.h"

//=======================================
//�R���X�g���N�^
//=======================================
CLight::CLight()
{

}

//=======================================
//�f�X�g���N�^
//=======================================
CLight::~CLight()
{

}

//=======================================
//���C�g�̏���������
//=======================================
HRESULT CLight::Init(void)
{
	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXVECTOR3 vecDir[3];	//�ݒ�p�����x�N�g��

	for (int nCntLight = 0; nCntLight < 3; nCntLight++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&m_alight[nCntLight], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		m_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U����ݒ�
		m_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_alight[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);
		m_alight[2].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

		//���C�g�̕�����ݒ�
		vecDir[0] = D3DXVECTOR3(0.22f, -0.97f, 0.54f);
		vecDir[1] = D3DXVECTOR3(0.38f, 0.78f, -0.54f);
		vecDir[2] = D3DXVECTOR3(-0.9f, -0.21f, -0.44f);

		//�x�N�g���𐳋K������
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);

		m_alight[nCntLight].Direction = vecDir[nCntLight];

		//���C�g��ݒ�
		pDevice->SetLight(nCntLight, &m_alight[nCntLight]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}

	//������Ԃ�
	return S_OK;
}

//=======================================
//���C�g�̏I������
//=======================================
void CLight::Uninit(void)
{

}

//=======================================
//���C�g�̍X�V����
//=======================================
void CLight::Update(void)
{

}