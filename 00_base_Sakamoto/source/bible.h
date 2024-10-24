//============================================
//
//	�����̏��� [bible.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _BIBLE_H_
#define _BIBLE_H_

#include "main.h"
#include "item.h"

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�����N���X
class CBible : public CItem
{
public:

	CBible(int nPriority = 3);
	~CBible();

	enum POS_TYPE
	{
		POS_TYPE_LEFTUP = 0,	//����
		POS_TYPE_RIGHTUP,		//�E��
		POS_TYPE_LEFTDOWN,		//����
		POS_TYPE_RIGHTDOWN,		//�E��
		POS_TYPE_MAX,			//�񋓂̍ő吔
	};

	static CBible* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	bool CollisionPlayer() override;
	void Move() override;

	// �ÓI�����o�֐�
	static CListManager<CBible>* GetList(void); // ���X�g�擾


	//�ʒu�ݒ�E�擾
	void SetPosType(const POS_TYPE& TYPE) { m_PosType = TYPE; }
	POS_TYPE GetPosType(void) { return m_PosType; }

private:
	void StateManager(void);		//��ԊǗ�

		// �ÓI�����o�ϐ�
	static CListManager<CBible>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CBible>::AIterator m_iterator; // �C�e���[�^�[

	POS_TYPE m_PosType;
	float m_fMove;
};

#endif
