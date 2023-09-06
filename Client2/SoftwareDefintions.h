#pragma once


#define OnMenuActionTransfer 17		

#define OnUpdateTheLobby 18
#define OnEncriptionOffOn 21
#define OnButtonSendTheDataTransfer 19
#define OnButtonAcceptedDataTransfer 20
#define OnSending 22
#define OnTake 23


#define OnMenuAction1 1																									//нажатие на клавишу меню
#define OnMenuAction2 2
#define OnMenuAction3 3


#define OnExitAction 4

#define OnButtonPlusPrivateRSA 7
#define OnButtonPlusPublicRSA 8
#define OnButtonGenerateKeysClickedRSA 5
#define OnButtonSaveKeysClickedRSA 9
#define OnButtonClearRSA 6

//*****************************************


#define OnButtonSelectTextFile 10
#define OnButtonClearTextFile 11
#define OnButtonSaveTextFile 12
#define OnButtonStart 13


//*****************************************



#define SizeBufferKeyPrivateRSA 16
#define SizeBufferKeyPublicRSA 16
#define TextBufferSize 256

char BufferKeyPrivateRSA[SizeBufferKeyPrivateRSA];
char BufferKeyPublicRSA[SizeBufferKeyPublicRSA];

//char Buffer[TextBufferSize];

char FileNameSize[256];
OPENFILENAMEA openFN;

//для Data transfer странички
//************************************************************************************************************************************
HWND hButtonSending, hButtonTake, hButtonServerMode, hButtonSendTheData, hButtonAcceptedData, hButtonDecryptionSelected;
HWND hStaticTextHost, hHostField, hStaticTextServer, hServerField, hButtonEncriptionOffOn, hStaticTextLobbySelection, hStaticTextEncryption, hStaticTextSendTheData, hStaticTextAcceptedData, hStaticTextTheEventLog, hStaticTextServerEventLog;
HWND SendTheDataField, AcceptedDataField, TheEventLogField, ServerEventLogField;
HWND hSelectedLobby;																										//Индикатор

//************************************************************************************************************************************

//для первой странички
//************************************************************************************************************************************
HWND hButton1, hButton2, hButton3, hButton4, hButton5, hButton6, hButton7, hButton8, hButton9;
HWND hStaticText1, hStaticText2, hStaticText3, hStaticText4, hStaticText5, hStaticText6, hStaticText7, hStaticText8, hStaticText9;
HWND hStaticTextN1, hStaticTextE, hStaticTextD;

HWND KeyField1, KeyField11, KeyField22;																								//переменная для хранения для записи в нее окна (ключи)
//************************************************************************************************************************************

//для второй странички
//************************************************************************************************************************************
HWND FileText1, FileText2;																								//переменная для хранения для записи в нее окна (текста)

HWND hButton2_1, hButton2_2, hButton2_3, hButton2_4;
HWND hStaticText2_1, hStaticText2_2;
//************************************************************************************************************************************



LRESULT CALLBACK  SoftMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);



void MainAddMenu(HWND hWnd);
void MainAddButtons(HWND hWnd);
//void SaveData(LPCSTR path);
void SaveData(LPCSTR path, HWND per);

//void LoadData(LPCSTR path);
void LoadData(LPCSTR path, HWND per);

void SetOpenFileWindow(HWND hWnd);

void EncryptionDecryption(HWND hWnd);