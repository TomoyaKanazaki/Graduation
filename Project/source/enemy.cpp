//============================================
//
//	�G�̏��� [enemy.cpp]
//	Author:sakamoto kai
//
//============================================
#include "enemy.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "debugproc.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "useful.h"
#include "CubeBlock.h"
#include "CubeColl.h"
#include "LevelModelEffect.h"
#include "slowManager.h"
#include "2DEffect.h"
#include "score.h"
#include "modelEffect.h"
#include "sound.h"

#define COLLISION_SIZE (D3DXVECTOR3(5.0f,60.0f,5.0f))		//���̓����蔻��
#define LIFE (30.0f)	// �̗�

namespace
{
	const float DISTANCE_RECEDE = 200.0f;	//�߂Â�����
	const float DISTANCE_APPROACH = 100.0f;	//�������鋗��
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CEnemy::CEnemy(int nPriority) :CObject(nPriority)
{
	m_pos = INITVECTOR3;
	m_posOld = INITVECTOR3;

	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_AtkPos = INITVECTOR3;

	m_nSeatchCount = 0;
	m_bSeatchLoop = false;

	m_nActionCount = 0;
	m_nAttackHit = false;

	SetSize(COLLISION_SIZE);

	m_State = STATE_SEATCH;
	m_nStateCount = 0;
	m_bJump = false;
	m_bDeath = false;
	m_bAttack = false;

	m_fLife = LIFE;

	m_CollisionRot = 0.0f;

	m_ColorA = 1.0f;

	for (int nCnt = 0; nCnt < SCREEN_POS_NUM; nCnt++)
	{
		m_ScreenPos[nCnt] = D3DXVECTOR3(-100.0f, -100.0f, 0.0f);
	}

	for (int nCnt = 0; nCnt < SEARCH_POS_NUM; nCnt++)
	{
		m_SeatchPos[nCnt] = D3DXVECTOR3(0.0f, -10000.0f, 0.0f);
	}

	for (int nCnt = 0; nCnt < MODEL_NUM; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
		m_aModelName[nCnt] = "";
	}

	m_size = COLLISION_SIZE;
	m_SearchDistance = 0.0f;

	m_pMotion = nullptr;
	m_nNumModel = 0;
	m_EnemyType = ENEMY_ASHIGARU;

	m_pSlow = nullptr;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CEnemy::~CEnemy()
{

}

//====================================================================
//��������
//====================================================================
CEnemy* CEnemy::Create(char* aModelName)
{
	// ����
	CEnemy* pInstance = new CEnemy();

	//����������
	if (FAILED(pInstance->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}
	
	// ���f���֘A����������
	pInstance->InitModel(aModelName);

	return pInstance;
}

//====================================================================
//����������
//====================================================================
HRESULT CEnemy::Init(void)
{
	//��ސݒ�
	SetType(CObject::TYPE_ENEMY3D);

	// �X���[�̐���(�z���A�^�O�̐ݒ�)
	m_pSlow = CSlowManager::Create(m_pSlow->CAMP_ENEMY, m_pSlow->TAG_ENEMY);

	return S_OK;
}

//====================================================================
//�������ێ�����I�u�W�F�N�g�̐���
//====================================================================
void CEnemy::MyObjCreate(void)
{

}

//====================================================================
//�I������
//====================================================================
void CEnemy::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Uninit();
		delete m_apModel[nCntModel];
		m_apModel[nCntModel] = NULL;
	}

	//���[�V�����̏I������
	if (m_pMotion != NULL)
	{
		//���[�V�����̔j��
		delete m_pMotion;
		m_pMotion = NULL;
	}

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CEnemy::Update(void)
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
void CEnemy::TitleUpdate(void)
{
	//���[�V�����̍X�V
	m_pMotion->Update();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CEnemy::GameUpdate(void)
{
	//�f�o�b�O�\���̎擾
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	//�X�N���[�����W�ւ̕ϊ�����
	ScreenCollision();

	// �ߋ��̈ʒu���L�^
	m_posOld = m_pos;

	// �ʒu�X�V����
	UpdatePos();

	if (m_bDeath == false)
	{
		UpdateState();
	}

	if (m_pos.y <= 0.0f)
	{
		m_bJump = false;
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}

	if (m_bDeath == true)
	{
		// ���S����
		Death();
	}

	if (m_pMotion != nullptr)
	{
		//���[�V�����̍X�V
		m_pMotion->Update();
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CEnemy::Draw(void)
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

	if (m_bDeath == true)
	{
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			m_apModel[nCntModel]->SetColorType(CModel::COLORTYPE_TRUE_A);
			m_apModel[nCntModel]->SetColorA(m_ColorA);
		}
	}
	else
	{
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			m_apModel[nCntModel]->SetColorType(CModel::COLORTYPE_FALSE);
		}
	}

	//���f���̕`��(�S�p�[�c)
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}
}

//====================================================================
// �ڐG�_���[�W����
//====================================================================
void CEnemy::HitDamage(float fDamage)
{
	m_fLife -= fDamage;

	if (m_State == STATE_SEATCH)
	{
		m_State = STATE_BATTLE;
	}

	if (m_fLife <= 0.0f && m_bDeath == false)
	{
		m_fLife = 0.0f;
		m_bAttack = false;

		if (m_State == STATE_BATTLE)
		{
			m_bJump = true;
			m_move.y = 8.0f;
		}

		switch (m_EnemyType)
		{
		case CEnemy::ENEMY_ASHIGARU:
			CGame::GetScore()->AddScore(500);
			break;

		case CEnemy::ENEMY_BOWMAN:
			CGame::GetScore()->AddScore(800);
			break;

		case CEnemy::ENEMY_SAMURAI:
			CGame::GetScore()->AddScore(1200);
			break;

		default:
			break;
		}

		m_bDeath = true;
	}
}

//====================================================================
// ���f���֘A�̏���������
//====================================================================
void CEnemy::InitModel(const char* pFilename)
{
	strcpy(&m_cFileName[0], pFilename);

	//���f���̐���
	LoadLevelData(pFilename);

	//���[�V�����̐���
	if (m_pMotion == NULL)
	{
		//���[�V�����̐���
		m_pMotion = new CMotion;
	}

	//����������
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData(pFilename);
}

//====================================================================
// �ʒu�X�V����
//====================================================================
void CEnemy::UpdatePos(void)
{
	//�d��
	m_move.y -= 0.5f;

	//���S���̐������
	if(m_bDeath == true && m_bJump == true)
	{
		m_move.x = sinf(m_rot.y + D3DX_PI * 0.5f) * 10.0f;
		m_move.z = cosf(m_rot.y + D3DX_PI * 0.5f) * 10.0f;
	}

	//�����W��
	m_move.x = m_move.x * 0.5f;
	if (m_move.x <= 0.0001f && m_move.x >= -0.0001f)
	{
		m_move.x = 0.0f;
	}

	m_move.z = m_move.z * 0.5f;
	if (m_move.z <= 0.0001f && m_move.z >= -0.0001f)
	{
		m_move.z = 0.0f;
	}

	// �ϐ��錾
	float fSpeed = 1.0f;	// �X���[�p default1.0f�ŏ�����
	if (m_pSlow)
	{
		fSpeed = m_pSlow->GetValue();

		if (m_pMotion)
		{
			m_pMotion->SetSlowVaule(fSpeed);
		}
	}

	//Y���̈ʒu�X�V
	m_pos.y += m_move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_Y);

