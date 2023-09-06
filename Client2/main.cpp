//#include <stdio.h>
#include <string>

#include "Client.h"

#include <Windows.h>
#include "SoftwareDefintions.h"
#include "resource.h"

// Для использования функций связанных с выбором папки
#include <ShlObj.h>

#include <locale>
#include <codecvt>
#include "rsa_keycreator.h"
#include "rsa_key.h"
#include "rsa_encrypt_decrypt.h"


//==== config ====//
int domain = AF_INET,
service = SOCK_STREAM,
protocol = 0,
port = 85,
backlog = 16,
maxUsers = 256;
const std::string serverHost = "192.168.0.103";
const std::string clientLogs = "./clientLogs.txt";
//================//

Andrew::FileSharingClient* client;
RSA_Encrypt_Decrypt encryptDecrypt;

bool isDataTransferTabVisiable = true;
bool isKeysTabVisible = false;
bool isEncryptionTabVisible = false;
bool isEncryptionEnabled = false;

RSA_Keys keys;

char buffer[256];
char buffer_1[256];

int currentState = 1;

int currentStateServer = SendMessage(hButtonServerMode, BM_GETCHECK, 0, 0);

LRESULT CALLBACK SoftMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
std::string getEncryptionStatusString(bool isEncryptionEnabled);
RSA_Keys generateKeys(RSA_KeyCreator& keyCreator);

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR pCmdLine, int nCmdShow) {
    std::locale::global(std::locale(""));

    client = new Andrew::FileSharingClient(
        domain,
        service,
        protocol,
        port,
        serverHost,
        clientLogs
    );
    while (!client->auth());

    UNREFERENCED_PARAMETER(hPrevInst);
    UNREFERENCED_PARAMETER(pCmdLine);

    WNDCLASS SoftMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)), L"ShelbyWindowClass", SoftMainProcedure);

    if (!RegisterClassW(&SoftMainClass)) {
        return -1;
    }

    MSG SoftMainMessage = { 0 };

    CreateWindow(L"ShelbyWindowClass", L"Local network and encryption", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 550, 500, NULL, NULL, NULL, NULL);

    while (GetMessage(&SoftMainMessage, NULL, NULL, NULL)) {
        TranslateMessage(&SoftMainMessage);
        DispatchMessage(&SoftMainMessage);
    }

    return 0;
}

WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {
    WNDCLASS NWC = { 0 };

    NWC.hIcon = Icon;
    NWC.hCursor = Cursor;
    NWC.hInstance = hInst;
    NWC.lpszClassName = Name;
    NWC.hbrBackground = BGColor;
    NWC.lpfnWndProc = Procedure;

    return NWC;
}

