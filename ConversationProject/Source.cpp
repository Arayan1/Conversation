#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define NUMBER_GEN 10			// keywordAndAnswer.txtのキーワード数:20
#define NUMBER_GR 20			// greeting.txtのキーワード数:20
#define NUMBER_EN 20			// encourage.txtのキーワード数:20

typedef struct Respond {		// キーワードに応じて、疑問に対する答えと肯定文に対するコメントを用意
	char keyword[11];			// 全角5文字まで入力可
	char answerQ1[41];		// 全角20文字まで入力可
	char answerQ2[31];		// 全角15文字まで入力可
	char answerP[31];			// 全角15文字まで入力可
}Respond;

bool checkQ(char str[]){		// 疑問形かどうかチェックする関数

	char strQ1[] = "か";		// 疑問形かどうかチェックする文字
	char strQ2[] = "?";		// 疑問形かどうかチェックする文字

	//一致しない場合はNULL
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

	// ランダム値生成用
	srand((unsigned int)time(NULL));

	fpGen = fopen("keywordAndAnswer.txt", "r");
	fpGr = fopen("greeting.txt", "r");
	fpEn = fopen("encourage.txt", "r");

	if ((fpGen == NULL) | (fpGr == NULL)){
		printf("ファイルをオープンできませんでした。\n");
		return 1;
	}

	// ファイルのデータを構造体の配列に読み込む
	for (int i = 0; i < NUMBER_GEN; i++){
		judge = fscanf(fpGen, "%s %s %s %s", general[i].keyword, general[i].answerQ1, general[i].answerQ2, general[i].answerP);		// 1バイト分自動的に減らされて取り込まれる
		if (judge == -1) break;
		numberOfKeyword++;
	}

	// ファイルのデータを配列に読み込む
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
	// 用意されているデータの確認用に表示
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

		greetingFlag = 0;		// greetingFlagが 1 で戻ってきたときに 0 で初期化
		count = 0;
		counter = 0;

		printf("あなた: ");

		while (1){
			if (_kbhit()){
				gets(input);
				break;
			}
			count++;
			if (count % 600000 == 0){
				random = rand() % numberOfEncourage;	// 0 ～ (numberOfEncourage-1)の間のランダム値生成
				printf("\nKISTくん: %s\nあなた: ", encourage[random]);
			}
		}


		for (int i = 0; i < numberOfGreeting; i++){
			if (strstr(input, greeting[i]) != NULL)
			{
				printf("KISTくん: %s\n", input);
				greetingFlag = 1;
				break;
			}
		}

		if (greetingFlag == 0){

			if (strstr(input, "bye") != NULL) {
				printf("KISTくん: bye-bye! (^-^)/~~ \n");
				break;
			}

			if (checkQ(input)){

				for (int i = 0; i < numberOfKeyword; i++){
					if (strstr(input, general[i].keyword) != NULL)
					{
						printf("KISTくん: %s\n", general[i].answerQ1);
						break;
					}
					counter++;
				}
				if (counter == numberOfKeyword)printf("KISTくん: ・・・。\n");
			}

			else {
				for (int i = 0; i < numberOfKeyword; i++){
					if (strstr(input, general[i].keyword) != NULL)
					{
						printf("KISTくん: %s\n", general[i].answerP);
						break;
					}
					counter++;
				}
				if (counter == numberOfKeyword)printf("KISTくん: ・・・。\n");
			}
		}
	}

	fclose(fpGen);
	fclose(fpGr);

	return 0;

}