	//X���̈ʒu�X�V
	m_pos.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_X);

	//Z���̈ʒu�X�V
	m_pos.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_Z);
}

//====================================================================
// �ǂƂ̓����蔻��
//====================================================================
void CEnemy::CollisionWall(useful::COLLISION XYZ)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == TYPE_CUBEBLOCK)
			{//��ނ��u���b�N�̎�

				CCubeBlock* pBlock = (CCubeBlock*)pObj;	// �u���b�N���̎擾

				D3DXVECTOR3 pos = pBlock->GetPos();
				D3DXVECTOR3 posOld = pBlock->GetPosOld();
				D3DXVECTOR3 Move = pBlock->GetMove();
				D3DXVECTOR3 Size = pBlock->GetSize();

				// ��`�̓����蔻��
				if (useful::CollisionBlock(pos, posOld, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
				{

				}
			}

			if (type == TYPE_CUBECOLL)
			{//��ނ��u���b�N�̎�

				CCubeColl* pBlock = (CCubeColl*)pObj;	// �u���b�N���̎擾

				D3DXVECTOR3 pos = pBlock->GetPos();
				D3DXVECTOR3 posOld = pBlock->GetPosOld();
				D3DXVECTOR3 Move = pBlock->GetMove();
				D3DXVECTOR3 Size = pBlock->GetSize();

				// ��`�̓����蔻��
				if (useful::CollisionBlock(pos, posOld, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
				{

				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//�v���C���[���G����
//====================================================================
void CEnemy::SearchPlayer(void)
{
	bool SetBattle = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == TYPE_PLAYER3D)
			{//��ނ��G�̎�
				CPlayer* pPlayer = (CPlayer*)pObj;

				if (useful::CollisionCircle(m_pos, pPlayer->GetPos(), m_SearchDistance) == true)
				{
					if (useful::CollisionLine(m_pos, pPlayer->GetPos(), m_rot.y - D3DX_PI * 0.25f) == true &&
						useful::CollisionLine(m_pos, pPlayer->GetPos(), m_rot.y + D3DX_PI * 0.25f) == false)
					{
						SetBattle = true;

						CManager::GetInstance()->GetDebugProc()->Print("���G�͈͓��I\n");

						if (SortObject(pPlayer->GetPos()) == true)
						{
							SetBattle = false;

							CManager::GetInstance()->GetDebugProc()->Print("�Ԃɕǂ�����I\n");
						}
					}
				}
			}

			pObj = pObjNext;
		}
	}

	if (SetBattle == true)
	{
		// �퓬���
		m_State = STATE_BATTLE;

		// ����̉�
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BATTLE_ENTRY);

	}
}

//====================================================================
//�}�b�v�I�u�W�F�N�g�ƕǂƂ̃\�[�g����
//====================================================================
bool CEnemy::SortObject(D3DXVECTOR3 pos)
{
	float fDistance = sqrtf((m_pos.x - pos.x) * (m_pos.x - pos.x) + (m_pos.z - pos.z) * (m_pos.z - pos.z));

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == TYPE_CUBEBLOCK)
			{//��ނ��u���b�N�̎�
				CCubeBlock* pBlock = (CCubeBlock*)pObj;

				if (pBlock->GetPos().y <= 200.0f)
				{
					if (useful::CollisionCircle(m_pos, pBlock->GetPos(), m_SearchDistance) == true)
					{
						if (useful::CollisionLine(m_pos, pBlock->GetPos(), m_rot.y - D3DX_PI * 0.50f) == true &&
							useful::CollisionLine(m_pos, pBlock->GetPos(), m_rot.y + D3DX_PI * 0.50f) == false)
						{
							D3DXVECTOR3 CrossPos = INITVECTOR3;
							float ObjDistance = 10000.0f;

							//��_�̈ʒu�����߂�
							for (int nCnt = 0; nCnt < 4; nCnt++)
							{
								switch (nCnt)
								{
								case 0:
									CrossPos = useful::CrossIntersection(m_pos, pos,
										D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
										D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
										m_SearchDistance);
									break;

								case 1:
									CrossPos = useful::CrossIntersection(m_pos, pos,
										D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
										D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
										m_SearchDistance);
									break;

								case 2:
									CrossPos = useful::CrossIntersection(m_pos, pos,
										D3DXVECTOR3(pBlock->GetPos().x - pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
										D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
										m_SearchDistance);
									break;

								case 3:
									CrossPos = useful::CrossIntersection(m_pos, pos,
										D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z - pBlock->GetSize().z),
										D3DXVECTOR3(pBlock->GetPos().x + pBlock->GetSize().x, pBlock->GetPos().y, pBlock->GetPos().z + pBlock->GetSize().z),
										m_SearchDistance);
									break;
								}

								ObjDistance = sqrtf((m_pos.x - CrossPos.x) * (m_pos.x - CrossPos.x) + (m_pos.z - CrossPos.z) * (m_pos.z - CrossPos.z));

								//CManager::GetInstance()->GetDebugProc()->Print("[�G�F%f %f] [�v�F%f %f] [�ǁF%f %f] \n", m_pos.x, m_pos.z, pos.x, pos.z, CrossPos.x, CrossPos.z);
								//CManager::GetInstance()->GetDebugProc()->Print("%f   :   %f\n", fDistance, ObjDistance);

								if (ObjDistance < fDistance)
								{
									return true;
								}
							}
						}
					}
				}				
			}

			pObj = pObjNext;
		}
	}
	return false;
}

