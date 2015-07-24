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

	char input[41] = "�����b�������āI";	// �uKist����v�ɕ\�������镶����i�[�p
	char buffInput[40];					// �ꕶ������ʂɕ\�������Ă������߂ɗp��
	buffInput[0] = '\0';

	int judge = 0;			// �e�L�X�g�t�@�C���̃f�[�^���\���̂̔z��ɓǂݍ��ލۂ́A
							// ����or���s�̔��f�p(���s�����ꍇ�̓G���[�Ƃ���EOF(-1))
	int frameCount = 0;		// �O��̃��[�U�[���͏I��(Enter����)����̃t���[�����i�[�p
	int random = 0;			// �����_���l�i�[�p
	int charCount = 0;
	int elementCount = 0;

	int greetingFlag = 0;
	int flag = 0;		// ���[�U�[�̓��͂��I���������ǂ������f���邽�߂ɗp��(���͒�flag�́u1�v)
	int EndFlag = 0;		// bye���܂ޕ����񂪓��͂��ꂽ�Ƃ�EndFlag���u1�v�Ƃ��I������
	int shakeFlag = 0;
	int leftMoveFlag = 0; //�Z���̕\�������炷�t���O

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
	Handle = LoadGraph("chardefo.png");			// Kist����摜�����[�h

	int Handle1;
	Handle1 = LoadGraph("background.jpg");		// �w�i�摜�����[�h

	int Handle2;
	Handle2 = LoadGraph("waku.png");		// �g�����[�h

	int Handle3;
	Handle3 = LoadGraph("hukidasi.png");		// �ӂ����������[�h

	// �L�[���̓n���h��(�L�����Z���Ȃ��S�p�����L�萔�l���͂Ȃ�)
	InputHandle = MakeKeyInput(50, FALSE, FALSE, FALSE);

	// ���[�v�J�n
	while (ProcessMessage() == 0)
	{

		greetingFlag = 0;		// greetingFlag�� 1 �Ŗ߂��Ă����Ƃ��� 0 �ŏ�����

		ClearDrawScreen();

		// �w�i�`��
		DrawGraph(0, 0, Handle1, TRUE);

		DrawGraph(0, 0, Handle3, TRUE);


		if (flag == 0){

			SetActiveKeyInput(InputHandle);
			flag = 1;

		}

		// ���[�U�[���͒�(���͑҂�)�̂Ƃ�
		if (flag == 1){

			// �z��input�̗v�f����I�[����'\0'�̈ʒu�擾(1����X�^�[�g)
			while (endChar != element){

				element = input[elementCount];
				elementCount++;
			}

			// �z��input����z��buffInput��1����(�S�p)���ǉ����Ă���
			if ((frameCount % 6 == 0) && (charCount <= elementCount - 3)){

				buffInput[charCount] = input[charCount];
				buffInput[charCount + 1] = input[charCount + 1];
				buffInput[charCount + 2] = '\0';

				charCount += 2;
			}


				DrawFormatString(540, 75, Black, "%s", buffInput);


			// shakeFlag���u1�v�̂Ƃ��́AKist��������E�ɗh�炷
			if (shakeFlag == 1){
				if ((frameCount % 8) <= 3){
					
					DrawGraph(166, 20, Handle, TRUE);
					DrawGraph(0, 18, Handle2, TRUE);
					DrawString(150, 500, "���Ȃ�:", Black);
				}
				else {
					
					DrawGraph(176, 20, Handle, TRUE);
					DrawGraph(0, 18, Handle2, TRUE);
					DrawString(150, 500, "���Ȃ�:", Black);

				}
			}
			else {
				DrawGraph(170, 20, Handle, TRUE);// shakeFlag���u0�v�Ƃ��͒ʏ�`��(�Î~��)
				DrawGraph(0, 18, Handle2, TRUE);
				DrawString(150, 500, "���Ȃ�:", Black);
			}

			// �I������
			if (EndFlag == 1){
				if (charCount == elementCount-1){

					Sleep(2000);
					break;
				}
			}

			

			// ���[�U�[�̓��͓r���̕������`��
			DrawKeyInputString(210, 500, InputHandle);
			SetKeyInputStringColor(Black, FALSE, White, FALSE, FALSE, White, FALSE, FALSE, FALSE, FALSE);
			frameCount++;

			// ��莞�ԕԓ����Ȃ��ƁA��b�𑣂��������\��������
			if (frameCount % 200 == 0){
				random = rand() % numberOfEncourage;	// 0 �` (numberOfEncourage-1)�̊Ԃ̃����_���l����
				strcpy(input, encourage[random]);		// ��b�𑣂��������input�Ɋi�[
				
				// �e�ϐ���������
				charCount = 0;
				element = '@';
				elementCount = 0;

				// encourage.txt�ɂ����āA3�Ԗڂ�5�Ԗڂɏ����ꂽ������(�u�g�[�N�g�[�N!�v�Ɓu����������b���I�v)�ɑ΂��ẮA
				// �uangry.png�v�̉摜��Ή������AshakeFlag���u1�v�Ƃ���
				if ((random == 2) | (random == 4)){
					Handle = LoadGraph("angry.png");
					shakeFlag = 1;
				}
				else {
					Handle = LoadGraph("chardefo.png");
					shakeFlag = 0;
				}

			}

			// ���͂��I�������Ƃ�
			if (CheckKeyInput(InputHandle) != 0){

				// �e�ϐ��܂��͔z���������
				element = '@';
				flag = 0;
				shakeFlag = 0;
				frameCount = 0;
				charCount = 0;
				elementCount = 0;
				Handle = LoadGraph("chardefo.png");

				// ���͂��ꂽ�������input�Ɋi�[
				strcpy(input, "");
				GetKeyInputString(input, InputHandle);

				// �p�ς݂̃C���v�b�g�n���h�����폜����
				DeleteKeyInput(InputHandle);

				// �L�[���̓n���h�������(�L�����Z���Ȃ��S�p�����L�萔�l���͂Ȃ�)
				InputHandle = MakeKeyInput(50, FALSE, FALSE, FALSE);

				// ���[�U�[�ɂ���ē��͂��ꂽ������(input)�̒��ɁAgreeting.txt�ɏ����ꂽ�����񂪑��݂��邩�T��(�����greetingFlag���u1�v�Ƃ���)
				for (int i = 0; i < numberOfGreeting; i++){
					if (strstr(input, greeting[i]) != NULL)
					{
						greetingFlag = 1;
						break;
					}
				}

				if (greetingFlag == 0){					

					// ���͂��ꂽ������Ɂubye�v�̕����񂪂���΁A�z��input�Ɂu�o�C�o�C�E�E�E�v���i�[���A�摜sad.png�����[�h����
					if (strstr(input, "bye") != NULL) {
						char bye[41] = "�o�C�o�C�E�E�E";
						strcpy(input, bye);
						Handle = LoadGraph("sad.png");
						EndFlag = 1;
					}

					// ���͂��ꂽ������Ɂubye�v�̕����񂪂Ȃ��Ƃ�
					if (EndFlag != 1){

						// ���͂��ꂽ���̂��^��`���ǂ����`�F�b�N
						if (checkQ(input)){

							// ���[�U�[�ɂ���ē��͂��ꂽ������(input)�̒��ɁAkeywordAndAnswer.txt�ɏ����ꂽ�L�[���[�h�����݂��邩�T��
							for (int i = 0; i < numberOfKeyword; i++){
								if (strstr(input, keyword[i].keyword) != NULL)
								{
									if (strstr(input,"�Z��")){
										leftMoveFlag = 1;
									}

									strcpy(input, keyword[i].answerQ1);		// ���̃L�[���[�h�ɑΉ�����A�^��`�ɑ΂��铚��(answerQ1)��z��input�Ɋi�[
									
									switch (i){

									case 1:
									case 2: Handle = LoadGraph("happy.png");
											break;
									case 3:
									case 4:
									case 5: Handle = LoadGraph("sad.png");
											break;
									}
									
									break;
								}

								// ���[�U�[�ɂ���ē��͂��ꂽ������̒��ɁAkeywordAndAnswer.txt�ɏ����ꂽ�L�[���[�h�����݂��Ȃ��Ƃ�
								// �u���������Ȃ��ȁE�E�E�v��input�Ɋi�[���A��������킷
								if (i == numberOfKeyword - 1)strcpy(input, "���������Ȃ��ȁE�E�E�B");

							}
						}

						// ���͂��ꂽ���̂��^��`�łȂ��Ƃ�
						else {

							// ���[�U�[�ɂ���ē��͂��ꂽ������(input)�̒��ɁAkeywordAndAnswer.txt�ɏ����ꂽ�L�[���[�h�����݂��邩�T��
							for (int i = 0; i < numberOfKeyword; i++){
								if (strstr(input, keyword[i].keyword) != NULL)
								{
									strcpy(input, keyword[i].answerP);			// ���̃L�[���[�h�ɑΉ�����A�m��`�ɑ΂��铚��(answerP)��z��input�Ɋi�[
									
									if ((i == 1) | (i == 2) | (i == 8)){
										Handle = LoadGraph("happy.png");
									}

									break;
								}

								// ���[�U�[�ɂ���ē��͂��ꂽ������̒��ɁAkeywordAndAnswer.txt�ɏ����ꂽ�L�[���[�h�����݂��Ȃ��Ƃ�
								// �u�E�E�E�B�v��input�Ɋi�[���A����܂�����ߍ���
								if (i == numberOfKeyword - 1)strcpy(input, "�E�E�E�B");
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
