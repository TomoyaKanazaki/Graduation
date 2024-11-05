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
#include "devil.h"
#include "bowabowa.h"
#include "DevilHole.h"

#include "renderer.h"
#include "SampleObj2D.h"
#include "SampleObj3D.h"
#include "SampleObjBillboard.h"
#include "SampleObjX.h"
#include "SampleLevelModel.h"
#include "light.h"

#include "slowManager.h"
#include "cross.h"
#include "scrollarrow.h"
#include "enemyMedaman.h"
#include "enemyBonbon.h"
#include "enemyYoungDevil.h"
#include "item_manager.h"
#include "bible.h"
#include "MapSystem.h"
#include "RailBlock.h"
#include "Rail.h"

#include "bowabowa.h"

namespace
{
	const int SAMPLE_NAMESPACE = 0;

	const int BOTTOM_FIELD_VTX_WIDTH = 32;		// �����̉���
	const int BOTTOM_FIELD_VTX_HEIGHT = 32;		// �����̏c��
	const char* BOTTOM_FIELD_TEX = "data\\TEXTURE\\Field\\Tile000.png";		// �����̃e�N�X�`��
	const D3DXVECTOR3 BOTTOM_FIELD_POS = D3DXVECTOR3(0.0f, -500.0f, 0.0f);	// �����̈ʒu
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
CObjmeshField* CGame::m_pMapField = nullptr;
CCubeBlock* CGame::m_pCubeBlock = nullptr;
CPlayer* CGame::m_pPlayer = nullptr;
CDevil* CGame::m_pDevil = nullptr;
CBoss* CGame::m_pBoss = nullptr;
bool CGame::m_bGameEnd = false;
bool CGame::m_bGameClear = false;
bool CGame::m_bEvent = false;
bool CGame::m_bEventEnd = false;
bool CGame::m_Wireframe = false;
bool CGame::m_Slow = false;
bool CGame::m_bDevilHoleFinish = false;
int CGame::m_nTutorialWave = 0;
int CGame::m_nEventCount = 0;
int CGame::m_nEventWave = 0;
int CGame::m_nEventNumber = 0;
int CGame::m_nNumBowabowa = 0;
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
	m_bDevilHoleFinish = false;
	m_EventHeight = 0.0f;
	m_BGColorA = 1.0f;
	m_nEventCount = 0;
	m_fEvectFinish = 0.0f;
	m_fEventAngle = 0.0f;
	m_nTutorialWave = 0;
	m_nEventNumber = 0;
	m_nNumBowabowa = 0;
	CManager::GetInstance()->GetCamera()->SetBib(false);
	CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);
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
	////BGM�̍Đ�
	//CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_TUTORIAL);

	//�N���A�t���O�̃f�t�H���g���I���ɂ��Ă���
	m_bGameClear = true;

	//�^�C���̋N��
	CGame::GetTime()->SetStopTime(false);

	// �^�C���̐���
	m_pTime = CTime::Create();
	m_pTime->SetStartTime(timeGetTime());
	m_pTime->SetTime(0);

	//�X�e�[�W�w�i�⏰�̐���
	m_pMeshDomeUp = CObjmeshDome::Create();
	m_pMeshDomeUp->SetTexture("data\\TEXTURE\\rain_clown.jpg");

	m_pMapField = CObjmeshField::Create(16, 12);
	m_pMapField->SetPos(INITVECTOR3);

	// �X�����u�i�����ڂ����̉��j
	CObjectX* pObjX = CObjectX::Create("data\\MODEL\\mawasiguruma.x");
	pObjX->SetPos(D3DXVECTOR3(900.0f, 0.0f, -600.0f));

	CObjectX* pObjX2 = CObjectX::Create("data\\MODEL\\mawasiguruma.x");
	pObjX2->SetPos(D3DXVECTOR3(900.0f, 0.0f, 600.0f));
	pObjX2->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	CObjectX* pObjX3 = CObjectX::Create("data\\MODEL\\mawasiguruma.x");
	pObjX3->SetPos(D3DXVECTOR3(-900.0f, 0.0f, -600.0f));

	CObjectX* pObjX4 = CObjectX::Create("data\\MODEL\\mawasiguruma.x");
	pObjX4->SetPos(D3DXVECTOR3(-900.0f, 0.0f, 600.0f));
	pObjX4->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	// �����̐���
	CObjmeshField* pBottonField = CObjmeshField::Create(BOTTOM_FIELD_VTX_WIDTH, BOTTOM_FIELD_VTX_HEIGHT);
	pBottonField->SetTexture(BOTTOM_FIELD_TEX);
	pBottonField->SetPos(BOTTOM_FIELD_POS);
	m_bGameEnd = false;

	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetPos(D3DXVECTOR3(0.0f, 0.0f, 100.0f));

	m_pDevil = CDevil::Create();
	m_pDevil->SetPos(D3DXVECTOR3(0.0f, 100.0f, 500.0f));

	m_pScore = CScore::Create();
	m_pScore->SetScore(CManager::GetInstance()->GetEndScore());

	int nData[5] = {0,0,0,0,0};
	CMapSystem::GetInstance()->SetGritBool(8, 6, true);
	CRailBlock* pBlock = CRailBlock::Create(8, 6, false, 5, &nData[0]);
	pBlock->SetPos(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	pBlock->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));

	//CMapSystem::GetInstance()->SetGritBool(8, 7, true);
	//pBlock = CRailBlock::Create(8, 7);
	//pBlock->SetPos(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	//pBlock->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));

	//CRail* pRail = CRail::Create("data\\MODEL\\TestRail.x");
	//pRail->SetPos(D3DXVECTOR3(100.0f, 0.0f, 0.0f));

	CBowabowa* pBowabowa = nullptr;
	CCross* pCross = nullptr;

	//�X�e�[�W�̓ǂݍ���
	switch (CManager::GetInstance()->GetStage())
	{
	case 0:
		LoadStageBlock("data\\TXT\\STAGE\\Block00.txt");


		// �{���{���̐���
		pBowabowa = CBowabowa::Create("data\\MODEL\\Testbowabowa.x");
		pBowabowa->SetWightNumber(9);
		pBowabowa->SetHeightNumber(5);
		pBowabowa = nullptr;
		delete pBowabowa;

		//�\���˂̐���
		pCross = CCross::Create("data\\MODEL\\zyuzika.x");
		pCross->SetWightNumber(2);
		pCross->SetHeightNumber(4);
		pCross = nullptr;
		delete pCross;
		break;

	case 1:
		LoadStageBlock("data\\TXT\\STAGE\\Block01.txt");

		CDevilHole* pDevilHole = CDevilHole::Create("data\\MODEL\\DevilHole.x");
		// ��������
		CBible* pBible = nullptr;
		pBible = CBible::Create("data\\MODEL\\02_item\\holybible.x");
		pBible->SetPosType(CBible::POS_TYPE_LEFTUP);
		pBible->SetMapScroll(false);

		pBible = CBible::Create("data\\MODEL\\02_item\\holybible.x");
		pBible->SetPosType(CBible::POS_TYPE_RIGHTUP);
		pBible->SetMapScroll(false);

		pBible = CBible::Create("data\\MODEL\\02_item\\holybible.x");
		pBible->SetPosType(CBible::POS_TYPE_LEFTDOWN);
		pBible->SetMapScroll(false);

		pBible = CBible::Create("data\\MODEL\\02_item\\holybible.x");
		pBible->SetPosType(CBible::POS_TYPE_RIGHTDOWN);
		pBible->SetMapScroll(false);

		pBible = nullptr;
		delete pBible;

		break;
	}
	CScrollArrow* pScrollAllow = nullptr;
	pScrollAllow = CScrollArrow::Create();
	pScrollAllow->SetPos((D3DXVECTOR3(80.0f, 120.0f, 0.0f)));

	pScrollAllow = CScrollArrow::Create();
	pScrollAllow->SetPos((D3DXVECTOR3(1200.0f, 120.0f, 0.0f)));

	CEnemyMedaman* pMedaman = CEnemyMedaman::Create("data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt");
	pMedaman->SetPos(D3DXVECTOR3(-300.0f, 0.0f, 0.0f));

	CEnemyBonbon* pBonbon = CEnemyBonbon::Create("data\\TXT\\MOTION\\01_enemy\\motion_bonbon.txt");
	pBonbon->SetPos(D3DXVECTOR3(-300.0f, 0.0f, 0.0f));

	CEnemyYoungDevil* pYoungDevil = CEnemyYoungDevil::Create("data\\TXT\\MOTION\\01_enemy\\motion_smalldevil.txt");
	pYoungDevil->SetPos(D3DXVECTOR3(-300.0f, 0.0f, 0.0f));

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

	if (CManager::GetInstance()->GetFade()->GetFade() == CFade::FADE_NONE)
	{
		//�X�e�[�W�N���A�����̐ݒ�
		switch (CManager::GetInstance()->GetStage())
		{
		case 0:
			// �{���{���̃��X�g�\����������Δ�����
			if (CBowabowa::GetList() == nullptr)
			{
				m_bGameEnd = true;
			}
			break;

		case 1:
			if (m_bDevilHoleFinish == true)
			{
				m_bGameEnd = true;
			}
			break;
		}

		//�X�e�[�W�N���A���̏���
		if (m_bGameEnd == true)
		{
			if (m_bGameClear == true)
			{
				StageClear(CManager::GetInstance()->GetStage());
			}
			else
			{
				CFade::SetFade(CScene::MODE_RESULT);
				m_pTime->SetStopTime(true);
				CManager::GetInstance()->SetEndScore(m_pScore->GetScore());
			}
		}
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CGame::Draw(void)
{

}

//====================================================================
//�X�e�[�W�N���A����
//====================================================================
void CGame::StageClear(int Stage)
{
	if (Stage == 1)
	{
		CManager::GetInstance()->SetStage(0);

		CFade::SetFade(CScene::MODE_RESULT);
		m_pTime->SetStopTime(true);
		CManager::GetInstance()->SetEndScore(m_pScore->GetScore());
	}
	else
	{
		CManager::GetInstance()->SetStage(Stage + 1);

		CFade::SetFade(CScene::MODE_GAME);
		m_pTime->SetStopTime(true);
		CManager::GetInstance()->SetEndScore(m_pScore->GetScore());
	}
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
// �u���b�N�̓ǂݍ��ݔz�u
//====================================================================
void CGame::LoadStageBlock(const char* pFilename)
{
	//�t�@�C�����J��
	FILE* pFile = fopen(pFilename, "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		char Getoff[32] = {};
		char boolLife[32] = {};
		char aString[128] = {};			//�S�~��
		char aStartMessage[32] = {};	//�X�^�[�g���b�Z�[�W
		char aSetMessage[32] = {};		//�Z�b�g���b�Z�[�W
		char aEndMessage[32] = {};		//�I�����b�Z�[�W

		fscanf(pFile, "%s", &aStartMessage[0]);
		if (strcmp(&aStartMessage[0], "STARTSETSTAGE") == 0)
		{
			CMapSystem *pMapSystem = CMapSystem::GetInstance();
			D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();
			float MapSystemGritSize = pMapSystem->GetGritSize() * 0.5f;

			while (1)
			{
				fscanf(pFile, "%s", &aSetMessage[0]);
				if (strcmp(&aSetMessage[0], "STARTSETBLOCK") == 0)
				{
					int WightNumber, HeightNumber;

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &WightNumber);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &HeightNumber);

					pMapSystem->SetGritBool(WightNumber, HeightNumber, true);
					CCubeBlock* pBlock = CCubeBlock::Create();
					pBlock->SetWightNumber(WightNumber);
					pBlock->SetHeightNumber(HeightNumber);
					pBlock->SetPos(D3DXVECTOR3(MapSystemPos.x + WightNumber * 100.0f, 50.0f, MapSystemPos.z - HeightNumber * 100.0f));
					pBlock->SetSize(D3DXVECTOR3(MapSystemGritSize, MapSystemGritSize, MapSystemGritSize));

					fscanf(pFile, "%s", &aEndMessage[0]);
					if (strcmp(&aEndMessage[0], "ENDSETBLOCK") == 0)
					{
						break;
					}
				}
				else if (strcmp(&aSetMessage[0], "ENDSETSTAGE") == 0)
				{
					break;
				}
			}
		}
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***\n");
	}
}

