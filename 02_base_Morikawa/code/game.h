//========================================
//
//�Q�[��������[game.h]
//Author�F�X��x��
//
//========================================
#ifndef _GAME_H_
#define _GAME_H_
#include "main.h"
#include "manager.h"

//========================================
//�O���錾
//========================================
class CPlayer;
class CFade;
class CTimer;
class CStageManager;
class CStageEdittor;
class CObject2D;

//========================================
//�Q�[���N���X
//========================================
class CGame : public CScene
{
public:
	CGame();	//�R���X�g���N�^
	~CGame();	//�f�X�g���N�^

	//�����o�֐�
	static CGame *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void PauseState(bool bPauseState) { m_bPause = bPauseState; }	//�|�[�Y��Ԃ��ǂ���

	static CGame* GetInstance(void) { return m_pGame; }		// �Q�[���}�l�[�W���[�̏��
	
private:
	static CGame *m_pGame;		// �Q�[���}�l�[�W���[�̃|�C���^

	int m_nTransition;			// �J�E���^�[
	bool m_bOver; // �t�F�[�h�ݒ�t���O
	bool m_bClear; // �t�F�[�h�ݒ�t���O

	bool m_bPause;				// �|�[�Y��Ԃ�ON/OFF

	CFade* m_pFade;					// �t�F�[�h�̃|�C���^
	CPlayer* m_pPlayer;				// �v���C���[�|�C���^
	CTimer* m_pTimer;				// �^�C�}�[�̃|�C���^
	CStageManager* m_pStageManager;	// �X�e�[�W�}�l�[�W���[
	CObject2D* m_Obj2D;

#ifdef _DEBUG

	CStageEdittor* m_pEdittor;		// �G�f�B�^�[

#endif // _DEBUG
};

#endif