LRESULT CALLBACK SoftMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_COMMAND:
        switch (wp) {


        case OnMenuActionTransfer:
            isDataTransferTabVisiable = true;
            isKeysTabVisible = false;
            isEncryptionTabVisible = false;
            //**********************************************************************************
            ShowWindow(hStaticTextHost, SW_SHOW);
            ShowWindow(hHostField, SW_SHOW);
            ShowWindow(hStaticTextServer, SW_SHOW);
            ShowWindow(hServerField, SW_SHOW);
            ShowWindow(hButtonServerMode, SW_SHOW);
            ShowWindow(hStaticTextLobbySelection, SW_SHOW);
            ShowWindow(hSelectedLobby, SW_SHOW);
            ShowWindow(hStaticTextEncryption, SW_SHOW);
            ShowWindow(hButtonEncriptionOffOn, SW_SHOW);
            ShowWindow(hStaticTextSendTheData, SW_SHOW);
            ShowWindow(hButtonSendTheData, SW_SHOW);
            ShowWindow(SendTheDataField, SW_SHOW);
            ShowWindow(hStaticTextAcceptedData, SW_SHOW);
            ShowWindow(hButtonAcceptedData, SW_SHOW);
            ShowWindow(AcceptedDataField, SW_SHOW);
            ShowWindow(hStaticTextTheEventLog, SW_SHOW);
            ShowWindow(hStaticTextServerEventLog, SW_SHOW);
            ShowWindow(TheEventLogField, SW_SHOW);
            ShowWindow(ServerEventLogField, SW_SHOW);
            ShowWindow(hButtonSending, SW_SHOW);
            ShowWindow(hButtonTake, SW_SHOW);
            //**********************************************************************************
            ShowWindow(hStaticText1, SW_HIDE);
            ShowWindow(hButton1, SW_HIDE);
            ShowWindow(hStaticTextN1, SW_HIDE);
            ShowWindow(KeyField1, SW_HIDE);
            ShowWindow(hStaticTextD, SW_HIDE);
            ShowWindow(KeyField11, SW_HIDE);
            ShowWindow(hStaticText2, SW_HIDE);
            ShowWindow(hButton2, SW_HIDE);
            ShowWindow(hStaticTextE, SW_HIDE);
            ShowWindow(KeyField22, SW_HIDE);
            ShowWindow(hButton3, SW_HIDE);
            ShowWindow(hButton4, SW_HIDE);
            ShowWindow(hButton5, SW_HIDE);
            //**********************************************************************************
           

            //**********************************************************************************




            break;
        case OnMenuAction2:
            isKeysTabVisible = true;
            isDataTransferTabVisiable = false;
            isEncryptionTabVisible = false;
            //**********************************************************************************
            ShowWindow(hStaticText1, SW_SHOW);
            ShowWindow(hButton1, SW_SHOW);
            ShowWindow(hStaticTextN1, SW_SHOW);
            ShowWindow(KeyField1, SW_SHOW);
            ShowWindow(hStaticTextD, SW_SHOW);
            ShowWindow(KeyField11, SW_SHOW);
            ShowWindow(hStaticText2, SW_SHOW);
            ShowWindow(hButton2, SW_SHOW);
            ShowWindow(hStaticTextE, SW_SHOW);
            ShowWindow(KeyField22, SW_SHOW);
            ShowWindow(hButton3, SW_SHOW);
            ShowWindow(hButton4, SW_SHOW);
            ShowWindow(hButton5, SW_SHOW);
            //**********************************************************************************

            //**********************************************************************************
            ShowWindow(hStaticTextHost, SW_HIDE);
            ShowWindow(hHostField, SW_HIDE);
            ShowWindow(hStaticTextServer, SW_HIDE);
            ShowWindow(hServerField, SW_HIDE);
            ShowWindow(hButtonServerMode, SW_HIDE);
            ShowWindow(hStaticTextLobbySelection, SW_HIDE);
            ShowWindow(hSelectedLobby, SW_HIDE);
            ShowWindow(hStaticTextEncryption, SW_HIDE);
            ShowWindow(hButtonEncriptionOffOn, SW_HIDE);
            ShowWindow(hStaticTextSendTheData, SW_HIDE);
            ShowWindow(hButtonSendTheData, SW_HIDE);
            ShowWindow(SendTheDataField, SW_HIDE);
            ShowWindow(hStaticTextAcceptedData, SW_HIDE);
            ShowWindow(hButtonAcceptedData, SW_HIDE);
            ShowWindow(AcceptedDataField, SW_HIDE);
            ShowWindow(hStaticTextTheEventLog, SW_HIDE);
            ShowWindow(hStaticTextServerEventLog, SW_HIDE);
            ShowWindow(TheEventLogField, SW_HIDE);
            ShowWindow(ServerEventLogField, SW_HIDE);
            ShowWindow(hButtonSending, SW_HIDE);
            ShowWindow(hButtonTake, SW_HIDE);
            //**********************************************************************************

            break;
        case OnMenuAction3:
            isEncryptionTabVisible = true;
            isDataTransferTabVisiable = false;
            isKeysTabVisible = false;
            
            
           
            //**********************************************************************************

            //**********************************************************************************
            ShowWindow(hStaticText1, SW_HIDE);
            ShowWindow(hButton1, SW_HIDE);
            ShowWindow(hStaticTextN1, SW_HIDE);
            ShowWindow(KeyField1, SW_HIDE);
            ShowWindow(hStaticTextD, SW_HIDE);
            ShowWindow(KeyField11, SW_HIDE);
            ShowWindow(hStaticText2, SW_HIDE);
            ShowWindow(hButton2, SW_HIDE);
            ShowWindow(hStaticTextE, SW_HIDE);
            ShowWindow(KeyField22, SW_HIDE);
            ShowWindow(hButton3, SW_HIDE);
            ShowWindow(hButton4, SW_HIDE);
            ShowWindow(hButton5, SW_HIDE);
            //**********************************************************************************
            ShowWindow(hStaticTextHost, SW_HIDE);
            ShowWindow(hHostField, SW_HIDE);
            ShowWindow(hStaticTextServer, SW_HIDE);
            ShowWindow(hServerField, SW_HIDE);
            ShowWindow(hButtonServerMode, SW_HIDE);
            ShowWindow(hStaticTextLobbySelection, SW_HIDE);
            ShowWindow(hSelectedLobby, SW_HIDE);
            ShowWindow(hStaticTextEncryption, SW_HIDE);
            ShowWindow(hButtonEncriptionOffOn, SW_HIDE);
            ShowWindow(hStaticTextSendTheData, SW_HIDE);
            ShowWindow(hButtonSendTheData, SW_HIDE);
            ShowWindow(SendTheDataField, SW_HIDE);
            ShowWindow(hStaticTextAcceptedData, SW_HIDE);
            ShowWindow(hButtonAcceptedData, SW_HIDE);
            ShowWindow(AcceptedDataField, SW_HIDE);
            ShowWindow(hStaticTextTheEventLog, SW_HIDE);
            ShowWindow(hStaticTextServerEventLog, SW_HIDE);
            ShowWindow(TheEventLogField, SW_HIDE);
            ShowWindow(ServerEventLogField, SW_HIDE);
            ShowWindow(hButtonSending, SW_HIDE);
            ShowWindow(hButtonTake, SW_HIDE);
            //**********************************************************************************
            break;
        case OnButtonClearTextFile:
            SetWindowTextA(FileText1, "");
            SetWindowTextA(FileText2, "");
            break;
        case OnButtonSelectTextFile:
            if (GetOpenFileNameA(&openFN)) {
                LoadData(FileNameSize, FileText1);
            }
            break;
        case OnButtonSaveTextFile:
            if (GetSaveFileNameA(&openFN)) {
                SaveData(FileNameSize, FileText2);
            }
            break;
        case OnExitAction:
            PostQuitMessage(0);
            break;
        case OnButtonGenerateKeysClickedRSA:
        {            
            RSA_KeyCreator keyCreator(25000LL);
            keys = generateKeys(keyCreator);           

            SetWindowTextA(KeyField1, std::to_string(keys.publicKey.get_n()).c_str());
            SetWindowTextA(KeyField11, std::to_string(keys.privateKey.get_a()).c_str());
            SetWindowTextA(KeyField22, std::to_string(keys.publicKey.get_a()).c_str());
        }
        break;
        case OnButtonSaveKeysClickedRSA:
            if (GetSaveFileNameA(&openFN)) {
                SaveData(FileNameSize, KeyField1);
            }

            if (GetSaveFileNameA(&openFN)) {
                SaveData(FileNameSize, KeyField11);
            }

            if (GetSaveFileNameA(&openFN)) {
                SaveData(FileNameSize, KeyField22);
            }
            break;

        case OnSending:
        {
            GetWindowTextA(SendTheDataField, buffer, sizeof(buffer));
            std::string path(buffer);
            std::string name = fs::path(path).filename().string();
            Andrew::File file;
            file.bRead(path);
            //ну типа ключи уже сгенерированы
            if (isEncryptionEnabled)
                encryptDecrypt.Encrypt(file, keys.publicKey);

            client->uploadFile(file);
        }
        break;
        
        case OnTake:
        {
            GetWindowTextA(AcceptedDataField, buffer, sizeof(buffer));
            GetWindowTextA(hSelectedLobby, buffer_1, sizeof(buffer_1));
            std::string path(buffer);
            std::string lobbyId(buffer_1);

            Andrew::File file = client->downloadFile(atoi(lobbyId.c_str()));
            //ну типа ключи уже сгенерированы
            if (isEncryptionEnabled)
                encryptDecrypt.Decrypt(file, keys.privateKey);

            file.bWrite(path + "\\" + file.getFilename());
        }
        break;


        
        case OnEncriptionOffOn:
        {
            
            isEncryptionEnabled = !isEncryptionEnabled;
            SetWindowTextA(hStaticTextEncryption, getEncryptionStatusString(isEncryptionEnabled).c_str());
        }
        break;


        case OnUpdateTheLobby:
        {
            std::vector<Andrew::LobbyDto> lobbies = client->getLobbies();
            std::string avaliable = "";
            for (int i = 0; i < lobbies.size(); i++)
            {
                Andrew::LobbyDto& lobby = lobbies[i];
                avaliable += "id: " + std::to_string(lobby.id) + " | file: " + lobby.dataName + "\r\n";
                
            }
            SetWindowTextA(ServerEventLogField, avaliable.c_str());
        }
        break;


        case OnButtonSendTheDataTransfer:
            if (GetOpenFileNameA(&openFN)) {
                // Файл был выбран, выполните необходимые действия
                LoadData(openFN.lpstrFile, hWnd);
                SetWindowTextA(SendTheDataField, openFN.lpstrFile); // Установка адреса файла в окно SendTheDataField
            }
            break;


        case OnButtonAcceptedDataTransfer:
        {
            BROWSEINFOA browseInfo = { 0 };
            browseInfo.hwndOwner = hWnd;
            browseInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
            LPITEMIDLIST pidl = SHBrowseForFolderA(&browseInfo);

            if (pidl != NULL)
            {
                char folderPath[MAX_PATH];
                if (SHGetPathFromIDListA(pidl, folderPath))
                {
                    // Получаем адрес выбранной папки
                    SetWindowTextA(AcceptedDataField, folderPath);
                }

                CoTaskMemFree(pidl);
            }
        }
        break;

        case OnButtonPlusPrivateRSA:
            if (GetOpenFileNameA(&openFN)) {
                LoadData(FileNameSize, KeyField1);
            }
            if (GetOpenFileNameA(&openFN)) {
                LoadData(FileNameSize, KeyField11);
            }
            break;
        case OnButtonPlusPublicRSA:
            if (GetOpenFileNameA(&openFN)) {
                LoadData(FileNameSize, KeyField1);
            }
            if (GetOpenFileNameA(&openFN)) {
                LoadData(FileNameSize, KeyField22);
            }
            break;
        case OnButtonClearRSA:
            SetWindowTextA(KeyField1, "");
            SetWindowTextA(KeyField11, "");
            SetWindowTextA(KeyField22, "");
            break;
        }
        break;
    case WM_CREATE:
    {
        MainAddMenu(hWnd);
        SetOpenFileWindow(hWnd);

        //***************************************************************************************************************************************************************
        hStaticTextHost = CreateWindowA("static", "Host:", WS_VISIBLE | WS_CHILD | ES_CENTER, 200, 5, 90, 20, hWnd, NULL, NULL, NULL);
        hHostField = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_CENTER, 190, 25, 125, 20, hWnd, NULL, NULL, NULL);
        hButtonServerMode = CreateWindowA("button", "Update the lobby", WS_VISIBLE | WS_CHILD | ES_CENTER, 370, 155, 120, 20, hWnd, (HMENU)OnUpdateTheLobby, NULL, NULL);
        hStaticTextLobbySelection = CreateWindowA("static", "Choose a lobby:", WS_VISIBLE | WS_CHILD | ES_CENTER, 250, 290, 110, 20, hWnd, NULL, NULL, NULL);
        hStaticTextEncryption = CreateWindowA("static", getEncryptionStatusString(isEncryptionEnabled).c_str() , WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 60, 120, 20, hWnd, NULL, NULL, NULL);
        hButtonEncriptionOffOn = CreateWindowA("button", "On/Off", WS_VISIBLE | WS_CHILD | ES_CENTER, 135, 55, 80, 20, hWnd, (HMENU)OnEncriptionOffOn, NULL, NULL);
        hStaticTextSendTheData = CreateWindowA("static", "Choose to send:", WS_VISIBLE | WS_CHILD | ES_CENTER, 1, 80, 120, 20, hWnd, NULL, NULL, NULL);
        hButtonSendTheData = CreateWindowA("button", "+", WS_VISIBLE | WS_CHILD | ES_CENTER, 115, 78, 20, 20, hWnd, (HMENU)OnButtonSendTheDataTransfer, NULL, NULL);                             //повозиться с кнопкой!!!!!!
        SendTheDataField = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 2, 100, 200, 20, hWnd, NULL, NULL, NULL);

        hStaticTextAcceptedData = CreateWindowA("static", "Choose where to take:", WS_VISIBLE | WS_CHILD | ES_CENTER, 1, 120, 160, 20, hWnd, NULL, NULL, NULL);
        hButtonAcceptedData = CreateWindowA("button", "+", WS_VISIBLE | WS_CHILD | ES_CENTER, 155, 118, 20, 20, hWnd, (HMENU)OnButtonAcceptedDataTransfer, NULL, NULL);                          //повозиться с кнопкой!!!!!!
        AcceptedDataField = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 2, 140, 200, 20, hWnd, NULL, NULL, NULL);
        //hStaticTextTheEventLog = CreateWindowA("static", "The event log:", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 160, 100, 20, hWnd, NULL, NULL, NULL);
        hStaticTextServerEventLog = CreateWindowA("static", "Available lobbies:", WS_VISIBLE | WS_CHILD | ES_CENTER, 250, 160, 120, 20, hWnd, NULL, NULL, NULL);
        hSelectedLobby = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_CENTER, 360, 290, 125, 20, hWnd, NULL, NULL, NULL);

        //TheEventLogField = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 2, 180, 250, 240, hWnd, NULL, NULL, NULL);
        ServerEventLogField = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 250, 180, 280, 100, hWnd, NULL, NULL, NULL);

        hButtonSending = CreateWindowA("button", "Send ->", WS_VISIBLE | WS_CHILD | ES_CENTER, 230, 100, 100, 20, hWnd, (HMENU)OnSending, NULL, NULL);
        hButtonTake = CreateWindowA("button", "<- Take", WS_VISIBLE | WS_CHILD | ES_CENTER, 230, 140, 100, 20, hWnd, (HMENU)OnTake, NULL, NULL);

        //***************************************************************************************************************************************************************

        //***************************************************************************************************************************************************************
        hStaticText1 = CreateWindowA("static", "Private key:", WS_VISIBLE | WS_CHILD | ES_CENTER, 220, 5, 100, 20, hWnd, NULL, NULL, NULL);
        hButton1 = CreateWindowA("button", "+", WS_VISIBLE | WS_CHILD | ES_CENTER, 310, 5, 20, 20, hWnd, (HMENU)OnButtonPlusPrivateRSA, NULL, NULL);
        hStaticTextN1 = CreateWindowA("static", "n=", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 55, 15, 20, hWnd, NULL, NULL, NULL);
        KeyField1 = CreateWindowA("edit", "key number n", WS_VISIBLE | WS_CHILD | ES_CENTER, 25, 55, 200, 20, hWnd, NULL, NULL, NULL);
        hStaticTextD = CreateWindowA("static", "d=", WS_VISIBLE | WS_CHILD | ES_CENTER, 270, 30, 15, 20, hWnd, NULL, NULL, NULL);
        KeyField11 = CreateWindowA("edit", "key number 1 d", WS_VISIBLE | WS_CHILD | ES_CENTER, 290, 30, 200, 20, hWnd, NULL, NULL, NULL);
        hStaticText2 = CreateWindowA("static", "Public key:", WS_VISIBLE | WS_CHILD | ES_CENTER, 220, 55, 100, 20, hWnd, NULL, NULL, NULL);
        hButton2 = CreateWindowA("button", "+", WS_VISIBLE | WS_CHILD | ES_CENTER, 310, 55, 20, 20, hWnd, (HMENU)OnButtonPlusPublicRSA, NULL, NULL);
        hStaticTextE = CreateWindowA("static", "e=", WS_VISIBLE | WS_CHILD | ES_CENTER, 270, 80, 15, 20, hWnd, NULL, NULL, NULL);
        KeyField22 = CreateWindowA("edit", "key number 2 e", WS_VISIBLE | WS_CHILD | ES_CENTER, 290, 80, 200, 20, hWnd, NULL, NULL, NULL);
        hButton3 = CreateWindowA("button", "Generate keys", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 105, 100, 20, hWnd, (HMENU)OnButtonGenerateKeysClickedRSA, NULL, NULL);
        hButton4 = CreateWindowA("button", "Save keys", WS_VISIBLE | WS_CHILD | ES_CENTER, 115, 105, 100, 20, hWnd, (HMENU)OnButtonSaveKeysClickedRSA, NULL, NULL);
        hButton5 = CreateWindowA("button", "Clear", WS_VISIBLE | WS_CHILD | ES_CENTER, 225, 105, 100, 20, hWnd, (HMENU)OnButtonClearRSA, NULL, NULL);
        //***************************************************************************************************************************************************************



        //***************************************************************************************************************************************************************

        // Установка IP-адреса в поле hHostField
        if (!client->getHost().empty()) {
            SetWindowTextA(hHostField, client->getHost().c_str());
        }
        else {
            SetWindowTextA(hHostField, "127.0.0.1");                    //SetWindowTextA(hHostField, "Failed to retrieve IP address");  
        }
        //***************************************************************************************************************************************************************


        //RSA


        if (isDataTransferTabVisiable) {
            ShowWindow(hStaticTextHost, SW_SHOW);
            ShowWindow(hHostField, SW_SHOW);
            ShowWindow(hStaticTextServer, SW_SHOW);
            ShowWindow(hServerField, SW_SHOW);
            ShowWindow(hButtonServerMode, SW_SHOW);
            ShowWindow(hStaticTextLobbySelection, SW_SHOW);
            ShowWindow(hSelectedLobby, SW_SHOW);
            ShowWindow(hStaticTextEncryption, SW_SHOW);
            ShowWindow(hButtonEncriptionOffOn, SW_SHOW);
            ShowWindow(hStaticTextSendTheData, SW_SHOW);
            ShowWindow(hButtonSendTheData, SW_SHOW);
            ShowWindow(SendTheDataField, SW_SHOW);
            ShowWindow(hStaticTextAcceptedData, SW_SHOW);
            ShowWindow(hButtonAcceptedData, SW_SHOW);
            ShowWindow(AcceptedDataField, SW_SHOW);
            ShowWindow(hStaticTextTheEventLog, SW_SHOW);
            ShowWindow(hStaticTextServerEventLog, SW_SHOW);
            ShowWindow(TheEventLogField, SW_SHOW);
            ShowWindow(ServerEventLogField, SW_SHOW);
            ShowWindow(hButtonSending, SW_SHOW);
            ShowWindow(hButtonTake, SW_SHOW);
        }
        else {
            ShowWindow(hStaticTextHost, SW_HIDE);
            ShowWindow(hHostField, SW_HIDE);
            ShowWindow(hStaticTextServer, SW_HIDE);
            ShowWindow(hServerField, SW_HIDE);
            ShowWindow(hButtonServerMode, SW_HIDE);
            ShowWindow(hStaticTextLobbySelection, SW_HIDE);
            ShowWindow(hSelectedLobby, SW_HIDE);
            ShowWindow(hStaticTextEncryption, SW_HIDE);
            ShowWindow(hButtonEncriptionOffOn, SW_HIDE);
            ShowWindow(hStaticTextSendTheData, SW_HIDE);
            ShowWindow(hButtonSendTheData, SW_HIDE);
            ShowWindow(SendTheDataField, SW_HIDE);
            ShowWindow(hStaticTextAcceptedData, SW_HIDE);
            ShowWindow(hButtonAcceptedData, SW_HIDE);
            ShowWindow(AcceptedDataField, SW_HIDE);
            ShowWindow(hStaticTextTheEventLog, SW_HIDE);
            ShowWindow(hStaticTextServerEventLog, SW_HIDE);
            ShowWindow(TheEventLogField, SW_HIDE);
            ShowWindow(ServerEventLogField, SW_HIDE);
            ShowWindow(hButtonSending, SW_HIDE);
            ShowWindow(hButtonTake, SW_HIDE);
        }



        if (isKeysTabVisible) {
            ShowWindow(hStaticText1, SW_SHOW);
            ShowWindow(hButton1, SW_SHOW);
            ShowWindow(hStaticTextN1, SW_SHOW);
            ShowWindow(KeyField1, SW_SHOW);
            ShowWindow(hStaticTextD, SW_SHOW);
            ShowWindow(KeyField11, SW_SHOW);
            ShowWindow(hStaticText2, SW_SHOW);
            ShowWindow(hButton2, SW_SHOW);
            ShowWindow(hStaticTextE, SW_SHOW);
            ShowWindow(KeyField22, SW_SHOW);
            ShowWindow(hButton3, SW_SHOW);
            ShowWindow(hButton4, SW_SHOW);
            ShowWindow(hButton5, SW_SHOW);
        }
        else {
            ShowWindow(hStaticText1, SW_HIDE);
            ShowWindow(hButton1, SW_HIDE);
            ShowWindow(hStaticTextN1, SW_HIDE);
            ShowWindow(KeyField1, SW_HIDE);
            ShowWindow(hStaticTextD, SW_HIDE);
            ShowWindow(KeyField11, SW_HIDE);
            ShowWindow(hStaticText2, SW_HIDE);
            ShowWindow(hButton2, SW_HIDE);
            ShowWindow(hStaticTextE, SW_HIDE);
            ShowWindow(KeyField22, SW_HIDE);
            ShowWindow(hButton3, SW_HIDE);
            ShowWindow(hButton4, SW_HIDE);
            ShowWindow(hButton5, SW_HIDE);
        }



    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    }
    return 0;
}



