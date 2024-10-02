//============================================
//
//	�G�f�B�b�g [Edit.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _EDIT_H_
#define _EDIT_H_

#include "main.h"
#include "object.h"

//�O���錾
class CObjmeshCube;
class CMapModel;
class CStair;
class CGimmick;

//�}�b�v�N���X
class CEdit
{
public:
//-------------------------------------------------------------------------
// �I�u�W�F�N�g�ꗗ
//-------------------------------------------------------------------------
	enum OBJ_HIT
	{
		OBJ_HIT_DARUMA = 0,			// �����
		OBJ_HIT_KOUSI,				// �i�q
		OBJ_HIT_TORO,				// ����
		OBJ_HIT_WOOD,				// ��
		OBJ_HIT_ANDON,				// �s��
		OBJ_HIT_ARMOR,				// �Z����
		OBJ_HIT_BARREL,				// �M
		OBJ_HIT_BUCKET,				// ���C��
		OBJ_HIT_CASTLE,				// ��
		OBJ_HIT_SCREEN,				// ����
		OBJ_HIT_GATE,				// ��
		OBJ_HIT_CANDLE,				// ��C
		OBJ_HIT_CUSHION,			// ���z�c
		OBJ_HIT_KAKEJIKUTURU,		// �|�����i�߁j
		OBJ_HIT_KAKEJIKUKAME,		// �|�����i�T�j
		OBJ_HIT_KETTLE,				// �₩��
		OBJ_HIT_TOWER,				// �������̍�
		OBJ_HIT_SWORDRACK,			// ���|��
		OBJ_HIT_SWORDRACKWALL,		// ���Ǌ|��
		OBJ_HIT_VASETHIN,			// �ׂ���
		OBJ_HIT_WELL,				// ���
		OBJ_HIT_VASEBIG,			// �傫����
		OBJ_HIT_PILLAR,				// ��
		OBJ_HIT_WOODDESK,			// ��
		OBJ_HIT_WOOD_MANY_BG,		// ��������̖�
		OBJ_HIT_GLASSDESK,			// �K���X�̊�
		OBJ_HIT_GLASSDESK_WEAPON,	// �K���X�̊��̒��̕���
		OBJ_HIT_TATAMI_FLOOR,		// ���オ���
		OBJ_HIT_RICEBALES,			// �ĕU
		OBJ_HIT_BOSSFENCE,			// �{�X�p�t�F���X
		OBJ_HIT_CHEST,				// �I
		OBJ_HIT_HEARTH,				// �͘F��
		OBJ_HIT_FUSUMA00,			// ���i�P���j
		OBJ_HIT_FUSUMA01,			// ���i�S���j4�i�[�j
		OBJ_HIT_FUSUMA02,			// ���i�S���j2�i���j
		OBJ_HIT_FUSUMA03,			// ���i�S���j1�i�[�j
		OBJ_HIT_FUSUMA04,			// ���i�S���j3�i���j
		OBJ_HIT_KAKEJIKUONI,		// �|�����i�S�j
		OBJ_HIT_KAKEJIKUYAMA,		// �|�����i�R�j
		OBJ_HIT_TAIMATU,			// ����
		OBJ_HIT_ONNI_KAMEN,			// �S�̉���
		OBJ_HIT_FOUNDATION,			// �y��
		OBJ_HIT_SCAFFOLDING,		// ����
		OBJ_HIT_SUPPORTPOST,		// �x����
		OBJ_HIT_MAX					// ���̗񋓌^�̑���
	};

	enum BLOCK_INIT_TYPE
	{
		BLOCK_INIT_TYPE_NORMAL = 0,	// �ʏ�̏����^
		BLOCK_INIT_TYPE_WALL_Z,		// Z�ǂ̏����^
		BLOCK_INIT_TYPE_WALL_X,		// X�ǂ̏����^
		BLOCK_INIT_TYPE_MAX			// ���̗񋓌^�̑���
	};

	enum BLOCK_TEXTURE
	{
		BLOCK_TEXTURE_00 = 0,		// �ʏ�̏����^
		BLOCK_TEXTURE_01,			// 
		BLOCK_TEXTURE_02,			// 
		BLOCK_TEXTURE_03,			// 
		BLOCK_TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	struct OBJINFO
	{
		char* pFilename;	// �t�@�C����
		char* pCommentname;	// �R�����g��
	};

	CEdit();
	~CEdit();

	enum EDITTYPE
	{
		EDITTYPE_BLOCK = 0,
		EDITTYPE_MODELHIT,
		EDITTYPE_MODELNOT,
		EDITTYPE_ENEMY,
		EDITTYPE_MAX,
	};

	static CEdit* Create();
	void Uninit(void);
	void Update(void);

protected:

private:
	void Move(void);			//�ړ�����
	void CollisionBlock(void);	//�u���b�N�Ƃ̓����蔻��
	void CollisionXModel(void);	//X���f���Ƃ̓����蔻��
	void SaveData(void);		//�G�f�B�b�g���̕ۑ�
	void SaveBlock(void);		//�u���b�N�̕ۑ�
	void SaveXModel(void);		//X���f���̕ۑ�
	void UpdateBlock(void);		//�u���b�N�̍X�V
	void UpdateXModel(void);	//X���f���̍X�V
	void AppearCollision(void);	//�����蔻��̉���

	int m_nPressCount;			//�{�^�����̓J�E���g
	int m_nBlockTextureIdx;		//�u���b�N�̃e�N�X�`���ԍ�
	D3DXVECTOR3 m_EditPos;		//�ݒu�ʒu
	D3DXVECTOR3 m_EditRot;		//�ݒu�p�x
	D3DXVECTOR3 m_EditSize;		//�ݒu�T�C�Y
	float MoveWidth;			//�ړ���
	float m_fRotValue;			//��]��
	EDITTYPE m_EditType;		//�G�f�B�b�g�Ŕz�u����I�u�W�F�N�g�̎��
	CObjmeshCube* m_pEditBlock;	//�u���b�N�̃|�C���^
	CMapModel* m_pEditModel;	//�}�b�v���f���̃|�C���^
	
	int m_nModelNumber;			//�ړ���
	int m_nGimmickNumber;		//�M�~�b�N�ԍ�
	int m_nBlockNumber;			//�u���b�N�ԍ�
	bool m_bModelCollision;		//�����蔻���t�^���邩�ǂ���
	bool m_bAppearCollision;	//�����蔻����������邩�ǂ���

	static OBJINFO m_aModelInfo[OBJ_HIT_MAX];	// �e�N�X�`���̏��
};
#endif