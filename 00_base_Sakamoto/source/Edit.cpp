//============================================
//
//	�G�f�B�b�g [Edit.cpp]
//	Author:sakamoto kai
//
//============================================
#include "Edit.h"
#include "object.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objmeshCube.h"
#include "CubeBlock.h"
#include "RailBlock.h"
#include "Rail.h"
#include "debugproc.h"
#include "MapModel.h"
#include "XModel.h"
#include "CubeEffect.h"
#include "MapSystem.h"
#include "useful.h"

//�}�N����`
namespace
{
	const char* DATA_BLOCK_NAME("data\\TXT\\STAGE\\Block.txt");
	const char* DATA_RAILBLOCK_NAME("data\\TXT\\STAGE\\RailBlock.txt");
	const char* DATA_XMODEL_NAME("data\\TXT\\STAGE\\XModel.txt");
	const char* DATA_STAIR_NAME("data\\TXT\\STAGE\\Stair.txt");
	const char* DATA_GIMMICK_NAME("data\\TXT\\STAGE\\Gimmick.txt");

	// �`���[�g���A���p
	//const char* DATA_BLOCK_NAME("data\\TXT\\STAGE\\Block_Tutorial_000.txt");
	//const char* DATA_XMODEL_NAME("data\\TXT\\STAGE\\XModel_Tutorial_000.txt");
	//const char* DATA_STAIR_NAME("data\\TXT\\STAGE\\Stair_Tutorial_000.txt");
	//const char* DATA_GIMMICK_NAME("data\\TXT\\STAGE\\Gimmick_Tutorial_000.txt");

	// �u���b�N�̏����ʒu�iY�j
	const float BLOCK_INIT_POS_Y[CEdit::BLOCK_INIT_TYPE_MAX] =
	{
		50.0f,
		50.0f,
		50.0f,
	};

	// �u���b�N�̏����T�C�Y
	const D3DXVECTOR3 BLOCK_INIT_SIZE[CEdit::BLOCK_INIT_TYPE_MAX] =
	{
		D3DXVECTOR3(50.0f, 50.0f,50.0f),
		D3DXVECTOR3(10.0f,50.0f,50.0f),
		D3DXVECTOR3(50.0f,10.0f, 50.0f),
	};

	// �u���b�N�̃e�N�X�`���̎��
	const char *BLOCK_TEXTURE_TYPE[CEdit::BLOCK_TEXTURE_MAX] =
	{
		"data\\TEXTURE\\Wood000.png",
		"data\\TEXTURE\\Wood001.png",
		"data\\TEXTURE\\Wood002.png",
		"data\\TEXTURE\\Wood003.png",
	};
}

