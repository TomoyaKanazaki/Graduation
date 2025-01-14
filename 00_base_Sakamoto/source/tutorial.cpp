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
#include "tutorialUi.h"

#include "sound.h"
#include "shadow.h"

#include "BgObjManager.h"

//====================================================================
// �萔��`
//====================================================================
namespace
{
	const D3DXVECTOR3 CHECK_POS[]
	{// ���ꂼ��̈ʒu
		{ 170.0f, 60.0f, 0.0f },  // �ړ��̍��W
		{ 170.0f, 115.0f, 0.0f }, // �\���ˍ��W
		{ 170.0f, 170.0f, 0.0f }, // �{���{���̍��W
		{ 170.0f, 225.0f, 0.0f }, // �U���̍��W
		{ 170.0f, 280.0f, 0.0f }, // �����̍��W
		{ 170.0f, 334.0f, 0.0f }, // �f�r���z�[���̍��W
	};

	const std::string TUTORIAL_TEX[]
	{// �`���[�g���A���e�L�X�g�̃e�N�X�`���p�X
		{ "data\\TEXTURE\\UI\\tutorial_text_00.png" },	// �ړ��̃e�L�X�g
		{ "data\\TEXTURE\\UI\\tutorial_text_01.png" },	// �\���ˍ��W
		{ "data\\TEXTURE\\UI\\tutorial_text_03.png" },	// �{���{���̍��W
		{ "data\\TEXTURE\\UI\\tutorial_text_02.png" },	// �U���̍��W
		{ "data\\TEXTURE\\UI\\tutorial_text_004.png" },	// �����̍��W
		{ "data\\TEXTURE\\UI\\tutorial_text_05.png" },	// �f�r���z�[���̍��W
	};

