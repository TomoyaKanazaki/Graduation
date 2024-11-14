//============================================
//
//	�K�w�\���Ń��[�V�������s�����f���̃T���v�� [SampleLevelModel.cpp]
//	Author:sakamoto kai
//
//============================================
#include "SampleLevelModel.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "character.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "object3D.h"
#include "debugproc.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 40.0f, 20.0f);		//���̓����蔻��
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CSampleLvModel::CSampleLvModel(int nPriority) :CObject(nPriority)
{
	SetWidth(SAMPLE_SIZE.x);
	SetHeight(SAMPLE_SIZE.y);
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_bJump = false;
	m_nActionCount = 0;
	m_Action = ACTION_WAIT;
	m_AtkAction = ACTION_WAIT;
	m_nAttackHit = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_AtkPos = INITVECTOR3;
	m_CollisionRot = 0.0f;

	m_pCharacter = nullptr;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CSampleLvModel::~CSampleLvModel()
{

}

//====================================================================
//��������
//====================================================================
CSampleLvModel* CSampleLvModel::Create()
{
	CSampleLvModel* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//�v���C���[�̐���
		pPlayer = new CSampleLvModel();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pPlayer->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pPlayer;
}

//====================================================================
//����������
//====================================================================
HRESULT CSampleLvModel::Init(void)
{
	//��ސݒ�
	SetType(CObject::TYPE_SAMPLE);

	// �L�����N�^�[�̐���
	if (m_pCharacter == nullptr)
	{
		m_pCharacter->Create("data\\TXT\\motion_player.txt");
	}

	CMotion* pMotion = m_pCharacter->GetMotion();

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:

		if (pMotion != nullptr)
		{
			// ���[�V�����ݒ�
			pMotion->Set(ACTION_TITLE, 0);
		}

		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		break;

	case CScene::MODE_RESULT:
		break;
	}

	return S_OK;
}

//====================================================================
//�������ێ�����I�u�W�F�N�g�̐���
//====================================================================
void CSampleLvModel::MyObjCreate(void)
{

}

