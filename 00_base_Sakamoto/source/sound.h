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
		// SE
		SOUND_LABEL_SE_DEATH = 0,	// �^�}�S���̎��S
		SOUND_LABEL_SE_DEVILHOLE,	// �f�r���z�[��
		SOUND_LABEL_SE_BREAKEGG,	// ���������
		SOUND_LABEL_SE_ENEMROTATION,	// �G�̕����]��	
		SOUND_LABEL_SE_FIRE,	// �^�}�S���̉Ή�����
		SOUND_LABEL_SE_GET_BIBLE,	// �����̎擾
		SOUND_LABEL_SE_STAGECLEAR,	// �X�e�[�W�N���A
		SOUND_LABEL_SE_WALK,	// �^�}�S���̓k��	
		SOUND_LABEL_SE_FLAP,	// �H�΂���	
		SOUND_LABEL_SE_RESULT,	// ���U���g�̌��ʉ�
		SOUND_LABEL_SE_FAIL,	// ���s��
		SOUND_LABEL_SE_EXPLOSION,	// ����
		SOUND_LABEL_SE_BORN,	// �^�}�S�������܂��
		SOUND_LABEL_SE_EAT,	// �^�}�S���̕ߐH
		SOUND_LABEL_SE_GET_CROSS,	// �\����
		SOUND_LABEL_SE_GET_BOWA,	// �ڂ��ڂ�
		SOUND_LABEL_SE_SELECT,		// �I��
		SOUND_LABEL_SE_ENTER,		// ����

		// BGM
		SOUND_LABEL_BGM_TITLE,	// �^�C�g��bgm
		SOUND_LABEL_BGM_RESULT,	// ���U���gbgm
		SOUND_LABEL_BGM_STAGE1,	// �X�e�[�W1bgm	
		SOUND_LABEL_BGM_STAGE2,	// �X�e�[�W2bgm	

		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//*****************************************************************************
	// �����o�֐��錾
	//*****************************************************************************
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);

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
