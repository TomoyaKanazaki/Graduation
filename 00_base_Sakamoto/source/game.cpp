//============================================
//
//	�Q�[����� [game.cpp]
//	Author:sakamoto kai
//
//============================================
#include "game.h"
#include "fade.h"
#include "objmeshField.h"
#include "GamePlayer.h"
#include "player.h"
#include "camera.h"
#include "timer.h"
#include "Score.h"
#include "devil.h"
#include "DevilHole.h"
#include "renderer.h"
#include "slowManager.h"
#include "enemyYoungDevil.h"
#include "RailBlock.h"
#include "RollRock.h"
#include "bowabowa.h"
#include "ScrollDevice.h"
#include "SlopeDevice.h"
#include "mask.h"
#include "signal.h"
#include "pause.h"
#include "EventMovie.h"
#include "Cross.h"
#include "MapMove.h"
#include "Motion.h"

#include "sound.h"
#include "shadow.h"

namespace
{
	const int SAMPLE_NAMESPACE = 0;

	const float LETTERBOX_HEIGHT = 100.0f;		//���o���̏㉺�̍��|���S���̑���

	const CMapSystem::GRID FIELD_GRID = { 64, 64 }; // ���̏��̃T�C�Y
	const char* BOTTOM_FIELD_TEX = "data\\TEXTURE\\Field\\tile_test_02.png";		// �����̃e�N�X�`��
	const D3DXVECTOR3 BOTTOM_FIELD_POS = D3DXVECTOR3(0.0f, -1000.0f, 0.0f);	// �����̈ʒu
	const int BIBLE_OUTGRIT = 2;	// �������}�b�v�̊O�����牽�}�X�����ɂ��邩

	const std::string SCROLL_DEVICE_MODEL_WIDTH = "data\\TXT\\MOTION\\02_staging\\00_ScrollDevice\\motion_scrolldevice_w.txt";
	const std::string SCROLL_DEVICE_MODEL_HEIGHT = "data\\TXT\\MOTION\\02_staging\\00_ScrollDevice\\motion_scrolldevice_h.txt";
	const std::string SCROLL_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";

	const std::string SLOPE_DEVICE_MODEL = "data\\TXT\\MOTION\\02_staging\\01_SlopeDevice\\motion_slopedevice.txt";
	const std::string SLOPE_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";

	const D3DXCOLOR MASK_DEFAULT_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �ʏ�̃X�e���V���J���[(��)
	const D3DXCOLOR MASK_PLAYER_COLOR = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);			// �^�}�S���̃X�e���V���J���[(��)
	const D3DXCOLOR MASK_MULTI_PLAYER_COLOR = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	// 2P�^�}�S���̃X�e���V���J���[(���F)
	const D3DXCOLOR MASK_MEDAMAN_COLOR = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);			// ���_�}���̃X�e���V���J���[(�s���N)
	const D3DXCOLOR MASK_BONBON_COLOR = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);			// �{�����{���̃X�e���V���J���[(�I�����W)
	const D3DXCOLOR MASK_YUNGDEVIL_COLOR = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);		// �q�f�r���̃X�e���V���J���[(��)
	const D3DXCOLOR MASK_ITEM_COLOR = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);			// �A�C�e���̃X�e���V���J���[(��)

	const int PLAYER_REF = 2;		// �v���C���[�̃X�e���V���Q�ƒl
	const int ITEM_REF = 4;			// �A�C�e���̃X�e���V���Q�ƒl
	const int MEDAMAN_REF = 102;	// ���_�}���̃X�e���V���Q�ƒl
}

//�ÓI�����o�ϐ��錾
CGame* CGame::m_pGame = nullptr;