//====================================================================
//�I������
//====================================================================
void CSampleLvModel::Uninit(void)
{
	// �L�����N�^�[�̏I������
	if (m_pCharacter != nullptr)
	{
		// �L�����N�^�[�̔j��
		m_pCharacter->Uninit();
		delete m_pCharacter;
		m_pCharacter = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CSampleLvModel::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//�^�C�g���ł̍X�V����
//====================================================================
void CSampleLvModel::TitleUpdate(void)
{
	// �L�����N�^�[�̍X�V
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Update();
	}
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CSampleLvModel::GameUpdate(void)
{
	//D3DXVECTOR3 ScreenCenter = D3DXVECTOR3(640.0f,360.0f,0.0f);
	//D3DXVECTOR3 ScreenPos[4] = { INITVECTOR3 };
	//D3DXVECTOR3 pos[4] = { INITVECTOR3 };

	//pos[0] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y, GetPos().z);
	//pos[1] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z);
	//pos[2] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y, GetPos().z);
	//pos[3] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z);

	////�r���[�|�[�g�̐ݒ�
	//D3DVIEWPORT9 viewport;
	//viewport.X = 0;
	//viewport.Y = 0;
	//viewport.Width = SCREEN_WIDTH;
	//viewport.Height = SCREEN_HEIGHT;
	//viewport.MaxZ = 0.0f;
	//viewport.MinZ = 0.0f;

	////�}�g���b�N�X�̎擾
	//D3DXMATRIX ViewMatrix = CManager::GetInstance()->GetCamera()->GetViewMatrix();
	//D3DXMATRIX ProjectionMatrix = CManager::GetInstance()->GetCamera()->GetProjectionMatrix();

	////D3DXVec3Project(&screenPosition, &pos, &viewport, &projectionMatrix, &viewMatrix, D3DXMatrixIdentity(&mtx));

	////3D�̍��W���X�N���[�����W�ɕϊ�
	//for (int nCnt = 0; nCnt < 4; nCnt++)
	//{
	//	D3DXVec3Project(&ScreenPos[nCnt], &pos[nCnt], &viewport, &ProjectionMatrix, &ViewMatrix, D3DXMatrixIdentity(&m_mtxWorld));

	//	C2DEffect* pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(50.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//	pEffect->SetPos(ScreenPos[nCnt]);
	//}

	////�p�x�ύX
	//CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();

	//if (pKey->GetPress(DIK_RIGHT) == true)
	//{
	//	m_CollisionRot += 0.01f;
	//}

	//if (pKey->GetPress(DIK_LEFT) == true)
	//{
	//	m_CollisionRot -= 0.01f;
	//}

	////�f�o�b�O�p�̃G�t�F�N�g����
	//for (int nCnt = 0; nCnt < 10; nCnt++)
	//{
	//	C2DEffect* pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(30.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	pEffect->SetPos(D3DXVECTOR3(
	//		ScreenCenter.x + sinf(m_CollisionRot) * 50.0f * nCnt,
	//		ScreenCenter.y + cosf(m_CollisionRot) * 50.0f * nCnt,
	//		0.0f));

	//	pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(30.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	pEffect->SetPos(D3DXVECTOR3(
	//		ScreenCenter.x + sinf(m_CollisionRot) * -50.0f * nCnt,
	//		ScreenCenter.y + cosf(m_CollisionRot) * -50.0f * nCnt,
	//		0.0f));
	//}

	////�v���C���[�ƓG�̈ʒu�֌W�������O��
	//int Left = 0;
	//int	Right = 0;
	//for (int nCnt = 0; nCnt < 4; nCnt++)
	//{
	//	D3DXVECTOR3 vecToPos;
	//	D3DXVECTOR3 vecLine;
	//	float A;

	//	vecToPos.x = (ScreenPos[nCnt].x) - (ScreenCenter.x);
	//	vecToPos.y = (ScreenPos[nCnt].y) - (ScreenCenter.y);

	//	vecLine.x = sinf(m_CollisionRot);
	//	vecLine.y = cosf(m_CollisionRot);
	//	vecLine.z = 0.0f;

	//	A = (vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y);

	//	if (A < 0)
	//	{
	//		Right++;
	//	}
	//	else
	//	{
	//		Left++;
	//	}
	//}

	//DebugProc::Print(DebugProc::POINT_LEFT, "�� : �E �� %d : %d\n", Left, Right);



	//���[�V�����̊Ǘ�
	ActionState();

	//��Ԃ̊Ǘ�
	StateManager();

	// �L�����N�^�[�̍X�V
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Update();
	}
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CSampleLvModel::ActionState(void)
{
	// �L�����N�^�[�̐���
	if (m_pCharacter == nullptr)
	{
		return;
	}

	CMotion* pMotion = m_pCharacter->GetMotion();

	if (pMotion == nullptr)
	{
		return;
	}

	//�W�����v���[�V����
	if (m_bJump == true)
	{
		if (m_Action != ACTION_JAMP)
		{
			m_Action = ACTION_JAMP;
			pMotion->Set(ACTION_JAMP, 5);
		}
	}
	//�ړ����[�V����
	else if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
	{
		if (m_Action != ACTION_MOVE)
		{
			m_Action = ACTION_MOVE;
			pMotion->Set(ACTION_MOVE, 5);
		}
	}
	//�j���[�g�������[�V����
	else
	{
		if (m_Action != ACTION_WAIT)
		{
			m_Action = ACTION_WAIT;
			pMotion->Set(ACTION_WAIT, 5);
		}
	}
}

//====================================================================
//��ԊǗ�
//====================================================================
void CSampleLvModel::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;

	case STATE_DEATH:
		break;

	case STATE_WAIT:
		break;

	case STATE_DAMAGE:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CSampleLvModel::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �L�����N�^�[�̕`��
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Draw();
	}
}