//�ÓI�����o�ϐ��錾
CEdit::OBJINFO CEdit::m_aModelInfo[OBJ_HIT_MAX] =
{
	{ "data\\MODEL\\MapObj\\Props\\daruma.x","�_���}" },
	{ "data\\MODEL\\MapObj\\Props\\kousi001.x","�i�q" },
	{ "data\\MODEL\\MapObj\\Props\\toro.x","����" },
	{ "data\\MODEL\\MapObj\\Props\\wood.x","��" },
	{ "data\\MODEL\\MapObj\\Props\\andon.x","�s��" },
	{ "data\\MODEL\\MapObj\\Props\\Armor.x","�Z����" },
	{ "data\\MODEL\\MapObj\\Props\\Barrel.x","�M" },
	{ "data\\MODEL\\MapObj\\Props\\Bucket.x","���C��" },
	{ "data\\MODEL\\MapObj\\Props\\castle001.x","��" },
	{ "data\\MODEL\\MapObj\\Props\\FoldingScreen.x","����" },
	{ "data\\MODEL\\MapObj\\Props\\Gate.x","��" },
	{ "data\\MODEL\\MapObj\\Props\\HandCandle.x","��C" },
	{ "data\\MODEL\\MapObj\\Props\\JapaneseCushion.x","���z�c" },
	{ "data\\MODEL\\MapObj\\Props\\Kakejiku000.x","�|�����i�߁j" },
	{ "data\\MODEL\\MapObj\\Props\\Kakejiku001.x","�|�����i�T�j" },
	{ "data\\MODEL\\MapObj\\Props\\Kettle.x","�₩��" },
	{ "data\\MODEL\\MapObj\\Props\\LookoutTower.x","�������̍�" },
	{ "data\\MODEL\\MapObj\\Props\\SwordRack.x","���|��" },
	{ "data\\MODEL\\MapObj\\Props\\SwordWallHanging.x","���Ǌ|��" },
	{ "data\\MODEL\\MapObj\\Props\\VaseThin.x","�ׂ���" },
	{ "data\\MODEL\\MapObj\\Props\\Well.x","���" },
	{ "data\\MODEL\\MapObj\\Props\\VaseBig.x","�傫����" },
	{ "data\\MODEL\\MapObj\\Props\\Pillar.x","��" },
	{ "data\\MODEL\\MapObj\\Props\\wooddesk.x","�؂̊�" },
	{ "data\\MODEL\\MapObj\\Props\\wood_many_bg.x","��������̖؂�����܂�" },
	{ "data\\MODEL\\MapObj\\Props\\glassdesk.x","�K���X�̊�" },
	{ "data\\MODEL\\MapObj\\Props\\glassdesk_weapon.x","�K���X�̊��̒��ɕ���" },
	{ "data\\MODEL\\MapObj\\Props\\Tatami_floor.x","���オ���" },
	{ "data\\MODEL\\MapObj\\Props\\rice_bales.x","�ĕU" },
	{ "data\\MODEL\\MapObj\\Props\\Boss_Fence.x","�{�X�p��" },
	{ "data\\MODEL\\MapObj\\Props\\chest.x","�I" },
	{ "data\\MODEL\\MapObj\\Props\\Hearth.x","�͘F��" },
	{ "data\\MODEL\\MapObj\\Props\\Fusuma000.x","���i�P���j" },
	{ "data\\MODEL\\MapObj\\Props\\Fusuma001.x","���i�S���j4�i�[�j" },
	{ "data\\MODEL\\MapObj\\Props\\Fusuma002.x","���i�S���j3�i���j" },
	{ "data\\MODEL\\MapObj\\Props\\Fusuma003.x","���i�S���j2�i���j" },
	{ "data\\MODEL\\MapObj\\Props\\Fusuma004.x","���i�S���j1�i�[�j" },
	{ "data\\MODEL\\MapObj\\Props\\Kakejiku002.x","�|�����i�S�j" },
	{ "data\\MODEL\\MapObj\\Props\\Kakejiku003.x","�|�����i�R�j" },
	{ "data\\MODEL\\MapObj\\Props\\Torch.x","����" },
	{ "data\\MODEL\\MapObj\\Props\\oni_kamen.x","�S�̉���" },
	{ "data\\MODEL\\MapObj\\Props\\Foundation.x","��̓y��" },
	{ "data\\MODEL\\MapObj\\Props\\Scaffolding.x","����" },
	{ "data\\MODEL\\MapObj\\Props\\SupportPost.x","�x����" },
};

