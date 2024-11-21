//============================================
//
//	�Q�[����� [game.cpp]
//	Author:sakamoto kai
//
//============================================
#include "game.h"
#include "fade.h"
#include "objmeshField.h"
#include "player.h"
#include "camera.h"
#include "timer.h"
#include "Score.h"
#include "Edit.h"
#include "devil.h"
#include "DevilHole.h"
#include "renderer.h"
#include "slowManager.h"
#include "enemyYoungDevil.h"
#include "RailBlock.h"
#include "RollRock.h"
#include "bowabowa.h"
#include "SlopeDevice.h"
#include "mask.h"

namespace
{
	const int SAMPLE_NAMESPACE = 0;

	const int BOTTOM_FIELD_VTX_WIDTH = 64;		// �����̉���
	const int BOTTOM_FIELD_VTX_HEIGHT = 64;		// �����̏c��
	const char* BOTTOM_FIELD_TEX = "data\\TEXTURE\\Field\\outside.jpg";		// �����̃e�N�X�`��
	const D3DXVECTOR3 BOTTOM_FIELD_POS = D3DXVECTOR3(0.0f, -1500.0f, 0.0f);	// �����̈ʒu
	const int BIBLE_OUTGRIT = 3;	// �������}�b�v�̊O�����牽�}�X�����ɂ��邩

	const char* SLOPE_DEVICE_MODEL = "data\\TXT\\MOTION\\02_staging\\00_SlopeDevice\\motion_slopedevice.txt";
}

