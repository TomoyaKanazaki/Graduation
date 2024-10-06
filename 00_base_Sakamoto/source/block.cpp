//============================================
//
//	�u���b�N�̃X�[�p�[�N���X [block.cpp]
//	Author:nakamura riku
//
//============================================
#include "block.h"
#include "Player2D.h"
#include "renderer.h"
#include "manager.h"
#include "useful.h"

// �}�N����`

//====================================================================
// �R���X�g���N�^
//====================================================================
CBlock::CBlock(int nPriority) : CObject2D(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CBlock::~CBlock()
{

}

//====================================================================
// ����������
//====================================================================
HRESULT CBlock::Init(void)
{
	CObject2D::Init();

	//�V����cpp���쐬�������͐V����TYPE��񋓂ɒǉ����Ďw�肷�邱��
	SetType(CObject::TYPE_BLOCK);

	return S_OK;
}

//====================================================================
// �I������
//====================================================================
void CBlock::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
// �X�V����
//====================================================================
void CBlock::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	m_posOld = pos;

	pos += m_move;

	if (pos.y < GetHeight() * -4.0f)
		Uninit();

	SetPos(pos);

	//���_���̍X�V
	CObject2D::Update();

	//----���_����ύX�������ꍇ�͈ȉ��̃R�[�h���g�p���邱��-----
	//VERTEX_2D* pVtx;	//���_�|�C���^������

	////���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	//GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	////�ʒu
	//pVtx[0].pos = m_pos;
	//pVtx[1].pos = m_pos;
	//pVtx[2].pos = m_pos;
	//pVtx[3].pos = m_pos;

	////�F
	//pVtx[0].col;
	//pVtx[1].col;
	//pVtx[2].col;
	//pVtx[3].col;

	////�e�N�X�`��
	//pVtx[0].tex;
	//pVtx[1].tex;
	//pVtx[2].tex;
	//pVtx[3].tex;

	////���_�o�b�t�@���A�����b�N����
	//GetVtxBuff()->Unlock();
}

//====================================================================
// �`�揈��
//====================================================================
void CBlock::Draw(void)
{
	CObject2D::Draw();
}

//====================================================================
// ����u���b�N
//====================================================================
// ��������
//====================================================================
CBlockBase* CBlockBase::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority)
{
	CBlockBase* pBlock = NULL;

	if (pBlock == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pBlock = new CBlockBase();
	}

	if (pBlock != NULL)
	{
		pBlock->SetPos(pos);
		pBlock->SetWidth(fWight);
		pBlock->SetHeight(fHeight);
		pBlock->SetMove(move);

		//�I�u�W�F�N�g�̏���������
		if (FAILED(pBlock->Init()))
		{//���������������s�����ꍇ
			return NULL;
		}
	}

	return pBlock;
}