void MainAddMenu(HWND hWnd) {

	HMENU RootMenu = CreateMenu();																				//Корень меню
	HMENU SubMenu = CreateMenu();                                                                               //Для Encryption и Data transfer

	HMENU SubMenuRight = CreateMenu();                                                                          //Для Encryption стрелка в RSA
	HMENU SubMenuRight1 = CreateMenu();                                                                         //Добавление в корень простого подменю
																												
	
    AppendMenu(SubMenu, MF_STRING, OnMenuActionTransfer, L"Data transfer");
	AppendMenu(SubMenu, MF_POPUP, (UINT_PTR)SubMenuRight, L"Encryption");
	AppendMenu(SubMenuRight, MF_POPUP, (UINT_PTR)SubMenuRight1, L"RSA");
	AppendMenu(SubMenuRight1, MF_STRING, OnMenuAction2, L"Keys");



	AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(SubMenu, MF_STRING, OnExitAction, L"Exit");
	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"Settings");

																												//Прмсвоение окну
	SetMenu(hWnd, RootMenu);

}




																																								//Сохранение в одно место
/*void SaveData(LPCSTR path) {																										

	HANDLE FileToSave = CreateFileA(
		path,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	int saveLenth = GetWindowTextLength(KeyField1)+1;
	char* data = new char[saveLenth];

	saveLenth = GetWindowTextA(KeyField1, data, saveLenth);

	DWORD BytesIterated;
	WriteFile(FileToSave, data, saveLenth, &BytesIterated, NULL);

	CloseHandle(FileToSave);

	delete[] data;
}*/


