//============================================
//
//	�}�b�v���f�� [MapModel.cpp]
//	Author:sakamoto kai
//
//============================================
#include "MapModel.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "CubeEffect.h"
#include "useful.h"
#include "camera.h"
#include "2DEffect.h"

//�}�N����`
namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(100.0f, 100.0f, 100.0f);
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CMapModel::CMapModel(int nPriority) : CObjectX(nPriority)
{
	SetPos(INITVECTOR3);
	m_CollisionPos = INITVECTOR3;
	m_Spin = INITVECTOR3;
	m_bCollision = true;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_pDebugBlock = nullptr;		//�f�o�b�O�p�u���b�N
	collisionSize = INITVECTOR3;
	m_bBreakable = false;
	m_bDebris = false;
	m_bBreakEffect = false;
	m_bBreakBound = 0;

	for (int i = 0; i < mapmodel_const::SCREENPOS_NUM; i++)
	{
		m_ScreenPos[i] = INITVECTOR3;
	}
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CMapModel::~CMapModel()
{

}

//====================================================================
//��������
//====================================================================
CMapModel* CMapModel::Create(char* pModelName)
{
	CMapModel* pSample = NULL;

	if (pSample == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample = new CMapModel();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample->Init(pModelName)))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pSample;
}

//====================================================================
//����������
//====================================================================
HRESULT CMapModel::Init(char* pModelName)
{
	SetType(CObject::TYPE_MAPMODEL);

	CObjectX::Init(pModelName);

	SetCollisionSize(GetSize());

	//���[�h���Ƃɏ����l��ݒ�o����
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
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
//�I������
//====================================================================
void CMapModel::Uninit(void)
{
	if (m_pDebugBlock != nullptr)
	{
		m_pDebugBlock->Uninit();
		m_pDebugBlock = nullptr;
	}

	CObjectX::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CMapModel::Update(void)
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
void CMapModel::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	//�ʒu�X�V
	pos += m_move;

	SetPos(pos);

	//���_���̍X�V
	CObjectX::Update();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CMapModel::GameUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	//�X�V�O�̈ʒu���ߋ��̈ʒu�Ƃ���
	SetPosOld(pos);

	if (m_bBreakEffect == true)
	{
		Break(&pos);
	}

	pos += m_move;

	//�ʒu�X�V
	CObjectX::SetPos(pos);

	//�X�N���[�����W�ւ̕ϊ�����
	ScreenCollision();

	//�傫���̐ݒ�
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//��ԊǗ�
	StateManager();

	//���_���̍X�V
	CObjectX::Update();

	//3D�̍��W���X�N���[�����W�ɕϊ�
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		useful::Vec3Project(m_ScreenPos, GetPos());
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CMapModel::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//��ԊǗ�
//====================================================================
void CMapModel::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ACTION:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//�u���b�N�̐���
//====================================================================
void CMapModel::CreateBlock(void)
{
	if (m_pDebugBlock == nullptr)
	{
		m_pDebugBlock = CCubeEffect::Create();
		m_pDebugBlock->SetPos(GetPos());
		m_pDebugBlock->SetSize(GetSize());

		if (m_bCollision == true)
		{
			m_pDebugBlock->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
		}
		else
		{
			m_pDebugBlock->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f));
		}
	}
}

//====================================================================
//�u���b�N�̍X�V
//====================================================================
void CMapModel::UpdateBlock(void)
{
	if (m_pDebugBlock != nullptr)
	{
		m_pDebugBlock->SetPos(GetPos());
		m_pDebugBlock->SetSize(GetSize());

		if (m_bCollision == true)
		{
			m_pDebugBlock->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
		}
		else
		{
			m_pDebugBlock->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f));
		}
	}
}

//====================================================================
// �󂳂ꂽ
//====================================================================
void CMapModel::Break(D3DXVECTOR3* pos)
{
	D3DXVECTOR3 Rot = GetRot();

	Rot += m_Spin;

	if (Rot.x >= D3DX_PI * 0.5f)
	{
		Rot.x = D3DX_PI * 0.5f;
		m_Spin = INITVECTOR3;
	}
	else if(Rot.x <= D3DX_PI * -0.5f)
	{
		Rot.x = D3DX_PI * -0.5f;
		m_Spin = INITVECTOR3;
	}

	SetRot(Rot);

	if (m_bBreakBound <= 3)
	{
		m_move.x = m_move.x * 0.9f;
		m_move.z = m_move.z * 0.9f;

		m_move.y -= 0.98f;
	}
	else
	{
		pos->y = 1.0f;
		m_move = INITVECTOR3;
	}

	if (pos->y < 0.0f)
	{
		if (m_bBreakBound <= 3)
		{
			m_bBreakBound++;
			m_move.y *= -0.62f;
		}
	}
}

//====================================================================
//�X�N���[�����W�ɕϊ����鏈��
//====================================================================
void CMapModel::ScreenCollision()
{
	//���_�ݒ�
	D3DXVECTOR3 pos[mapmodel_const::SCREENPOS_NUM] = { INITVECTOR3 };

	pos[0] = D3DXVECTOR3(GetPos().x - collisionSize.x, GetPos().y, GetPos().z + collisionSize.z);
	pos[1] = D3DXVECTOR3(GetPos().x - collisionSize.x, GetPos().y + collisionSize.y, GetPos().z + collisionSize.z);
	pos[2] = D3DXVECTOR3(GetPos().x + collisionSize.x, GetPos().y, GetPos().z + collisionSize.z);
	pos[3] = D3DXVECTOR3(GetPos().x + collisionSize.x, GetPos().y + collisionSize.y, GetPos().z + collisionSize.z);
	pos[4] = D3DXVECTOR3(GetPos().x - collisionSize.x, GetPos().y, GetPos().z - collisionSize.z);
	pos[5] = D3DXVECTOR3(GetPos().x - collisionSize.x, GetPos().y + collisionSize.y, GetPos().z - collisionSize.z);
	pos[6] = D3DXVECTOR3(GetPos().x + collisionSize.x, GetPos().y, GetPos().z - collisionSize.z);
	pos[7] = D3DXVECTOR3(GetPos().x + collisionSize.x, GetPos().y + collisionSize.y, GetPos().z - collisionSize.z);

	//�r���[�|�[�g�̐ݒ�
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 0.0f;
	viewport.MinZ = 0.0f;

	//�}�g���b�N�X�̎擾
	D3DXMATRIX ViewMatrix = CManager::GetInstance()->GetCamera()->GetViewMatrix();
	D3DXMATRIX ProjectionMatrix = CManager::GetInstance()->GetCamera()->GetProjectionMatrix();

	//D3DXVec3Project(&screenPosition, &pos, &viewport, &projectionMatrix, &viewMatrix, D3DXMatrixIdentity(&mtx));

	//3D�̍��W���X�N���[�����W�ɕϊ�
	for (int nCnt = 0; nCnt < mapmodel_const::SCREENPOS_NUM; nCnt++)
	{
		D3DXVec3Project(&m_ScreenPos[nCnt], &pos[nCnt], &viewport, &ProjectionMatrix, &ViewMatrix, D3DXMatrixIdentity(&m_mtxWorld));

		//C2DEffect* pEffect = C2DEffect::Create();
		//pEffect->SetPos(m_ScreenPos[nCnt]);
	}
}
