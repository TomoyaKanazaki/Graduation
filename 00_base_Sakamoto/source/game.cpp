//============================================
//
//	�Q�[����� [game.cpp]
//	Author:sakamoto kai
//
//============================================
#include "game.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "objmeshField.h"
#include "objmeshWall.h"
#include "objmeshCylinder.h"
#include "objmeshDome.h"
#include "CubeBlock.h"
#include "player.h"
#include "Pause.h"
#include "camera.h"
#include "sound.h"
#include "time.h"
#include "Score.h"
#include "debugproc.h"
#include "Xmodel.h"
#include "texture.h"
#include "effect.h"
#include "Edit.h"

#include "renderer.h"
#include "SampleObj2D.h"
#include "SampleObj3D.h"
#include "SampleObjBillboard.h"
#include "SampleObjX.h"
#include "SampleLevelModel.h"
#include "light.h"

#include "slowManager.h"

namespace
{
	const int SAMPLE_NAMESPACE = 0;
}

//�ÓI�����o�ϐ��錾
CEdit* CGame::m_pEdit = nullptr;
CPause* CGame::m_pPause = nullptr;
CScore* CGame::m_pScore = nullptr;
CTime* CGame::m_pTime = nullptr;
CObject2D* CGame::m_p2DSample = nullptr;
CObject3D* CGame::m_p3DSample = nullptr;
CObjectBillboard* CGame::m_pBillboardSample = nullptr;
CObjectX* CGame::m_pXModelSample = nullptr;
CObjmeshField* CGame::m_pMeshFieldSample = nullptr;
CObjmeshWall* CGame::m_pMeshWallSample = nullptr;
CObjmeshCylinder* CGame::m_pMeshCylinderSample = nullptr;
CObjmeshDome* CGame::m_pMeshDomeUp = nullptr;
CObjmeshField* CGame::m_pMeshField = nullptr;
CCubeBlock* CGame::m_pCubeBlock = nullptr;
CPlayer* CGame::m_pPlayer = nullptr;
CBoss* CGame::m_pBoss = nullptr;
bool CGame::m_bGameEnd = false;
bool CGame::m_bEvent = false;
bool CGame::m_bEventEnd = false;
bool CGame::m_Wireframe = false;
bool CGame::m_Slow = false;
int CGame::m_nTutorialWave = 0;
int CGame::m_nEventCount = 0;
int CGame::m_nEventWave = 0;
int CGame::m_nEventNumber = 0;
float CGame::m_fEvectFinish = 0.0f;
float CGame::m_fEventAngle = 0.0f;
float CGame::m_EventHeight = 0.0f;
float CGame::m_NameColorA = 0.0f;
float CGame::m_BGColorA = 1.0f;
D3DXVECTOR3 CGame::m_EventPos = D3DXVECTOR3(0.0f, 300.0f, 0.0f);
D3DXVECTOR3 CGame::m_BGRot = INITVECTOR3;

//====================================================================
//�R���X�g���N�^
//====================================================================
CGame::CGame()
{
	m_bGameEnd = false;
	m_bEvent = false;
	m_bEventEnd = false;
	m_EventHeight = 0.0f;
	m_BGColorA = 1.0f;
	m_nEventCount = 0;
	m_fEvectFinish = 0.0f;
	m_fEventAngle = 0.0f;
	m_nTutorialWave = 0;
	m_nEventNumber = 0;
	CManager::GetInstance()->GetCamera()->SetBib(false);
	CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_CONTROL);

}

//====================================================================
//�f�X�g���N�^
//====================================================================
CGame::~CGame()
{

}