//====================================================================
//�R���X�g���N�^
//====================================================================
CGame::CGame()
{
	m_bGameEnd = false;
	m_pEventMovie = nullptr;
	m_bEvent = false;
	m_bEventEnd = false;
	m_bDevilHoleFinish = false;
	m_BGColorA = 1.0f;
	m_nTutorialWave = 0;
	m_nNumBowabowa = 0;

	for (int nCnt = 0; nCnt < NUM_CAMERA; nCnt++)
	{
		CManager::GetInstance()->GetCamera(nCnt)->SetBib(false);
		CManager::GetInstance()->GetCamera(nCnt)->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);
	}

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_pBGCharacter[nCnt] = nullptr;
	}
	m_nBGCount = 0;

	m_pPause = nullptr;
	m_pTime = nullptr;
	m_pMeshDomeUp = nullptr;
	m_pMapField = nullptr;
	m_pCubeBlock = nullptr;
	m_pDevil = nullptr;
	m_pPlayerMask = nullptr;
	m_pEnemyMask = nullptr;
	m_pItemMask = nullptr;

	m_bGameClear = false;
	m_Wireframe = false;
	m_Slow = false;
	m_bDevilHoleFinish = false;

	m_nTutorialWave = 0;
	m_nNumBowabowa = 0;

	m_BGColorA = 1.0f;

	m_BGRot = INITVECTOR3;

	LetterBox[0] = nullptr;
	LetterBox[1] = nullptr;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CGame::~CGame()
{
}

//====================================================================
//�C���X�^���X�擾
//====================================================================
CGame* CGame::GetInstance(void)
{
	if (m_pGame == nullptr)
	{
		m_pGame = new CGame;
	}
	return m_pGame;
}


//====================================================================
//����������
//====================================================================
HRESULT CGame::Init(void)
{
	////BGM�̍Đ�
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_STAGE1);

	if (m_pPause == nullptr)
	{
		m_pPause = CPause::Create();
	}

	// �v���C���[�E�A�C�e���E���_�}���̃X�e���V���J���[�̐ݒ�
	CMask::Create(PLAYER_REF, MASK_PLAYER_COLOR);
	CMask::Create(ITEM_REF, MASK_ITEM_COLOR);
	CMask::Create(MEDAMAN_REF, MASK_MEDAMAN_COLOR);

	//�N���A�t���O�̃f�t�H���g���I���ɂ��Ă���
	m_bGameClear = true;

	//�X�e�[�W����0�ɂ���
	CManager::GetInstance()->SetStage(0);

	//�^�C���̋N��
	CGame::GetTime()->SetStopTime(false);

	// �^�C���̐���
	m_pTime = CTimer::Create();
	m_pTime->SetStartTime(timeGetTime());
	m_pTime->SetTime(0);
	m_pTime->SetStopTime(true);	//�^�C���̐i�s���~�߂�

	// �}�b�v�̐���
	CMapSystem::GetInstance();
	CMapSystem::Load("data\\TXT\\STAGE\\map01.csv");

	//�f�r���̐���
	m_pDevil = CDevil::Create();

	// �C�x���g�̊J�n
	//m_bEvent = true;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		LetterBox[nCnt] = CObject2D::Create();
		LetterBox[nCnt]->SetPos(D3DXVECTOR3(640.0f, nCnt * 720.0f, 0.0f));
		LetterBox[nCnt]->SetSize(D3DXVECTOR3(1280.0f, LETTERBOX_HEIGHT, 0.0f));
		LetterBox[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		LetterBox[nCnt]->SetTexture("data\\TEXTURE\\Test.jpg");
	}

	// �����̐���
	auto grid = FIELD_GRID;
	CObjmeshField* pBottonField = CObjmeshField::Create(grid);
	pBottonField->SetTexture(BOTTOM_FIELD_TEX);
	pBottonField->SetPos(BOTTOM_FIELD_POS);
	m_bGameEnd = false;

	// �w�i���f���ݒ菈��
	SetBgObjTest();

	if (m_pEventMovie == nullptr)
	{
		m_pEventMovie = CEventMovie::Create();
	}

	// �\�t�g�N���[���̐���
	CItem::Create(CItem::TYPE_SOFTCREAM, CMapSystem::GetInstance()->GetCenter());


	// �v���C���[�𐶐�����
	for (int i = 0; i < CManager::GetInstance()->GetGameMode(); ++i)
	{
		m_pPlayer.push_back(CGamePlayer::Create(i));
	}

	//�w�i�I�u�W�F�N�g�̐���

	//�E�R
	CObjectX* pRMountain = CObjectX::Create("data\\MODEL\\RightMountain.x");
	pRMountain->SetPos(D3DXVECTOR3(2000.0f, -1000.0f, 2000.0f));

	//���R
	CObjectX* pLMountain = CObjectX::Create("data\\MODEL\\LeftMountain.x");
	pLMountain->SetPos(D3DXVECTOR3(-1900.0f, -1000.0f, 1500.0f));

	//�}�O�}
	CObject3D* pMaguma = CObject3D::Create();
	pMaguma->SetPos(D3DXVECTOR3(0.0f, -950.0f, 0.0f));
	pMaguma->SetSize(D3DXVECTOR3(3000.0f, 0.0f, 6500.0f));
	pMaguma->SetScrollSpeed(D3DXVECTOR2(0.0f, -0.0001f));
	pMaguma->SetTexture("data\\TEXTURE\\MAGUMA.png");

	//���_�}��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_pBGCharacter[nCnt] == nullptr)
		{
			m_pBGCharacter[nCnt] = CObjectCharacter::Create(false);
			m_pBGCharacter[nCnt]->SetTxtCharacter(SCROLL_DEVICE_ENEMY_MODEL, 0);

			m_pBGCharacter[nCnt]->SetPos(D3DXVECTOR3(
				-1975.0f + sinf(D3DX_PI * (0.5f * nCnt)) * 400.0f,
				-200.0f,
				1500.0f + cosf(D3DX_PI * (0.5f * nCnt)) * 250.0f));

			m_pBGCharacter[nCnt]->SetRot(D3DXVECTOR3(
				0.0f,
				sinf(D3DX_PI * (0.5f * (nCnt - 1))),
				0.0f));

			m_pBGCharacter[nCnt]->GetMotion()->Set(1, 0);
		}
	}

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CGame::Uninit(void)
{
	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}

	if (m_pEventMovie != nullptr)
	{
		m_pEventMovie->Uninit();
		m_pEventMovie = nullptr;
	}

	// �v���C���[�̉��
	while(1)
	{
		if (m_pPlayer.size() <= 0) { m_pPlayer.clear(); break; }
		m_pPlayer.back()->SetDeathFlag(true);
		m_pPlayer.pop_back();
	}

	CManager::GetInstance()->GetSound()->Stop();

	// �X���[���̑S�폜
	CSlowManager::ReleaseAll();

	// �}�b�v�V�X�e���̏I��
	CMapSystem::GetInstance()->Uninit();

	//�S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

	CScene::Uninit();

	if (m_pGame != nullptr)
	{
		delete m_pGame;
		m_pGame = nullptr;
	}
}