//====================================================================
//�R���X�g���N�^
//====================================================================
CEdit::CEdit()
{
	m_nPressCount = 0;
	m_nBlockNumber = 0;
	m_nModelNumber = 0;
	m_nGimmickNumber = 0;
	m_nBlockTextureIdx = 0;
	m_nRailMax = 0;

	m_pEditBlock = nullptr;
	if (m_pEditBlock == nullptr)
	{
		m_pEditBlock = CObjmeshCube::Create();
		m_pEditBlock->SetSize(D3DXVECTOR3(100.0f, 10.0f, 100.0f));
		m_pEditBlock->SetTexture(BLOCK_TEXTURE_TYPE[m_nBlockTextureIdx]);
		m_pEditBlock->SetAppear(false);
	}

	m_pEditModel = nullptr;
	if (m_pEditModel == nullptr)
	{
		m_pEditModel = CMapModel::Create(m_aModelInfo[m_nModelNumber].pFilename);
		m_pEditModel->SetAppear(false);
		AppearCollision();
	}

	m_EditRailBlock = nullptr;
	if (m_EditRailBlock == nullptr)
	{
		m_EditRailBlock = CRailBlock::Create(0, 0, true, 0, 0);
		m_EditRailBlock->SetPos(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
		m_EditRailBlock->SetAppear(false);
	}

	m_EditPos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
	m_EditRot = INITVECTOR3;
	m_EditSize = D3DXVECTOR3(50.0f, 50.0f, 50.0f);
	m_EditType = EDITTYPE_BLOCK;
	MoveWidth = 100.0f;
	m_fRotValue = D3DX_PI * 0.5f;
	m_bModelCollision = true;
	m_bAppearCollision = false;

	m_MapGritWight = 0;
	m_MapGritHeight = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CEdit::~CEdit()
{

}

//====================================================================
//��������
//====================================================================
CEdit* CEdit::Create()
{
	CEdit* pMap = nullptr;

	if (pMap == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pMap = new CEdit();
	}

	return pMap;
}

//====================================================================
//�I������
//====================================================================
void CEdit::Uninit(void)
{
	if (m_pEditBlock != nullptr)
	{
		m_pEditBlock->SetDeathFlag(true);
		m_pEditBlock = nullptr;
	}

	if (m_pEditModel != nullptr)
	{
		m_pEditModel->SetDeathFlag(true);
		m_pEditModel = nullptr;
	}
}

//====================================================================
//�X�V����
//====================================================================
void CEdit::Update(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//�ړ�����
	Move();

	if (pInputKeyboard->GetTrigger(DIK_9) == true)
	{
		// Y���̈ʒu������
		m_EditPos.y = 0.0f;

		if (m_EditType == EDITTYPE_RAILBLOCK)
		{
			m_EditType = EDITTYPE_BLOCK;
			m_pEditBlock->SetAppear(true);
			m_pEditModel->SetAppear(false);
			m_EditRailBlock->SetAppear(false);

		}
		else if (m_EditType == EDITTYPE_BLOCK)
		{
			m_EditType = EDITTYPE_RAILBLOCK;
			m_pEditBlock->SetAppear(false);
			m_pEditModel->SetAppear(false);
			m_EditRailBlock->SetAppear(true);
		}
		else if (m_EditType == EDITTYPE_MODELHIT)
		{
			m_EditType = EDITTYPE_BLOCK;
			m_pEditBlock->SetAppear(true);
			m_pEditModel->SetAppear(false);
			m_EditRailBlock->SetAppear(false);

			m_EditPos.y = BLOCK_INIT_POS_Y[m_nBlockNumber];
			m_EditSize = BLOCK_INIT_SIZE[m_nBlockNumber];
		}
	}

	//�f�o�b�O�\���̎擾
	DebugProc::Print(DebugProc::POINT_LEFT, "========== {�G�f�B�b�g���[�h�N����} ==========\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "�G�f�B�b�g�̈ʒu [%f]:[%f]:[%f]\n", m_EditPos.x, m_EditPos.y, m_EditPos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "�ϓ��̕� [%f]\n", MoveWidth);
	DebugProc::Print(DebugProc::POINT_LEFT, "�����̕� [%f]\n", m_fRotValue);
	DebugProc::Print(DebugProc::POINT_LEFT, "-----[�������]-----\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "�G�f�B�b�g�ʒu�̈ړ� : [W][A][S][D][LShift][LControl]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "�J�����ʒu�̈ړ�     : [T][F][G][H][RShift][RControl]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "���_�̈ړ�           : [I][J][K][L]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "�ړ�������/���Z�b�g  : [��][��] / [N]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "����������/���Z�b�g  : [��][��] / [M]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "�ݒu���̕ۑ�       : [F9]\n");

	switch (m_EditType)
	{
	case CEdit::EDITTYPE_BLOCK:

		UpdateBlock();

		break;
	case CEdit::EDITTYPE_MODELHIT:

		UpdateXModel();

		break;
	case CEdit::EDITTYPE_RAILBLOCK:

		UpdateRailBlock();

		break;
	case CEdit::EDITTYPE_MAX:
		break;
	default:
		break;
	}

	if (pInputKeyboard->GetTrigger(DIK_F9) == true)
	{
		SaveData();
	}

	DebugProc::Print(DebugProc::POINT_LEFT, "========== {�G�f�B�b�g���[�h�N����} ==========\n");
}

//====================================================================
//	�u���b�N�̍X�V����
//====================================================================
void CEdit::UpdateBlock(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

	m_pEditBlock->SetAppear(true);
	m_pEditBlock->SetPos(D3DXVECTOR3(MapPos.x + m_MapGritWight * 100.0f, 50.0f, MapPos.z - m_MapGritHeight * 100.0f));

	m_pEditBlock->SetSize(m_EditSize);

	if (pInputKeyboard->GetTrigger(DIK_E) == true)
	{
		m_EditSize.x += MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_Q) == true)
	{
		m_EditSize.x -= MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_Y) == true)
	{
		m_EditSize.y += MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_R) == true)
	{
		m_EditSize.y -= MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_O) == true)
	{
		m_EditSize.z += MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_U) == true)
	{
		m_EditSize.z -= MoveWidth;
	}

	// �e���v���[�g��ޕύX�i�����j
	if (pInputKeyboard->GetTrigger(DIK_7) == true)
	{
		m_nBlockNumber--;

		if (m_nBlockNumber < BLOCK_INIT_TYPE_NORMAL)
		{
			m_nBlockNumber = BLOCK_INIT_TYPE_MAX - 1;
		}

		// �����u���b�N���ݒ�
		m_EditPos.y = BLOCK_INIT_POS_Y[m_nBlockNumber];
		m_EditSize = BLOCK_INIT_SIZE[m_nBlockNumber];
	}

	// �e���v���[�g��ޕύX�i�~���j
	if (pInputKeyboard->GetTrigger(DIK_8) == true)
	{
		m_nBlockNumber++;

		if (m_nBlockNumber >= BLOCK_INIT_TYPE_MAX)
		{
			m_nBlockNumber = BLOCK_INIT_TYPE_NORMAL;
		}

		// �����u���b�N���ݒ�
		m_EditPos.y = BLOCK_INIT_POS_Y[m_nBlockNumber];
		m_EditSize = BLOCK_INIT_SIZE[m_nBlockNumber];
	}

	// �e�N�X�`����ޕύX�i�����j
	if (pInputKeyboard->GetTrigger(DIK_5) == true)
	{
		m_nBlockTextureIdx--;

		if (m_nBlockTextureIdx < BLOCK_TEXTURE_00)
		{
			m_nBlockTextureIdx = BLOCK_TEXTURE_MAX - 1;
		}

		m_pEditBlock->SetTexture(BLOCK_TEXTURE_TYPE[m_nBlockTextureIdx]);
	}

	// �e�N�X�`����ޕύX�i�~���j
	if (pInputKeyboard->GetTrigger(DIK_6) == true)
	{
		m_nBlockTextureIdx++;

		if (m_nBlockTextureIdx >= BLOCK_TEXTURE_MAX)
		{
			m_nBlockTextureIdx = BLOCK_TEXTURE_00;
		}

		m_pEditBlock->SetTexture(BLOCK_TEXTURE_TYPE[m_nBlockTextureIdx]);
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		CCubeBlock* pBlock = CCubeBlock::Create();
		pBlock->SetPos(m_pEditBlock->GetPos());
		pBlock->SetSize(m_pEditBlock->GetSize());
		pBlock->SetWightNumber(m_MapGritWight);
		pBlock->SetHeightNumber(m_MapGritHeight);
		pBlock->SetTexture(BLOCK_TEXTURE_TYPE[m_nBlockTextureIdx]);
		AppearCollision();
	}

	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true)
	{
		CollisionBlock();
	}

	//�f�o�b�O�\���̎擾
	DebugProc::Print(DebugProc::POINT_LEFT, "�u���b�N�̃T�C�Y [%f]:[%f]:[%f]\n", m_pEditBlock->GetSize().x, m_pEditBlock->GetSize().y, m_pEditBlock->GetSize().z);
	DebugProc::Print(DebugProc::POINT_LEFT, "�T�C�Y�ύX�L�[ : X�� [Q- : E+] \n");
	DebugProc::Print(DebugProc::POINT_LEFT, "�T�C�Y�ύX�L�[ : Y�� [R- : Y+] \n");
	DebugProc::Print(DebugProc::POINT_LEFT, "�T�C�Y�ύX�L�[ : Z�� [U- : O+] \n");
	DebugProc::Print(DebugProc::POINT_LEFT, "�u���b�N�̃e���v���[�g�ύX�L�[ : [7 : 8] \n");
	DebugProc::Print(DebugProc::POINT_LEFT, "�u���b�N�̃e�N�X�`���ύX�L�[ : [5 : 6] \n");
}

