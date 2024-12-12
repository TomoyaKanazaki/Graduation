//============================================
//
//	�`���[�g���A����� [tutorial.cpp]
//	Author:morikawa shunya
//
//============================================
#include "tutorial.h"
#include "fade.h"
#include "objmeshField.h"
#include "TutorialPlayer.h"
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
#include "objmeshField.h"
#include "Cross.h"
#include "MapMove.h"
#include "pause.h"
#include "tutorialCheck.h"
#include "bible.h"

#include "sound.h"
#include "shadow.h"

//====================================================================
// �萔��`
//====================================================================
namespace
{
	const D3DXVECTOR3 CHECK_POS[]
	{
		{ 0.0f, 0.0f, 0.0f },	 // NONE�̍��W
		{ 50.0f, 110.0f, 0.0f }, // �ړ��̍��W
		{ 50.0f, 160.0f, 0.0f }, // �\���ˍ��W
		{ 50.0f, 200.0f, 0.0f }, // �U�����̍��W
		{ 50.0f, 250.0f, 0.0f }, // �{���{���̍��W
		{ 50.0f, 295.0f, 0.0f }, // �����̍��W
		{ 50.0f, 360.0f, 0.0f }, // �f�r���z�[���̍��W
	};

	const int BIBLE_OUTGRIT = 2;			// �������}�b�v�̊O�����牽�}�X�����ɂ��邩
	const int STENCIL_REF_PLAYER = 2;		// �v���C���[�̃X�e���V���̎Q�ƒl
	const int STENCIL_REF_ITEM = 4;			// �A�C�e���̃X�e���V���̎Q�ƒl
	const int STENCIL_REF_MEDAMAN = 102;	// ���_�}���̃X�e���V���̎Q�ƒl
	const int WAVE_MIDDLE = 4;				// �`���[�g���A��4�i�K��
	const int WAVE_MAX = 6;					// �`���[�g���A���ő�i�K

	const char* BOTTOM_FIELD_TEX = "data\\TEXTURE\\Field\\outside.jpg";		// �����̃e�N�X�`��
	const char* SCROLL_DEVICE_MODEL = "data\\TXT\\MOTION\\02_staging\\00_ScrollDevice\\motion_scrolldevice.txt";
	const char* SCROLL_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";
	const char* SLOPE_DEVICE_MODEL = "data\\TXT\\MOTION\\02_staging\\01_SlopeDevice\\motion_slopedevice.txt";
	const char* SLOPE_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";
	const char* TUTORIAL_GUIDE = "data\\TEXTURE\\UI\\tutorial_guid.png";	// �`���[�g���A���K�C�h�̃e�N�X�`��
	const char* CHECK_MARKER_TEX = "data\\TEXTURE\\UI\\tutorial_check.png";	// �`�F�b�N�}�[�J�[�e�N�X�`��

	const CMapSystem::GRID FIELD_GRID = { 64, 64 }; // ���̏��̃T�C�Y
	const CMapSystem::GRID BIBLE_POS = { 11, 10 };	// �����̈ʒu

	const D3DXVECTOR3 BOTTOM_FIELD_POS = D3DXVECTOR3(0.0f, -1000.0f, 0.0f);	// �����̈ʒu
	const D3DXVECTOR3 GUIDE_POS = D3DXVECTOR3(200.0f, 225.0f, 0.0f);	// �`���[�g���A���K�C�h�̈ʒu
	const D3DXVECTOR3 GUIDE_SIZE = D3DXVECTOR3(420.0f, 360.0f, 0.0f);	// �`���[�g���A���K�C�h�̃T�C�Y
	const D3DXVECTOR3 MARKER_POS = D3DXVECTOR3(50.0f, 160.0f, 0.0f);	// �}�[�J�[�ʒu
	const D3DXVECTOR3 MARKER_SIZE = D3DXVECTOR3(50.0f, 50.0f, 0.0f);	// �}�[�J�[�T�C�Y

	const D3DXCOLOR MASK_DEFAULT_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �ʏ�̃X�e���V���J���[(��)
	const D3DXCOLOR MASK_PLAYER_COLOR = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);			// �^�}�S���̃X�e���V���J���[(��)
	const D3DXCOLOR MASK_MULTI_PLAYER_COLOR = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	// 2P�^�}�S���̃X�e���V���J���[(���F)
	const D3DXCOLOR MASK_MEDAMAN_COLOR = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);			// ���_�}���̃X�e���V���J���[(�s���N)
	const D3DXCOLOR MASK_BONBON_COLOR = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);			// �{�����{���̃X�e���V���J���[(�I�����W)
	const D3DXCOLOR MASK_YUNGDEVIL_COLOR = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);		// �q�f�r���̃X�e���V���J���[(��)
	const D3DXCOLOR MASK_ITEM_COLOR = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);			// �A�C�e���̃X�e���V���J���[(��)
}

