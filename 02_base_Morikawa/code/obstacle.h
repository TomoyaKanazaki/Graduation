//========================================
//
// ��Q���̏���[obstacle.h]
// Author�F��������
//
//========================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "player.h"
#include "stageobj.h"

//========================================
// ��Q���N���X
//========================================
class CObstacle : public CStageObj
{
public:

	// ���
	enum EType
	{
		TYPE_RIVER = 0,	// ��
		TYPE_STONE,		// ��
		TYPE_HOLE,		// ��
		TYPE_SOFTROCK,	// �_�炩����
		TYPE_MAX		// �񋓌^�̑���
	};

	CObstacle();		// �R���X�g���N�^
	~CObstacle();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Kill() override;

	virtual bool Collision(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rSize, const CPlayer::PLAYERSTATE state) override;	// �v���C���[�Ƃ̓����蔻��

	// �ÓI�����o�֐�
	static CObstacle* Create(const EType type, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot);
	static CListManager<CObstacle> GetList() { return m_List; }	// ���X�g�擾

private:

	// �����o�ϐ�
	EType m_type;		// ���
	static CListManager<CObstacle> m_List;	// ���X�g
};

#endif