//====================================================================
//	���[���u���b�N�̍X�V����
//====================================================================
void CEdit::UpdateRailBlock(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

	m_EditRailBlock->SetAppear(true);
	m_EditRailBlock->SetWightNumber(m_MapGritWight);
	m_EditRailBlock->SetHeightNumber(m_MapGritHeight);

	m_EditRailBlock->SetSize(m_EditSize);
	m_EditRailBlock->EditRailUpdate();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{

		CMapSystem::GetInstance()->SetGritBool(m_MapGritWight, m_MapGritHeight, true);
		CRailBlock* pBlock = CRailBlock::Create(m_MapGritWight, m_MapGritHeight, false, m_nRailMax, &m_nRailMove[0]);
		pBlock->SetPos(CMapSystem::GetInstance()->GetGritPos(m_MapGritWight, m_MapGritHeight));
		pBlock->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
		m_nRailMax = 0;
	}

	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		m_nRailMove[m_nRailMax] = 0;
		m_EditRailBlock->EditRailSet(0);
		m_nRailMax++;
	}

	if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		m_nRailMove[m_nRailMax] = 1;
		m_EditRailBlock->EditRailSet(1);
		m_nRailMax++;
	}

	if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{
		m_nRailMove[m_nRailMax] = 2;
		m_EditRailBlock->EditRailSet(2);
		m_nRailMax++;
	}

	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{
		m_nRailMove[m_nRailMax] = 3;
		m_EditRailBlock->EditRailSet(3);
		m_nRailMax++;
	}

	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true)
	{
		CollisionRailBlock();
	}

	//�f�o�b�O�\���̎擾
	DebugProc::Print(DebugProc::POINT_LEFT, "�u���b�N�̃T�C�Y [%f]:[%f]:[%f]\n", m_EditRailBlock->GetSize().x, m_EditRailBlock->GetSize().y, m_EditRailBlock->GetSize().z);
	DebugProc::Print(DebugProc::POINT_LEFT, "�T�C�Y�ύX�L�[ : X�� [Q- : E+] \n");
	DebugProc::Print(DebugProc::POINT_LEFT, "�T�C�Y�ύX�L�[ : Y�� [R- : Y+] \n");
	DebugProc::Print(DebugProc::POINT_LEFT, "�T�C�Y�ύX�L�[ : Z�� [U- : O+] \n");
	DebugProc::Print(DebugProc::POINT_LEFT, "�u���b�N�̃e���v���[�g�ύX�L�[ : [7 : 8] \n");
	DebugProc::Print(DebugProc::POINT_LEFT, "�u���b�N�̃e�N�X�`���ύX�L�[ : [5 : 6] \n");
}