//==========================================
//  �ÓI�x������
//==========================================
static_assert(NUM_ARRAY(CHECK_POS) == CTutorial::TYPE_MAX, "ERROR : Type Count Missmatch");

//====================================================================
// �ÓI�����o�ϐ��錾
//====================================================================
CTutorial* CTutorial::m_pTutorial = nullptr;

//====================================================================
// �R���X�g���N�^
//====================================================================
CTutorial::CTutorial():
m_bTutorialEnd(false),		// �Q�[���I���̃t���O
m_bDevilHoleFinish(false),	// �f�r���z�[����4�������܂������ǂ���
m_BGColorA(1.0f),			// �C�x���g�w�i�̓����x
m_nTutorialWave(0),			// �`���[�g���A���̒i�K
m_nNumBowabowa(0),			// �{���{���̑���
m_nNumCross(0),				// �\���˂̑���
m_nNumEnemy(0),				// �G�̑���
m_pPause(nullptr),			// �|�[�Y�̃|�C���^
m_pDevil(nullptr),			// �f�r���̃|�C���^
m_bTutorialClear(false),	// �Q�[���N���A�̃t���O
m_Wireframe(false),			// ���C���[�t���[���؂�ւ�
m_Slow(false),				// �X���[���o�t���O
m_pTutorialGuide(nullptr),	// �`���[�g���A���K�C�h�̃|�C���^
InitPlayerPos(D3DXVECTOR3()),	// �v���C���[�̏����ʒu
m_nNumBible(0)				// �����̑���
{
	for (int nCnt = 0; nCnt < NUM_CAMERA; ++nCnt)
	{// �J��������
		CManager::GetInstance()->GetCamera(nCnt)->SetBib(false);
		CManager::GetInstance()->GetCamera(nCnt)->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);
	}
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CTutorial::~CTutorial()
{
}

//====================================================================
// �C���X�^���X�擾
//====================================================================
CTutorial* CTutorial::GetInstance(void)
{
	if (m_pTutorial == nullptr)
	{// �C���X�^���X����
		m_pTutorial = new CTutorial;
	}

	return m_pTutorial;
}

//====================================================================
// ����������
//====================================================================
HRESULT CTutorial::Init(void)
{
	//BGM�̍Đ�
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_STAGE1);

	if (m_pPause == nullptr)
	{// �|�[�Y�̐���
		m_pPause = CPause::Create();
	}

	// �v���C���[�E�A�C�e���E���_�}���̃X�e���V���J���[�̐ݒ�
	CMask::Create(2, MASK_PLAYER_COLOR);
	CMask::Create(4, MASK_ITEM_COLOR);
	CMask::Create(102, MASK_MEDAMAN_COLOR);

	if (m_pTutorialGuide == nullptr)
	{// �`���[�g���A���K�C�h�̐���
		m_pTutorialGuide = CObject2D::Create();
	}
	if (m_pTutorialGuide != nullptr)
	{// �e�N�X�`���E�ʒu�E�T�C�Y�ݒ�
		m_pTutorialGuide->SetTexture(TUTORIAL_GUIDE);
		m_pTutorialGuide->SetPos(GUIDE_POS);
		m_pTutorialGuide->SetSize(GUIDE_SIZE);
	}

	//�N���A�t���O�̃f�t�H���g���I���ɂ��Ă���
	m_bTutorialClear = true;

	//�^�C���̋N��
	CTutorial::GetTime()->SetStopTime(false);

	// �}�b�v�̐���
	CMapSystem::GetInstance()->Init();
	CMapSystem::Load("data\\TXT\\STAGE\\map01.csv");

	//�f�r���̐���
	m_pDevil = CDevil::Create();

	// �\���˂̑����ۑ�
	m_nNumCross = CCross::GetList()->GetList().size();

	// �{���{���̑����ۑ�
	m_nNumBowabowa = CBowabowa::GetList()->GetList().size();

	// �G�̑����ۑ�
	m_nNumEnemy = CEnemy::GetList()->GetList().size();

	for (int i = 0; i < TYPE_MAX; ++i)
	{
		m_bCheck[i] = false;
	}
	
	// �����̐���
	auto grid = FIELD_GRID;
	CObjmeshField* pBottonField = CObjmeshField::Create(grid);
	pBottonField->SetTexture(BOTTOM_FIELD_TEX);
	pBottonField->SetPos(BOTTOM_FIELD_POS);

	// �w�i���f���ݒ菈��
	SetBgObjTest();

	// �v���C���[�𐶐�����
	for (int i = 0; i < CManager::GetInstance()->GetGameMode(); ++i)
	{
		m_pPlayer.push_back(CTutorialPlayer::Create(i));
		m_gridPlayer.push_back(m_pPlayer.at(i)->GetGrid());
	}

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CTutorial::Uninit(void)
{
	if (m_pPause != nullptr)
	{// �|�[�Y�̏I��
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}

	// �v���C���[�̉��
	while (1)
	{
		if (m_pPlayer.size() <= 0) { m_pPlayer.clear(); break; }
		m_pPlayer.back()->SetDeathFlag(true);
		m_pPlayer.pop_back();
	}

	// �T�E���h��~
	CManager::GetInstance()->GetSound()->Stop();

	// �X���[���̑S�폜
	CSlowManager::ReleaseAll();

	// �}�b�v�V�X�e���̏I��
	CMapSystem::GetInstance()->Uninit();

	//�S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

	// �V�[���̏I��
	CScene::Uninit();

	if (m_pTutorial != nullptr)
	{
		delete m_pTutorial;
		m_pTutorial = nullptr;
	}
}

