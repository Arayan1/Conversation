#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define NUMBER_GEN 10			// keywordAndAnswer.txt�̃L�[���[�h��:20
#define NUMBER_GR 20			// greeting.txt�̃L�[���[�h��:20
#define NUMBER_EN 20			// encourage.txt�̃L�[���[�h��:20

typedef struct Respond {		// �L�[���[�h�ɉ����āA�^��ɑ΂��铚���ƍm�蕶�ɑ΂���R�����g��p��
	char keyword[11];			// �S�p5�����܂œ��͉�
	char answerQ1[41];		// �S�p20�����܂œ��͉�
	char answerQ2[31];		// �S�p15�����܂œ��͉�
	char answerP[31];			// �S�p15�����܂œ��͉�
}Respond;

bool checkQ(char str[]){		// �^��`���ǂ����`�F�b�N����֐�

	char strQ1[] = "��";		// �^��`���ǂ����`�F�b�N���镶��
	char strQ2[] = "?";		// �^��`���ǂ����`�F�b�N���镶��

	//��v���Ȃ��ꍇ��NULL
	if ((strstr(str, strQ1) != NULL) | (strstr(str, strQ2) != NULL)) return TRUE;
	else return FALSE;
}


int main(void){

	FILE *fpGen, *fpGr, *fpEn;

	Respond general[NUMBER_GEN];
	char greeting[NUMBER_GR][20];
	char encourage[NUMBER_EN][20];

	int numberOfKeyword = 0;
	int numberOfGreeting = 0;
	int numberOfEncourage = 0;

	char input[50];

	int judge = 0;
	int greetingFlag = 0;
	int count = 0;
	int counter = 0;
	int random = 0;

	// �����_���l�����p
	srand((unsigned int)time(NULL));

	fpGen = fopen("keywordAndAnswer.txt", "r");
	fpGr = fopen("greeting.txt", "r");
	fpEn = fopen("encourage.txt", "r");

	if ((fpGen == NULL) | (fpGr == NULL)){
		printf("�t�@�C�����I�[�v���ł��܂���ł����B\n");
		return 1;
	}

	// �t�@�C���̃f�[�^���\���̂̔z��ɓǂݍ���
	for (int i = 0; i < NUMBER_GEN; i++){
		judge = fscanf(fpGen, "%s %s %s %s", general[i].keyword, general[i].answerQ1, general[i].answerQ2, general[i].answerP);		// 1�o�C�g�������I�Ɍ��炳��Ď�荞�܂��
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

	/*
	// �p�ӂ���Ă���f�[�^�̊m�F�p�ɕ\��
	for (int i = 0; i < numberOfKeyword; i++){
		printf("%s %s %s %s\n", general[i].keyword, general[i].answerQ1, general[i].answerQ2, general[i].answerP);
	}

	for (int i = 0; i < numberOfGreeting; i++){
	printf("%s\n", greeting[i]);
	}

	for (int i = 0; i < numberOfEncourage; i++){
	printf("%s\n", encourage[i]);
	}
	*/

	while (1){

		greetingFlag = 0;		// greetingFlag�� 1 �Ŗ߂��Ă����Ƃ��� 0 �ŏ�����
		count = 0;
		counter = 0;

		printf("���Ȃ�: ");

		while (1){
			if (_kbhit()){
				gets(input);
				break;
			}
			count++;
			if (count % 600000 == 0){
				random = rand() % numberOfEncourage;	// 0 �` (numberOfEncourage-1)�̊Ԃ̃����_���l����
				printf("\nKIST����: %s\n���Ȃ�: ", encourage[random]);
			}
		}


		for (int i = 0; i < numberOfGreeting; i++){
			if (strstr(input, greeting[i]) != NULL)
			{
				printf("KIST����: %s\n", input);
				greetingFlag = 1;
				break;
			}
		}

		if (greetingFlag == 0){

			if (strstr(input, "bye") != NULL) {
				printf("KIST����: bye-bye! (^-^)/~~ \n");
				break;
			}

			if (checkQ(input)){

				for (int i = 0; i < numberOfKeyword; i++){
					if (strstr(input, general[i].keyword) != NULL)
					{
						printf("KIST����: %s\n", general[i].answerQ1);
						break;
					}
					counter++;
				}
				if (counter == numberOfKeyword)printf("KIST����: �E�E�E�B\n");
			}

			else {
				for (int i = 0; i < numberOfKeyword; i++){
					if (strstr(input, general[i].keyword) != NULL)
					{
						printf("KIST����: %s\n", general[i].answerP);
						break;
					}
					counter++;
				}
				if (counter == numberOfKeyword)printf("KIST����: �E�E�E�B\n");
			}
		}
	}

	fclose(fpGen);
	fclose(fpGr);

	return 0;

}