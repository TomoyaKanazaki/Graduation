//==============================================================
//
//�����_�����O����[renderer.h]
//Author:����������
//
//==============================================================
#ifndef _RENDERER_H_		//���̃}�N����`������Ă��Ȃ�������
#define _RENDERER_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�����_���[�N���X
class CRenderer
{
public:
	CRenderer();		//�R���X�g���N�^
	~CRenderer();		//�f�X�g���N�^
	
	HRESULT Init(HWND hWnd, BOOL bWindow);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }		//�f�o�C�X�̎擾
	LPD3DXFONT GetFont(void) { return m_pFont; }		//�t�H���g�̎擾

	void SetFPS(int nFps) { m_nCountFPS = nFps; }		//FPS�̐ݒ�

private:

	void DrawFPS(void);						//FPS�̕\��

	LPDIRECT3D9 m_pD3D;						//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;			//Direct3D�f�o�C�X�ւ̃|�C���^

	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	bool m_bUpdate;			//�X�V�������邩

	LPD3DXFONT m_pFont;						//�t�H���g�ւ̃|�C���^
	int m_nCountFPS;						//FPS�J�E���^
	bool g_bDispDebug = false;				//�f�o�b�O�\����ON/OFF
};

#endif