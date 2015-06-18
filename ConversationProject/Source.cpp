#define _CRT_SECURE_NO_WARNINGS

#include "DxLib.h"
#include <windows.h>
#include <time.h>

#define NUMBER_GEN 10			// keywordAndAnswer.txt�̃L�[���[�h��:20
#define NUMBER_GR 20			// greeting.txt�̃L�[���[�h��:20
#define NUMBER_EN 20			// encourage.txt�̃L�[���[�h��:20


typedef struct Respond {
	char keyword[11];
	char answerQ1[41];
	char answerQ2[41];
	char answerP[41];
}Respond;

bool checkQ(char str[]){        // �^��`���ǂ����`�F�b�N����֐�

	char strQ1[] = "��"; // �^��`���ǂ����`�F�b�N���镶��
	char strQ2[] = "?"; // �^��`���ǂ����`�F�b�N���镶��

	//��v���Ȃ��ꍇ��NULL
	if ((strstr(str, strQ1) != NULL) | (strstr(str, strQ2) != NULL)) return
		TRUE;
	else return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR
	lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)        // �c�w���C�u��������������
	{
		return -1;            // �G���[���N�����璼���ɏI��
	}

	SetDrawScreen(DX_SCREEN_BACK);

	FILE *fpGen, *fpGr, *fpEn;

	Respond general[NUMBER_GEN];
	char greeting[NUMBER_GR][20];
	char encourage[NUMBER_EN][20];

	int numberOfKeyword = 0;
	int numberOfGreeting = 0;
	int numberOfEncourage = 0;

	char input[41] = "�����b��������!";

	int judge = 0;
	int count = 0;
	int counter = 0;
	int random = 0;

	int greetingFlag = 0;
	int flag = 0;
	int EndFlag = 0;
	int shakeFlag = 0;

	// �L�[���̓n���h���p
	int InputHandle;
	int Red = GetColor(255, 0, 0);
	int White = GetColor(255, 255, 255);

	// �����_���l�����p
	srand((unsigned int)time(NULL));

	fpGen = fopen("keywordAndAnswer.txt", "r");
	fpGr = fopen("greeting.txt", "r");
	fpEn = fopen("encourage.txt", "r");


	if ((fpGen == NULL) | (fpGr == NULL)){
		DrawFormatString(140, 200, Red, "�t�@�C�����I�[�v���ł��܂���ł����B\n");
		return 1;
	}


	// �t�@�C���̃f�[�^���\���̂̔z��ɓǂݍ���
	for (int i = 0; i < NUMBER_GEN; i++){
		judge = fscanf(fpGen, "%s %s %s %s", general[i].keyword,
			general[i].answerQ1, general[i].answerQ2, general[i].answerP);        // 1�o�C�g�������I�Ɍ��炳��Ď�荞�܂��
		if (judge == -1) break;
		numberOfKeyword++;
	}

	// �t�@�C���̃f�[�^��z��ɓǂݍ���
	for (int i = 0; i < NUMBER_GR; i++){
		judge = fscanf(fpGr, "%s", greeting[i]);
		if (judge == -1) break;
		numberOfGreeting++;
	}

	for (int i = 0; i < NUMBER_EN; i++){
		judge = fscanf(fpEn, "%s", encourage[i]);
		if (judge == -1) break;
		numberOfEncourage++;
	}

	int Handle;     // �摜���̓n���h���p
	Handle = LoadGraph("Koala.jpg"); // �摜�����[�h

	// �L�[���̓n���h�������(�L�����Z���Ȃ��S�p�����L�萔�l���͂���Ȃ�)
	InputHandle = MakeKeyInput(50, FALSE, FALSE, FALSE);

	while (ProcessMessage() == 0)
	{

		greetingFlag = 0; // greetingFlag�� 1 �Ŗ߂��Ă����Ƃ��� 0 �ŏ�����

		ClearDrawScreen();

		if (shakeFlag == 1){
			DrawGraph(166 + 10 * (count % 2), 20, Handle, TRUE);

		}
		else DrawGraph(170, 20, Handle, TRUE);

		DrawString(170, 400, "���Ȃ�:", White);

		if (flag == 0){

			SetActiveKeyInput(InputHandle);
			flag = 1;

		}

		if (flag == 1){

			CheckKeyInput(InputHandle);

			DrawFormatString(170, 280, Red, "KIST����: %s", input);



			if (EndFlag == 1)
			{
				ScreenFlip();
				Sleep(2000);
				break;
			}

			// ���͓r���̕������`��
			DrawKeyInputString(230, 400, InputHandle);

			count++;

			if (count % 200 == 0){
				random = rand() % numberOfEncourage;	// 0 �` (numberOfEncourage-1)�̊Ԃ̃����_���l����
				strcpy(input, encourage[random]);
				if ((random == 2) | (random == 4)){
					Handle = LoadGraph("angryKoala.jpg");
					shakeFlag = 1;
				}
				else {
					Handle = LoadGraph("Koala.jpg");
					shakeFlag = 0;
				}

			}

			// ���͂��I�����Ă���ꍇ�͏I��
			if (CheckKeyInput(InputHandle) != 0){

				flag = 0;
				count = 0;
				Handle = LoadGraph("Koala.jpg");

				// ���͂��ꂽ��������擾
				strcpy(input, "");
				GetKeyInputString(input, InputHandle);

				// �p�ς݂̃C���v�b�g�n���h�����폜����
				DeleteKeyInput(InputHandle);

				// �L�[���̓n���h�������(�L�����Z���Ȃ��S�p�����L�萔�l���͂���Ȃ�)
				InputHandle = MakeKeyInput(50, FALSE, FALSE, FALSE);


				for (int i = 0; i < numberOfGreeting; i++){
					if (strstr(input, greeting[i]) != NULL)
					{
						greetingFlag = 1;
						break;
					}
				}

				if (greetingFlag == 0){

					if (strstr(input, "bye") != NULL) {
						char bye[41] = "�o�C�o�`�C!";
						strcpy(input, bye);
						Handle = LoadGraph("byeKoala.jpg");
						EndFlag = 1;
					}
					if (EndFlag != 1){
						if (checkQ(input)){

							for (int i = 0; i < numberOfKeyword; i++){
								if (strstr(input, general[i].keyword) != NULL)
								{
									strcpy(input, general[i].answerQ1);
									break;
								}
								counter++;
							}

							if (counter == numberOfKeyword)strcpy(input, "�E�E�E�B");
						}

						else {
							for (int i = 0; i < numberOfKeyword; i++){
								if (strstr(input, general[i].keyword) != NULL)
								{
									strcpy(input, general[i].answerP);
									break;
								}
								counter++;
							}

							if (counter == numberOfKeyword)strcpy(input, "�E�E�E�B");
						}
					}
				}
			}
		}

		ScreenFlip();
	}

	fclose(fpGen);
	fclose(fpGr);
	fclose(fpEn);

	DxLib_End();

	return 0;
}