//�ÓI�����o�ϐ��錾
CEdit* CGame::m_pEdit = nullptr;
CPause* CGame::m_pPause = nullptr;
CScore* CGame::m_pScore = nullptr;
CTimer* CGame::m_pTime = nullptr;
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
CPlayer* CGame::m_pPlayer[NUM_PLAYER] = {};
CDevil* CGame::m_pDevil = nullptr;
CBoss* CGame::m_pBoss = nullptr;
CMask* CGame::m_pMask = nullptr;

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

	// 2D�}�X�N�̐���
	m_pMask = CMask::Create();

	//�N���A�t���O�̃f�t�H���g���I���ɂ��Ă���
	m_bGameClear = true;

	//�^�C���̋N��
	CGame::GetTime()->SetStopTime(false);

	// �^�C���̐���
	m_pTime = CTimer::Create();
	m_pTime->SetStartTime(timeGetTime());
	m_pTime->SetTime(0);

	// �w�i���f���ݒ菈���i���j
	SetBgObjTest();

	CMapSystem::GetInstance()->Init();

	//�}�b�v�̃O���b�g�̍ő�l���擾
	int nMapWightMax = CMapSystem::GetInstance()->GetWightMax();
	int nMapHeigtMax = CMapSystem::GetInstance()->GetHeightMax();

	//���̐���
	m_pMapField = CObjmeshField::Create(nMapWightMax -1, nMapHeigtMax - 1);
	m_pMapField->SetPos(INITVECTOR3);

	// �����̐���
	CObjmeshField* pBottonField = CObjmeshField::Create(BOTTOM_FIELD_VTX_WIDTH, BOTTOM_FIELD_VTX_HEIGHT);
	pBottonField->SetTexture(BOTTOM_FIELD_TEX);
	pBottonField->SetPos(BOTTOM_FIELD_POS);
	m_bGameEnd = false;

	//�f�r���̐���
	m_pDevil = CDevil::Create();
	m_pDevil->SetPos(D3DXVECTOR3(0.0f, 100.0f, 500.0f));

	if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_SINGLE)
	{
		//�v���C���[�̐���
		m_pPlayer[0] = CPlayer::Create();
		m_pPlayer[0]->SetPos(CMapSystem::GetInstance()->GetGritPos(CMapSystem::GRID(11, 9)));
		m_pPlayer[0]->SetPlayNumber(0);
	}
	else if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
	{
		//�v���C���[�̐���
		m_pPlayer[0] = CPlayer::Create();
		m_pPlayer[0]->SetPos(CMapSystem::GetInstance()->GetGritPos(CMapSystem::GRID(11, 9)));
		m_pPlayer[0]->SetPlayNumber(0);

		m_pPlayer[1] = CPlayer::Create();
		m_pPlayer[1]->SetPos(CMapSystem::GetInstance()->GetGritPos(CMapSystem::GRID(11, 4)));
		m_pPlayer[1]->SetPlayNumber(1);
	}
	else
	{
		//�v���C���[�̐���
		m_pPlayer[0] = CPlayer::Create();
		m_pPlayer[0]->SetPos(CMapSystem::GetInstance()->GetGritPos(CMapSystem::GRID(11, 9)));
		m_pPlayer[0]->SetPlayNumber(0);
	}

	m_pScore = CScore::Create();
	m_pScore->SetScore(CManager::GetInstance()->GetEndScore());

	//���[���u���b�N�̐���
	LoadStageRailBlock("data\\TXT\\STAGE\\RailBlock.txt");

	CDevilHole* pDevilHole = nullptr;

	//�X�e�[�W�̓ǂݍ���
	switch (CManager::GetInstance()->GetStage())
	{
	case 0:
		CMapSystem::Load("data\\TXT\\STAGE\\Block.txt");

		pDevilHole = CDevilHole::Create("data\\MODEL\\DevilHole.x");
		pDevilHole->SetGrid(CMapSystem::GRID(11, 7));
		CMapSystem::GetInstance()->SetGritBool(11, 7, true);

		// TODO : �O�������o���𗘗p����
		{
			// ���E�����擾
			int nWidth = CMapSystem::GetInstance()->GetWightMax();
			int nHeight = CMapSystem::GetInstance()->GetHeightMax();

			for (int i = 1; i < nWidth; i++)
			{
				for (int nCnt = 1; nCnt < nHeight; nCnt++)
				{// �A�C�e�������ꏊ�Ƀ{���{������
					if (CMapSystem::GetInstance()->GetGritBool(i, nCnt)) { continue; }

					if (rand() % 5) { continue; }

					CItem::Create(CItem::TYPE_BOWABOWA, CMapSystem::GRID(i, nCnt));
				}
			}
		}

		// �\���˂̐���
		CItem::Create(CItem::TYPE_CROSS, CMapSystem::GRID(11, 2));
		CItem::Create(CItem::TYPE_CROSS, CMapSystem::GRID(20, 4));
		CItem::Create(CItem::TYPE_CROSS, CMapSystem::GRID(22, 9));
		CItem::Create(CItem::TYPE_CROSS, CMapSystem::GRID(6, 11));
		CItem::Create(CItem::TYPE_CROSS, CMapSystem::GRID(16, 11));

		// �\�t�g�N���[���̐���
		CItem::Create(CItem::TYPE_SOFTCREAM, CMapSystem::GetInstance()->GetCenter());

		break;

	case 1:
		CMapSystem::Load("data\\TXT\\STAGE\\Block.txt");

		pDevilHole = CDevilHole::Create("data\\MODEL\\DevilHole.x");
		pDevilHole->SetGrid(CMapSystem::GRID(11,7));
		CMapSystem::GetInstance()->SetGritBool(11, 7, true);

		// ��������
		CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(BIBLE_OUTGRIT - 1, BIBLE_OUTGRIT - 1));
		CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(NUM_WIGHT - BIBLE_OUTGRIT, BIBLE_OUTGRIT - 1));
		CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(BIBLE_OUTGRIT - 1, NUM_HEIGHT - BIBLE_OUTGRIT));
		CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(NUM_WIGHT - BIBLE_OUTGRIT, NUM_HEIGHT - BIBLE_OUTGRIT));

		break;
	}

	//�]�����̐���
	CRollRock *pRock = CRollRock::Create("data\\MODEL\\BlockTest.x");
	D3DXVECTOR3 RockPos = CMapSystem::GetInstance()->GetGritPos(CMapSystem::GRID(16, 2));
	pRock->SetPos(D3DXVECTOR3(RockPos.x, 50.0f, RockPos.z));

	// �G�̐���
	CEnemy::Create(CEnemy::ENEMY_MEDAMAN, CMapSystem::GRID(9, 7));
	CEnemy::Create(CEnemy::ENEMY_BONBON, CMapSystem::GRID(11, 5));
	CEnemy::Create(CEnemy::ENEMY_LITTLEDEVIL, CMapSystem::GRID(13, 7));

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

	CScene::Uninit();
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

	CMapSystem::GetInstance()->Update();

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