//====================================================================
// �����蔻�菈��
//====================================================================
void CBlockBase::Collision(CPlayer2D* pPlayer2D)
{
	// �v���C���[���ϐ�
	D3DXVECTOR3 playerPos = pPlayer2D->GetPos();
	D3DXVECTOR3 playerPosOld = pPlayer2D->GetPosOld();
	D3DXVECTOR3 playerMove = pPlayer2D->GetMove();
	float playerHeight = pPlayer2D->GetHeight() * 0.5f;
	float playerWidth = pPlayer2D->GetWidth() * 0.5f;
	bool playerLanding = pPlayer2D->GetLanding();

	// �u���b�N���ϐ�
	D3DXVECTOR3 blockPos = GetPos();
	D3DXVECTOR3 blockPosOld = GetPosOld();
	D3DXVECTOR3 blockMove = GetMove();
	float blockHeight = GetHeight() * 0.5f;
	float blockWidth = GetWidth() * 0.5f;

	D3DXVECTOR3 posDef = playerPos;

	// �㉺����
	if ((playerPos.x + playerWidth > blockPos.x - blockWidth &&
		playerPos.x + playerWidth < blockPos.x + blockWidth) ||
		(playerPos.x - playerWidth > blockPos.x - blockWidth &&
		playerPos.x - playerWidth < blockPos.x + blockWidth))
	{
		if (playerPos.y + playerHeight > blockPos.y - blockHeight &&
			playerPos.y + playerHeight < blockPos.y + blockHeight)
		{
			posDef.y = blockPos.y - blockHeight - playerHeight + blockMove.y;
			playerMove.y = 0.0f;
			playerLanding = true;
		}

		if (playerPos.y - playerHeight > blockPos.y - blockHeight &&
			playerPos.y - playerHeight < blockPos.y + blockHeight)
		{
			posDef.y = blockPos.y + blockHeight + playerHeight;
			playerMove.y = 0.0f;
		}
	}

	// ���E����
	if ((playerPos.y + playerHeight > blockPos.y - blockHeight &&
		playerPos.y + playerHeight < blockPos.y + blockHeight) ||
		(playerPos.y - playerHeight > blockPos.y - blockHeight &&
			playerPos.y - playerHeight < blockPos.y + blockHeight))
	{
		if (playerPos.x + playerWidth > blockPos.x - blockWidth &&
			playerPos.x + playerWidth < blockPos.x + blockWidth &&
			playerPosOld.x + playerWidth < blockPos.x - blockWidth)
		{
			posDef.x = blockPos.x - playerWidth - blockWidth;
			posDef.y = playerPos.y;
			playerLanding = false;
			playerMove.x *= -1.0f;
		}

		if (playerPos.x - playerWidth > blockPos.x - blockWidth &&
			playerPos.x - playerWidth < blockPos.x + blockWidth &&
			playerPosOld.x - playerWidth > blockPos.x + blockWidth)
		{
			posDef.x = blockPos.x + playerWidth + blockWidth;
			posDef.y = playerPos.y;
			playerLanding = false;
			playerMove.x *= -1.0f;
		}
	}

	playerPos = posDef;

	pPlayer2D->SetPos(playerPos);
	pPlayer2D->SetMove(playerMove);
	pPlayer2D->SetLanding(playerLanding);
}

//====================================================================
// ���u���b�N
//====================================================================
// ��������
//====================================================================
CBlockSpike* CBlockSpike::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority)
{
	CBlockSpike* pBlock = NULL;

	if (pBlock == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pBlock = new CBlockSpike();
	}

	if (pBlock != NULL)
	{
		pBlock->SetPos(pos);
		pBlock->SetWidth(fWight);
		pBlock->SetHeight(fHeight);
		pBlock->SetMove(move);

		//�I�u�W�F�N�g�̏���������
		if (FAILED(pBlock->Init()))
		{//���������������s�����ꍇ
			return NULL;
		}
	}

	return pBlock;
}

//====================================================================
// �����蔻�菈��
//====================================================================
void CBlockSpike::Collision(CPlayer2D* pPlayer2D)
{
	// �v���C���[���ϐ�
	D3DXVECTOR3 playerPos = pPlayer2D->GetPos();
	float playerHeight = pPlayer2D->GetHeight() * 0.5f;
	float playerWidth = pPlayer2D->GetWidth() * 0.5f;

	// �u���b�N���ϐ�
	D3DXVECTOR3 blockPos = GetPos();
	float blockHeight = GetHeight() * 0.5f;
	float blockWidth = GetWidth() * 0.5f;

	// �㉺���E�ŏd�Ȃ��Ă���
	if (((playerPos.x + playerWidth > blockPos.x - blockWidth &&
		playerPos.x + playerWidth < blockPos.x + blockWidth) ||
		(playerPos.x - playerWidth > blockPos.x - blockWidth &&
			playerPos.x - playerWidth < blockPos.x + blockWidth)) &&
		((playerPos.y + playerHeight > blockPos.y - blockHeight &&
			playerPos.y + playerHeight < blockPos.y + blockHeight) ||
			(playerPos.y - playerHeight > blockPos.y - blockHeight &&
				playerPos.y - playerHeight < blockPos.y + blockHeight)))
	{
		// ���ɓ����������̏���
		pPlayer2D->Death();
	}
}

//====================================================================
// �R�C���u���b�N
//====================================================================
// ��������
//====================================================================
CBlockCoin* CBlockCoin::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority)
{
	CBlockCoin* pBlock = NULL;

	if (pBlock == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pBlock = new CBlockCoin();
	}

	if (pBlock != NULL)
	{
		pBlock->SetPos(pos);
		pBlock->SetWidth(fWight);
		pBlock->SetHeight(fHeight);
		pBlock->SetMove(move);

		//�I�u�W�F�N�g�̏���������
		if (FAILED(pBlock->Init()))
		{//���������������s�����ꍇ
			return NULL;
		}
	}

	return pBlock;
}

