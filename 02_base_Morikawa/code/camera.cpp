//=======================================
//
//�J����[camera.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================
#include "camera.h"   
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "useful.h"

//=======================================
// �萔��`
//=======================================
namespace
{
    const float CAMERA_POS_Y = 375.0f;		// �J������Y���W�̏����ʒu
    const float CAMERA_POS_Z = -500.0f;	// �J������Z���W�̏����ʒu
    const float CAMERA_DISTANCE = 400.0f;	// �J�����Ƃ̋���
    const float CAMERA_SPEED = 1.5f;		// �J�����̈ړ����x
    const float CMAERA_INERTIA = 0.2f;		// �J�����̊���
    const float CAMERA_R_DISTANCE = 25.0f;	// �����_�̋���
    const float CAMERA_V_MOVE = 0.03f;		// ���_�̈ړ����x
    const float CAMERA_R_INERTIA = 0.2f;	// �����_�̊���
    const float CAMERA_V_INERTIA = 0.2f;	// ���_�̊���
    const float CAMERA_DISTANCE_TITLE = 300.0f;		// �J����
	const D3DXVECTOR3 DISTANCE_V_R = D3DXVECTOR3(0.0f, 500.0f, -15000.0f);
	const D3DXVECTOR3 DISTANCE_R = D3DXVECTOR3(500.0f, 250.0f, 0.0f);
    const D3DXVECTOR3 DISTANCE_V = D3DXVECTOR3(0.0f, 250.0f, -2000.0f);
}

//=======================================
//�R���X�g���N�^
//=======================================
CCamera::CCamera()
{
    m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���_
    m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����_
    m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//������x�N�g��
    m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
    m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
    m_VDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_RDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_fDistance = 0.0f;							//����
}

//=======================================
//�f�X�g���N�^
//=======================================
CCamera::~CCamera()
{

}

//=======================================
//�J�����̏���������
//=======================================
HRESULT CCamera::Init(void)
{
    //�����_
    m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    //���_
    m_posV = m_posR + DISTANCE_V;

    //������x�N�g��
    m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    //����
    m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    //������Ԃ�
    return S_OK;
}

//=======================================
//�J�����̏I������
//=======================================
void CCamera::Uninit(void)
{

}

//=======================================
//�J�����̍X�V����
//=======================================
void CCamera::Update(void)
{
#ifdef _DEBUG
    //�f�o�b�O�\��
    DebugProc::Print(DebugProc::POINT_LEFT, "���_�̈ʒu�F%f�A%f�A%f\n", m_posV.x, m_posV.y, m_posV.z);
    DebugProc::Print(DebugProc::POINT_LEFT, "�����_�̈ʒu�F%f�A%f�A%f\n", m_posR.x, m_posR.y, m_posR.z);
    DebugProc::Print(DebugProc::POINT_LEFT, "�J�����̌����F%f�A%f�A%f\n", m_rot.x, m_rot.y, m_rot.z);
#endif
}

//=======================================
//�J�����̐ݒ菈��
//=======================================
void CCamera::SetCamera(void)
{
    //CRenderer�^�̃|�C���^
    CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

    //�f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

    //�v���W�F�N�V�����}�g���b�N�X�̏�����
    D3DXMatrixIdentity(&m_mtxProjection);

    //�v���W�F�N�V�����}�g���b�N�X��ݒ�[���s���e]
    D3DXMatrixOrthoLH(&m_mtxProjection,		//�v���W�F�N�V�����}�g���b�N�X
                        SCREEN_WIDTH,	//��ʂ̕�
                        SCREEN_HEIGHT,	//��ʂ̍���
                        10.0f,		//Z�l�̍ŏ��l
						400000.0f);	//Z�l�̍ő�l

    //�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
    pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);



    // �v���W�F�N�V�����}�g���b�N�X�̏�����
    D3DXMatrixIdentity(&m_mtxProjection);

    // �v���W�F�N�V�����}�g���b�N�X�̍쐬
    D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
        D3DXToRadian(45.0f),	// ����p
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// �A�X�y�N�g��
        10.0f,		// ��O�̐���
        150000.0f);	// ���s���̐���

// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
    pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);


    //�r���[�}�g���b�N�X�̏�����
    D3DXMatrixIdentity(&m_mtxView);

    //�r���[�}�g���b�N�X�̍쐬
    D3DXMatrixLookAtLH(&m_mtxView,
                        &m_posV,	//���_
                        &m_posR,	//�����_
                        &m_vecU);	//������x�N�g��

    //�r���[�}�g���b�N�X�̐ݒ�
    pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=======================================
//�����_�̈ړ�
//=======================================
void CCamera::CameraMoveV(void)
{
    // �L�[�{�[�h�̏��擾
    CInputKeyboard *pInputKeyboard = nullptr;
    pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

    // �R���g���[���[�̏��擾
    CInputPad *pInputPad = nullptr; 
    pInputPad = CManager::GetInstance()->GetInputPad();

    //C�L�[�������ꂽ
    if (pInputKeyboard->GetPress(DIK_C) == true
        || pInputPad->GetRStickXPress(CInputPad::BUTTON_R_STICK, 0) > 0)
    {
        m_rot.y += CAMERA_V_MOVE;

        //�p�x�̐��K��
        m_rot.y = RotNor(m_rot.y);
    }
    //Z�L�[�������ꂽ
    else if (pInputKeyboard->GetPress(DIK_Z) == true
             || pInputPad->GetRStickXPress(CInputPad::BUTTON_R_STICK, 0) < 0)
    {
        m_rot.y -= CAMERA_V_MOVE;

        //�p�x�̐��K��
        m_rot.y = RotNor(m_rot.y);
    }

    //�ʒu���X�V
    m_posV += m_move;

    m_posR += m_move;

    //�ړ��ʂ��X�V(����������)
    m_move.x += (0.0f - m_move.x) * CMAERA_INERTIA;
    m_move.y += (0.0f - m_move.y) * CMAERA_INERTIA;
    m_move.z += (0.0f - m_move.z) * CMAERA_INERTIA;
}

//=======================================
//rot�̐��K��
//=======================================
float CCamera::RotNor(float RotN)
{
    //3.14�𒴂����Ƃ��ɔ��΂ɂ���
    if (RotN > D3DX_PI)
    {
        RotN -= D3DX_PI * 2;
    }

    //-3.14�𒴂����Ƃ��ɔ��΂ɂ���
    if (RotN < -D3DX_PI)
    {
        RotN += D3DX_PI * 2;
    }
    return RotN;
}

//=======================================
//�J�����Ǐ]
//=======================================
void CCamera::Following(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    m_posR = pos;
    m_posR.y = DISTANCE_R.y;
    m_posR.x += DISTANCE_R.x;
    m_posV = m_posR + DISTANCE_V;
}

//=======================================
// �^�C�g���p�J����
//=======================================
void CCamera::TitleCamera()
{
    m_rot.y += 0.005f;

    //3.14�𒴂����Ƃ��ɔ��΂ɂ���
    if (m_rot.y > D3DX_PI)
    {
        m_rot.y -= D3DX_PI * 2;
    }

    //-3.14�𒴂����Ƃ��ɔ��΂ɂ���
    if (m_rot.y < -D3DX_PI)
    {
        m_rot.y += D3DX_PI * 2;
    }

    m_fDistance = 500.0f;

    // ���_
    m_posV.x = m_posR.x + sinf(m_rot.y) * m_fDistance;
    m_posV.z = m_posR.z + cosf(m_rot.y) * m_fDistance;
}

//=======================================
// ���Z��Ƃ̔���
//=======================================
void CCamera::CollisionArena()
{
    D3DXVECTOR3 vec;
    D3DXVec3Normalize(&vec, &m_posV);

    if (USEFUL::CollisionCircle(m_posV, Constance::ARENA_SIZE))
    {// ���Z��ɓ���������
        m_posV.x = vec.x * Constance::ARENA_SIZE;
        m_posV.z = vec.z * Constance::ARENA_SIZE;
    }
}

//=======================================
//�����̎擾
//=======================================
D3DXVECTOR3 CCamera::GetRot(void)
{
    return m_rot;
}

//=======================================
//���_�̎擾
//=======================================
D3DXVECTOR3 CCamera::GetPosV(void)
{
    return m_posV;
}

//=======================================
//�����_�̎擾
//=======================================
D3DXVECTOR3 CCamera::GetPosR(void)
{
    return m_posR;
}