//====================================================================
//�v���C���[�Ɗԍ�������鏈��
//====================================================================
bool CEnemy::DistancePlayer(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();

	switch (CScene::GetMode())
	{
	case CScene::MODE_GAME:

		pPlayer = CGame::GetPlayer();

		break;
	case CScene::MODE_TUTORIAL:

		pPlayer = CTutorial::GetPlayer();

		break;
	}

	bool bOK = false;

	m_rot.y = useful::PosDirection(m_pos, pPlayer->GetPos(), 0.0f);

	if (useful::CollisionCircle(m_pos, pPlayer->GetPos(), DISTANCE_RECEDE) == false)
	{
		m_move.x = sinf(D3DX_PI * -0.5f + m_rot.y) * 6.0f;
		m_move.z = cosf(D3DX_PI * -0.5f + m_rot.y) * 6.0f;
	}
	else if (useful::CollisionCircle(m_pos, pPlayer->GetPos(), DISTANCE_APPROACH) == true)
	{
		m_move.x = -sinf(D3DX_PI * -0.5f + m_rot.y) * 6.0f;
		m_move.z = -cosf(D3DX_PI * -0.5f + m_rot.y) * 6.0f;
	}
	else
	{
		bOK = true;
	}
	return bOK;
}

//====================================================================
//�����ȊO�̓G�������o������
//====================================================================
void CEnemy::CollisionPush(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾
			float fAngle = 0.0f;

			if (type == TYPE_ENEMY3D && pObj != this)
			{//��ނ��u���b�N�̎�
				CEnemy* pEnemy = (CEnemy*)pObj;

				if (useful::CollisionCircle(m_pos, pEnemy->GetPos(), 30.0f) == true)
				{
					fAngle = atan2f(pEnemy->GetPos().z - m_pos.z, m_pos.x - pEnemy->GetPos().x) + D3DX_PI * 0.5f;
					m_move.x = sinf(fAngle) * 0.2f;
					m_move.z = cosf(fAngle) * 0.2f;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//�X�N���[�����W�ɕϊ����鏈��
//====================================================================
void CEnemy::ScreenCollision()
{
	//���_�ݒ�
	D3DXVECTOR3 pos[SCREEN_POS_NUM] = { INITVECTOR3 };

	pos[0] = D3DXVECTOR3(GetPos().x - m_size.x, GetPos().y, GetPos().z + m_size.z);
	pos[1] = D3DXVECTOR3(GetPos().x - m_size.x, GetPos().y + m_size.y, GetPos().z + m_size.z);
	pos[2] = D3DXVECTOR3(GetPos().x + m_size.x, GetPos().y, GetPos().z + m_size.z);
	pos[3] = D3DXVECTOR3(GetPos().x + m_size.x, GetPos().y + m_size.y, GetPos().z + m_size.z);
	pos[4] = D3DXVECTOR3(GetPos().x - m_size.x, GetPos().y, GetPos().z - m_size.z);
	pos[5] = D3DXVECTOR3(GetPos().x - m_size.x, GetPos().y + m_size.y, GetPos().z - m_size.z);
	pos[6] = D3DXVECTOR3(GetPos().x + m_size.x, GetPos().y, GetPos().z - m_size.z);
	pos[7] = D3DXVECTOR3(GetPos().x + m_size.x, GetPos().y + m_size.y, GetPos().z - m_size.z);

	//�r���[�|�[�g�̐ݒ�
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;

	//�}�g���b�N�X�̎擾
	D3DXMATRIX ViewMatrix = CManager::GetInstance()->GetCamera()->GetViewMatrix();
	D3DXMATRIX ProjectionMatrix = CManager::GetInstance()->GetCamera()->GetProjectionMatrix();

	//D3DXVec3Project(&screenPosition, &pos, &viewport, &projectionMatrix, &viewMatrix, D3DXMatrixIdentity(&mtx));

	//3D�̍��W���X�N���[�����W�ɕϊ�
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		D3DXVec3Project(&m_ScreenPos[nCnt], &pos[nCnt], &viewport, &ProjectionMatrix, &ViewMatrix, D3DXMatrixIdentity(&m_mtxWorld));

		//C2DEffect* pEffect = C2DEffect::Create();
		//pEffect->SetPos(m_ScreenPos[nCnt]);
	}
}

//====================================================================
// ���S����
//====================================================================
void CEnemy::Death(void)
{
	if (m_pMotion->GetFinish() == true)
	{
		m_ColorA -= 0.005f;

		if (m_ColorA <= 0.0f)
		{
			Uninit();
		}
	}
}

//====================================================================
// ��ԍX�V
//====================================================================
void CEnemy::UpdateState()
{
	//��Ԃ̊Ǘ�
	switch (m_State)
	{
	case CEnemy::STATE_BATTLE:

		//�퓬��Ԃ̊Ǘ�
		BattleStateManager();

		break;

	case CEnemy::STATE_SEATCH:

		//�T����Ԃ̊Ǘ�
		SearchStateManager();

		//�v���C���[�̍��G
		SearchPlayer();

		break;

	default:
		break;
	}
}

//====================================================================
// ����̎c���G�t�F�N�g�𐶐�����
//====================================================================
void CEnemy::CreateWeaponEffect(int ModelNumber)
{
	CModelEffect *pEffect = CModelEffect::Create(m_apModel[ModelNumber]->GetModelName());
	pEffect->SetMtxWorld(m_apModel[ModelNumber]->GetMtxWorld());
	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
}

//====================================================================
//���[�h����
//====================================================================
void CEnemy::LoadLevelData(const char* pFilename)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen(pFilename, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		int ModelParent = 0;
		D3DXVECTOR3 ModelPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 ModelRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		char ModelName[128] = {};
		int nCntModel = 0;
		int nCntParts = 0;
		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		char aString[128] = {};				//�S�~��
		char aMessage[128] = {};			//�X�^�[�g�ƃG���h�̃��b�Z�[�W
		char aBool[128] = {};				//bool�ϊ��p���b�Z�[�W

		// �ǂݍ��݊J�n-----------------------------------------------------
		while (1)
		{//�uSCRIPT�v��T��
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// ���f�����ǂݍ���-----------------------------------------------------
				while (1)
				{//�uNUM_MODEL�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%d", &m_nNumModel);		//���f�����̐ݒ�
						break;
					}
				}

				//���f���t�@�C���̓ǂݍ���
				while (1)
				{//�uMODEL_FILENAME�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%s", &ModelName[0]);		//�ǂݍ��ރ��f���̃p�X���擾

						m_apModel[nCntModel] = CModel::Create(&ModelName[0]);
						m_apModel[nCntModel]->SetColorType(CModel::COLORTYPE_FALSE);
						nCntModel++;
					}
					if (nCntModel >= m_nNumModel)
					{
						nCntModel = 0;
						break;
					}
				}

				// �L�����N�^�[���ǂݍ���-----------------------------------------------------
				while (1)
				{//�uPARTSSET�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "PARTSSET") == 0)
					{
						while (1)
						{//�e��ϐ���T��
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "INDEX") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &nCntModel);	//�C���f�b�N�X��ݒ�
							}
							if (strcmp(&aMessage[0], "PARENT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParent);	//�e���f���̃C���f�b�N�X��ݒ�

								if (ModelParent == -1)
								{
									m_apModel[nCntModel]->SetParent(NULL);
								}
								else
								{
									m_apModel[nCntModel]->SetParent(m_apModel[ModelParent]);
								}
							}
							if (strcmp(&aMessage[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelPos.x);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�
								fscanf(pFile, "%f", &ModelPos.y);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�
								fscanf(pFile, "%f", &ModelPos.z);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�

								m_apModel[nCntModel]->SetPos(ModelPos);
								m_apModel[nCntModel]->SetStartPos(ModelPos);
							}
							if (strcmp(&aMessage[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelRot.x);				////�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.y);				////�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.z);				////�����̏����ݒ�

								m_apModel[nCntModel]->SetRot(ModelRot);
								m_apModel[nCntModel]->SetStartRot(ModelRot);
							}
							if (strcmp(&aMessage[0], "END_PARTSSET") == 0)
							{
								break;
							}
						}
						nCntModel++;
						if (nCntModel >= m_nNumModel)
						{
							break;
						}
					}
				}
			}
			if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***\n");
	}
}