//====================================================================
// �����蔻�菈��
//====================================================================
void CBlockCoin::Collision(CPlayer2D* pPlayer2D)
{
	// �v���C���[���ϐ�
	D3DXVECTOR3 playerPos = pPlayer2D->GetPos();
	float playerHeight = pPlayer2D->GetHeight() * 0.5f;
	float playerWidth = pPlayer2D->GetWidth() * 0.5f;

	// �u���b�N���ϐ�
	D3DXVECTOR3 blockPos = GetPos();
	float blockHeight = GetHeight() * 0.5f;
	float blockWidth = GetWidth() * 0.5f;

	// �㉺���E�ŏd�Ȃ��Ă���
	if (((playerPos.x + playerWidth > blockPos.x - blockWidth &&
		playerPos.x + playerWidth < blockPos.x + blockWidth) ||
		(playerPos.x - playerWidth > blockPos.x - blockWidth &&
			playerPos.x - playerWidth < blockPos.x + blockWidth)) &&
		((playerPos.y + playerHeight > blockPos.y - blockHeight &&
			playerPos.y + playerHeight < blockPos.y + blockHeight) ||
			(playerPos.y - playerHeight > blockPos.y - blockHeight &&
				playerPos.y - playerHeight < blockPos.y + blockHeight)))
	{
		// �R�C���ɓ����������̏���
		Uninit();
	}
}

//====================================================================
// �o�l�u���b�N
//====================================================================
// ��������
//====================================================================
CBlockSpring* CBlockSpring::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority)
{
	CBlockSpring* pBlock = NULL;

	if (pBlock == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pBlock = new CBlockSpring();
	}

	if (pBlock != NULL)
	{
		pBlock->SetPos(pos);
		pBlock->SetWidth(fWight);
		pBlock->SetHeight(fHeight);
		pBlock->SetMove(move);

		//�I�u�W�F�N�g�̏���������
		if (FAILED(pBlock->Init()))
		{//���������������s�����ꍇ
			return NULL;
		}
	}

	return pBlock;
}

//====================================================================
// �����蔻�菈��
//====================================================================
void CBlockSpring::Collision(CPlayer2D* pPlayer2D)
{
	// �v���C���[���ϐ�
	D3DXVECTOR3 playerPos = pPlayer2D->GetPos();
	D3DXVECTOR3 playerPosOld = pPlayer2D->GetPosOld();
	D3DXVECTOR3 playerMove = pPlayer2D->GetMove();
	float playerHeight = pPlayer2D->GetHeight() * 0.5f;
	float playerWidth = pPlayer2D->GetWidth() * 0.5f;
	bool playerLanding = pPlayer2D->GetLanding();

	// �u���b�N���ϐ�
	D3DXVECTOR3 blockPos = GetPos();
	D3DXVECTOR3 blockPosOld = GetPosOld();
	D3DXVECTOR3 blockMove = GetMove();
	float blockHeight = GetHeight() * 0.5f;
	float blockWidth = GetWidth() * 0.5f;

	D3DXVECTOR3 posDef = playerPos;

	// �㉺����
	if ((playerPos.x + playerWidth > blockPos.x - blockWidth &&
		playerPos.x + playerWidth < blockPos.x + blockWidth) ||
		(playerPos.x - playerWidth > blockPos.x - blockWidth &&
			playerPos.x - playerWidth < blockPos.x + blockWidth))
	{
		if (playerPos.y + playerHeight > blockPos.y - blockHeight &&
			playerPos.y + playerHeight < blockPos.y + blockHeight)
		{
			posDef.y = blockPos.y - blockHeight - playerHeight + blockMove.y;

			if (!CManager::GetInstance()->GetPause())
				playerMove.y = -10.0f;
		}

		if (playerPos.y - playerHeight > blockPos.y - blockHeight &&
			playerPos.y - playerHeight < blockPos.y + blockHeight)
		{
			posDef.y = blockPos.y + blockHeight + playerHeight;
			playerMove.y = 0.0f;
		}
	}

	// ���E����
	if ((playerPos.y + playerHeight > blockPos.y - blockHeight &&
		playerPos.y + playerHeight < blockPos.y + blockHeight) ||
		(playerPos.y - playerHeight > blockPos.y - blockHeight &&
			playerPos.y - playerHeight < blockPos.y + blockHeight))
	{
		if (playerPos.x + playerWidth > blockPos.x - blockWidth &&
			playerPos.x + playerWidth < blockPos.x + blockWidth &&
			playerPosOld.x + playerWidth < blockPos.x - blockWidth)
		{
			posDef.x = blockPos.x - playerWidth - blockWidth;
			posDef.y = playerPos.y;
			playerLanding = false;
			playerMove.x *= -1.0f;
		}

		if (playerPos.x - playerWidth > blockPos.x - blockWidth &&
			playerPos.x - playerWidth < blockPos.x + blockWidth &&
			playerPosOld.x - playerWidth > blockPos.x + blockWidth)
		{
			posDef.x = blockPos.x + playerWidth + blockWidth;
			posDef.y = playerPos.y;
			playerLanding = false;
			playerMove.x *= -1.0f;
		}
	}

	playerPos = posDef;

	pPlayer2D->SetPos(playerPos);
	pPlayer2D->SetMove(playerMove);
	pPlayer2D->SetLanding(playerLanding);
}