//====================================================================
//	X���f���̍X�V����
//====================================================================
void CEdit::UpdateXModel(void)
{
	//�e����̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CXModel* pXModel = CManager::GetInstance()->GetXModel();

	m_pEditModel->SetAppear(true);
	m_pEditModel->SetPos(m_EditPos);

	if (pInputKeyboard->GetTrigger(DIK_4) == true)
	{
		m_bAppearCollision = !m_bAppearCollision;
		AppearCollision();
	}
	if (pInputKeyboard->GetTrigger(DIK_5) == true)
	{
		m_bModelCollision = m_pEditModel->GetCollision();

		m_bModelCollision = !m_bModelCollision;

		m_pEditModel->SetCollision(m_bModelCollision);
	}

	if (pInputKeyboard->GetTrigger(DIK_6) == true)
	{
		m_EditRot.y += D3DX_PI * 0.5f;
		m_pEditModel->SwapSize();

		if(m_EditRot.y >= D3DX_PI * 2.0f)
		{
			m_EditRot.y = 0.0f;
		}

		m_pEditModel->SetRot(m_EditRot);
	}

	if (pInputKeyboard->GetTrigger(DIK_7) == true)
	{
		m_nModelNumber--;

		if (m_nModelNumber < OBJ_HIT_DARUMA)
		{
			m_nModelNumber = OBJ_HIT_MAX - 1;
		}

		m_pEditModel->SetXModelIdx(pXModel->Regist(m_aModelInfo[m_nModelNumber].pFilename));
		m_pEditModel->SizeVtxSet();


		if (m_EditRot.y < 1.57f)
		{

		}
		else if (m_EditRot.y < 3.14f)
		{
			m_pEditModel->SwapSize();
		}
		else if (m_EditRot.y < 4.71f)
		{

		}
		else
		{
			m_pEditModel->SwapSize();
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_8) == true)
	{
		m_nModelNumber++;

		if (m_nModelNumber >= OBJ_HIT_MAX)
		{
			m_nModelNumber = OBJ_HIT_DARUMA;
		}

		m_pEditModel->SetXModelIdx(pXModel->Regist(m_aModelInfo[m_nModelNumber].pFilename));
		m_pEditModel->SizeVtxSet();

		if (m_EditRot.y < 1.57f)
		{

		}
		else if (m_EditRot.y < 3.14f)
		{
			m_pEditModel->SwapSize();
		}
		else if (m_EditRot.y < 4.71f)
		{

		}
		else
		{
			m_pEditModel->SwapSize();
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_X) == true)
	{
		m_EditRot.x += m_fRotValue;
		m_pEditModel->SwapSize();

		useful::NormalizeAngle(&m_EditRot);

		m_pEditModel->SetRot(m_EditRot);
	}

	if (pInputKeyboard->GetTrigger(DIK_Z) == true)
	{
		m_EditRot.y += m_fRotValue;
		m_pEditModel->SwapSize();

		useful::NormalizeAngle(&m_EditRot);

		m_pEditModel->SetRot(m_EditRot);
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		CMapModel* pModel = CMapModel::Create(m_aModelInfo[m_nModelNumber].pFilename);
		pModel->SetEditIdx(m_nModelNumber);
		pModel->SetPos(m_EditPos);
		pModel->SetRot(m_EditRot);

		if (m_EditRot.y < 1.57f)
		{

		}
		else if (m_EditRot.y < 3.14f)
		{
			pModel->SwapSize();
		}
		else if (m_EditRot.y < 4.71f)
		{

		}
		else
		{
			pModel->SwapSize();
		}

		pModel->SetCollision(m_bModelCollision);
		AppearCollision();
	}

	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true)
	{
		CollisionXModel();
	}

	m_pEditModel->UpdateBlock();

	//�f�o�b�O�\���̎擾
	DebugProc::Print(DebugProc::POINT_LEFT, "���f���̓����蔻��؂�ւ� : [5]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "������90�x��]: [6]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "���f���ύX: [7 : 8]\n");
	if (m_bModelCollision == true)
	{
		DebugProc::Print(DebugProc::POINT_LEFT, "���f���̓����蔻�� : [ TRUE ]\n");
	}
	else
	{
		DebugProc::Print(DebugProc::POINT_LEFT, "���f���̓����蔻�� : [ FALSE ]\n");
	}
	DebugProc::Print(DebugProc::POINT_LEFT, "���f���̐���: %s\n", m_aModelInfo[m_nModelNumber].pCommentname);
}

