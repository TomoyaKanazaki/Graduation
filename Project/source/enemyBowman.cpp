//============================================
//
//	�|���̏��� [enemyBowman.cpp]
//	Author:��{�@�ėB
//
//============================================
#include "LevelModelEffect.h"
#include "enemyBowman.h"
#include "BowmanArrow.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "object3D.h"
#include "debugproc.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "useful.h"
#include "effect.h"
#include "sound.h"
#include "shrinkingBillboard.h"

namespace
{
	const float MOVE_SPEED(1.0f);
	const float BOWMAN_SEARCHDISTANCE(1200.0f);		// �|���̍��G����
	const int SHRINKBB_ACT_FRAME(80);			// ���k���o�����^�C�~���O
	const int SHRINKBB_SHRINK_FRAME(30);		// ���k�t���[��
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CEnemyBowman::CEnemyBowman(int nPriority) : CEnemy(nPriority)
{
	m_Action = ACTION_BWAIT;
	m_AtkAction = ACTION_BWAIT;

	m_SState = SEARCHSTATE(0);
	m_BState = BATTLESTATE(0);
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CEnemyBowman::~CEnemyBowman()
{

}

//====================================================================
//��������
//====================================================================
CEnemyBowman* CEnemyBowman::Create(const char* pFilename)
{
	// ����
	CEnemyBowman* pInstance = new CEnemyBowman();

	// ����������
	if (FAILED(pInstance->Init()))
	{//���������������s�����ꍇ
		return NULL;
	}

	// ���f���֘A�̏�����
	pInstance->InitModel(pFilename);

	return pInstance;
}

//====================================================================
//����������
//====================================================================
HRESULT CEnemyBowman::Init(void)
{
	// �p���N���X�̏���������
	CEnemy::Init();

	SetEnemyType(CEnemy::ENEMY_BOWMAN);		//�G�̎�ސݒ�
	SetSearchDistance(800.0f);				//���G�����ݒ�
	SetLife(10.0f);							//���C�t�̐ݒ�

	m_BState = CEnemyBowman::BATTLESTATE_WAIT;

	return S_OK;
}

//====================================================================
//�������ێ�����I�u�W�F�N�g�̐���
//====================================================================
void CEnemyBowman::MyObjCreate(void)
{

}

//====================================================================
//�I������
//====================================================================
void CEnemyBowman::Uninit(void)
{
	// �p���N���X�̏I������
	CEnemy::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CEnemyBowman::Update(void)
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
void CEnemyBowman::TitleUpdate(void)
{
	// �p���N���X�̃^�C�g���X�V����
	CEnemy::TitleUpdate();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CEnemyBowman::GameUpdate(void)
{
	// �p���N���X�̃Q�[���X�V����
	CEnemy::GameUpdate();

	//���[�V�����̊Ǘ�
	ActionState();
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CEnemyBowman::ActionState(void)
{
	// ���擾
	D3DXVECTOR3 move = GetMove();	// �ړ���

	switch (GetState())
	{
	case CEnemy::STATE_BATTLE:

		//���S���[�V����
		if (GetDeath() == true)
		{
			if (m_Action != ACTION_BDEATH)
			{
				m_Action = ACTION_BDEATH;
				GetMotion()->Set(ACTION_BDEATH, 10);
			}
		}
		//�U�����[�V����
		else if (m_BState == BATTLESTATE_ATTACK)
		{
			if (m_Action != ACTION_ATTACK)
			{
				m_Action = ACTION_ATTACK;
				GetMotion()->Set(ACTION_ATTACK, 0);
			}
		}
		//�_���[�W���[�V����
		else if (m_BState == BATTLESTATE_DAMAGE)
		{
			if (m_Action != ACTION_DAMAGE)
			{
				m_Action = ACTION_DAMAGE;
				GetMotion()->Set(ACTION_DAMAGE, 7);
			}

			if (GetMotion()->GetFinish() == true)
			{
				m_BState = BATTLESTATE_WAIT;
			}
		}
		//�ړ����[�V����
		else if (move.x > 0.1f || move.x < -0.1f || move.z > 0.1f || move.z < -0.1f)
		{
			if (m_Action != ACTION_BMOVE)
			{
				m_Action = ACTION_BMOVE;
				GetMotion()->Set(ACTION_BMOVE, 5);
			}
		}
		//�j���[�g�������[�V����
		else
		{
			if (m_Action != ACTION_BWAIT)
			{
				m_Action = ACTION_BWAIT;
				GetMotion()->Set(ACTION_BWAIT, 5);
			}
		}
		break;

	case CEnemy::STATE_SEATCH:

		//���S���[�V����
		if (GetDeath() == true)
		{
			if (m_Action != ACTION_SDEATH)
			{
				m_Action = ACTION_SDEATH;
				GetMotion()->Set(ACTION_SDEATH, 30);
			}
		}
		//�ړ����[�V����
		else if (move.x > 0.1f || move.x < -0.1f || move.z > 0.1f || move.z < -0.1f)
		{
			if (m_Action != ACTION_SMOVE)
			{
				m_Action = ACTION_SMOVE;
				GetMotion()->Set(ACTION_SMOVE, 5);
			}
		}
		//�j���[�g�������[�V����
		else
		{
			if (m_Action != ACTION_SWAIT)
			{
				m_Action = ACTION_SWAIT;
				GetMotion()->Set(ACTION_SWAIT, 5);
			}
		}

		break;
	default:
		break;
	}
}

//====================================================================
//�퓬��ԊǗ�
//====================================================================
void CEnemyBowman::BattleStateManager(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 fRot = GetRot();
	CModel** pModel;

	switch (m_BState)
	{
	case BATTLESTATE_NORMAL:
		break;

	case BATTLESTATE_DEATH:
		break;

	case BATTLESTATE_WAIT:

		// �G�����o������
		CEnemy::CollisionPush();

		//�G�̌����C��
		fRot.y = useful::PosDirection(GetPos(), pPlayer->GetPos(), 0.0f);
		SetRot(fRot);

		if (m_nStateCount <= 0)
		{
			m_BState = BATTLESTATE_ATTACK;
			m_nStateCount = 1000;
			SetAttack(true);
		}

		break;

	case BATTLESTATE_ATTACK:

		//�G�̌����C��
		fRot.y = useful::PosDirection(GetPos(), pPlayer->GetPos(), 0.0f);
		SetRot(fRot);

		pModel = GetLevelModel();
		pModel[17]->SetColor(D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f));
		pModel[17]->SetColorType(CModel::COLORTYPE_TRUE_ALL);

		if (m_nStateCount % 5 == 0)
		{
			CreateWeaponEffect(CEnemy::GetNumModel() - 1);
		}

		//��ɃG�t�F�N�g�\��
		if (GetMotion()->GetCounterCurrent() == SHRINKBB_ACT_FRAME)
		{
			CShrinkingBillboard* pEffect = CShrinkingBillboard::Create();
			D3DXVECTOR3 pos = GetPos();
			pEffect->SetPos(D3DXVECTOR3(pos.x, pos.y + 40.0f, pos.z));
			pEffect->SetRadius(10.0f, 100.0f);
			pEffect->SetFrameAll(SHRINKBB_SHRINK_FRAME);
		}

		if (GetMotion()->GetFinish() == true)
		{
			pModel[17]->SetColorType(CModel::COLORTYPE_FALSE);

			SetAttack(false);
			SetDamage(10.0f);
			m_BState = BATTLESTATE_WAIT;
			m_nStateCount = 120;

			// �|�̉�
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BOW);
		}
		break;

	case BATTLESTATE_DAMAGE:

		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//�T����ԊǗ�
//====================================================================
void CEnemyBowman::SearchStateManager(void)
{
	switch (m_SState)
	{
	case SEARCHSTATE_NORMAL:
		break;

	case SEARCHSTATE_WALK:

		// �ړI�n�ɕ���
		WalkPoint();

		break;

	case SEARCHSTATE_TRUN:

		// �ړI�n�����]����
		TurnPoint();

		break;

	case SEARCHSTATE_WAIT:
		break;

	case SEARCHSTATE_SEARCH:
		break;
	}
}

//====================================================================
//�_���[�W���菈��
//====================================================================
void CEnemyBowman::SetDamage(float Damage)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 AttackPos[3] = { INITVECTOR3 };

	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	float Length = sqrtf((PlayerPos.x - pos.x) * (PlayerPos.x - pos.x) + (PlayerPos.z - pos.z) * (PlayerPos.z - pos.z));
	float CrossLemgth = sqrtf((Length) * (Length) + (PlayerPos.y - 50.0f - pos.y) * (PlayerPos.y - 50.0f - pos.y));
	float HeightRot = 0.0f;

	if (PlayerPos.y < pos.y)
	{
		HeightRot = -(D3DX_PI / (CrossLemgth / Length));
	}
	else
	{
		HeightRot = (D3DX_PI / (CrossLemgth / Length));
	}

	//��𔭎�
	CBowmanArrow *pArrow = CBowmanArrow::Create("data\\MODEL\\weapon\\Arrow001.x");
	pArrow->SetPos(D3DXVECTOR3(pos.x, pos.y + 40.0f, pos.z));
	pArrow->SetRot(D3DXVECTOR3(rot.x, rot.y + D3DX_PI * 0.0f, rot.z));
	pArrow->SetMove(D3DXVECTOR3(
		sinf(rot.y + D3DX_PI * -0.5f) * -cosf(HeightRot) * 20.0f,
		sinf(HeightRot) * 20.0f,
		cosf(rot.y + D3DX_PI * -0.5f) * -cosf(HeightRot) * 20.0f));

	//for (int nCnt = 0; nCnt < 3; nCnt++)
	//{
	//	AttackPos[nCnt].x = pos.x + sinf(rot.y + D3DX_PI * -0.5f) * 70.0f * (nCnt + 1);
	//	AttackPos[nCnt].y = pos.y;
	//	AttackPos[nCnt].z = pos.z + cosf(rot.y + D3DX_PI * -0.5f) * 70.0f * (nCnt + 1);

	//	CEffect* pEffect = CEffect::Create();
	//	pEffect->SetPos(D3DXVECTOR3(AttackPos[nCnt].x, AttackPos[nCnt].y + 50.0f, AttackPos[nCnt].z));
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//	pEffect->SetLife(120);
	//	pEffect->SetRadius(20.0f);
	//	pEffect->SetDel(0.01f);
	//}

	//for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	//{
	//	//�I�u�W�F�N�g���擾
	//	CObject* pObj = CObject::GetTop(nCntPriority);

	//	while (pObj != NULL)
	//	{
	//		CObject* pObjNext = pObj->GetNext();

	//		CObject::TYPE type = pObj->GetType();			//��ނ��擾
	//		float fAngle = 0.0f;

	//		if (type == TYPE_PLAYER3D)
	//		{//��ނ��u���b�N�̎�
	//			CPlayer* pPlayer = (CPlayer*)pObj;

	//			for (int nCnt = 0; nCnt < 3; nCnt++)
	//			{
	//				if (useful::CollisionCircle(AttackPos[nCnt], pPlayer->GetPos(), 35.0f) == true)
	//				{
	//					pPlayer->HitDamage(Damage);

	//					fAngle = atan2f(pPlayer->GetPos().z - pos.z, pos.x - pPlayer->GetPos().x) + D3DX_PI * -0.5f;

	//					pPlayer->SetMove(D3DXVECTOR3(
	//						sinf(fAngle) * 10.0f,
	//						0.0f,
	//						cosf(fAngle) * 10.0f));
	//				}
	//			}
	//		}

	//		pObj = pObjNext;
	//	}
	//}
}

//====================================================================
// �ړI�ʒu�ɕ�������
//====================================================================
void CEnemyBowman::WalkPoint(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3* searchPos = GetSeatchPos();
	int nSeatchCount = GetSeatchCount();

	SetMove(D3DXVECTOR3(sinf(D3DX_PI * -0.5f + rot.y) * MOVE_SPEED, 0.0f, cosf(D3DX_PI * -0.5f + rot.y) * MOVE_SPEED));

	if (searchPos[nSeatchCount + 1].y <= -10000.0f)
	{
		if (useful::CollisionCircle(pos, searchPos[0], 5.0f) == true)
		{
			m_SState = SEARCHSTATE_TRUN;
			SetSeatchCount(0);
		}
	}
	else
	{
		if (useful::CollisionCircle(pos, searchPos[nSeatchCount + 1], 5.0f) == true)
		{
			m_SState = SEARCHSTATE_TRUN;
			SetSeatchCount(nSeatchCount + 1);
		}
	}

	CManager::GetInstance()->GetDebugProc()->Print("[�G] %f : %f : %f\n", pos.x, pos.y, pos.z);
}

//====================================================================
// �ړI�n��������]������
//====================================================================
void CEnemyBowman::TurnPoint(void)
{
	float fAngle = 0.0f;

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3* searchPos = GetSeatchPos();
	int nSeatchCount = GetSeatchCount();
	bool bSeatchLoop = GetSeatchLoop();

	if (searchPos[nSeatchCount + 1].y <= -10000.0f)
	{
		if (bSeatchLoop == true)
		{
			fAngle = atan2f(searchPos[0].z - pos.z, pos.x - searchPos[0].x);

			if (rot.y >= fAngle)
			{
				rot.y -= 0.04f;

				if (rot.y < fAngle)
				{
					rot.y = fAngle;
					m_SState = SEARCHSTATE_WALK;
				}
			}
			if (rot.y <= fAngle)
			{
				rot.y += 0.04f;

				if (rot.y > fAngle)
				{
					rot.y = fAngle;
					m_SState = SEARCHSTATE_WALK;
				}
			}
			SetRot(rot);
		}
		else
		{
			m_SState = SEARCHSTATE_WAIT;
		}

	}
	else
	{
		fAngle = atan2f(searchPos[nSeatchCount + 1].z - pos.z, pos.x - searchPos[nSeatchCount + 1].x);

		if (rot.y >= fAngle)
		{
			rot.y -= 0.04f;

			if (rot.y < fAngle)
			{
				rot.y = fAngle;
				m_SState = SEARCHSTATE_WALK;
			}
		}
		if (rot.y <= fAngle)
		{
			rot.y += 0.04f;

			if (rot.y > fAngle)
			{
				rot.y = fAngle;
				m_SState = SEARCHSTATE_WALK;
			}
		}
		SetRot(rot);
	}
}

//====================================================================
//�����ȊO�̓G�������o������
//====================================================================
void CEnemyBowman::CollisionPush(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3* screenPos = GetScreenPos();
	int nSeatchCount = GetSeatchCount();
	bool bSeatchLoop = GetSeatchLoop();

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
				CEnemyBowman* pEnemy = (CEnemyBowman*)pObj;

				if (useful::CollisionCircle(pos, pEnemy->GetPos(), 30.0f) == true)
				{
					fAngle = atan2f(pEnemy->GetPos().z - pos.z, pos.x - pEnemy->GetPos().x) + D3DX_PI * 0.5f;

					move.x = sinf(fAngle) * 0.2f;
					move.z = cosf(fAngle) * 0.2f;

					SetMove(move);
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CEnemyBowman::Draw(void)
{
	// �p���N���X�̕`�揈��
	CEnemy::Draw();
}

//====================================================================
// �ڐG�_���[�W����
//====================================================================
void CEnemyBowman::HitDamage(float fDamage)
{
	CEnemy::HitDamage(fDamage);

	if (GetLife() > 0)
	{
		m_BState = BATTLESTATE_DAMAGE;
		m_nStateCount = 100;
	}
}