//====================================================================
// ���ꂩ���̃u���b�N
//====================================================================
// ��������
//====================================================================
CBlockBroken* CBlockBroken::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority)
{
	CBlockBroken* pBlock = NULL;

	if (pBlock == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pBlock = new CBlockBroken();
	}

	if (pBlock != NULL)
	{
		pBlock->SetPos(pos);
		pBlock->SetWidth(fWight);
		pBlock->SetHeight(fHeight);
		pBlock->SetMove(move);

		//�I�u�W�F�N�g�̏���������
		if (FAILED(pBlock->Init()))
		{//���������������s�����ꍇ
			return NULL;
		}
	}

	return pBlock;
}

//====================================================================
// �����蔻�菈��
//====================================================================
void CBlockBroken::Collision(CPlayer2D* pPlayer2D)
{
	// �v���C���[���ϐ�
	D3DXVECTOR3 playerPos = pPlayer2D->GetPos();
	D3DXVECTOR3 playerPosOld = pPlayer2D->GetPosOld();
	D3DXVECTOR3 playerMove = pPlayer2D->GetMove();
	float playerHeight = pPlayer2D->GetHeight() * 0.5f;
	float playerWidth = pPlayer2D->GetWidth() * 0.5f;
	bool playerLanding = pPlayer2D->GetLanding();

	// �u���b�N���ϐ�
	D3DXVECTOR3 blockPos = GetPos();
	D3DXVECTOR3 blockPosOld = GetPosOld();
	D3DXVECTOR3 blockMove = GetMove();
	float blockHeight = GetHeight() * 0.5f;
	float blockWidth = GetWidth() * 0.5f;

	D3DXVECTOR3 posDef = playerPos;

	// �㉺����
	if ((playerPos.x + playerWidth > blockPos.x - blockWidth &&
		playerPos.x + playerWidth < blockPos.x + blockWidth) ||
		(playerPos.x - playerWidth > blockPos.x - blockWidth &&
			playerPos.x - playerWidth < blockPos.x + blockWidth))
	{
		if (playerPos.y + playerHeight > blockPos.y - blockHeight &&
			playerPos.y + playerHeight < blockPos.y + blockHeight)
		{
			posDef.y = blockPos.y - blockHeight - playerHeight + blockMove.y;
			playerMove.y = 0.0f;
			playerLanding = true;

			if (!CManager::GetInstance()->GetPause())
				m_nLife--;
		}

		if (playerPos.y - playerHeight > blockPos.y - blockHeight &&
			playerPos.y - playerHeight < blockPos.y + blockHeight)
		{
			posDef.y = blockPos.y + blockHeight + playerHeight;
			playerMove.y = 0.0f;
		}
	}

	// ���E����
	if ((playerPos.y + playerHeight > blockPos.y - blockHeight &&
		playerPos.y + playerHeight < blockPos.y + blockHeight) ||
		(playerPos.y - playerHeight > blockPos.y - blockHeight &&
			playerPos.y - playerHeight < blockPos.y + blockHeight))
	{
		if (playerPos.x + playerWidth > blockPos.x - blockWidth &&
			playerPos.x + playerWidth < blockPos.x + blockWidth &&
			playerPosOld.x + playerWidth < blockPos.x - blockWidth)
		{
			posDef.x = blockPos.x - playerWidth - blockWidth;
			posDef.y = playerPos.y;
			playerLanding = false;
			playerMove.x *= -1.0f;
		}

		if (playerPos.x - playerWidth > blockPos.x - blockWidth &&
			playerPos.x - playerWidth < blockPos.x + blockWidth &&
			playerPosOld.x - playerWidth > blockPos.x + blockWidth)
		{
			posDef.x = blockPos.x + playerWidth + blockWidth;
			posDef.y = playerPos.y;
			playerLanding = false;
			playerMove.x *= -1.0f;
		}
	}

	playerPos = posDef;

	if (m_nLife < 0)
	{
		m_nLife = 0;
		Uninit();
	}

	pPlayer2D->SetPos(playerPos);
	pPlayer2D->SetMove(playerMove);
	pPlayer2D->SetLanding(playerLanding);
}

