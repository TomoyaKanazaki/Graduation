//==============================================================
//
//���C������[main.cpp]
//Author:����������
//
//==============================================================
#include "main.h"
#include "manager.h"

//�}�N����`
#define CLASS_NAME			"Window Class"			//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME			"Meteor"		//�E�C���h�E�̖��O

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawDEBUG(void);

//==============================================================
//���C���֐�
//==============================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrey, LPSTR lpCmdLine, int nCmdShow)
{
	//�I�����Ƀ��������[�N���o��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CManager *pManager = NULL;
	HWND hWnd;				//�E�C���h�E�n���h��(���ʎq)
	MSG msg;				//���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;	//���ݎ���
	DWORD dwExecLastTime;	//�Ō�ɏ�����������
	DWORD dwFrameCount;		//�t���[���J�E���g
	DWORD dwFPSLastTime;	//�Ō��FPS���v����������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,							//�E�C���h�E�̃X�^�C��
		WindowProc,							//�E�C���h�E�̃X�^�C��
		0,									//0�ɂ���i�ʏ�͎g�p���Ȃ��j
		0,									//0�ɂ���i�ʏ�͎g�p���Ȃ��j
		hInstance,							//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),		//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),			//�}�E�X�J�[�\��
		HBRUSH(COLOR_WINDOW + 1),			//�N���C�A���g�̈�̔w�i�F
		NULL,								//���j���[�o�[
		CLASS_NAME,							//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),		//�t�@�C���̃A�C�R��
	};

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,		//�g���E�C���h�E�X�^�C��
		CLASS_NAME,					//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,				//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,		//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,				//�E�C���h�E�̍���X���W
		CW_USEDEFAULT,				//�E�C���h�E�̍���Y���W
		(rect.right - rect.left),	//�E�C���h�E�̕�
		(rect.bottom - rect.top),	//�E�C���h�E�̕�
		NULL,						//�e�E�C���h�E�̃n���h��
		NULL,						//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,					//�C���X�^���X�n���h��
		NULL);

	//�}�l�[�W���̐���
	if (pManager == NULL)
	{//���������g�p����ĂȂ�������

		pManager = new CManager;
	}

	if (pManager != NULL)
	{//���������m�ۂł��Ă���

#if _DEBUG
		if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
#else
		if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
#endif
		{//���������������s�����ꍇ

			return -1;
		}
	}

	//����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//����������
	dwExecLastTime = timeGetTime();		//���ݎ������擾�i�ۑ��j
	dwFrameCount = 0;					//����������
	dwFPSLastTime = timeGetTime();		//���ݎ������擾�i�ۑ��j

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);			//�E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);					//�N���C�A���g�̈���X�V

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���

			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);		//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕Ԋ�
				DispatchMessage(&msg);		//�E�C���h�E�v�����[����փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̏���

			dwCurrentTime = timeGetTime();	//���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��

				//FPS���v��
				pManager->SetFps((dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime));

				dwFPSLastTime = dwCurrentTime;		//FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					//�t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��

				dwExecLastTime = dwCurrentTime;		//�����J�n�̎���[���ݎ���]��ۑ�
				dwFrameCount++;						//�t���[���J�E���g�����Z

				if (pManager != NULL)
				{//���������m�ۂł��Ă���

					//�}�l�[�W���̍X�V����
					pManager->Update();

					//�}�l�[�W���̕`�揈��
					pManager->Draw();
				}

			}
		}
	}

	//�}�l�[�W���̔j��
	if (pManager != NULL)
	{//���������g�p����Ă���

		//�}�l�[�W���̏I������
		pManager->Uninit();

		//�������J��
		delete pManager;
		pManager = NULL;		//NULL���
	}

	//����\��߂�
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//==============================================================
//�E�C���h�E�v���V�[�W��
//==============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;												//�Ԃ�l���i�[
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//�E�C���h�E�̗̈�i�l�p�j

	switch (uMsg)
	{
	case WM_DESTROY:			//�E�C���h�E�j���̃��b�Z�[�W

		//WM_QUIT���b�Z�[�W��Ԃ�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:			//�L�[�����̃��b�Z�[�W

		switch (wParam)
		{
		case VK_ESCAPE:			//[ESC]�L�[�������ꂽ

			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:				//����{�^�������̃��b�Z�[�W

		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

		if (nID == IDYES)
		{
			//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;		//0��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//����̏�����Ԃ�
}

//==============================================================
//�f�o�b�O�\��
//==============================================================
void DrawDEBUG(void)
{
	//RECT rect = { 0,15,SCREEN_WIDTH,SCREEN_HEIGHT };
	//char aStr[256];
	//Camera *pCamera = GetCamera();

	////������ɑ��
	//wsprintf(&aStr[0], "\n---�Ə�����---\n��F[W]\n���F[S]\n���F[A]\n�E�F[D]\n\n---�e---\n���ˁF[SPACE]\n\n���U���g��ʁF[Enter]\n(�Q�[����ʂ�)");

	////�e�L�X�g�̕`��
	//g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT,
	//	D3DCOLOR_RGBA(255, 255, 255, 255));
}