//========================================
//
//�`���[�g���A��[tutorial.h]
//Author�F�X��x��
//
//========================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
#include "main.h"
#include "manager.h"

//========================================
//�O���錾
//========================================
class CObjectX;
class CObject3D;
class CLight;
class CPlayer;
class CIdxMeshField;
class CField;
class CFade;
class CGauge;
class CObject2D;
class CMapObject;

//========================================
//�`���[�g���A���N���X
//========================================
class CTutorial : public CScene
{
public:
	CTutorial();	//�R���X�g���N�^
	~CTutorial();	//�f�X�g���N�^

	//�����o�֐�
	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CObjectX* m_pObjectX;		// X�t�@�C���I�u�W�F�N�g�̃|�C���^
	CPlayer* m_pPlayer;			// �v���C���[�̃|�C���^
	CIdxMeshField* m_pIdxMesh;	// ���b�V���t�B�[���h�̃|�C���^
	CField* m_pField;			// �|���S���̃|�C���^
	CFade* m_pFade;				// �t�F�[�h�̃|�C���^
	CMapObject* m_pMobj;		// �}�b�v�I�u�W�F�N�g�̃|�C���^
	CObject2D* m_pObj2D;		// �I�u�W�F�N�g2D�̃|�C���^
	bool m_bEnd;
};

#endif