//====================================================================
// �^�C�g���u���b�N
//====================================================================
// ��������
//====================================================================
CBlockTitle* CBlockTitle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority)
{
	CBlockTitle* pBlock = NULL;

	if (pBlock == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pBlock = new CBlockTitle();
	}

	if (pBlock != NULL)
	{
		pBlock->SetPos(pos);
		pBlock->SetWidth(fWight);
		pBlock->SetHeight(fHeight);
		pBlock->SetMove(move);

		//�I�u�W�F�N�g�̏���������
		if (FAILED(pBlock->Init()))
		{//���������������s�����ꍇ
			return NULL;
		}
	}

	return pBlock;
}

//====================================================================
// �����蔻�菈��
//====================================================================
void CBlockTitle::Collision(CPlayer2D* pPlayer2D)
{
	// �v���C���[���ϐ�
	D3DXVECTOR3 playerPos = pPlayer2D->GetPos();
	D3DXVECTOR3 playerPosOld = pPlayer2D->GetPosOld();
	D3DXVECTOR3 playerMove = pPlayer2D->GetMove();
	float playerHeight = pPlayer2D->GetHeight() * 0.5f;
	float playerWidth = pPlayer2D->GetWidth() * 0.5f;
	bool playerLanding = pPlayer2D->GetLanding();

	// �u���b�N���ϐ�
	D3DXVECTOR3 blockPos = GetPos();
	D3DXVECTOR3 blockPosOld = GetPosOld();
	D3DXVECTOR3 blockMove = GetMove();
	float blockHeight = GetHeight() * 0.5f;
	float blockWidth = GetWidth() * 0.5f;

	D3DXVECTOR3 posDef = playerPos;

	// �㉺����
	if ((playerPos.x + playerWidth > blockPos.x - blockWidth &&
		playerPos.x + playerWidth < blockPos.x + blockWidth) ||
		(playerPos.x - playerWidth > blockPos.x - blockWidth &&
			playerPos.x - playerWidth < blockPos.x + blockWidth))
	{
		if (playerPos.y + playerHeight > blockPos.y - blockHeight &&
			playerPos.y + playerHeight < blockPos.y + blockHeight)
		{
			posDef.y = blockPos.y - blockHeight - playerHeight + blockMove.y;
			playerMove.y = 0.0f;
			playerLanding = true;
		}

		if (playerPos.y - playerHeight > blockPos.y - blockHeight &&
			playerPos.y - playerHeight < blockPos.y + blockHeight)
		{
			posDef.y = blockPos.y + blockHeight + playerHeight;
			playerMove.y = 0.0f;
		}
	}

	// ���E����
	if ((playerPos.y + playerHeight > blockPos.y - blockHeight &&
		playerPos.y + playerHeight < blockPos.y + blockHeight) ||
		(playerPos.y - playerHeight > blockPos.y - blockHeight &&
			playerPos.y - playerHeight < blockPos.y + blockHeight))
	{
		if (playerPos.x + playerWidth > blockPos.x - blockWidth &&
			playerPos.x + playerWidth < blockPos.x + blockWidth &&
			playerPosOld.x + playerWidth < blockPos.x - blockWidth)
		{
			posDef.x = blockPos.x - playerWidth - blockWidth;
			posDef.y = playerPos.y;
			playerMove.x *= -1.0f;
		}

		if (playerPos.x - playerWidth > blockPos.x - blockWidth &&
			playerPos.x - playerWidth < blockPos.x + blockWidth &&
			playerPosOld.x - playerWidth > blockPos.x + blockWidth)
		{
			posDef.x = blockPos.x + playerWidth + blockWidth;
			posDef.y = playerPos.y;
			playerMove.x *= -1.0f;
		}
	}

	playerPos = posDef;

	pPlayer2D->SetPos(playerPos);
	pPlayer2D->SetMove(playerMove);
	pPlayer2D->SetLanding(playerLanding);
}

