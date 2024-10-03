//==============================================================
//
//���C�g����[light.h]
//Author:����������
//
//==============================================================
#include "light.h"
#include "manager.h"
#include "renderer.h"

//==============================================================
//�R���X�g���N�^
//==============================================================
CLight::CLight()
{

}

//==============================================================
//�f�X�g���N�^
//==============================================================
CLight::~CLight()
{

}

//==============================================================
//���C�g�̏���������
//==============================================================
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾
	D3DXVECTOR3 vecDir[MAX_LIGHT];				//�ݒ�p�����x�N�g��

	//���C�g�̏����N���A����
	ZeroMemory(&m_alight, sizeof(D3DLIGHT9));

	//���C�g�̎�ނ�ݒ�
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		m_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		if (nCntLight == 0)
		{
			//���C�g�̊g�U����ݒ�
			m_alight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//���C�g�̕�����ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(0.2f, -0.8f, 0.8f);
		}
		else if (nCntLight == 1)
		{
			//���C�g�̊g�U����ݒ�
			m_alight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//���C�g�̕�����ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
		}
		else if (nCntLight == 2)
		{
			//���C�g�̊g�U����ݒ�
			m_alight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//���C�g�̕�����ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
		}
		
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		//�x�N�g���𐳋K������
		m_alight[nCntLight].Direction = vecDir[nCntLight];

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_alight[nCntLight]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//==============================================================
//���C�g�̏I������
//==============================================================
void CLight::Uninit(void)
{

}

//==============================================================
//���C�g�̍X�V����
//==============================================================
void CLight::Update(void)
{
	
}