//============================================
//
//	���[���u���b�N�̏��� [RailBlock.cpp]
//	Author:sakamoto kai
//
//============================================
#include "RailBlock.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//==========================================
//  �萔��`
//==========================================
namespace
{

}

//====================================================================
//�R���X�g���N�^
//====================================================================
CRailBlock::CRailBlock(int nPriority) :CCubeBlock(nPriority)
{

}

//====================================================================
//�f�X�g���N�^
//====================================================================
CRailBlock::~CRailBlock()
{

}

//====================================================================
//��������
//====================================================================
CRailBlock* CRailBlock::Create(void)
{
	CRailBlock* pObjectBlock = nullptr;

	if (pObjectBlock == nullptr)
	{
		//�I�u�W�F�N�g3D�̐���
		pObjectBlock = new CRailBlock();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pObjectBlock->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pObjectBlock;
}

//====================================================================
//����������
//====================================================================
HRESULT CRailBlock::Init(void)
{
	CCubeBlock::Init();

	SetTexture("data\\TEXTURE\\Wood001.png");

	SetType(TYPE_RAILBLOCK);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CRailBlock::Uninit(void)
{
	CCubeBlock::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CRailBlock::Update(void)
{
	CCubeBlock::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CRailBlock::Draw(void)
{
	CCubeBlock::Draw();
}