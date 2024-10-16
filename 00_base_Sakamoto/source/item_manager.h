#pragma once
//==========================================
//
//  �A�C�e���Ǘ��N���X(item_manager.h)
//  Author : Tomoya Kanazaki
//
//==========================================

//==========================================
//  �O���錾
//==========================================
class CItem;

//==========================================
//  �N���X��`
//==========================================
class CItemManager
{
public:

	// �z�u�^�C�v��
	enum EType
	{
		TYPE_BOWABOWA = 0, // �{���{���z�u���
		TYPE_BIBLE, // �����z�u���
		TYPE_MAX,
		TYPE_NONE
	};

	// �����o�֐�
	CItemManager();
	~CItemManager();

	void Init(const int nMap, const EType type);
	void Uninit();
	void Update();
	void Draw();

	void SetType(const EType type) { m_eType = type; ItemSeting(); } // �z�u�^�C�v�̕ύX

	// �ÓI�����o�֐�
	static CItemManager* Create(const int nMap, const EType type = TYPE_BOWABOWA);
	static HRESULT Load();
	static void UnLoad();

private:

	// �A�C�e���Ǘ��\����
	struct SItems
	{
		int nNum; // �A�C�e���z�u����
		D3DXVECTOR3* pPos; // �A�C�e���̔z�u���W
		CItem::TYPE* pType; // �A�C�e���̎��
	};

	// �����o�֐�
	void ItemSeting(); // �A�C�e���̐ݒu����

	// �����o�ϐ�
	EType m_eType; // �z�u�^�C�v
	SItems* m_pItems; //���ݎg���Ă���A�C�e���̃f�[�^

	// �ÓI�����o�ϐ�
	static int ms_nNumMap; // �z�u�����f�[�^�̐�
	static SItems* ms_pItems; // �A�C�e���Ǘ��p�ϐ�
};
