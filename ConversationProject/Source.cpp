#define _CRT_SECURE_NO_WARNINGS

#include "DxLib.h"
#include <windows.h>
#include <time.h>				// �v���O�����N�����ƂɁA��������闐���p�^�[�����قȂ���̂ɂ��邽��

#define NUMBER_KEY 20			// keywordAndAnswer.txt�̃L�[���[�h��:20
#define NUMBER_GR 20			// greeting.txt�̃L�[���[�h��:20
#define NUMBER_EN 20			// encourage.txt�̃L�[���[�h��:20

// keywordAndAnswer.txt�̃f�[�^��ǂݍ��ނ��߂̍\����
typedef struct Respond {
	char keyword[11];			// �L�[���[�h�i�[�p(�S�p5�����܂�)
	char answerQ1[41];		// �^��ɑ΂��铚��1�i�[�p(�S�p20�����܂�)
	char answerQ2[41];		// �^��ɑ΂��铚��2�i�[�p(�S�p20�����܂�)
	char answerP[41];			// �m�蕶�ɑ΂���R�����g�i�[�p(�S�p20�����܂�)
}Respond;

// �^��`���ǂ����`�F�b�N����֐�
bool checkQ(char str[]){       

	char strQ1[] = "��";		// �^��`���ǂ����`�F�b�N���镶��
	char strQ2[] = "?";		// �^��`���ǂ����`�F�b�N���镶��

	// ������Ɂu���v�܂��́u?�v���܂܂�Ă���΁ATRUE��Ԃ�
	if ((strstr(str, strQ1) != NULL) | (strstr(str, strQ2) != NULL)){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR
	lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);
	SetGraphMode(800, 600, 32);	// ��ʃT�C�Y��800�~600(px)�ɕύX 

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)		
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}

	SetDrawScreen(DX_SCREEN_BACK);

	FILE *fpKey, *fpGr, *fpEn;		// �e�e�L�X�g�t�@�C����ǂݍ��ނ��߂ɗp��

	Respond keyword[NUMBER_KEY];		// keywordAndAnswer.txt�̃L�[���[�h�i�[�p
									// (�z��̊e�v�f(�\����)�ɂ́ukeywordAndAnswer.txt�v��1�s����Ή�������)
	char greeting[NUMBER_GR][20];		// greeting.txt�̃L�[���[�h�i�[�p(�e�X�̃L�[���[�h�͑S�p10�����܂�)
	char encourage[NUMBER_EN][20];		// encourage.txt�̃L�[���[�h�i�[�p(�e�X�̃L�[���[�h�͑S�p10�����܂�)

	int numberOfKeyword = 0;			// ���ۂ�keywordAndAnswer.txt�̃L�[���[�h��
	int numberOfGreeting = 0;			// ���ۂ�greeting.txt�̃L�[���[�h��
	int numberOfEncourage = 0;		// ���ۂ�encourage.txt�̃L�[���[�h��

	char input[41] = "�����b��������!";	// �uKist����v�ɕ\�������镶����i�[�p
	char buffInput[40];					// �ꕶ������ʂɕ\�������Ă������߂ɗp��
	buffInput[0] = '\0';

	int judge = 0;			// �e�L�X�g�t�@�C���̃f�[�^���\���̂̔z��ɓǂݍ��ލۂ́A
							// ����or���s�̔��f�p(���s�����ꍇ�̓G���[�Ƃ���EOF(-1))
	int frameCount = 0;		// �O��̃��[�U�[���͏I��(Enter����)����̃t���[�����i�[�p
	int random = 0;			// �����_���l�i�[�p
	int charCount = 0;
	int elementCount = 0;

	int greetingFlag = 0;
	int flag = 0;
	int EndFlag = 0;
	int shakeFlag = 0;

	// �L�[���̓n���h���p
	int InputHandle;

	// �F�R�[�h�擾
	int Red = GetColor(255, 0, 0);
	int White = GetColor(255, 255, 255);
	int Black = GetColor(0, 0, 0);

	// �z��input�̗v�f����I�[����'\0'�̈ʒu�擾�̂Ƃ��Ɏg�p
	char endChar = '\0';
	char element = '@';

	// �����_���l�����p
	srand((unsigned int)time(NULL));

	fpKey = fopen("keywordAndAnswer.txt", "r");
	fpGr = fopen("greeting.txt", "r");
	fpEn = fopen("encourage.txt", "r");

	// �t�@�C�����I�[�v���ł��Ȃ��ꍇ�̏���
	if ((fpKey == NULL) | (fpGr == NULL) | (fpEn == NULL)){
		DrawFormatString(140, 200, Red, "�t�@�C�����I�[�v���ł��܂���ł����B\n");
		return 1;
	}

	// �e�e�L�X�g�t�@�C���̃f�[�^���\���̂̊e�z��ɓǂݍ���(�f�[�^�̐����擾)
	for (int i = 0; i < NUMBER_KEY; i++){
		judge = fscanf(fpKey, "%s %s %s %s", 
					  keyword[i].keyword, keyword[i].answerQ1, keyword[i].answerQ2, keyword[i].answerP);	// 1�o�C�g�������I�Ɍ��炳��Ď�荞�܂��
		if (judge == -1) break;
		numberOfKeyword++;
	}

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

	// �摜���̓n���h��
	int Handle;    
	Handle = LoadGraph("char1.png");			// �摜�����[�h

	int Handle1;
	Handle1 = LoadGraph("background.jpg");		// �摜�����[�h

	// �L�[���̓n���h��(�L�����Z���Ȃ��S�p�����L�萔�l���͂Ȃ�)
	InputHandle = MakeKeyInput(50, FALSE, FALSE, FALSE);

	// ���[�v�J�n
	while (ProcessMessage() == 0)
	{

		greetingFlag = 0;		// greetingFlag�� 1 �Ŗ߂��Ă����Ƃ��� 0 �ŏ�����

		ClearDrawScreen();

		DrawGraph(0, 0, Handle1, TRUE);

		if (shakeFlag == 1){
			DrawGraph(166 + 10 * (frameCount % 2), 20, Handle, TRUE);

		}
		else DrawGraph(170, 20, Handle, TRUE);

		DrawString(150, 500, "���Ȃ�:", Black);
		

		if (flag == 0){

			SetActiveKeyInput(InputHandle);
			flag = 1;

		}

		if (flag == 1){

			// �z��input�̗v�f����I�[����'\0'�̈ʒu�擾(1����X�^�[�g)

			while (endChar != element){

				element = input[elementCount];

				elementCount++;

			}

			CheckKeyInput(InputHandle);


			if ((frameCount % 6 == 0) && (charCount <= elementCount - 2)){

				buffInput[charCount] = input[charCount];
				buffInput[charCount + 1] = input[charCount + 1];
				buffInput[charCount + 2] = '\0';

				charCount += 2;
			}

			DrawFormatString(510, 150, Black, "KIST����: %s", buffInput);

			if (EndFlag == 1){

				if (charCount == elementCount - 2){

					ScreenFlip();
					Sleep(2000);
					break;
				}
			}

			// ���͓r���̕������`��
			DrawKeyInputString(210, 500, InputHandle);
			SetKeyInputStringColor(Black, FALSE, White, FALSE, FALSE, White, FALSE, FALSE, FALSE, FALSE);
			frameCount++;

			if (frameCount % 200 == 0){
				random = rand() % numberOfEncourage;	// 0 �` (numberOfEncourage-1)�̊Ԃ̃����_���l����
				strcpy(input, encourage[random]);
				charCount = 0;
				element = '@';
				elementCount = 0;
				if ((random == 2) | (random == 4)){
					Handle = LoadGraph("angryKoala.jpg");
					shakeFlag = 1;
				}
				else {
					Handle = LoadGraph("char1.png");
					shakeFlag = 0;
				}

			}

			// ���͂��I�����Ă���ꍇ�͏I��
			if (CheckKeyInput(InputHandle) != 0){

				strstr(buffInput, "@");
				element = '@';
				flag = 0;
				frameCount = 0;
				charCount = 0;
				elementCount = 0;
				Handle = LoadGraph("char1.png");

				// ���͂��ꂽ��������擾
				strcpy(input, "");
				GetKeyInputString(input, InputHandle);


				// �p�ς݂̃C���v�b�g�n���h�����폜����
				DeleteKeyInput(InputHandle);


				// �L�[���̓n���h�������(�L�����Z���Ȃ��S�p�����L�萔�l���͂Ȃ�)
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
								if (strstr(input, keyword[i].keyword) != NULL)
								{
									strcpy(input, keyword[i].answerQ1);
									break;
								}

								if (i == numberOfKeyword - 1)strcpy(input, "���������Ȃ��ȁE�E�E�B");

							}
						}

						else {
							for (int i = 0; i < numberOfKeyword; i++){
								if (strstr(input, keyword[i].keyword) != NULL)
								{
									strcpy(input, keyword[i].answerP);
									break;
								}

								if (i == numberOfKeyword - 1)strcpy(input, "����ɂ��Ă͋����Ȃ��B");
							}
						}
					}
				}
			}
		}

		ScreenFlip();
	}

	fclose(fpKey);
	fclose(fpGr);
	fclose(fpEn);

	DxLib_End();

	return 0;
}