//====================================================================
//�ړ�����
//====================================================================
void CEdit::Move(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	int MapWigthMax = CMapSystem::GetInstance()->GetWightMax();
	int MapHeightMax = CMapSystem::GetInstance()->GetHeightMax();

	if (pInputKeyboard->GetPress(DIK_W) == true ||
		pInputKeyboard->GetPress(DIK_A) == true ||
		pInputKeyboard->GetPress(DIK_S) == true ||
		pInputKeyboard->GetPress(DIK_D) == true ||
		pInputKeyboard->GetPress(DIK_LSHIFT) == true ||
		pInputKeyboard->GetPress(DIK_LCONTROL) == true)
	{
		m_nPressCount++;
	}
	else
	{
		m_nPressCount = 0;
	}

	if (m_nPressCount >= 30 && m_nPressCount % 2 == 0)
	{
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{
			m_EditPos.z += MoveWidth;
			m_MapGritHeight--;
		}

		if (pInputKeyboard->GetPress(DIK_S) == true)
		{
			m_EditPos.z -= MoveWidth;
			m_MapGritHeight++;
		}

		if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			m_EditPos.x -= MoveWidth;
			m_MapGritWight--;
		}

		if (pInputKeyboard->GetPress(DIK_D) == true)
		{
			m_EditPos.x += MoveWidth;
			m_MapGritWight++;
		}

		if (pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			m_EditPos.y += MoveWidth;
		}

		if (pInputKeyboard->GetPress(DIK_LCONTROL) == true)
		{
			m_EditPos.y -= MoveWidth;
		}
	}
	else
	{
		if (pInputKeyboard->GetTrigger(DIK_W) == true)
		{
			m_EditPos.z += MoveWidth;
			m_MapGritHeight--;
		}

		if (pInputKeyboard->GetTrigger(DIK_S) == true)
		{
			m_EditPos.z -= MoveWidth;
			m_MapGritHeight++;
		}

		if (pInputKeyboard->GetTrigger(DIK_A) == true)
		{
			m_EditPos.x -= MoveWidth;
			m_MapGritWight--;
		}

		if (pInputKeyboard->GetTrigger(DIK_D) == true)
		{
			m_EditPos.x += MoveWidth;
			m_MapGritWight++;
		}

		if (pInputKeyboard->GetTrigger(DIK_LSHIFT) == true)
		{
			m_EditPos.y += MoveWidth;
		}

		if (pInputKeyboard->GetTrigger(DIK_LCONTROL) == true)
		{
			m_EditPos.y -= MoveWidth;
		}
	}

	if (m_MapGritWight >= MapWigthMax)
	{
		m_MapGritWight = 0;
	}
	if (m_MapGritWight < 0)
	{
		m_MapGritWight = MapWigthMax - 1;
	}
	if (m_MapGritHeight >= MapHeightMax)
	{
		m_MapGritHeight = 0;
	}
	if (m_MapGritHeight < 0)
	{
		m_MapGritHeight = MapHeightMax - 1;
	}

	//���ύX
	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		m_fRotValue += D3DX_PI * 0.125f;
		useful::NormalizeRot(&m_fRotValue);
	}

	if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		m_fRotValue -= D3DX_PI * 0.125f;
		useful::NormalizeRot(&m_fRotValue);
	}

	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{
		MoveWidth += 3.0f;
	}

	if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{
		MoveWidth -= 3.0f;
	}

	//�����Z�b�g
	if (pInputKeyboard->GetTrigger(DIK_N) == true)
	{
		m_fRotValue = D3DX_PI * 0.5f;
	}

	if (pInputKeyboard->GetTrigger(DIK_M) == true)
	{
		MoveWidth = D3DX_PI * 0.5f;
	}
}