#ifdef _DEBUG

		if (pInputKeyboard->GetTrigger(DIK_F3))
		{
			StageClear(0);
		}

		if (pInputKeyboard->GetTrigger(DIK_F4))
		{
			StageClear(1);
		}

#endif // _DEBUG
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
// ���[���u���b�N�̓ǂݍ��ݔz�u
//====================================================================
void CGame::LoadStageRailBlock(const char* pFilename)
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
			CMapSystem* pMapSystem = CMapSystem::GetInstance();
			D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();
			float MapSystemGritSize = pMapSystem->GetGritSize() * 0.5f;

			while (1)
			{
				fscanf(pFile, "%s", &aSetMessage[0]);
				if (strcmp(&aSetMessage[0], "STARTSETRAILBLOCK") == 0)
				{
					int WightNumber, HeightNumber, nMax, RailMove[64];

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &WightNumber);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &HeightNumber);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &nMax);

					fscanf(pFile, "%s", &aString[0]);

					for (int nCnt = 0; nCnt < nMax; nCnt++)
					{
						fscanf(pFile, "%d", &RailMove[nCnt]);
					}

					CMapSystem::GetInstance()->SetGritBool(WightNumber, HeightNumber, true);
					CRailBlock* pBlock = CRailBlock::Create(WightNumber, HeightNumber, false, nMax, &RailMove[0]);
					pBlock->SetPos(D3DXVECTOR3(pBlock->GetPos().x, 50.0f, pBlock->GetPos().z));
					pBlock->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));

					fscanf(pFile, "%s", &aEndMessage[0]);
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
// �e�X�g�p�w�i�I�u�W�F�N�g�ݒ菈��
//====================================================================
void CGame::SetBgObjTest(void)
{
	// �X�����u�i�����ڂ����̉��j
	{
		CSlopeDevice* pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_MODEL);
		pSlopeDevice->SetPos(D3DXVECTOR3(1800.0f, BOTTOM_FIELD_POS.y, -500.0f));

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_MODEL);
		pSlopeDevice->SetPos(D3DXVECTOR3(-1800.0f, BOTTOM_FIELD_POS.y, -500.0f));
	}

	// �W���b�L
	{
		CObjectX* pJack = CObjectX::Create("data\\MODEL\\03_staging\\01_Jack\\jack.x");
		pJack->SetPos(D3DXVECTOR3(1300.0f, BOTTOM_FIELD_POS.y, 500.0f));
		pJack->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		CObjectX* pPost = CObjectX::Create("data\\MODEL\\03_staging\\01_Jack\\post.x");
		pPost->SetPos(D3DXVECTOR3(900.0f, BOTTOM_FIELD_POS.y, 500.0f));

		pJack = CObjectX::Create("data\\MODEL\\03_staging\\01_Jack\\jack.x");
		pJack->SetPos(D3DXVECTOR3(1300.0f, BOTTOM_FIELD_POS.y, -500.0f));
		pJack->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		pPost = CObjectX::Create("data\\MODEL\\03_staging\\01_Jack\\post.x");
		pPost->SetPos(D3DXVECTOR3(900.0f, BOTTOM_FIELD_POS.y, -500.0f));

		pJack = CObjectX::Create("data\\MODEL\\03_staging\\01_Jack\\jack.x");
		pJack->SetPos(D3DXVECTOR3(-1300.0f, BOTTOM_FIELD_POS.y, 500.0f));
		pJack->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		pPost = CObjectX::Create("data\\MODEL\\03_staging\\01_Jack\\post.x");
		pPost->SetPos(D3DXVECTOR3(-900.0f, BOTTOM_FIELD_POS.y, 500.0f));

		pJack = CObjectX::Create("data\\MODEL\\03_staging\\01_Jack\\jack.x");
		pJack->SetPos(D3DXVECTOR3(-1300.0f, BOTTOM_FIELD_POS.y, -500.0f));
		pJack->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		pPost = CObjectX::Create("data\\MODEL\\03_staging\\01_Jack\\post.x");
		pPost->SetPos(D3DXVECTOR3(-900.0f, BOTTOM_FIELD_POS.y, -500.0f));
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