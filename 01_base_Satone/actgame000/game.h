//==============================================================
//
//�Q�[������[game.h]
//Author:����������
//
//==============================================================
#ifndef _GAME_H_		//���̃}�N����`������Ă��Ȃ�������
#define _GAME_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "manager.h"

//�}�N����`
#define MAX_ENEMY_POS		(70)		//�ۑ�����ő�t���[����

//�O���錾
class CPlayer;			//�v���C���[�̏��
class CScore;			//�X�R�A�̏��
class CDeathUI;			//���SUI�̏��
class CItemUI;			//�A�C�e��UI�̏��
class CEdit;			//�G�f�B�b�g�̏��
class CPause;			//�|�[�Y��ʂ̏��
class CFade;			//�t�F�[�h

//�Q�[����ʃN���X�̒�`
class CGame : public CScene
{
public:

	//�Q�[�����[�h�̎��
	enum GAMEMODE
	{
		GAMEMODE_START = 0,			//�J�n
		GAMEMODE_GAME,				//�Q�[����
		GAMEMODE_OVER,				//�I��
		GAMEMODE_MAX
	};

	CGame();
	~CGame();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetEnablePause(bool bPouse);			//�|�[�Y�؂�ւ�

	static CScore *GetScore(void) { return m_pScore; }			//�X�R�A�̏��
	static CDeathUI *GetDeathUI(void) { return m_pDeathUI; }	//���S���̏��
	static CItemUI *GetItemUI(void) { return m_pItemUI; }		//�A�C�e�����̏��
	static void SetGameMode(GAMEMODE gameMode) { m_gameMode = gameMode; }		//�Q�[�����[�h�̐ݒ�
	static GAMEMODE GetGameMode(void) { return m_gameMode; }					//�Q�[�����[�h�̐ݒ�
	static CPlayer *GetPlayer(void) { return m_pPlayer; }		//�v���C���[�̏��擾
	static CPause *GetPause(void) { return m_pPause; }			//�|�[�Y��ʂ̏��
	static bool GetPauseDisp(void) { return m_bPause; }			//�|�[�Y���Ă邩�擾

private:
	void SetEnemy(void);		//�G�o������

	static CPlayer *m_pPlayer;		//�v���C���[�̏��
	static CEdit *m_pEdit;			//�G�f�B�b�g�̏��
	static CPause *m_pPause;		//�|�[�Y��ʂ̏��
	static CScore *m_pScore;		//�X�R�A�̏��
	static CDeathUI *m_pDeathUI;	//���SUI�̏��
	static CItemUI *m_pItemUI;		//�A�C�e��UI�̏��

	static bool m_bReset;			//���Z�b�g������
	static bool m_bPause;			//�|�[�Y��ʂ�
	static bool m_bEnemySpawnFirst;			//�G���o��������
	static bool m_bEnemySpawnSecond;		//�G���o��������

	static GAMEMODE m_gameMode;		//�Q�[�����[�h
	int m_nCntEnemy;				//�G�o���J�E���g
};

#endif