//====================================================================
//�X�V����
//====================================================================
void CTutorial::Update(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	DebugProc::Print(DebugProc::POINT_LEFT, "�Q�[���X�s�[�h : %f\n", CManager::GetInstance()->GetGameSpeed());
	DebugProc::Print(DebugProc::POINT_CENTER, "�`���[�g���A���i�K : %d\n", m_nTutorialWave);

	// �}�b�v�V�X�e���̍X�V
	CMapSystem::GetInstance()->Update();
	
	// �v���C���[���X�g���擾
	if (CPlayer::GetList() == nullptr) { assert(false); }
	std::list<CPlayer*> list = CPlayer::GetList()->GetList();    // ���X�g���擾

	int nNumPlayer = 0;

	for (CPlayer* player : list)
	{
		if (m_gridPlayer.at(nNumPlayer) != player->GetGrid()
			&& m_bCheck[TYPE_MOVE] == false)
		{// ���W����v���Ȃ�������
			CTutorialCheck::Create(CHECK_POS[TYPE_MOVE]);
			m_bCheck[TYPE_MOVE] = true;

			// �`���[�g���A���i�K��i�߂�
			m_nTutorialWave += 1;
		}

		if (player->GetItemType() == CPlayer::TYPE_CROSS
			&& m_bCheck[TYPE_CROSS] == false)
		{// �\���ˎ擾
			CTutorialCheck::Create(CHECK_POS[TYPE_CROSS]);
			m_bCheck[TYPE_CROSS] = true;

			// �`���[�g���A���i�K��i�߂�
			m_nTutorialWave += 1;
		}

		if (player->GetItemType() == CPlayer::TYPE_BIBLE
			&& m_bCheck[TYPE_BIBLE] == false)
		{// �����擾
			CTutorialCheck::Create(CHECK_POS[TYPE_BIBLE]);
			
			m_bCheck[TYPE_BIBLE] = true;

			// �`���[�g���A���i�K��i�߂�
			m_nTutorialWave += 1;
		}
	}

	if (m_nNumEnemy != CEnemy::GetList()->GetList().size()
		&& m_bCheck[TYPE_ATTACK] == false)
	{// �G�̑�������
		CTutorialCheck::Create(CHECK_POS[TYPE_ATTACK]);
		m_bCheck[TYPE_ATTACK] = true;

		// �`���[�g���A���i�K��i�߂�
		m_nTutorialWave += 1;
	}

	if (m_nNumBowabowa != CBowabowa::GetList()->GetList().size()
		&& m_bCheck[TYPE_BOWABOWA] == false)
	{// �{���{���̑�������
		CTutorialCheck::Create(CHECK_POS[TYPE_BOWABOWA]);
		m_bCheck[TYPE_BOWABOWA] = true;

		// �`���[�g���A���i�K��i�߂�
		m_nTutorialWave += 1;
	}

	if (m_nTutorialWave == WAVE_MIDDLE)
	{// �`���[�g���A����4�i�K�ڂ܂Ői�񂾂�
		if (CBible::GetList() == nullptr)
		{// 1���������̐���
			CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(BIBLE_POS));
		}
	}