void SaveData(LPCSTR path, HWND per) {																				//per это куда будет вводится "знаечение" или откуда, типа "edit"

	HANDLE FileToSave = CreateFileA(
		path,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	int saveLenth = GetWindowTextLength(per) + 1;
	char* data = new char[saveLenth];

	saveLenth = GetWindowTextA(per, data, saveLenth);

	DWORD BytesIterated;
	WriteFile(FileToSave, data, saveLenth, &BytesIterated, NULL);

	CloseHandle(FileToSave);

	delete[] data;
}


																																							//Загрузка из одного места
/*void LoadData(LPCSTR path) {

	HANDLE FileToLoad = CreateFileA(
		path,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD BytesIterated;

	char Buffer[TextBufferSize];

	ReadFile(FileToLoad, Buffer, TextBufferSize, &BytesIterated, NULL);

	SetWindowTextA(KeyField1, Buffer);

	CloseHandle(FileToLoad);

}*/


void LoadData(LPCSTR path, HWND per) {

	HANDLE FileToLoad = CreateFileA(
		path,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD BytesIterated;

	//*********************************************************																												//Работа с буфером, ошибки были, но исправил
	char Buffer[TextBufferSize];
	//*********************************************************

	ReadFile(FileToLoad, Buffer, TextBufferSize, &BytesIterated, NULL);

	SetWindowTextA(per, Buffer);

	CloseHandle(FileToLoad);

}




void SetOpenFileWindow(HWND hWnd) {

	ZeroMemory(&openFN, sizeof(openFN));
	openFN.lStructSize = sizeof(openFN);
	openFN.hwndOwner = hWnd;
	openFN.lpstrFile = FileNameSize;
	openFN.nMaxFile = sizeof(FileNameSize);
	openFN.lpstrFilter = "*.txt";
	openFN.lpstrFileTitle = NULL;
	openFN.nMaxFileTitle = 0;
	openFN.lpstrInitialDir = "E:/AppFly/Save";																		//openFN.lpstrInitialDir = NULL;
																													//надо указать свою папку, которая будет открываться
	openFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

}


std::string getEncryptionStatusString(bool isEncryptionEnabled) {
    std::string res = "Encryption: ";
    if (isEncryptionEnabled) res += "On";
    else res += "Off";
    return res;
}

RSA_Keys generateKeys(RSA_KeyCreator& keyCreator) {
    // Чтобы не было случаев, когда ключи равны
    while (true) {
        RSA_Keys keys = keyCreator.CreateKeys();
        if (keys.publicKey.get_a() != keys.privateKey.get_a()) return keys;
    }
}