//====================================================================
//�u���b�N�̓����蔻�菈��
//====================================================================
void CEdit::CollisionBlock(void)
{
	if (m_pEditBlock != nullptr)
	{
		// �}�b�v���f���̃��X�g�\����������Δ�����
		if (CCubeBlock::GetList() == nullptr) { return; }
		std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // ���X�g���擾

		// �}�b�v���f�����X�g�̒��g���m�F����
		for (CCubeBlock* pCubeModel : list)
		{
			D3DXVECTOR3 MyPos = m_pEditBlock->GetPos();
			D3DXVECTOR3 MySize = m_pEditBlock->GetSize() * 2.0f;
			D3DXVECTOR3 BlockPos = pCubeModel->GetPos();
			D3DXVECTOR3 BlockSize = pCubeModel->GetSize();
			D3DXVECTOR3 BlockMove = pCubeModel->GetMove();

			if (BlockPos.x + BlockSize.x > MyPos.x &&
				BlockPos.x - BlockSize.x < MyPos.x &&
				BlockPos.z + BlockSize.z > MyPos.z &&
				BlockPos.z - BlockSize.z < MyPos.z &&
				BlockPos.y + BlockSize.y > MyPos.y &&
				BlockPos.y - BlockSize.y < MyPos.y)
			{
				pCubeModel->Uninit();
			}
		}
	}
}

//====================================================================
//���[���u���b�N�̓����蔻�菈��
//====================================================================
void CEdit::CollisionRailBlock(void)
{
	if (m_EditRailBlock != nullptr)
	{
		// �}�b�v���f���̃��X�g�\����������Δ�����
		if (CRailBlock::GetList() == nullptr) { return; }
		std::list<CRailBlock*> list = CRailBlock::GetList()->GetList();    // ���X�g���擾

		// �}�b�v���f�����X�g�̒��g���m�F����
		for (CRailBlock* pRailBlock : list)
		{
			D3DXVECTOR3 MyPos = m_EditRailBlock->GetPos();
			D3DXVECTOR3 MySize = m_EditRailBlock->GetSize() * 2.0f;
			D3DXVECTOR3 BlockPos = pRailBlock->GetPos();
			D3DXVECTOR3 BlockSize = pRailBlock->GetSize();
			D3DXVECTOR3 BlockMove = pRailBlock->GetMove();

			if (BlockPos.x + BlockSize.x > MyPos.x &&
				BlockPos.x - BlockSize.x < MyPos.x &&
				BlockPos.z + BlockSize.z > MyPos.z &&
				BlockPos.z - BlockSize.z < MyPos.z &&
				BlockPos.y + BlockSize.y > MyPos.y &&
				BlockPos.y - BlockSize.y < MyPos.y &&
				m_EditRailBlock != pRailBlock)
			{
				pRailBlock->Uninit();
			}
		}
	}
}

//====================================================================
//�u���b�N�̓����蔻�菈��
//====================================================================
void CEdit::CollisionXModel(void)
{
	if (m_pEditModel != nullptr)
	{
		// �}�b�v���f���̃��X�g�\����������Δ�����
		if (CMapModel::GetList() == nullptr) { return; }
		std::list<CMapModel*> list = CMapModel::GetList()->GetList();    // ���X�g���擾

		// �}�b�v���f�����X�g�̒��g���m�F����
		for (CMapModel* pMapModel : list)
		{
			D3DXVECTOR3 MyPos = m_pEditModel->GetPos();
			D3DXVECTOR3 MySize = m_pEditModel->GetSize();
			D3DXVECTOR3 BlockPos = pMapModel->GetPos();
			D3DXVECTOR3 BlockSize = pMapModel->GetSize();

			if (MyPos.x + MySize.x > BlockPos.x &&
				MyPos.x - MySize.x < BlockPos.x &&
				MyPos.z + MySize.z > BlockPos.z &&
				MyPos.z - MySize.z < BlockPos.z &&
				MyPos.y + MySize.y > BlockPos.y &&
				MyPos.y - MySize.y < BlockPos.y)
			{// �폜
				pMapModel->Uninit();
			}
		}
	}
}

//====================================================================
//�����蔻��̉���
//====================================================================
void CEdit::AppearCollision(void)
{
	if (m_pEditModel != nullptr)
	{
		// �}�b�v���f���̃��X�g�\����������Δ�����
		if (CMapModel::GetList() == nullptr) { return; }
		std::list<CMapModel*> list = CMapModel::GetList()->GetList();    // ���X�g���擾

		// �}�b�v���f�����X�g�̒��g���m�F����
		for (CMapModel* pMapModel : list)
		{
			if (m_bAppearCollision == true)
			{//�������I���̎�
				//�����u���b�N�̐���
				pMapModel->CreateBlock();
			}
			else
			{//�������I�t�̎�
				//�����u���b�N�̍폜
				if (pMapModel->GetDebugBlock() != nullptr)
				{
					pMapModel->GetDebugBlock()->Uninit();
					pMapModel->SetDebugBlock(nullptr);
				}
			}
		}
	}
}