#if _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_3) == true)
	{// �`���[�g���A���i�K��4�ɂ���
		m_nTutorialWave = WAVE_MIDDLE;
	}

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

	if (CManager::GetInstance()->GetFade()->GetFade() == CFade::FADE_NONE)
	{
		//�X�e�[�W�N���A�����̐ݒ�
		switch (CManager::GetInstance()->GetStage())
		{
		case 0:
			// �{���{���̃��X�g�\����������Δ�����
			if (CBowabowa::GetList() == nullptr)
			{
				m_bTutorialEnd = true;
			}
			break;

		case 1:
			if (m_bDevilHoleFinish == true)
			{
				m_bTutorialEnd = true;
			}
			break;
		}

		//�X�e�[�W�N���A���̏���
		if (m_bTutorialEnd == true)
		{
			if (m_bTutorialEnd == true
				&& pInputKeyboard->GetTrigger(DIK_RETURN)
				&& m_nTutorialWave == WAVE_MAX)
			{// �D���ȃ^�C�~���O�ŃQ�[���ɑJ��
				CFade::SetFade(CScene::MODE_GAME);
			}
			else
			{
				CFade::SetFade(CScene::MODE_RESULT);

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

		//�|�[�Y�̍X�V����
		if (m_pPause != nullptr)

		{
			m_pPause->Update();
		}

#ifdef _DEBUG

		if (pInputKeyboard->GetTrigger(DIK_F6) == true)
		{// �`���[�g���A���ŏ�����
			CFade::SetFade(CScene::MODE_TUTORIAL);
		}

#endif // _DEBUG
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CTutorial::Draw(void)
{
	if (m_pPause != nullptr)
	{
		m_pPause->Draw();
	}
}

//====================================================================
//�X�e�[�W�i�s����
//====================================================================
void CTutorial::NextStage(void)
{
	// �}�b�v�̐���
	CMapSystem::GetInstance()->GetMove()->Init();
	CObjmeshField::GetListTop()->SetRot(INITVECTOR3);

	//�\���˂̍폜
	DeleteCross();

	// �\�t�g�N���[���̐���
	CItem::Create(CItem::TYPE_SOFTCREAM, CMapSystem::GetInstance()->GetCenter());

	//�����̐���
	CreateBible();

	//�X�e�[�W����i�߂�
	CManager::GetInstance()->SetStage(1);

	m_bTutorialEnd = false;
}

//====================================================================
//�\���˂̍폜
//====================================================================
void CTutorial::DeleteCross(void)
{
	// �\���˂̃��X�g�\����������Δ�����
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // ���X�g���擾

	// �\���˃��X�g�̒��g���m�F����
	for (CCross* pCross : list)
	{
		pCross->Uninit();
	}
}

//====================================================================
//�����̐���
//====================================================================
void CTutorial::CreateBible(void)
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
//�}�b�v�I�u�W�F�N�g�̍폜
//====================================================================
void CTutorial::DeleteMap(void)
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
void CTutorial::LoadStageRailBlock(const char* pFilename)
{
	//�t�@�C�����J��
	FILE* pFile = fopen(pFilename, "r");

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
void CTutorial::LoadStageMapModel(const char* pFilename)
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
void CTutorial::SetBgObjTest(void)
{

	// �}�b�v�ړ����u
	{
		CScrollDevice* pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(1300.0f, 0.0f, 0.0f));

		pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(-1300.0f, 0.0f, 0.0f));
	}

#if 0 // ����̃f�o�b�O�p�i�e�X�g���ł߂荞�ނ��߈ꎞ��~�j

	// �W���b�L
	{
		CSlopeDevice* pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetPos(D3DXVECTOR3(900.0f, BOTTOM_FIELD_POS.y, 500.0f));
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_TOP_LEFT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		pSlopeDevice->SetPos(D3DXVECTOR3(-900.0f, BOTTOM_FIELD_POS.y, 500.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_TOP_RIGHT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetPos(D3DXVECTOR3(900.0f, BOTTOM_FIELD_POS.y, -500.0f));
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_BOTTOM_LEFT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		pSlopeDevice->SetPos(D3DXVECTOR3(-900.0f, BOTTOM_FIELD_POS.y, -500.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_BOTTOM_RIGHT);
	}
#endif

}
