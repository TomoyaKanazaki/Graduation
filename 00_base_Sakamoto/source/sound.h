#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

class CSound
{
public:
	CSound();
	~CSound();

	//*****************************************************************************
	// �T�E���h�ꗗ
	//*****************************************************************************
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,			// BGM0						
		SOUND_LABEL_BGM_GAME,				// BGM1
		SOUND_LABEL_BGM_BOSS,				// BGM1
		SOUND_LABEL_BGM_RESULT,				// BGM2
		SOUND_LABEL_SE_SELECT_PUSH,			// �I����
		SOUND_LABEL_SE_ENTER_PUSH,			// ���艹
		SOUND_LABEL_SE_WARNING,				// �x����
		SOUND_LABEL_SE_BOSS_BULLET,			// �{�X�̎ˌ���
		SOUND_LABEL_SE_BOSS_RAIN,			// �{�X�̉J�U��������
		SOUND_LABEL_SE_BOSS_DASH,			// �{�X�̓ːi��
		SOUND_LABEL_SE_BOSS_SMOKE,			// �{�X�̉���
		SOUND_LABEL_SE_BOSS_SLASH,			// �{�X�̎a��
		SOUND_LABEL_SE_BOSS_BLOCK_SYURIKEN,	// �{�X�̎藠���h��
		SOUND_LABEL_SE_BOSS_SYURIKEN,		// �{�X�̎藠��
		SOUND_LABEL_SE_BOSS_BACKJUMP,		// �{�X�̌����
		SOUND_LABEL_SE_BOSS_KABUTO,			// �{�X�̊���
		SOUND_LABEL_SE_BOSS_MOW,			// �{�X�̓ガ����
		SOUND_LABEL_SE_DAMAGE_BOSS,			// �{�X�̃_���[�W
		SOUND_LABEL_SE_DAMAGE_PLAYER,		// �v���C���[�_���[�W��
		SOUND_LABEL_SE_ATTACK_NORMAL,		// �v���C���[�ʏ�U����
		SOUND_LABEL_SE_ATTACK_SKY,			// �v���C���[�󒆍U����
		SOUND_LABEL_SE_DODGE,				// �v���C���[�����
		SOUND_LABEL_SE_JUMP,				// �v���C���[�W�����v��
		SOUND_LABEL_SE_ATTENTION,			// ���ڐ؂�ւ���
		SOUND_LABEL_SE_DEATHDAMAGE,			// ���S�_���[�W��
		SOUND_LABEL_SE_DEATHEXPLOSION,		// ���S������
		SOUND_LABEL_SE_GOGOGOGO,			// �S�S�S�S�S�S�S��
		SOUND_LABEL_SE_HOWL,				// ���K��
		SOUND_LABEL_BGM_TUTORIAL,			// �`���[�g���A��BGM
		SOUND_LABEL_SE_SWORD_SWING_00,		// �E����U�艹_00
		SOUND_LABEL_SE_SWORD_SWING_01,		// �E����U�艹_01
		SOUND_LABEL_SE_SWORD_HIT,			// �E���q�b�g��
		SOUND_LABEL_SE_SHURIKEN_SHOT,		// �藠�����ˉ�
		SOUND_LABEL_SE_SHURIKEN_HIT,		// �藠���q�b�g��
		SOUND_LABEL_SE_TARGET_SWITCHING,	// �^�[�Q�b�g�؂�ւ���
		SOUND_LABEL_SE_SWORD_GUARD,			// ���̃K�[�h��
		SOUND_LABEL_SE_WRITE_BRUSH,			// �M�ŏ�����
		SOUND_LABEL_SE_SCROLL_OPEN,			// �������J����
		SOUND_LABEL_SE_FIRE_ACTIVE,			// �Γٔ�����
		SOUND_LABEL_SE_FIRE_0,				// ���̉�
		SOUND_LABEL_SE_BOTLE_BREAK,			// �r�̉��鉹
		SOUND_LABEL_SE_BOX_BREAK,			// �ؔ��̉��鉹
		SOUND_LABEL_SE_BOW,					// �|��
		SOUND_LABEL_SE_BATTLE_ENTRY,		// �퓬�˓���
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//*****************************************************************************
	// �����o�֐��錾
	//*****************************************************************************
	HRESULT InitSound(HWND hWnd);
	void UninitSound(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

private:

	//*****************************************************************************
	// �T�E���h���̍\���̒�`
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDINFO;

	//*****************************************************************************
	// �����o�֐��錾
	//*****************************************************************************
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	//*****************************************************************************
	// �����o�ϐ��錾
	//*****************************************************************************
	IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y

	// �T�E���h�̏��
	static SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX];
};
#endif