//====================================================================
//�ۑ�����
//====================================================================
void CEdit::SaveData(void)
{
	SaveBlock();
	SaveRailBlock();
	SaveXModel();
}

//====================================================================
//�u���b�N�̕ۑ�����
//====================================================================
void CEdit::SaveBlock(void)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen(DATA_BLOCK_NAME, "w");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		// �X�e�[�W���Z�[�u����J�n�̍��}
		fprintf(pFile, "%s\n\n", "STARTSETSTAGE");

		// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
		if (CCubeBlock::GetList() == nullptr) { return; }
		std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // ���X�g���擾

		// �L���[�u�u���b�N���X�g�̒��g���m�F����
		for (CCubeBlock* pCubeBlock : list)
		{
			fprintf(pFile, "%s\n", "STARTSETBLOCK");

			//�X�e�[�W���Z�[�u�����I���̍��}
			fprintf(pFile, "WIGHT %d\n", pCubeBlock->GetWightNumber());
			fprintf(pFile, "HEIGHT %d\n", pCubeBlock->GetHeightNumber());
			fprintf(pFile, "TEXTURE %s\n", pCubeBlock->GetTextureName());

			fprintf(pFile, "%s\n\n", "ENDSETBLOCK");
		}

		//�X�e�[�W���Z�[�u�����I���̍��}
		fprintf(pFile, "%s", "ENDSETSTAGE");

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***\n");
	}
}

//====================================================================
//���[���u���b�N�̕ۑ�����
//====================================================================
void CEdit::SaveRailBlock(void)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen(DATA_RAILBLOCK_NAME, "w");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		// �X�e�[�W���Z�[�u����J�n�̍��}
		fprintf(pFile, "%s\n\n", "STARTSETSTAGE");

		// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
		if (CRailBlock::GetList() == nullptr) { return; }
		std::list<CRailBlock*> list = CRailBlock::GetList()->GetList();    // ���X�g���擾

		// �L���[�u�u���b�N���X�g�̒��g���m�F����
		for (CRailBlock* pRailBlock : list)
		{
			if (pRailBlock != m_EditRailBlock)
			{
				fprintf(pFile, "%s\n", "STARTSETRAILBLOCK");

				//�X�e�[�W���Z�[�u�����I���̍��}
				fprintf(pFile, "WIGHT %d\n", pRailBlock->GetWightNumber());
				fprintf(pFile, "HEIGHT %d\n", pRailBlock->GetHeightNumber());
				fprintf(pFile, "MAX %d\n", pRailBlock->GetRailMax());
				fprintf(pFile, "MOVE ");

				for (int nCnt = 0; nCnt < pRailBlock->GetRailMax(); nCnt++)
				{
					fprintf(pFile, "%d ", pRailBlock->GetRailMove(nCnt));
				}
				fprintf(pFile, "\n");

				fprintf(pFile, "%s\n\n", "ENDSETRAILBLOCK");
			}
		}

		//�X�e�[�W���Z�[�u�����I���̍��}
		fprintf(pFile, "%s", "ENDSETSTAGE");

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***\n");
	}
}

//====================================================================
//X���f���̕ۑ�����
//====================================================================
void CEdit::SaveXModel(void)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen(DATA_XMODEL_NAME, "w");
	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

	// �}�b�v���f���̃��X�g�\����������Δ�����
		if (CMapModel::GetList() == nullptr) { return; }
		std::list<CMapModel*> list = CMapModel::GetList()->GetList();    // ���X�g���擾

		// �}�b�v���f�����X�g�̒��g���m�F����
		for (CMapModel* pMapModel : list)
		{
			fprintf(pFile, "%s\n", "STARTSETXMODEL");

			//�X�e�[�W���Z�[�u�����I���̍��}
			fprintf(pFile, "NAME %s\n", m_aModelInfo[pMapModel->GetEditIdx()].pFilename);
			fprintf(pFile, "NUMBER %d\n", pMapModel->GetEditIdx());
			fprintf(pFile, "POS %f %f %f\n", pMapModel->GetPos().x, pMapModel->GetPos().y, pMapModel->GetPos().z);
			fprintf(pFile, "ROT %f %f %f\n", pMapModel->GetRot().x, pMapModel->GetRot().y, pMapModel->GetRot().z);
			if (pMapModel->GetCollision() == true)
			{
				fprintf(pFile, "COLLISION 1\n");
			}
			else
			{
				fprintf(pFile, "COLLISION 0\n");
			}

			fprintf(pFile, "%s\n\n", "ENDSETXMODEL");

		}

		//�X�e�[�W���Z�[�u�����I���̍��}
		fprintf(pFile, "%s", "ENDSETSTAGE");

		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***\n");
	}
}