//====================================================================
//�X�V����
//====================================================================
void CGame::Update(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	DebugProc::Print(DebugProc::POINT_LEFT, "�Q�[���X�s�[�h : %f\n", CManager::GetInstance()->GetGameSpeed());

	CMapSystem::GetInstance()->Update();

	m_nBGCount++;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_pBGCharacter[nCnt] != nullptr)
		{
			m_pBGCharacter[nCnt]->SetPos(D3DXVECTOR3(
				-1975.0f + sinf(D3DX_PI * (0.5f * (nCnt + (float)m_nBGCount * 0.005f))) * 400.0f,
				-200.0f,
				1500.0f + cosf(D3DX_PI * (0.5f * (nCnt + (float)m_nBGCount * 0.005f))) * 250.0f));

			m_pBGCharacter[nCnt]->SetRot(D3DXVECTOR3(
				0.0f,
				sinf(D3DX_PI * (0.5f * ((nCnt - 1) + (float)m_nBGCount * 0.005f))),
				0.0f));
		}
	}

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
		if (m_pEventMovie != nullptr)
		{
			m_pEventMovie->Update();
		}
	}

	if (CManager::GetInstance()->GetPause() == true)
	{
		m_pTime->SetStopTime(true);		//�^�C���̐i�s���~�߂�
	}
	else
	{
		m_pTime->SetStopTime(false);	//�^�C���̐i�s��i�߂�
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

				int EndScore = 0;

				for (unsigned int nCnt = 0; nCnt < m_pPlayer.size(); nCnt++)
				{
					if (m_pPlayer.at(nCnt) != nullptr)
					{
						EndScore += m_pPlayer.at(nCnt)->GetScore()->GetScore();
					}
				}

				CManager::GetInstance()->SetEndScore(EndScore);

				if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
				{
					CManager::GetInstance()->SetEnd1PScore(m_pPlayer.at(0)->GetScore()->GetScore());
					CManager::GetInstance()->SetEnd2PScore(m_pPlayer.at(1)->GetScore()->GetScore());
				}
			}
		}

		//���^�[�{�b�N�X�̍X�V
		UpdateLetterBox();

		//�|�[�Y�̍X�V����
		if (m_pPause != nullptr)
	
		{
			m_pPause->Update();
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

		if (pInputKeyboard->GetTrigger(DIK_F5))
		{
			CFade::SetFade(CScene::MODE_TUTORIAL);
		}

#endif // _DEBUG
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CGame::Draw(void)
{
	if (m_pPause != nullptr)
	{
		m_pPause->Draw();
	}
}

//====================================================================
//�X�e�[�W�̏���������
//====================================================================
void CGame::ResetStage(void)
{
	//�C�x���g�t���O�𗧂Ă�
	m_bEvent = true;

	//���݂̃X�N���[����Ԃ�ۑ�����
	CMapSystem::GetInstance()->GetMove()->SetScroolSetState(CMapSystem::GetInstance()->GetMove()->GetState());

	//�ۑ������X�N���[����Ԃ��m�F����p�̕ϐ�
	//CMapMove::MOVE m_Scrool = CMapSystem::GetInstance()->GetMove()->GetScroolSetState();

	// �}�b�v�̏�����
	CMapSystem::GetInstance()->GetMove()->Init();
	CMapSystem::GetInstance()->GetMove()->SetStateCount(200);
	CObjmeshField::GetListTop()->SetRot(INITVECTOR3);

	// �X�����u�̃��X�g�\����������Δ�����
	if (CSlopeDevice::GetList() == nullptr) { return; }
	std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // ���X�g���擾

	// �X�����u�̃��X�g�̒��g���m�F����
	for (CSlopeDevice* pSlopeDevice : list)
	{
		// �����̌X�����u���㏸��ԂɕύX
		pSlopeDevice->ReSet();
	}

	if (m_pEventMovie != nullptr)
	{
		m_pEventMovie->SetEventType(CEventMovie::STATE_CHANGE);
	}

	m_bGameEnd = false;
}

//====================================================================
//�X�e�[�W�i�s����
//====================================================================
void CGame::NextStage(void)
{
	//�\���˂̍폜
	DeleteCross();

	////�}�b�v�̍폜
	//CMapSystem::MapDelete();

	//// �}�b�v�̐���
	//CMapSystem::Load("data\\TXT\\STAGE\\map06.csv");

	// �\�t�g�N���[���̐���
	CItem::Create(CItem::TYPE_SOFTCREAM, CMapSystem::GetInstance()->GetCenter());

	//�����̐���
	CreateBible();

	//�X�e�[�W����i�߂�
	CManager::GetInstance()->SetStage(1);
}

//====================================================================
//�\���˂̍폜
//====================================================================
void CGame::DeleteCross(void)
{
	// �f�r���z�[���̃��X�g�\����������Δ�����
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // ���X�g���擾

	// �f�r���z�[�����X�g�̒��g���m�F����
	for (CCross* pCross : list)
	{
		pCross->Uninit();
	}
}

//====================================================================
//�����̐���
//====================================================================
void CGame::CreateBible(void)
{
	//�O���b�h�ő�E�ŏ��ʒu�擾
	CMapSystem::GRID GMax = CMapSystem::GetInstance()->GetMove()->GetMaxGrid();
	CMapSystem::GRID GMin = CMapSystem::GetInstance()->GetMove()->GetMinGrid();

	// ��������
	CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GMin.x + BIBLE_OUTGRIT, GMin.z + BIBLE_OUTGRIT));
	CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GMax.x - BIBLE_OUTGRIT, GMin.z + BIBLE_OUTGRIT));
	CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GMin.x + BIBLE_OUTGRIT, GMax.z - BIBLE_OUTGRIT));
	CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GMax.x - BIBLE_OUTGRIT, GMax.z - BIBLE_OUTGRIT));
}

