#define _CRT_SECURE_NO_WARNINGS

#include "DxLib.h"
#include <windows.h>
#include <time.h>

#define NUMBER_GEN 10			// keywordAndAnswer.txtのキーワード数:20
#define NUMBER_GR 20			// greeting.txtのキーワード数:20
#define NUMBER_EN 20			// encourage.txtのキーワード数:20


typedef struct Respond {
	char keyword[11];
	char answerQ1[41];
	char answerQ2[41];
	char answerP[41];
}Respond;

bool checkQ(char str[]){        // 疑問形かどうかチェックする関数

	char strQ1[] = "か"; // 疑問形かどうかチェックする文字
	char strQ2[] = "?"; // 疑問形かどうかチェックする文字

	//一致しない場合はNULL
	if ((strstr(str, strQ1) != NULL) | (strstr(str, strQ2) != NULL)) return
		TRUE;
	else return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR
	lpCmdLine, int nCmdShow)
{
	SetGraphMode(800, 600, 32);
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)        // ＤＸライブラリ初期化処理
	{
		return -1;            // エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	FILE *fpGen, *fpGr, *fpEn;

	Respond general[NUMBER_GEN];
	char greeting[NUMBER_GR][20];
	char encourage[NUMBER_EN][20];

	int numberOfKeyword = 0;
	int numberOfGreeting = 0;
	int numberOfEncourage = 0;

	char input[41] = "何か話しかけて!";
	char buffInput[40];
	buffInput[0] = '\0';

	int Cr = GetColor(255, 255, 255);
	int judge = 0;
	int count = 0;
	int counter = 0;
	int random = 0;
	int charCount = 0;
	int elementCount = 0;

	int greetingFlag = 0;
	int flag = 0;
	int EndFlag = 0;
	int shakeFlag = 0;

	// キー入力ハンドル用
	int InputHandle;
	int Red = GetColor(255, 0, 0);
	int White = GetColor(255, 255, 255);
	int Black = GetColor(0, 0, 0);

	char endChar = '\0';
	char element = '@';

	// ランダム値生成用
	srand((unsigned int)time(NULL));

	fpGen = fopen("keywordAndAnswer.txt", "r");
	fpGr = fopen("greeting.txt", "r");
	fpEn = fopen("encourage.txt", "r");


	if ((fpGen == NULL) | (fpGr == NULL)){
		DrawFormatString(140, 200, Red, "ファイルをオープンできませんでした。\n");
		return 1;
	}


	// ファイルのデータを構造体の配列に読み込む
	for (int i = 0; i < NUMBER_GEN; i++){
		judge = fscanf(fpGen, "%s %s %s %s", general[i].keyword,
			general[i].answerQ1, general[i].answerQ2, general[i].answerP);        // 1バイト分自動的に減らされて取り込まれる
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

	int Handle;     // 画像入力ハンドル用
	Handle = LoadGraph("char1.png"); // 画像をロード

	int Handle1;
	Handle1 = LoadGraph("background.jpg"); // 画像をロード

	// キー入力ハンドルを作る(キャンセルなし全角文字有り数値入力なし)
	InputHandle = MakeKeyInput(50, FALSE, FALSE, FALSE);

	while (ProcessMessage() == 0)
	{

		greetingFlag = 0; // greetingFlagが 1 で戻ってきたときに 0 で初期化

		ClearDrawScreen();

		DrawGraph(0, 0, Handle1, TRUE);

		if (shakeFlag == 1){
			DrawGraph(166 + 10 * (count % 2), 20, Handle, TRUE);

		}
		else DrawGraph(170, 20, Handle, TRUE);

		DrawString(150, 500, "あなた:", Black);
		

		if (flag == 0){

			SetActiveKeyInput(InputHandle);
			flag = 1;

		}

		if (flag == 1){

			// 配列inputの'\0'の位置取得(1からスタート)

			while (endChar != element){

				element = input[elementCount];

				elementCount++;

			}

			CheckKeyInput(InputHandle);


			if ((count % 6 == 0) && (charCount <= elementCount - 2)){

				buffInput[charCount] = input[charCount];
				buffInput[charCount + 1] = input[charCount + 1];
				buffInput[charCount + 2] = '\0';

				charCount += 2;
			}

			DrawFormatString(510, 150, Red, "KISTくん: %s", buffInput);

			if (EndFlag == 1){

				if (charCount == elementCount - 2){

					ScreenFlip();
					Sleep(2000);
					break;
				}
			}

			// 入力途中の文字列を描画
			DrawKeyInputString(210, 500, InputHandle);

			count++;

			if (count % 200 == 0){
				random = rand() % numberOfEncourage;	// 0 ～ (numberOfEncourage-1)の間のランダム値生成
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

			// 入力が終了している場合は終了
			if (CheckKeyInput(InputHandle) != 0){

				strstr(buffInput, "@");
				element = '@';
				flag = 0;
				count = 0;
				charCount = 0;
				elementCount = 0;
				Handle = LoadGraph("char1.png");

				// 入力された文字列を取得
				strcpy(input, "");
				GetKeyInputString(input, InputHandle);


				// 用済みのインプットハンドルを削除する
				DeleteKeyInput(InputHandle);


				// キー入力ハンドルを作る(キャンセルなし全角文字有り数値入力なし)
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
						char bye[41] = "バイバ～イ!";
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

							if (counter == numberOfKeyword)strcpy(input, "・・・。");
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

							if (counter == numberOfKeyword)strcpy(input, "・・・。");
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
