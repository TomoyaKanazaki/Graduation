//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : MORIKAWA SHUNYA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�N���X
//*****************************************************************************
class CSound
{
public:
	//*****************************************************************************
	// �T�E���h�ꗗ
	//*****************************************************************************
	typedef enum
	{
		SOUND_LABEL_BGM_GAME = 0,	// �Q�[��BGM
		SOUND_LABEL_BGM_TUTORIAL,	// �`���[�g���A��BGM
		SOUND_LABEL_BGM_TITLE,		// �^�C�g��BGM
		SOUND_LABEL_BGM_WIN,		// ����BGM
		SOUND_LABEL_BGM_LOSE,		// �s�kBGM
		SOUND_LABEL_SE_WALK,		// �v���C���[�̕��s��SE
		SOUND_LABEL_SE_HIT,			// �v���C���[�q�b�g
		SOUND_LABEL_SE_ENEMYHIT,	// �G�q�b�g
		SOUND_LABEL_SE_ENTER,	// �G�q�b�g
		SOUND_LABEL_SE_JUMP,	// �W�����v
		SOUND_LABEL_SE_ROWLING,	// ��]
		SOUND_LABEL_SE_CHANGE,	// ��]
		SOUND_LABEL_SE_CLASH,	// �j��
		SOUND_LABEL_SE_REVERSE,	// �j��
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//*****************************************************************************
	// �T�E���h���̍\���̒�`
	//*****************************************************************************
	typedef struct
	{
		const char *m_pFilename;	// �t�@�C����
		int m_nCntLoop;		// ���[�v�J�E���g
	} SOUNDINFO;

	CSound();	//�R���X�g���N�^
	~CSound();	//�f�X�g���N�^

	//�����o�֐�
	HRESULT Init(HWND hWnd);		//������
	void Uninit(void);				//�I��
	HRESULT PlaySound(SOUND_LABEL label);	//�Đ�
	void Stop(SOUND_LABEL label);			//��~
	void Stop(void);	//���ׂĒ�~

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	//�����o�ϐ�
	IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y

	// �T�E���h�̏��
	SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
	{
		{ "data\\BGM\\game.wav", -1 },		//�Q�[��BGM
		{ "data\\BGM\\tutorial.wav", -1 },	//�`���[�g���A��BGM
		{ "data\\BGM\\title.wav", -1 },		//�^�C�g��BGM
		{ "data\\BGM\\win.wav", 0 },		//����BGM
		{ "data\\BGM\\lose.wav", 0 },		//�s�kBGM
		{ "data\\SE\\walk02.wav", 0 },		//���s��
		{ "data\\SE\\dmg_01.wav", 0 },		//�v���C���[�̃q�b�g
		{ "data\\SE\\EnemyDamage.wav", 0 },		//�G�q�b�g
		{ "data\\SE\\Enter.wav", 0 },		//�G�q�b�g
		{ "data\\SE\\se_jump.wav", 0 },		//�G�q�b�g
		{ "data\\SE\\se_drift.wav", 0 },		//�G�q�b�g
		{ "data\\SE\\change.wav", 0 },		//�G�q�b�g
		{ "data\\SE\\clash.wav", 0 },		//�G�q�b�g
		{ "data\\SE\\reverse.wav", 0 },		//�G�q�b�g
	};
};

#endif
