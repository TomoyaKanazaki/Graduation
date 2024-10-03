//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ����������
//
//=============================================================================
#ifndef _SOUND_H_		//���̃}�N����`������Ă��Ȃ�������
#define _SOUND_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�T�E���h�N���X�̒�`
class CSound
{
public:
	//*****************************************************************************
	// �T�E���h�ꗗ
	//*****************************************************************************
	enum SOUND_LABEL
	{
		SOUND_LABEL_BGM000 = 0,			// �^�C�g��
		SOUND_LABEL_BGM001,				// ���炷��BGM
		SOUND_LABEL_BGM002,				// �Q�[��BGM
		SOUND_LABEL_BGM003,				// ���U���gBGM(�N���A)
		SOUND_LABEL_BGMBOSS,			// �{�X��BGM
		SOUND_LABEL_SE_DAMAGE000,		// �_���[�W��0
		SOUND_LABEL_SE_DAMAGE001,		// �_���[�W��1
		SOUND_LABEL_SE_SHOT000,			// �V���b�g��0
		SOUND_LABEL_SE_ITEM000,			// �A�C�e����
		SOUND_LABEL_SE_ITEM001,			// �񕜉�
		SOUND_LABEL_SE_ENTER000,		// ���艹000
		SOUND_LABEL_SE_ENTER001,		// ���艹001
		SOUND_LABEL_SE_ENTER002,		// ���艹002
		SOUND_LABEL_SE_SWITCH,			// �X�C�b�`��
		SOUND_LABEL_SE_STEP,			// ������
		SOUND_LABEL_SE_JUMP000,			// �W�����v��
		SOUND_LABEL_SE_JUMP001,			// �W�����v��
		SOUND_LABEL_SE_DASH,			// �_�b�V����
		SOUND_LABEL_SE_DASH_AUTO,		// �I�[�g�_�b�V����
		SOUND_LABEL_SE_COLLAPSE00,		// ����鉹
		SOUND_LABEL_SE_COLLAPSE01,		// ���ꂽ��
		SOUND_LABEL_SE_APPEAR,			// �o�����鉹

		SOUND_LABEL_MAX,
	};

	CSound();		//�R���X�g���N�^
	~CSound();		//�f�X�g���N�^

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	static HRESULT Play(SOUND_LABEL label);
	static void Stop(SOUND_LABEL label);
	static void Stop(void);

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	//*****************************************************************************
	// �T�E���h���̍\���̒�`
	//*****************************************************************************
	struct SOUNDINFO
	{
		char *m_pFilename;	// �t�@�C����
		int m_nCntLoop;		// ���[�v�J�E���g
	};

	IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y
	static SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX];
};


#endif