//====================================================================
//�X�e�[�W�N���A����
//====================================================================
void CGame::StageClear(int Stage)
{
	if (Stage == 0)
	{
		ResetStage();
	}
	else
	{
		CFade::SetFade(CScene::MODE_RESULT);
		m_pTime->SetStopTime(true);

		int EndScore = 0;

		for (unsigned int nCnt = 0; nCnt < m_pPlayer.size(); nCnt++)
		{
			if (m_pPlayer.at(nCnt) != nullptr)
			{
				EndScore += m_pPlayer.at(nCnt)->GetScore()->GetScore();
			}
		}

		CManager::GetInstance()->SetEndScore(EndScore);

		if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
		{
			CManager::GetInstance()->SetEnd1PScore(m_pPlayer.at(0)->GetScore()->GetScore());
			CManager::GetInstance()->SetEnd2PScore(m_pPlayer.at(1)->GetScore()->GetScore());
		}
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CGame::UpdateLetterBox(void)
{
	if (m_bEvent)
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			D3DXVECTOR3 Height = LetterBox[nCnt]->GetSize();

			if (Height.y < LETTERBOX_HEIGHT)
			{
				Height.y += 2.0f;
			}

			LetterBox[nCnt]->SetSize(Height);
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			D3DXVECTOR3 Height = LetterBox[nCnt]->GetSize();

			if (Height.y > 0.0f)
			{
				Height.y -= 2.0f;
			}

			LetterBox[nCnt]->SetSize(Height);
		}
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

			CObject::OBJECT_TYPE type = pObj->GetType();			//��ނ��擾

			if (type == CObject::TYPE_CUBEBLOCK ||
				type == CObject::TYPE_ENEMY3D ||
				type == CObject::TYPE_MAPMODEL)
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
void CGame::LoadStageRailBlock(const std::string pFilename)
{
	//�t�@�C�����J��
	FILE* pFile = fopen(pFilename.c_str(), "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		char aString[128] = {};			//�S�~��
		char aStartMessage[32] = {};	//�X�^�[�g���b�Z�[�W
		char aSetMessage[32] = {};		//�Z�b�g���b�Z�[�W
		char aEndMessage[32] = {};		//�I�����b�Z�[�W

		fscanf(pFile, "%s", &aStartMessage[0]);
		if (strcmp(&aStartMessage[0], "STARTSETSTAGE") == 0)
		{
			CMapSystem* pMapSystem = CMapSystem::GetInstance();
			D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

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
					/*CRailBlock* pBlock = CRailBlock::Create(WightNumber, HeightNumber, false, nMax, &RailMove[0]);
					pBlock->SetPos(D3DXVECTOR3(pBlock->GetPos().x, 50.0f, pBlock->GetPos().z));
					pBlock->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));*/

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
void CGame::LoadStageMapModel(const std::string pFilename)
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
	// �}�b�v�ړ����u
	{
		// ���E
		CScrollDevice* pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL_WIDTH, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(1075.0f, 75.0f, 0.0f));

		pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL_WIDTH, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(-1075.0f, 75.0f, 0.0f));

		// �㉺
		pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL_HEIGHT, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(0.0f, 75.0f, 700.0f));
		pScrollDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));

		pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL_HEIGHT, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(0.0f, 75.0f, -700.0f));
		pScrollDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	}

	// �W���b�L
	{
		CSlopeDevice* pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetPos(D3DXVECTOR3(800.0f, BOTTOM_FIELD_POS.y, 450.0f));
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_TOP_LEFT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		pSlopeDevice->SetPos(D3DXVECTOR3(-800.0f, BOTTOM_FIELD_POS.y, 450.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_TOP_RIGHT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetPos(D3DXVECTOR3(800.0f, BOTTOM_FIELD_POS.y, -450.0f));
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_BOTTOM_LEFT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		pSlopeDevice->SetPos(D3DXVECTOR3(-800.0f, BOTTOM_FIELD_POS.y, -450.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_BOTTOM_RIGHT);
	}
}