//====================================================================
// ���f���̓ǂݍ��ݔz�u
//====================================================================
void CGame::LoadStageMapModel(const char* pFilename)
{
	////�t�@�C�����J��
	//FILE* pFile = fopen(pFilename, "r");

	//if (pFile != nullptr)
	//{//�t�@�C�����J�����ꍇ

	//	char Getoff[32] = {};
	//	char boolLife[32] = {};
	//	char aString[128] = {};			//�S�~��
	//	char aStartMessage[32] = {};	//�X�^�[�g���b�Z�[�W
	//	char aSetMessage[32] = {};		//�Z�b�g���b�Z�[�W
	//	char aEndMessage[32] = {};		//�I�����b�Z�[�W
	//	char aBool[8] = {};				//bool�^���b�Z�[�W
	//	bool Loop = false;

	//	fscanf(pFile, "%s", &aStartMessage[0]);
	//	if (strcmp(&aStartMessage[0], "STARTSETSTAGE") == 0)
	//	{
	//		while (1)
	//		{
	//			fscanf(pFile, "%s", &aSetMessage[0]);
	//			if (strcmp(&aSetMessage[0], "STARTSETXMODEL") == 0)
	//			{
	//				char aModelName[64] = {};		//���f���̃p�X��
	//				D3DXVECTOR3 pos;
	//				D3DXVECTOR3 rot;
	//				int nEditIndex = 0;

	//				fscanf(pFile, "%s", &aString[0]);
	//				fscanf(pFile, "%s", &aModelName[0]);

	//				fscanf(pFile, "%s", &aString[0]);
	//				fscanf(pFile, "%d", &nEditIndex);

	//				fscanf(pFile, "%s", &aString[0]);
	//				fscanf(pFile, "%f", &pos.x);
	//				fscanf(pFile, "%f", &pos.y);
	//				fscanf(pFile, "%f", &pos.z);

	//				fscanf(pFile, "%s", &aString[0]);
	//				fscanf(pFile, "%f", &rot.x);
	//				fscanf(pFile, "%f", &rot.y);
	//				fscanf(pFile, "%f", &rot.z);

	//				fscanf(pFile, "%s", &aString[0]);
	//				fscanf(pFile, "%s", &aBool[0]);	//���[�v���邩�ǂ�����ݒ�
	//				Loop = (strcmp(&aBool[0], "1") == 0 ? true : false);			//bool�^�̏�����

	//				CMapModel* pModel = CMapModel::Create(&aModelName[0]);
	//				pModel->SetPos(pos);
	//				pModel->SetRot(rot);
	//				pModel->SetEditIdx(nEditIndex);

	//				if (rot.y < 1.57f)
	//				{

	//				}
	//				else if (rot.y < 3.14f)
	//				{
	//					pModel->SwapSize();
	//				}
	//				else if (rot.y < 4.71f)
	//				{

	//				}
	//				else
	//				{
	//					pModel->SwapSize();
	//				}

	//				pModel->SetCollision(Loop);

	//				fscanf(pFile, "%s", &aEndMessage[0]);
	//				if (strcmp(&aEndMessage[0], "ENDSETXMODEL") != 0)
	//				{
	//					break;
	//				}
	//			}
	//			else if (strcmp(&aSetMessage[0], "ENDSETSTAGE") == 0)
	//			{
	//				break;
	//			}
	//		}
	//	}
	//	fclose(pFile);
	//}
	//else
	//{//�t�@�C�����J���Ȃ������ꍇ
	//	printf("***�t�@�C�����J���܂���ł���***\n");
	//}
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