//====================================================================
// �`���[�g���A���u���b�N
//====================================================================
// ��������
//====================================================================
CBlockTutorial* CBlockTutorial::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight, int nPriority)
{
	CBlockTutorial* pBlock = NULL;

	if (pBlock == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pBlock = new CBlockTutorial();
	}

	if (pBlock != NULL)
	{
		pBlock->SetPos(pos);
		pBlock->SetWidth(fWight);
		pBlock->SetHeight(fHeight);
		pBlock->SetMove(move);

		//�I�u�W�F�N�g�̏���������
		if (FAILED(pBlock->Init()))
		{//���������������s�����ꍇ
			return NULL;
		}
	}

	return pBlock;
}

//====================================================================
// �����蔻�菈��
//====================================================================
void CBlockTutorial::Collision(CPlayer2D* pPlayer2D)
{
	// �v���C���[���ϐ�
	D3DXVECTOR3 playerPos = pPlayer2D->GetPos();
	D3DXVECTOR3 playerPosOld = pPlayer2D->GetPosOld();
	D3DXVECTOR3 playerMove = pPlayer2D->GetMove();
	float playerHeight = pPlayer2D->GetHeight() * 0.5f;
	float playerWidth = pPlayer2D->GetWidth() * 0.5f;
	bool playerLanding = pPlayer2D->GetLanding();

	// �u���b�N���ϐ�
	D3DXVECTOR3 blockPos = GetPos();
	D3DXVECTOR3 blockPosOld = GetPosOld();
	D3DXVECTOR3 blockMove = GetMove();
	float blockHeight = GetHeight() * 0.5f;
	float blockWidth = GetWidth() * 0.5f;

	D3DXVECTOR3 posDef = playerPos;

	// �㉺����
	if ((playerPos.x + playerWidth > blockPos.x - blockWidth &&
		playerPos.x + playerWidth < blockPos.x + blockWidth) ||
		(playerPos.x - playerWidth > blockPos.x - blockWidth &&
			playerPos.x - playerWidth < blockPos.x + blockWidth))
	{
		if (playerPos.y + playerHeight > blockPos.y - blockHeight &&
			playerPos.y + playerHeight < blockPos.y + blockHeight)
		{
			posDef.y = blockPos.y - blockHeight - playerHeight + blockMove.y;
			playerMove.y = 0.0f;
			playerLanding = true;
		}

		if (playerPos.y - playerHeight > blockPos.y - blockHeight &&
			playerPos.y - playerHeight < blockPos.y + blockHeight)
		{
			posDef.y = blockPos.y + blockHeight + playerHeight;
			playerMove.y = 0.0f;
		}
	}

	// ���E����
	if ((playerPos.y + playerHeight > blockPos.y - blockHeight &&
		playerPos.y + playerHeight < blockPos.y + blockHeight) ||
		(playerPos.y - playerHeight > blockPos.y - blockHeight &&
			playerPos.y - playerHeight < blockPos.y + blockHeight))
	{
		if (playerPos.x + playerWidth > blockPos.x - blockWidth &&
			playerPos.x + playerWidth < blockPos.x + blockWidth &&
			playerPosOld.x + playerWidth < blockPos.x - blockWidth)
		{
			posDef.x = blockPos.x - playerWidth - blockWidth;
			posDef.y = playerPos.y;
			playerMove.x *= -1.0f;
		}

		if (playerPos.x - playerWidth > blockPos.x - blockWidth &&
			playerPos.x - playerWidth < blockPos.x + blockWidth &&
			playerPosOld.x - playerWidth > blockPos.x + blockWidth)
		{
			posDef.x = blockPos.x + playerWidth + blockWidth;
			posDef.y = playerPos.y;
			playerMove.x *= -1.0f;
		}
	}

	playerPos = posDef;

	pPlayer2D->SetPos(playerPos);
	pPlayer2D->SetMove(playerMove);
	pPlayer2D->SetLanding(playerLanding);
}