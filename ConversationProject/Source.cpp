#define _CRT_SECURE_NO_WARNINGS

#include "DxLib.h"
#include <windows.h>
#include <time.h>				// プログラム起動ごとに、生成される乱数パターンを異なるものにするため

#define NUMBER_KEY 20			// keywordAndAnswer.txtのキーワード数:20
#define NUMBER_GR 20			// greeting.txtのキーワード数:20
#define NUMBER_EN 20			// encourage.txtのキーワード数:20

// keywordAndAnswer.txtのデータを読み込むための構造体
typedef struct Respond {
	char keyword[11];			// キーワード格納用(全角5文字まで)
	char answerQ1[41];		// 疑問に対する答え1格納用(全角20文字まで)
	char answerQ2[41];		// 疑問に対する答え2格納用(全角20文字まで)
	char answerP[41];			// 肯定文に対するコメント格納用(全角20文字まで)
}Respond;

// 疑問形かどうかチェックする関数
bool checkQ(char str[]){       

	char strQ1[] = "か";		// 疑問形かどうかチェックする文字
	char strQ2[] = "?";		// 疑問形かどうかチェックする文字

	// 文字列に「か」または「?」が含まれていれば、TRUEを返す
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
	SetGraphMode(800, 600, 32);	// 画面サイズを800×600(px)に変更 

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	FILE *fpKey, *fpGr, *fpEn;		// 各テキストファイルを読み込むために用意

	Respond keyword[NUMBER_KEY];		// keywordAndAnswer.txtのキーワード格納用
									// (配列の各要素(構造体)には「keywordAndAnswer.txt」の1行分を対応させる)
	char greeting[NUMBER_GR][20];		// greeting.txtのキーワード格納用(各々のキーワードは全角10文字まで)
	char encourage[NUMBER_EN][20];		// encourage.txtのキーワード格納用(各々のキーワードは全角10文字まで)

	int numberOfKeyword = 0;			// 実際のkeywordAndAnswer.txtのキーワード数
	int numberOfGreeting = 0;			// 実際のgreeting.txtのキーワード数
	int numberOfEncourage = 0;		// 実際のencourage.txtのキーワード数

	char input[41] = "何か話しかけて!";	// 「Kistくん」に表示させる文字列格納用
	char buffInput[40];					// 一文字ずつ画面に表示させていくために用意
	buffInput[0] = '\0';

	int judge = 0;			// テキストファイルのデータを構造体の配列に読み込む際の、
							// 成功or失敗の判断用(失敗した場合はエラーとしてEOF(-1))
	int frameCount = 0;		// 前回のユーザー入力終了(Enter入力)からのフレーム数格納用
	int random = 0;			// ランダム値格納用
	int charCount = 0;
	int elementCount = 0;

	int greetingFlag = 0;
	int flag = 0;
	int EndFlag = 0;
	int shakeFlag = 0;

	// キー入力ハンドル用
	int InputHandle;

	// 色コード取得
	int Red = GetColor(255, 0, 0);
	int White = GetColor(255, 255, 255);
	int Black = GetColor(0, 0, 0);

	// 配列inputの要素から終端文字'\0'の位置取得のときに使用
	char endChar = '\0';
	char element = '@';

	// ランダム値生成用
	srand((unsigned int)time(NULL));

	fpKey = fopen("keywordAndAnswer.txt", "r");
	fpGr = fopen("greeting.txt", "r");
	fpEn = fopen("encourage.txt", "r");

	// ファイルがオープンできない場合の処理
	if ((fpKey == NULL) | (fpGr == NULL) | (fpEn == NULL)){
		DrawFormatString(140, 200, Red, "ファイルをオープンできませんでした。\n");
		return 1;
	}

	// 各テキストファイルのデータを構造体の各配列に読み込む(データの数も取得)
	for (int i = 0; i < NUMBER_KEY; i++){
		judge = fscanf(fpKey, "%s %s %s %s", 
					  keyword[i].keyword, keyword[i].answerQ1, keyword[i].answerQ2, keyword[i].answerP);	// 1バイト分自動的に減らされて取り込まれる
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

	// 画像入力ハンドル
	int Handle;    
	Handle = LoadGraph("char1.png");			// 画像をロード

	int Handle1;
	Handle1 = LoadGraph("background.jpg");		// 画像をロード

	// キー入力ハンドル(キャンセルなし全角文字有り数値入力なし)
	InputHandle = MakeKeyInput(50, FALSE, FALSE, FALSE);

	// ループ開始
	while (ProcessMessage() == 0)
	{

		greetingFlag = 0;		// greetingFlagが 1 で戻ってきたときに 0 で初期化

		ClearDrawScreen();

		DrawGraph(0, 0, Handle1, TRUE);

		if (shakeFlag == 1){
			DrawGraph(166 + 10 * (frameCount % 2), 20, Handle, TRUE);

		}
		else DrawGraph(170, 20, Handle, TRUE);

		DrawString(150, 500, "あなた:", Black);
		

		if (flag == 0){

			SetActiveKeyInput(InputHandle);
			flag = 1;

		}

		if (flag == 1){

			// 配列inputの要素から終端文字'\0'の位置取得(1からスタート)

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

			DrawFormatString(510, 150, Black, "KISTくん: %s", buffInput);

			if (EndFlag == 1){

				if (charCount == elementCount - 2){

					ScreenFlip();
					Sleep(2000);
					break;
				}
			}

			// 入力途中の文字列を描画
			DrawKeyInputString(210, 500, InputHandle);
			SetKeyInputStringColor(Black, FALSE, White, FALSE, FALSE, White, FALSE, FALSE, FALSE, FALSE);
			frameCount++;

			if (frameCount % 200 == 0){
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
				frameCount = 0;
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
								if (strstr(input, keyword[i].keyword) != NULL)
								{
									strcpy(input, keyword[i].answerQ1);
									break;
								}

								if (i == numberOfKeyword - 1)strcpy(input, "言いたくないな・・・。");

							}
						}

						else {
							for (int i = 0; i < numberOfKeyword; i++){
								if (strstr(input, keyword[i].keyword) != NULL)
								{
									strcpy(input, keyword[i].answerP);
									break;
								}

								if (i == numberOfKeyword - 1)strcpy(input, "それについては興味ない。");
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