//====================================================================
//����������
//====================================================================
HRESULT CGame::Init(void)
{
	//CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_TUTORIAL);
	CGame::GetTime()->SetStopTime(false);

	m_pMeshDomeUp = CObjmeshDome::Create();
	m_pMeshDomeUp->SetTexture("data\\TEXTURE\\rain_clown.jpg");

	m_pMeshField = CObjmeshField::Create(21, 21);
	m_pMeshField->SetPos(INITVECTOR3);

	m_bGameEnd = false;

	// �^�C���̐���
	m_pTime = CTime::Create();
	m_pTime->SetStartTime(timeGetTime());
	m_pTime->SetTime(0);

	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

#if _DEBUG
	if (m_pEdit == nullptr)
	{
		m_pEdit = CEdit::Create();
	}
#endif
	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CGame::Uninit(void)
{
	// �X���[���̑S�폜
	CSlowManager::ReleaseAll();

	//�S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

	m_pBoss = nullptr;

#if _DEBUG
	if (m_pEdit != nullptr)
	{
		//m_pEdit->Uninit();
		delete m_pEdit;
		m_pEdit = nullptr;
	}
#endif
}

//====================================================================
//�X�V����
//====================================================================
void CGame::Update(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	DebugProc::Print(DebugProc::POINT_LEFT, "�Q�[���X�s�[�h : %f\n", CManager::GetInstance()->GetGameSpeed());

#if _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_0) == true)
	{
		m_Wireframe = (m_Wireframe == true) ? false : true;

		if (m_Wireframe == true)
		{
			//�����_�[�X�e�[�g�̐ݒ�
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else
		{
			//�����_�[�X�e�[�g�̐ݒ�
			m_pDevice->SetRenderState(D3DRS_FILLMODE, 0);
		}
	}

	if (CManager::GetInstance()->GetEdit() == true)
	{
		CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_CONTROL);
		m_pEdit->Update();
	}

	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		float Speed = CManager::GetInstance()->GetGameSpeed();

		Speed -= 0.90f;

		CManager::GetInstance()->SetGameSpeed(Speed);
	}

	if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{
		float Speed = CManager::GetInstance()->GetGameSpeed();

		Speed += 0.90f;

		CManager::GetInstance()->SetGameSpeed(Speed);
	}

	if (pInputKeyboard->GetTrigger(DIK_3) == true)
	{
		m_pTime->SetStartTime(0);
		m_pTime->SetTime(0);
	}

	if (CManager::GetInstance()->GetGameSpeed() <= 1.0f)
	{
		m_Slow = true;
	}
	else
	{
		m_Slow = false;
	}

#endif

	if (m_bEvent == true)
	{
		//�C�x���g�̍X�V
		EventUpdate();
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		CFade::SetFade(CScene::MODE_RESULT);
		m_pTime->SetStopTime(true);
		CManager::GetInstance()->SetEndScore(m_pTime->GetTimeNumber());
	}

	if (m_bGameEnd == true)
	{
		CFade::SetFade(CScene::MODE_RESULT);
		m_pTime->SetStopTime(true);
		CManager::GetInstance()->SetEndScore(m_pTime->GetTimeNumber());
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CGame::Draw(void)
{

}

//====================================================================
//�C�x���g�̍X�V
//====================================================================
void CGame::EventUpdate(void)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();
	float EventFinish = 0.0f;

	if (m_nEventNumber == 0) //=====================================================================
	{
		switch (m_nEventWave)
		{
		case 0:		//�{�X��m�����[�V�����ɂ���
			m_nEventWave++;
			m_nEventCount = 150;
			break;

		default:
			m_bEvent = false;
			m_nEventWave = 0;
			m_nEventNumber++;
			break;
		}
	}

	//�C�x���g�J�E���g
	if (m_nEventCount > 0)
	{
		m_nEventCount--;
	}
}

//====================================================================
//�}�b�v�I�u�W�F�N�g�̍폜
//====================================================================
void CGame::DeleteMap(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == CObject::TYPE_CUBEBLOCK ||
				type == CObject::TYPE_CUBEDAMEGE ||
				type == CObject::TYPE_ENEMY3D ||
				type == CObject::TYPE_MAPMODEL ||
				type == CObject::TYPE_STAIR)
			{//��ނ��}�b�v�֘A�̎�
				pObj->Uninit();
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// �T���v���n�������ă��i�Q�[���ɂ͐�Ύg��Ȃ����j
//====================================================================
void CGame::Sample(void)
{
	////�e��I�u�W�F�N�g�̐���------------------------------------
	//CObject2D* pCbject2D = CObject2D::Create();
	//pCbject2D->SetPos(D3DXVECTOR3(640.0, 360.0f, 0.0f));

	//CObject3D *pCbject3D = CObject3D::Create();
	//pCbject3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//CObjectBillboard* Billboard = CObjectBillboard::Create();
	//Billboard->SetPos(D3DXVECTOR3(100.0f, 0.0f, 0.0f));

	//CObjectX* pObjectX = CObjectX::Create("data\\MODEL\\player00.x");
	//pObjectX->SetPos(D3DXVECTOR3(200.0f, 0.0f, 0.0f));

	//CObjmeshCube* pObjCube = CObjmeshCube::Create();
	//pObjCube->SetPos(D3DXVECTOR3(300.0f, 0.0f, 0.0f));

	//CObjmeshField *pObjField = CObjmeshField::Create();
	//pObjField->SetPos(D3DXVECTOR3(400.0f, 0.0f, 0.0f));

	//CObjmeshWall* pObjWall = CObjmeshWall::Create();
	//pObjWall->SetPos(D3DXVECTOR3(500.0f, 0.0f, 0.0f));

	//CObjmeshCylinder* pObjCylinder = CObjmeshCylinder::Create();
	//pObjCylinder->SetPos(D3DXVECTOR3(600.0f, 0.0f, 0.0f));

	////�e�I�u�W�F�N�g�̎q�N���X�̐���-----------------------------------------
	//m_p2DSample = CSampleObj2D::Create(7);
	//m_p2DSample->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
	//m_p2DSample->SetWidth(1280.0f);
	//m_p2DSample->SetHeight(720.0f);
	//m_p2DSample->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f));
	//m_p2DSample->SetMultiTarget(true);

	//CSampleObj3D* pSampleObj3D = CSampleObj3D::Create();
	//pSampleObj3D->SetPos(D3DXVECTOR3(-100.0f, 0.0f, 0.0f));

	//CSampleObjBillboard* pSampleObjBillboard = CSampleObjBillboard::Create();
	//pSampleObjBillboard->SetPos(D3DXVECTOR3(-200.0f, 0.0f, 0.0f));

	//CSampleObjectX* pSampleObjX = CSampleObjectX::Create("data\\MODEL\\enemy.x");
	//pSampleObjX->SetPos(D3DXVECTOR3(-300.0f, 0.0f, 0.0f));

	//CEnemy* pEnemy = CEnemy::Create();
	//pEnemy->SetPos(D3DXVECTOR3(-500.0f, 0.0f, 0.0f));

	//CSampleLvModel* pSampleLvModel = CSampleLvModel::Create();
	//pSampleLvModel->SetPos(D3DXVECTOR3(-400.0f, 0.0f, 0.0f));
}