	const D3DXVECTOR3 TEXT_POS[]
	{// �e�L�X�g�̈ʒu
		{ D3DXVECTOR3(5.0f, 0.0f, 0.0f) },	// �ړ��̃e�L�X�g
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f) },		// �\���ˍ��W
		{ D3DXVECTOR3(115.0f, 0.0f, 0.0f) },	// �{���{���̍��W
		{ D3DXVECTOR3(107.5f, 0.0f, 0.0f) },	// �U���̍��W
		{ D3DXVECTOR3(10.0f, 5.0f, 0.0f) },		// �����̍��W
		{ D3DXVECTOR3(115.0f, 0.0f, 0.0f) },	// �f�r���z�[���̍��W
	};

	const D3DXVECTOR3 TEXT_SIZE[]
	{// ���ꂼ��̃e�L�X�g�̃T�C�Y
		{ D3DXVECTOR3(250.0f, 100.0f, 0.0f) },	// �ړ��̃e�L�X�g
		{ D3DXVECTOR3(250.0f, 40.0f, 0.0f) },	// �\���ˍ��W
		{ D3DXVECTOR3(500.0f, 50.0f, 0.0f) },	// �{���{���̍��W
		{ D3DXVECTOR3(500.0f, 50.0f, 0.0f) },	// �U���̍��W
		{ D3DXVECTOR3(275.0f, 80.0f, 0.0f) },	// �����̍��W
		{ D3DXVECTOR3(500.0f, 50.0f, 0.0f) },	// �f�r���z�[���̍��W
	};

	const int BIBLE_OUTGRIT = 2;			// �������}�b�v�̊O�����牽�}�X�����ɂ��邩
	const int STENCIL_REF_PLAYER = 2;		// �v���C���[�̃X�e���V���̎Q�ƒl
	const int STENCIL_REF_ITEM = 4;			// �A�C�e���̃X�e���V���̎Q�ƒl
	const int STENCIL_REF_MEDAMAN = 102;	// ���_�}���̃X�e���V���̎Q�ƒl
	const int WAVE_MIDDLE = 4;				// �`���[�g���A��4�i�K��
	const int WAVE_MAX = 6;					// �`���[�g���A���ő�i�K
	const int PLAYER_REF = 2;		// �v���C���[�̃X�e���V���Q�ƒl
	const int ITEM_REF = 4;			// �A�C�e���̃X�e���V���Q�ƒl
	const int MEDAMAN_REF = 102;	// ���_�}���̃X�e���V���Q�ƒl

	const std::string BOTTOM_FIELD_TEX = "data\\TEXTURE\\Field\\outside.jpg";		// �����̃e�N�X�`��
	const std::string SCROLL_DEVICE_MODEL = "data\\TXT\\MOTION\\02_staging\\00_ScrollDevice\\motion_scrolldevice.txt";
	const std::string SCROLL_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";
	const std::string CHECK_MARKER_TEX = "data\\TEXTURE\\UI\\tutorial_check.png";	// �`�F�b�N�}�[�J�[�e�N�X�`��
	const std::string BUTTON_TEX = "data\\TEXTURE\\UI\\return_title.png";			// �J�ڃ{�^���e�N�X�`��

	const std::string CHECKBOX_TEX = "data\\TEXTURE\\UI\\tutorial_check_box.png";		// �`�F�b�N�{�b�N�X�̃e�N�X�`��
	const std::string TUTORIAL_FRAME_TEX = "data\\TEXTURE\\UI\\tutorial_frame.png";		// �`���[�g���A���K�C�h�̊O�g�̃e�N�X�`��
	const std::string ACHIEVEMENT_TEX = "data\\TEXTURE\\UI\\tutorial_achievement.png";	// ���ڒB���̃e�N�X�`��

	const float CHECK_POS_Y = 200.0f;		// �`�F�b�N�}�[�J�[��y���W

	const CMapSystem::GRID FIELD_GRID = { 64, 64 }; // ���̏��̃T�C�Y
	const CMapSystem::GRID BIBLE_POS = { 11, 10 };	// �����̈ʒu

	const D3DXVECTOR3 BOTTOM_FIELD_POS = D3DXVECTOR3(0.0f, -1000.0f, 0.0f);	// �����̈ʒu
	const D3DXVECTOR3 GUIDE_SIZE = D3DXVECTOR3(420.0f, 360.0f, 0.0f);	// �`���[�g���A���K�C�h�̃T�C�Y
	const D3DXVECTOR3 BOX_SIZE = D3DXVECTOR3(50.0f, 50.0f, 0.0f);		// �`�F�b�N�{�b�N�X�̃T�C�Y
	const D3DXVECTOR3 MARKER_SIZE = D3DXVECTOR3(50.0f, 50.0f, 0.0f);	// �}�[�J�[�T�C�Y
	const D3DXVECTOR3 BUTTON_POS = D3DXVECTOR3(1100.0f, 650.0f, 0.0f);	// �J�ڃ{�^���̈ʒu
	const D3DXVECTOR3 BUTTON_SIZE = D3DXVECTOR3(300.0f, 250.0f, 0.0f);	// �J�ڃ{�^���̃T�C�Y
	const D3DXVECTOR3 TEXTURE_CENTER_POS = D3DXVECTOR3(225.0f, 200.0f, 0.0f);	// �e�N�X�`���̒��S���W
	const D3DXVECTOR3 ACHIEVEMENT_SIZE = D3DXVECTOR3(300.0f, 100.0f, 0.0f);	// �e�N�X�`���̒��S���W

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
static_assert(NUM_ARRAY(TUTORIAL_TEX) == CTutorial::TYPE_MAX, "ERROR : Type Count Missmatch");
static_assert(NUM_ARRAY(TEXT_POS) == CTutorial::TYPE_MAX, "ERROR : Type Count Missmatch");
static_assert(NUM_ARRAY(TEXT_SIZE) == CTutorial::TYPE_MAX, "ERROR : Type Count Missmatch");

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
InitPlayerPos(D3DXVECTOR3()),	// �v���C���[�̏����ʒu
m_nNumBible(0)				// �����̑���
{
	for (int nCnt = 0; nCnt < NUM_CAMERA; ++nCnt)
	{// �J��������
		CManager::GetInstance()->GetCamera(nCnt)->SetBib(false);
		CManager::GetInstance()->GetCamera(nCnt)->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);
	}

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_bSound[nCnt] = false;
	}

	for (int i = 0; i < TYPE_MAX; ++i)
	{// �}�[�J�[�\��
		m_bCheck[i] = false;
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
		// �T�E���h�̒�~
		CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_SE_SIGN_UP);
		CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_SE_SIGN_DOWN);
		CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_SE_SIGN_RIGHT);
		CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_SE_SIGN_LEFT);

		m_pPause = CPause::Create();
	}

	// �v���C���[�E�A�C�e���E���_�}���̃X�e���V���J���[�̐ݒ�
	CMask::Create(PLAYER_REF, MASK_PLAYER_COLOR);
	CMask::Create(ITEM_REF, MASK_ITEM_COLOR);
	CMask::Create(MEDAMAN_REF, MASK_MEDAMAN_COLOR);

	//�N���A�t���O�̃f�t�H���g���I���ɂ��Ă���
	m_bTutorialClear = true;

	//�^�C���̋N��
	CTutorial::GetTime()->SetStopTime(false);

	// �}�b�v�̐���
	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	pMapSystem->Init();
	pMapSystem->CreateMap(pMapSystem->GetSelectMap());

	//�f�r���̐���
	m_pDevil = CDevil::Create();

	// �\���˂̑����ۑ�
	if (CCross::GetList() == nullptr) { assert(false); }
	m_nNumCross = CCross::GetList()->GetList().size();

	// �{���{���̑����ۑ�
	if (CBowabowa::GetList() == nullptr) { assert(false); }
	m_nNumBowabowa = CBowabowa::GetList()->GetList().size();

	// �G�̑����ۑ�
	m_nNumEnemy = CEnemy::GetList()->GetList().size();

	// �`���[�g���A���K�C�h�̊O�g
	CTutorialUi::Create(TEXTURE_CENTER_POS, GUIDE_SIZE, TUTORIAL_FRAME_TEX, 1.0f);
	
	for (int i = 0; i < TYPE_MAX; ++i)
	{// �񋓕��`�F�b�N�{�b�N�X�̐���
		CTutorialUi::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[i].x,
							TEXTURE_CENTER_POS.y + CHECK_POS[i].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z),
							BOX_SIZE, CHECKBOX_TEX, 1.0f);

		//CTutorialUi::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x + CHECK_POS[i].x,
		//					TEXTURE_CENTER_POS.y + CHECK_POS[i].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z),
		//					BOX_SIZE, CHECKBOX_TEX, 1.0f);
	}

	for (int i = 0; i < TYPE_MAX; ++i)
	{// �e�L�X�g�̐���
		m_pText.push_back(CTutorialUi::Create(
			D3DXVECTOR3(TEXTURE_CENTER_POS.x + TEXT_POS[i].x, CHECK_POS[i].y - TEXT_POS[i].y, CHECK_POS[i].z),
						TEXT_SIZE[i], TUTORIAL_TEX[i], 1.0f));
	}

	// �J�ڃ{�^���̐���
	CTutorialUi::Create(BUTTON_POS, BUTTON_SIZE, BUTTON_TEX, 1.0f);

	for (int i = 0; i < TYPE_MAX; ++i)
	{// �`�F�b�N�}�[�J�[��\��
		m_bCheck[i] = false;
	}
	
	// �����̐���
	auto grid = FIELD_GRID;
	CObjmeshField* pBottonField = CObjmeshField::Create(grid);
	pBottonField->SetTexture(BOTTOM_FIELD_TEX);
	pBottonField->SetPos(BOTTOM_FIELD_POS);

	// �w�i���f���ݒ菈��
	SetBgObjTest();

	for (int i = 0; i < CManager::GetInstance()->GetGameMode(); ++i)
	{// �v���C���[�𐶐�����
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
	//CMapSystem::GetInstance()->Uninit();

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
	DebugProc::Print(DebugProc::POINT_CENTER, "3�Ő��������i�K�ɐݒ�\n");

	// �}�b�v�V�X�e���̍X�V
	CMapSystem::GetInstance()->Update();
	
	// �v���C���[���X�g���擾
	if (CPlayer::GetList() == nullptr) { assert(false); }
	std::list<CPlayer*> list = CPlayer::GetList()->GetList();

	// �f�r���z�[�����X�g���擾
	if (CDevilHole::GetList() == nullptr) { assert(false); }
	std::list<CDevilHole*> DevilHolelist = CDevilHole::GetList()->GetList();

	// �G�l�~�[���X�g���擾
	if (CEnemy::GetList() == nullptr) { assert(false); }
	std::list<CEnemy*> Enemylist = CEnemy::GetList()->GetList();

	for (CPlayer* player : list)
	{
		if (player->GetState() == CPlayer::STATE_WALK
			&& m_bCheck[TYPE_MOVE] == false)
		{// ���W����v���Ȃ�������
			CTutorialCheck::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[TYPE_MOVE].x,
				TEXTURE_CENTER_POS.y + CHECK_POS[TYPE_MOVE].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z));

			// �}�[�J�[�\��
			m_bCheck[TYPE_MOVE] = true;

			if (m_bCheck[TYPE_MOVE] == true && m_bSound[TYPE_MOVE] == false)
			{
				// �T�E���h�Đ�
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHECK);

				m_bSound[TYPE_MOVE] = true;
			}

			if (m_bCheck[TYPE_MOVE] == true)
			{// �e�L�X�g�̕s�����x��������
				SetUIAlpha(TYPE_MOVE, 0.5f);
			}

			// �`���[�g���A���i�K��i�߂�
			m_nTutorialWave += 1;
		}

		if (CCross::GetList() != nullptr
			&& player->GetItemType() == CPlayer::TYPE_CROSS
			&& m_bCheck[TYPE_CROSS] == false)
		{// �\���ˎ擾
			CTutorialCheck::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[TYPE_CROSS].x,
									TEXTURE_CENTER_POS.y + CHECK_POS[TYPE_CROSS].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z));

			// �}�[�J�[�\��
			m_bCheck[TYPE_CROSS] = true;

			if (m_bCheck[TYPE_CROSS] == true && m_bSound[TYPE_CROSS] == false)
			{
				// �T�E���h�Đ�
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHECK);

				m_bSound[TYPE_CROSS] = true;
			}

			if (m_bCheck[TYPE_CROSS] == true)
			{// �e�L�X�g�̕s�����x��������
				SetUIAlpha(TYPE_CROSS, 0.5f);
			}

			// �`���[�g���A���i�K��i�߂�
			m_nTutorialWave += 1;
		}

		if (player->GetItemType() == CPlayer::TYPE_BIBLE
			&& m_bCheck[TYPE_BIBLE] == false)
		{// �����擾
			CTutorialCheck::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[TYPE_BIBLE].x,
									TEXTURE_CENTER_POS.y + CHECK_POS[TYPE_BIBLE].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z));

			// �}�[�J�[�\��
			m_bCheck[TYPE_BIBLE] = true;

			if (m_bCheck[TYPE_BIBLE] == true && m_bSound[TYPE_BIBLE] == false)
			{
				// �T�E���h�Đ�
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHECK);

				m_bSound[TYPE_BIBLE] = true;
			}

			if (m_bCheck[TYPE_BIBLE] == true)
			{// �e�L�X�g�̕s�����x��������
				SetUIAlpha(TYPE_BIBLE, 0.5f);
			}

			// �`���[�g���A���i�K��i�߂�
			m_nTutorialWave += 1;
		}
	}

	if (m_nNumEnemy != CEnemy::GetList()->GetList().size()
		&& m_bCheck[TYPE_ATTACK] == false)
	{// �G�̑�������
		CTutorialCheck::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[TYPE_ATTACK].x,
								TEXTURE_CENTER_POS.y + CHECK_POS[TYPE_ATTACK].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z));

		// �}�[�J�[�\��
		m_bCheck[TYPE_ATTACK] = true;

		if (m_bCheck[TYPE_ATTACK] == true && m_bSound[TYPE_ATTACK] == false)
		{
			// �T�E���h�Đ�
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHECK);

			m_bSound[TYPE_ATTACK] = true;
		}

		if (m_bCheck[TYPE_ATTACK] == true)
		{// �e�L�X�g�̕s�����x��������
			SetUIAlpha(TYPE_ATTACK, 0.5f);
		}

		// �`���[�g���A���i�K��i�߂�
		m_nTutorialWave += 1;
	}

	if (CBowabowa::GetList() != nullptr 
		&& m_nNumBowabowa != CBowabowa::GetList()->GetList().size()
		&& m_bCheck[TYPE_BOWABOWA] == false)
	{// �{���{���̑�������
		CTutorialCheck::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[TYPE_BOWABOWA].x,
								TEXTURE_CENTER_POS.y + CHECK_POS[TYPE_BOWABOWA].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z));

		// �}�[�J�[�\��
		m_bCheck[TYPE_BOWABOWA] = true;

		if (m_bCheck[TYPE_BOWABOWA] == true && m_bSound[TYPE_BOWABOWA] == false)
		{
			// �T�E���h�Đ�
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHECK);

			m_bSound[TYPE_BOWABOWA] = true;
		}

		if (m_bCheck[TYPE_BOWABOWA] == true)
		{// �e�L�X�g�̕s�����x��������
			SetUIAlpha(TYPE_BOWABOWA, 0.5f);
		}

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

	for (CDevilHole* pDevilHole : DevilHolelist)
	{// �f�r���z�[���̒��g���m�F
		if (pDevilHole->IsSet())
		{// �f�r���z�[����4�ӏ��̓�1�ӏ����܂���
			CTutorialCheck::Create(D3DXVECTOR3(TEXTURE_CENTER_POS.x - CHECK_POS[TYPE_DEVILHOLE].x,
									TEXTURE_CENTER_POS.y + CHECK_POS[TYPE_DEVILHOLE].y - CHECK_POS_Y, TEXTURE_CENTER_POS.z));

			// �}�[�J�[�\��
			m_bCheck[TYPE_DEVILHOLE] = true;

			if (m_bCheck[TYPE_DEVILHOLE] == true && m_bSound[TYPE_DEVILHOLE] == false)
			{
				// �T�E���h�Đ�
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CHECK);

				m_bSound[TYPE_DEVILHOLE] = true;
			}

			if (m_bCheck[TYPE_DEVILHOLE] == true)
			{// �e�L�X�g�̕s�����x��������
				SetUIAlpha(TYPE_DEVILHOLE, 0.5f);
			}

			// �`���[�g���A���i�K��i�߂�
			m_nTutorialWave += 1;
		}
	}

	if (m_nTutorialWave >= WAVE_MAX)
	{
		// �l��WAVE_MAX�����Ȃ��悤��
		m_nTutorialWave = WAVE_MAX;

		// ���ڒB���e�N�X�`���\��
		//CTutorialUi::Create(TEXTURE_CENTER_POS, ACHIEVEMENT_SIZE, ACHIEVEMENT_TEX, 1.0f);
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
			if (m_nTutorialWave == WAVE_MAX)
			{// 6�i�K�ڂ̂Ƃ�
				m_bTutorialEnd = true;
			}
			break;
		}

		//�X�e�[�W�N���A���̏���
		if (m_bTutorialEnd == true)
		{
			if (pInputKeyboard->GetTrigger(DIK_RETURN))
			{// �D���ȃ^�C�~���O�őJ��
				CFade::SetFade(CScene::MODE_TITLE);
			}
		}

		//�|�[�Y�̍X�V����
		if (m_pPause != nullptr)
		{
			m_pPause->Update();
		}
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
void CTutorial::LoadStageRailBlock(const std::string pFilename)
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
// �e�X�g�p�w�i�I�u�W�F�N�g�ݒ菈��
//====================================================================
void CTutorial::SetBgObjTest(void)
{

	// �}�b�v�ړ����u
	{
		//CScrollDevice* pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL, SCROLL_DEVICE_ENEMY_MODEL);
		//pScrollDevice->SetPos(D3DXVECTOR3(1300.0f, 0.0f, 0.0f));

		//pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL, SCROLL_DEVICE_ENEMY_MODEL);
		//pScrollDevice->SetPos(D3DXVECTOR3(-1300.0f, 0.0f, 0.0f));
	}
}

//==========================================
//  UI�̓����x��ύX
//==========================================
void CTutorial::SetUIAlpha(const CHECKTYPE type, const float fAlpha)
{
	m_pText.at(type)->SetAlpha(fAlpha);
}
