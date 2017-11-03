// Glazeur.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"
#define WINVER          0x0501
#define _WIN32_WINNT    0x0501
#define _WIN32_WINDOWS  0x0501
#include<stdio.h>
#include<stdarg.h>
#include<conio.h>
//#define _ITERATOR_DEBUG_LEVEL 0
#include<algorithm>
#include<string.h>
#include<vector>
#include<windows.h>
#include<winioctl.h>
#include<setupapi.h>
#include <InitGuid.h>
//#include <winsock.h>

//#include <sstream>
//#include <iostream>
#include<fcntl.h>
#include<io.h>
#define _O_U16TEXT 0x20000 /* file mode is UTF16 no BOM (translated) */

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "wsock32.lib")

// Keyboard device interface GUID.
//
#define IOCTL_KEYBOARD_SET_INDICATORS CTL_CODE(FILE_DEVICE_KEYBOARD, 0x0002, METHOD_BUFFERED, FILE_ANY_ACCESS)
DEFINE_GUID(GUID_DEVINTERFACE_KEYBOARD, 0x884B96C3, 0x56EF, 0x11D1, 0xBC, 0x8C, 0x00, 0xA0, 0xC9, 0x14, 0x05, 0xDD);

typedef struct _KEYBOARD_INDICATOR_PARAMETERS {
	//
	// Unit identifier.  Specifies the device unit for which this
	// request is intended.
	//
	USHORT UnitId;
	//
	// LED indicator state.
	//
	USHORT    LedFlags;

} KEYBOARD_INDICATOR_PARAMETERS, *PKEYBOARD_INDICATOR_PARAMETERS;

typedef struct {
	PWCHAR			pSection;			// Pointer to section
	PWCHAR			pDots[21];			// Pointers to Dotted patterns
	PWCHAR			pRems[21];			// Pointers to Remarks
	long			iTags[21];			// Pointers to Tagged labels
	long			iKeys[47];			// Pointers to Keying labels
	long			iReps;				// Repeator index
	PWCHAR			pReps[21];			// Repeators points
	long			nReps[21];			// Repeators numerators
	long			dReps[21];			// Repeators denominators
	long			iLoop;				// Current row looping
	long			iLoops;				// Maximum row looping
	long			iRow;				// Section current row
	PWCHAR			ptr;				// Pointer to parsing symbol
	long			iDelayPeriod;		// Section speed - delay (ms)
	long			iDelayMultiplier;	// Section speed - multiplier
	long			iDelayDivider;		// Section speed - divider
	long			iNumerator;			// Current numerator
	long			iDenominator;		// Current denominator
	long			iKeyboard;			// Current keyboard
	long			jSection;			// Jobing section
} KLEDS_SECTION, *PKLEDS_SECTION;

typedef struct {
	KLEDS_SECTION	Scenario;		// Scenario state
	KLEDS_SECTION	Sections[10];	// Sections states
	long unsigned	leds[10];		// Keyboards indicators shifts
	long unsigned	bits;			// Keyboards indicators states
	long unsigned	order;			// Keyboards order
	long unsigned	errors;
	long			speed;			// Keyboards LEDs refreshing frequency
	long			duration;		//
	WCHAR			log[128];
} KLEDS, *PKLEDS;

KLEDS	LEDs;

/////////////////////////////////////////////////////
int		iSession;
int		nKeyboards;
WCHAR	szKeyboards[256][256];
HANDLE	hKbd[256];
HANDLE	hThread;
HANDLE	hStdIn;
HANDLE	hStdOut;

////////////////////////////////////////////////////////////////////////////////
/* Код сервера на языке Си */

/*#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
*/
int		port	=	23;
#include <WinSock2.h>
#include <WS2tcpip.h>

// Необходимо, чтобы линковка происходила с DLL-библиотекой
// Для работы с сокетам
#pragma comment(lib, "Ws2_32.lib")

/*
void	parse_socket(PKLEDS pk, char **cmd) {
{
    WSADATA wsaData; // служебная структура для хранение информации
    // о реализации Windows Sockets
    // старт использования библиотеки сокетов процессом
    // (подгружается Ws2_32.dll)
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Если произошла ошибка подгрузки библиотеки
    if (result != 0) {
//        cerr << "WSAStartup failed: " << result << "\n";
        return;// result;
    }

    struct addrinfo* addr = NULL; // структура, хранящая информацию
    // об IP-адресе  слущающего сокета

    // Шаблон для инициализации структуры адреса
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET; // AF_INET определяет, что будет
    // использоваться сеть для работы с сокетом
    hints.ai_socktype = SOCK_STREAM; // Задаем потоковый тип сокета
    hints.ai_protocol = IPPROTO_TCP; // Используем протокол TCP
    hints.ai_flags = AI_PASSIVE; // Сокет будет биндиться на адрес,
    // чтобы принимать входящие соединения

    // Инициализируем структуру, хранящую адрес сокета - addr
    // Наш HTTP-сервер будет висеть на 8000-м порту локалхоста
    result = getaddrinfo("127.0.0.1", "8000", &hints, &addr);

    // Если инициализация структуры адреса завершилась с ошибкой,
    // выведем сообщением об этом и завершим выполнение программы
    if (result != 0) {
        cerr << "getaddrinfo failed: " << result << "\n";
        WSACleanup(); // выгрузка библиотеки Ws2_32.dll
        return 1;
    }

    // Создание сокета
    int listen_socket = socket(addr->ai_family, addr->ai_socktype,
        addr->ai_protocol);
    // Если создание сокета завершилось с ошибкой, выводим сообщение,
    // освобождаем память, выделенную под структуру addr,
    // выгружаем dll-библиотеку и закрываем программу
    if (listen_socket == INVALID_SOCKET) {
        cerr << "Error at socket: " << WSAGetLastError() << "\n";
        freeaddrinfo(addr);
        WSACleanup();
        return 1;
    }

    // Привязываем сокет к IP-адресу
    result = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);

    // Если привязать адрес к сокету не удалось, то выводим сообщение
    // об ошибке, освобождаем память, выделенную под структуру addr.
    // и закрываем открытый сокет.
    // Выгружаем DLL-библиотеку из памяти и закрываем программу.
    if (result == SOCKET_ERROR) {
        cerr << "bind failed with error: " << WSAGetLastError() << "\n";
        freeaddrinfo(addr);
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    // Инициализируем слушающий сокет
    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "listen failed with error: " << WSAGetLastError() << "\n";
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }


    const int max_client_buffer_size = 1024;
    char buf[max_client_buffer_size];
    int client_socket = INVALID_SOCKET;

    for (;;) {
        // Принимаем входящие соединения
        client_socket = accept(listen_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            cerr << "accept failed: " << WSAGetLastError() << "\n";
            closesocket(listen_socket);
            WSACleanup();
            return 1;
        }

        result = recv(client_socket, buf, max_client_buffer_size, 0);

        std::stringstream response; // сюда будет записываться ответ клиенту
        std::stringstream response_body; // тело ответа

        if (result == SOCKET_ERROR) {
            // ошибка получения данных
            cerr << "recv failed: " << result << "\n";
            closesocket(client_socket);
        } else if (result == 0) {
            // соединение закрыто клиентом
            cerr << "connection closed...\n";
        } else if (result > 0) {
            // Мы знаем фактический размер полученных данных, поэтому ставим метку конца строки
            // В буфере запроса.
            buf[result] = '\0';

            // Данные успешно получены
            // формируем тело ответа (HTML)
            response_body << "<title>Test C++ HTTP Server</title>\n"
                << "<h1>Test page</h1>\n"
                << "<p>This is body of the test page...</p>\n"
                << "<h2>Request headers</h2>\n"
                << "<pre>" << buf << "</pre>\n"
                << "<em><small>Test C++ Http Server</small></em>\n";

            // Формируем весь ответ вместе с заголовками
            response << "HTTP/1.1 200 OK\r\n"
                << "Version: HTTP/1.1\r\n"
                << "Content-Type: text/html; charset=utf-8\r\n"
                << "Content-Length: " << response_body.str().length()
                << "\r\n\r\n"
                << response_body.str();

            // Отправляем ответ клиенту с помощью функции send
            result = send(client_socket, response.str().c_str(),
                response.str().length(), 0);

            if (result == SOCKET_ERROR) {
                // произошла ошибка при отправле данных
                cerr << "send failed: " << WSAGetLastError() << "\n";
            }
            // Закрываем соединение к клиентом
            closesocket(client_socket);
        }
    }

    // Убираем за собой
    closesocket(listen_socket);
    freeaddrinfo(addr);
    WSACleanup();
    return 0;
}
*/
void	parse_socket(PKLEDS pk, char **cmd) {
	struct sockaddr_in stSockAddr;

	 WSADATA wsaData;

    if(WSAStartup(0x202, &wsaData) != 0)
		return;

	int i32SocketFD = socket(AF_INET, SOCK_STREAM, 0);

	if (i32SocketFD == -1) {
		perror("ERROR: Create socket");
		exit(EXIT_FAILURE);
	}

	memset(&stSockAddr, 0, sizeof (stSockAddr));

	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(port);
	stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//

	if (bind(i32SocketFD, (struct sockaddr*) &stSockAddr, sizeof (stSockAddr)) == -1) {
		perror("ERROR: Binding socket");

		close(i32SocketFD);
		exit(EXIT_FAILURE);
	}

	if (listen(i32SocketFD, 10) == -1) {
		perror("ERROR: Listing");

		close(i32SocketFD);
		exit(EXIT_FAILURE);
	}

	while(GetKeyState(VK_ESCAPE) >= 0) {
		int i32ConnectFD = accept(i32SocketFD, 0, 0);
		printf("Socket ID - %08X\r\n", i32ConnectFD);

		if (i32ConnectFD < 0) {
			perror("ERROR: Receive");
			close(i32SocketFD);
			exit(EXIT_FAILURE);
		}

		/* выполнение операций чтения и записи ... */

//		shutdown(i32ConnectFD, SHUT_RDWR);

		close(i32ConnectFD);
	}
	close(i32SocketFD);
    WSACleanup();
}

////////////////////////////////////////////////////////////////////////////////
int		GetDeviceSymbolic(HANDLE hDevice, SP_DEVICE_INTERFACE_DATA *pDIData, WCHAR *szPath) {
	char	Buffer[4096];
	SP_DEVICE_INTERFACE_DETAIL_DATA_W	*pDetail = (SP_DEVICE_INTERFACE_DETAIL_DATA_W	*)Buffer;
	DWORD	dwSize = sizeof *pDetail;
	DWORD	dwError;
	do {
		memset(pDetail, 0, dwSize);
		pDetail->cbSize = sizeof SP_DEVICE_INTERFACE_DETAIL_DATA_W;
		if(!!SetupDiGetDeviceInterfaceDetailW(hDevice, pDIData, pDetail, dwSize, &dwSize, NULL))
			wcscpy(szPath, pDetail->DevicePath);
		dwError = GetLastError();
	} while(ERROR_INSUFFICIENT_BUFFER == dwError || ERROR_INVALID_USER_BUFFER == dwError);
	return dwError;
}

int		GetKeyboardSymbolics() {
DWORD						dwIndex = 0;
SP_DEVICE_INTERFACE_DATA	DIData;
HANDLE const hDevInfo = SetupDiGetClassDevsW(
		(_GUID *)&GUID_DEVINTERFACE_KEYBOARD,
		NULL, // Enumerator.
		NULL, // HWND of parent window.
		DIGCF_DEVICEINTERFACE | DIGCF_PRESENT
		);
	nKeyboards = 0;
	DIData.cbSize = sizeof SP_DEVICE_INTERFACE_DATA;
	if(hDevInfo != INVALID_HANDLE_VALUE) {
		while(!!SetupDiEnumDeviceInterfaces(hDevInfo, NULL, (GUID *)&GUID_DEVINTERFACE_KEYBOARD, dwIndex ++, &DIData)) {
			GetDeviceSymbolic(hDevInfo, &DIData, szKeyboards[nKeyboards]);
			if(!wcsstr(szKeyboards[nKeyboards], L"rdp_kbd"))
				++ nKeyboards;
		}
		dwIndex = GetLastError();
		if(ERROR_NO_MORE_ITEMS == dwIndex)
			return nKeyboards;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
long	SetLED(PKLEDS pkl, long index, long level) {
	long			triplet	= (index - 1) / 3 + 1;
	long unsigned	mask	= 001111111111 * (1 << (index - 1) % 3);
	//
	if(index > 0 && index <= 27 && level >= 0 && level <= 100)
		pkl->leds[triplet] = (pkl->leds[triplet] & ~mask) ^ (mask >> (10 - level / 10) * 3);
	else
		return 0;
	return 1;
}

long	ShowLevel(long x, long y, long unsigned bits) {
	long unsigned	levels[11] = {
		//0xF9F9F9F9,
		0x2588,
		0x25A0,
		0x2591,
		0x2592,
		0x2593
		//0xB0, 0xB1, 0xB2, 0xDB
	};
	long	level = 0;
	COORD	xy;
	//
	for(bits &= 001111111111; bits > 0; bits >>= 3)
		level += bits & 1;
	xy.X = x + 0, xy.Y = y + 0; FillConsoleOutputCharacterW(hStdOut, levels[level % 5], 1, xy, &bits);
	FillConsoleOutputAttribute(hStdOut, level / 5 ? 0x8A : 0x82, 1, xy, &bits);
	return 0;
}long	ShowLevel_alt(long x, long y, long unsigned bits) {
	long unsigned	levels[11] = {
		//0xF9F9F9F9,
		0x00B7,
		0x2022,
		0x25A0,
		0x2666,
		0x25CB,
		0x263C,
		0x25D8,
		0x2591,
		0x2592,
		0x2593,
		0x2588
		//0xB0, 0xB1, 0xB2, 0xDB
	};
	long	level = 0;
	COORD	xy;
	//
	for(bits &= 001111111111; bits > 0; bits >>= 3)
		level += bits & 1;
	xy.X = x + 0, xy.Y = y + 0; FillConsoleOutputCharacterW(hStdOut, levels[level], 1, xy, &bits);
	return 0;
}
long	ShowLevel_old(long x, long y, long unsigned bits) {
	long unsigned	levels[11] = {
		//0xF9F9F9F9,
		0xFAFAFAFA,
		0xFA2A2AFA,
		0x2A2A2A2A,
		0x2AB0B02A,
		0xB0B0B0B0,
		0xB0B1B1B0,
		0xB1B1B1B1,
		0xB1B2B2B1,
		0xB2B2B2B2,
		0xB2DBDBB2,
		0xDBDBDBDB
		//0xB0, 0xB1, 0xB2, 0xDB
	};
	long	level = 0;
	COORD	xy;
	//
	for(bits &= 001111111111; bits > 0; bits >>= 3)
		level += bits & 1;
	xy.X = x + 0, xy.Y = y + 0; FillConsoleOutputCharacter(hStdOut, (levels[level] >> 24) & 255, 1, xy, &bits);
	xy.X = x + 1, xy.Y = y + 0; FillConsoleOutputCharacter(hStdOut, (levels[level] >> 16) & 255, 1, xy, &bits);
	xy.X = x + 2, xy.Y = y + 0; FillConsoleOutputCharacter(hStdOut, 0, 1, xy, &bits);
	xy.X = x + 0, xy.Y = y + 1; FillConsoleOutputCharacter(hStdOut, (levels[level] >>  8) & 255, 1, xy, &bits);
	xy.X = x + 1, xy.Y = y + 1; FillConsoleOutputCharacter(hStdOut, (levels[level] >>  0) & 255, 1, xy, &bits);
	xy.X = x + 2, xy.Y = y + 1; FillConsoleOutputCharacter(hStdOut, 0, 1, xy, &bits);
	xy.X = x + 0, xy.Y = y + 2; FillConsoleOutputCharacter(hStdOut, 0, 3, xy, &bits);
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
DWORD	WINAPI	ShowLEDs_Thread(LPVOID pkLEDs) {
	INPUT	kbdInput[4];
	KEYBOARD_INDICATOR_PARAMETERS	kbi[10], *pkbi;
	PKLEDS			pkl = (PKLEDS)pkLEDs;
	COORD			xy = {0, 0};
	long			order, speed, device;
	long unsigned	index, leds;
	long unsigned	counter, triplex;
	//
	for(index = 0; index < 4; ++ index) {
		kbdInput[index].type = INPUT_KEYBOARD;
		kbdInput[index].ki.wScan = 0;
		kbdInput[index].ki.time = 0;
		kbdInput[index].ki.wVk = VK_SCROLL;
		kbdInput[index].ki.dwFlags = index & 1 ? KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP : KEYEVENTF_EXTENDEDKEY;
	}
	//
	do {
		speed = pkl->speed;
		order = pkl->order;
		if(order < 1 || speed < 1) {
			Sleep(125);
			SendInput(4, kbdInput, sizeof INPUT);
			continue;
		}
		Sleep(1000 / speed);
		index = 0;
		counter = (counter + 1) % 80;
		while((device = order % 10) > 0 && (counter % 10 > 0)) {
			pkbi = &kbi[-- device];
			leds = pkl->leds[++ index];
			triplex = (pkl->leds[0] >> ((leds >> ((counter >> ((leds >> 30) & 3)) % 10) * 3) & 7) * 3) & 7;
			//triplex = (pkl->leds[0] >> 3 * ((leds >> counter % 10 * 3) & 7)) & 7;
			if(pkbi->LedFlags != triplex) {
				pkbi->LedFlags = triplex;
	//			if((counter >> (leds >> 30)) % 10 > 0)
				if(!DeviceIoControl(hKbd[device], IOCTL_KEYBOARD_SET_INDICATORS, pkbi, sizeof *pkbi, NULL, 0, &triplex, NULL))
					++ pkl->errors;
			}
			order /= 10;
		}
//		if(counter == 27)
			xy.X = 0;
			xy.Y = 0; //FillConsoleOutputAttribute(hStdOut, 0x8A, 9, xy, &triplex);
			xy.Y = 1; //FillConsoleOutputAttribute(hStdOut, 0x8A, 9, xy, &triplex);
			xy.Y = 2; //FillConsoleOutputAttribute(hStdOut, 0x8A, 9, xy, &triplex);
			xy.X = 9; FillConsoleOutputAttribute(hStdOut, 0x8E, 71, xy, &triplex);
			xy.Y = 1; FillConsoleOutputAttribute(hStdOut, 0x8E, 71, xy, &triplex);
			xy.Y = 0; FillConsoleOutputAttribute(hStdOut, 0x8E, 71, xy, &triplex);
			for(index = 0; index < 27; ++ index)
				ShowLevel(index / 3 * 1, 0 + index % 3, pkl->leds[index / 3 + 1] >> index % 3);
				//ShowLevel(53 + index / 3 * 3, 0 + index % 3 * 3, pkl->leds[index / 3 + 1] >> index % 3);
	} while(order >= 0 && speed >= 0);
	keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | 0, 0); keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | 0, 0); keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	ExitThread(0);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
void(&parse_chunk)(char **);
void(&play_cycle)(PKLEDS, char **);
DWORD(WINAPI &leds_cycle)(LPVOID);

void	parse_null(PKLEDS, char **) {}

void	play_null(PKLEDS, char **) {}

////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
This is header of scenario as commentary block.
Don't use any special symbols at here.
The printed ASCII-characters only!
§1 - Section #1
	<<Body of section #1...>>
§2	<<Body of section #2...>>

Body of scenario at here
	<<<Body of scenario...>>>
*******************************************************************************/
#define	GUY_MEASURE		0x00A7	//	§	Measure
////////
///////	Glazeur Accessing Strings
#define	GAS_READ_ME		"session_%08X\\Read_Me.txt"		//	Ignoring section
#define	GAS_SECTIONS	"session_%08X\\section%d.txt"	//	Indexed sections
#define	GAS_SCENARIO	"session_%08X\\scenario.txt"	//	Body of scenario
#define	GAS_SCRIPT		"session_%08X\\script.txt"		//	Linked script
////////////////////////////////
///////	Glazeur Abandoned Prefix
#define	GAP_UNICODE		0xFEFF	//		Unicode prefix
////////////////////////////////
///////	Glazeurs Using Ignores
#define	GUI_TABULATION	0x0009	// [->]	Tabulation key
#define	GUI_TABULATIONS	0x2409	// [HT]	Tabulation signature
#define	GUI_TABULATIONG	0x21E5	// ->|	Tabulation glyph
////////////////////////////////
///////	Glazeur Usings Indexators
#define	GUI_SUPER_ZERO	0x2070	// ^0^	Superscript index 0
#define	GUI_SUPER_ONE	0x00B9	// ^1^	Superscript index 1
#define	GUI_SUPER_TWO	0x00B2	// ^2^	Superscript index 2
#define	GUI_SUPER_THREE	0x00B3	// ^3^	Superscript index 3
#define	GUI_SUPER_FOUR	0x2074	// ^4^	Superscript index 4
#define	GUI_SUPER_NINE	0x2079	// ^9^	Superscript index 9
#define	GUI_LOWER_ZERO	0x2080	// _0_	Subscript index 0
#define	GUI_LOWER_NINE	0x2089	// _9_	Subscript index 9
#define	GUI_DOTTED_TAG	0x2488	//  1.	Dotted number 1.
#define	GUI_DOTTED_TOP	0x249B	//	20.	Dotted number 20.
#define	GUI_CIRCLE_TAG	0x2460	// (1)	Circled number 1
#define	GUI_CIRCLE_TOP	0x2473	// (20)	Circled number 20
#define	GUI_CIRCLE_KEY	0x24B6	// (A)	Circled key A
#define	GUI_CIRCLE_ZED	0x24CF	// (Z)	Circled key Z
#define	GUI_DOT_PATTERN	0x2800	// :::	Braille pattern
////////////////////////////////
///////	Glazeur Execution Markup
#define	GEM_SECTION		0x00A7	//	§	Section
#define	GEM_MEASURE_SET	0x20E3	// [ ]	Set measure
#define	GEM_FADE_IN		0x21E1	// /|\	Fade in effect
#define	GEM_FADE_OUT	0x21E3	// \|/	Fade out effect
#define	GEM_FADE_AND	0x2227	// /\	Fade AND effect
#define	GEM_FADE_OR		0x2228	// \/	Fade OR effect
#define	GEM_FADE_XOR	0x2295	// (+)	Fade XOR effect
#define	GEM_FADE_LO		0x270E	// ^\	Lower bright  - 10%
#define	GEM_FADE_MID	0x270F	// --	Middle bright - 50%
#define	GEM_FADE_HI		0x2710	// _/	High bright   - 90%
#define	GEM_DEVICE_SET	0x2328	// [:]	Keyboard select
#define	GEM_STATE_LOAD	0x2397	// [[]	Load state
#define	GEM_STATE_SAVE	0x2398	// []]	Save state
#define	GEM_STATE_NULL	0x239A	// [/]	Clear state
#define	GEM_SHOW_DELAY	0x23F1	// (^)	Show delay pause
#define	GEM_SHOW_SPEED	0x23F2	// (-)	Show speed
#define	GEM_LOOP_TIMES	0x267B	// /_\	Looping for n-times
#define	GEM_SHOW_ALERT	0x26A0	// /!\	Alert
#define	GEM_STATE_STOP	0x26D4	// (=)	Stop/Halt
#define	GEM_SECTION_UP	0x2934	// /\	Measure prev. line
#define	GEM_SECTION_DWN	0x2935	// \/	Measure next line
#define	GEM_SECTION_RUN	0x27B0	// _o_	Measure line run
////////
#define	GEM_RUN_UP		0x25B2	// [^]	Run LEDs to up
#define	GEM_RUN_RIGHT	0x25B6	// [>]	Run LEDs to right
#define	GEM_RUN_DOWN	0x25BC	// [v]	Run LEDs to down
#define	GEM_RUN_LEFT	0x25C0	// [<]	Run LEDs to left
////////
#define	GEM_STATE_PRINT	0x2399	// [~]	Print the status
#define	GEM_SETUP_HZ	0x3390	// Hz	Refreshing frequency for LEDs
#define	GEM_SETUP_MS	0x33B3	// ms	Animation delay
////////////////////////////////
///////	Glazeur Enhanced Token
#define	GET_KEY(wc)	(GUI_CIRCLE_TAG <= (wc) && (wc) <= GUI_CIRCLE_TOP ? (wc) - GUI_CIRCLE_TAG + 1 :\
						GUI_CIRCLE_KEY <= (wc) && (wc) <= GUI_CIRCLE_ZED ? (wc) - GUI_CIRCLE_KEY + 21 : -1)
#define	GET_TAB(wc)	(GUI_TABULATION == (wc) || GUI_TABULATIONG == (wc) || GUI_TABULATIONS == (wc))
#define	GET_TAG(wc)	(GUI_DOTTED_TAG <= (wc) && (wc) <= GUI_DOTTED_TOP ? (wc) - GUI_DOTTED_TAG + 1 : -1)
#define	GET_LED(wc)	(GUI_DOT_PATTERN == ((wc) & ~255))
#define	GET_SUB(wc)	(GUI_LOWER_ZERO <= (wc) && (wc) <= GUI_LOWER_NINE ? (wc) - GUI_LOWER_ZERO : -1)
#define	GET_PWR(wc)	((GUI_SUPER_FOUR <= (wc) && (wc) <= GUI_SUPER_NINE) || (wc) == GUI_SUPER_ZERO ? (wc) - GUI_SUPER_ZERO :\
						GUI_SUPER_ONE == (wc) ? 1 : GUI_SUPER_TWO == (wc) ? 2 : GUI_SUPER_THREE == (wc) ? 3 : -1)

WCHAR	wszMeasures[10][6144]	= {0};	// wide-characters string
int		ispMeasures[10]			= {0};	// string positions indexes

int	sprintfex(WCHAR *pwcsz, const char *fmt, ...) {
	int static const	numbers[2][10] = {
		{0x2080, 0x2081, 0x2082, 0x2083, 0x2084, 0x2085, 0x2086, 0x2087, 0x2088, 0x2089},
		{0x2070, 0x00B9, 0x00B2, 0x00B3, 0x2074, 0x2075, 0x2076, 0x2077, 0x2078, 0x2079}
	};
	char static		cfmt[256] = {'%'};
	va_list	args;
	int		len = 0;
	int		i, j;
	char	ascii;
	bool	super;
	//
	va_start(args, fmt);
	i = 0;
	while(fmt[i]) {
		if(fmt[i] != '%' && fmt[i] >= ' ' && fmt[i] <= '~')
			pwcsz[len ++] = fmt[i ++];
		else
		if(fmt[i] == '%' && fmt[i + 1] == '%')
			pwcsz[len ++] = fmt[++ i],
			++ i;
		else
		if(fmt[i] == '%' && (fmt[i + 1] == '_' || fmt[i + 1] == '^')) {
			super = fmt[++ i] == '^';
			j = 1;
			do {
				ascii = fmt[++ i];
				cfmt[j ++] = ascii;
			} while(!(ascii >= 'a' && ascii <= 'z') && !(ascii >= 'A' && ascii <= 'Z') && ascii >= ' ');
			cfmt[j ++] = 0;
			sprintf(&cfmt[j], cfmt, va_arg(args, int));
			while(cfmt[j] >= '0' && cfmt[j] <= '9')
				pwcsz[len ++] = numbers[super & 1][cfmt[j ++] - '0'];
			++ i;
		} else
		if(fmt[i] == '%') {
			j = 1;
			cfmt[j ++] = 0;
			do {
				ascii = fmt[++ i];
				cfmt[j ++] = ascii;
				cfmt[j ++] = 0;
			} while(!(ascii >= 'a' && ascii <= 'z') && !(ascii >= 'A' && ascii <= 'Z') && ascii >= ' ');
			cfmt[j ++] = 0;
			cfmt[j ++] = 0;
			len += swprintf(&pwcsz[len], (wchar_t const*)cfmt, va_arg(args, int));
			++ i;
		}
	}
	va_end(args);
	return len;
}
//const double& v = *(double*) NULL
void	parse_fraction(long &piNumerator, long &piDenominator, PWCHAR &ppwc = *(PWCHAR*)0) {
	int static const	iFractions[] = {
		0x2152010A,	//	1/10 .1
		0x21510109,	//	1/9	0.111
		0x215B0108,	//	1/8	0.125
		0x21500107,	//	1/7	0.143
		0x21590106,	//	1/6	0.167
		0x21550105,	//	1/5	0.2
		0x00BC0104,	//	1/4	0.25
		0x21530103,	//	1/3	0.333
		0x215C0308,	//	3/8	0.375
		0x21560205,	//	2/5	0.4
		0x00BD0102,	//	1/2	0.5
		0x21570305,	//	3/5	0.6
		0x215D0508,	//	5/8	0.625
		0x21540203,	//	2/3	0.667
		0x00BE0304,	//	3/4	0.75
		0x21580405,	//	4/5	0.8
		0x215A0506,	//	5/6	0.833
		0x215E0708,	//	7/8	0.875
		0x21890003	//	0/3
	};
	int		i;
	// Read the decimal or octal number
	if(&ppwc) {
		if(*ppwc > '0' && *ppwc <= '9') {
			piNumerator = 0;
			while(*ppwc >= '0' && *ppwc <= '9')
				piNumerator = piNumerator * 10 + *&*ppwc ++ - '0';
		} else
		if(*ppwc == '0') {
			piNumerator = 0;
			while(*ppwc >= '0' && *ppwc <= '7')
				piNumerator = piNumerator * 8 + *&*ppwc ++ - '0';
		} else
			piNumerator = -1;
		// Define the fraction
		piDenominator = -1;
		i = 10000;
		if(piNumerator > 0)
			switch(*ppwc) {
			case 0x25:		i /= 10;
			case 0x2030:	i /= 10;
			case 0x2031:	piDenominator = i;
					&*ppwc ++;
					return;
			}
	}
	do {
		i = sizeof iFractions / sizeof *iFractions;
		if(&ppwc)
			while(i -- > 0)
				if(HIWORD(iFractions[i]) == *ppwc) {
					if(piNumerator < 1)
						piNumerator = HIBYTE(iFractions[i]);
					else
					if(piDenominator < 1)
						piNumerator = piNumerator * LOBYTE(iFractions[i]) + HIBYTE(iFractions[i]);
					else
						piNumerator *= HIBYTE(iFractions[i]);
					if(piDenominator < 1)
						piDenominator = LOBYTE(iFractions[i]);
					else
						piDenominator *= LOBYTE(iFractions[i]);
					&*ppwc ++;
					break;
				}
		if(i > 0)
			for(i = 2; piNumerator / i > 0 && piDenominator / i > 0; ++ i)
				if(piNumerator == piNumerator / i * i && piDenominator == piDenominator / i * i) {
					piNumerator /= i;
					piDenominator /= i;
					i = 1;
				}
		while(piNumerator > 1 && piDenominator > 1 && (piNumerator >= 65536 || piDenominator >= 65536))
			piNumerator >>= 1,
			piDenominator >>= 1;
		if(!&ppwc)
			return;
	} while(i >= 0);
}
/*
  2    1*3+2    5
1--- = ----- = ---
  3      3      3

  2   3     5*3     15
1--- --- = ----- = ----
  3   4     3*4     12
*/
bool	play_effect(PKLEDS pk, PKLEDS_SECTION pks, PWCHAR &pwc) {
	int				mode = 0;
	char unsigned	code;
	long			index;
	long unsigned	lo, hi, base = 001111111111;
	//
	switch(*pwc) {
	case GEM_FADE_IN:
	case GEM_FADE_OUT:
	case GEM_FADE_AND:
	case GEM_FADE_OR:
	case GEM_FADE_XOR:
		mode = *pwc ++;
	}
	switch(*pwc) {
	case GEM_FADE_LO:
		base >>= 3 * 4;
	case GEM_FADE_MID:
		base >>= 3 * 4;
	case GEM_FADE_HI:
		base >>= 3 * 1;
		++ pwc;
	}

	if(*pwc >= 0x2800 && *pwc <= 0x28FF) {
		index = pk->Scenario.iKeyboard;
		if(index < 1 || index > 9)
			return false;
		while(*pwc >= 0x2800 && *pwc <= 0x28FF) {
			code = LOBYTE(*pwc ++);
			if(code == 0xC0) {
				if(pks->iNumerator >= 1 && pks->iNumerator <= 20) {
					if(pks->pDots[pks->iNumerator])
						*pks->pDots = pks->pDots[pks->iNumerator];
					pks->iNumerator = -1;
				}
				if(*pks->pDots) {
					code = *pks->pDots[0];
					if(*pks->pDots[0] >= 0x2800)
						++ pks->pDots[0];
				}
			}
			switch(mode) {
			case GEM_FADE_IN:
				lo = base * (code & 7),
				hi = base * ((code >> 3) & 7);
				if((code & 0100) == 0)
					pk->leds[index] = (pk->leds[index] & ~lo) | ((pk->leds[index] << 3) & lo) | (pk->leds[index] & lo);
				index = index % 9 + 1;
				if((code & 0200) == 0)
					pk->leds[index] = (pk->leds[index] & ~hi) | ((pk->leds[index] << 3) & hi) | (pk->leds[index] & hi);
				index = index % 9 + 1;
				continue;
			case GEM_FADE_OUT:
				lo = (base & ~7) * (code & 7),
				hi = (base & ~7) * ((code >> 3) & 7);
				if((code & 0100) == 0)
					pk->leds[index] = (pk->leds[index] & ~lo) | ((pk->leds[index] & lo) >> 3);
				index = index % 9 + 1;
				if((code & 0200) == 0)
					pk->leds[index] = (pk->leds[index] & ~hi) | ((pk->leds[index] & hi) >> 3);
				index = index % 9 + 1;
				continue;
			case GEM_FADE_AND:
				lo = base * (code & 7),
				hi = base * ((code >> 3) & 7);
				if((code & 0100) == 0)
					pk->leds[index] &= (pk->leds[index] & 030000000000) | lo;
				index = index % 9 + 1;
				if((code & 0200) == 0)
					pk->leds[index] &= (pk->leds[index] & 030000000000) | hi;
				index = index % 9 + 1;
				continue;
			case GEM_FADE_OR:
				lo = base * (code & 7),
				hi = base * ((code >> 3) & 7);
				if((code & 0100) == 0)
					pk->leds[index] = (pk->leds[index] & 037777777777) | lo;
				index = index % 9 + 1;
				if((code & 0200) == 0)
					pk->leds[index] = (pk->leds[index] & 037777777777) | hi;
				index = index % 9 + 1;
				continue;
			case GEM_FADE_XOR:
				lo = base * (code & 7),
				hi = base * ((code >> 3) & 7);
				if((code & 0100) == 0)
					pk->leds[index] = (pk->leds[index] & 037777777777) ^ lo;
				index = index % 9 + 1;
				if((code & 0200) == 0)
					pk->leds[index] = (pk->leds[index] & 037777777777) ^ hi;
				index = index % 9 + 1;
				continue;
			default:
				lo = base * (code & 7),
				hi = base * ((code >> 3) & 7);
				if((code & 0100) == 0)
					pk->leds[index] = (pk->leds[index] & 030000000000) | lo;
				index = index % 9 + 1;
				if((code & 0200) == 0)
					pk->leds[index] = (pk->leds[index] & 030000000000) | hi;
				index = index % 9 + 1;
				continue;
			}
		}
		return true;
	}
	if(GEM_RUN_UP == *pwc && pks->iDenominator < 0) {
		pwc ++;
		for(index = 1; index <= 9; ++ index) {
			lo = pk->leds[index];
			pk->leds[index] = (lo & 030000000000) | ((lo & 006666666666) >> 1) | ((lo & 001111111111) << 2);
		}
		return true;
	}
	if(GEM_RUN_RIGHT == *pwc && pks->iDenominator < 0) {
		pwc ++;
		index = 9;
		hi = pk->leds[index];
		while(index > 1)
			pk->leds[index --] = pk->leds[index - 1];
		pk->leds[index] = hi;
		return true;
	}
	if(GEM_RUN_DOWN == *pwc && pks->iDenominator < 0) {
		pwc ++;
		for(index = 1; index <= 9; ++ index) {
			hi = pk->leds[index];
			pk->leds[index] = (hi & 030000000000) | ((hi & 004444444444) >> 2) | ((hi & 003333333333) << 1);
		}
		return true;
	}
	if(GEM_RUN_LEFT == *pwc && pks->iDenominator < 0) {
		pwc ++;
		index = 1;
		lo = pk->leds[index];
		while(index < 9)
			pk->leds[index ++] = pk->leds[index + 1];
		pk->leds[index] = lo;
		return true;
	}
	if(GEM_DEVICE_SET == *pwc && pks->iDenominator < 0) {
		if(pks->iNumerator < 0 && mode == GEM_FADE_IN)
			pks->iKeyboard = pks->iKeyboard % 9 + 1;
		else
		if(pks->iNumerator < 0 && mode == GEM_FADE_OUT)
			pks->iKeyboard = (pks->iKeyboard + 7) % 9 + 1;
		else
			return false;
		pwc ++;
		return true;
	}
	if(mode != 0)
		-- pwc;
	return false;
}

////////////////////////////////////////////////////////////////////////////////
//	FUNCTION:	Get measure row
//	PURPOSE:	Find the target row
//	Example:
//	|This is measure's commentary
//	|	[1]		This is row #1
//	|This is measure's commentary #2	
//	|	[2]		This is row #2
//	|This is measure's finish
//	|
////////////////////////////////////////////////////////////////////////////////
long	parse_section_rows(PKLEDS_SECTION pks, PWCHAR &pwc, long row) {
	long	rows	= 0;
	long	index;
	//
	do {
		if(*pwc < ' ' && !GET_TAB(*pwc))
			return -1;
		if(GET_KEY(*pwc) > 0)
			++ pwc;
		index = GET_TAG(*pwc);
		if(index > 0)
			++ pwc;
		else
			index = 0;
		if(GET_LED(*pwc)) {
			if(0x28C0 == *pwc)
				++ pwc;
			pks->pDots[index] = pwc,
			pks->pDots[0] = pwc;
		} else
		if(GET_TAB(*pwc))
			pks->iTags[index] = rows,
			pks->iTags[0] = rows,
			++ rows,
			-- row;
		else
			pks->pRems[index] = pwc,
			pks->pRems[0] = pwc;
		if(row < 0)
			break;
		while(0x000D != *pwc && *pwc != 0x000A) {
			if(!*pwc ++)
				return -1;
		}
		if((0x000D == pwc[0] && pwc[1] == 0x000A)
		|| (0x000A == pwc[0] && pwc[1] == 0x000D))
			pwc += 2;
		else
			pwc ++;
	} while(row >= 0);
	return rows;
}
bool	parse_section(PKLEDS pk, long iSection, PWCHAR *ppwc = 0) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;        // screen buffer information
	PKLEDS_SECTION	pks = &pk->Sections[iSection];
	WCHAR	*pwc	= pks->pSection;
	WCHAR	*prem, *plog;
	long	jSection;		//	Job-Section index
	long	kSection;		//	Key-Section index
	long	iRow, i, tmp;
	long	iRowIndex, iRowLoops;
	long	index;
	long	nTabs	= 0;
	//
	long	iDelay		= 0;
	//
	if(ppwc)
		pwc = ++ *ppwc;
	else {
		if(!pwc)
			return false;
		iRow = parse_section_rows(pks, pwc, pks->iRow);
		if(iRow < 0)
			return false;
		while(GET_TAB(*pwc))
			++ pwc;
	}
	iRowIndex = pks->iRow ++;
	iRowLoops = pks->iLoop;
	pks->iLoop = 0;
	pks->iLoops = pks->iLoops > 0 ? pks->iLoops : 1;
	pks->iReps = -1;
	jSection = pks->jSection;
	while(*pwc >= ' ' && !GET_TAB(*pwc)) {
		if(GetKeyState(VK_ESCAPE) < 0)
			return false;
		while(*pwc == ' ')
			++ pwc;
		i = GET_TAG(*pwc);
		if(i > 0) {
			if(pks->iTags[i] > 0)
				pks->iRow = pks->iTags[i];
			++ pwc;
			continue;
		}
		i = GET_KEY(*pwc);
		if(i > 0) {
			if(pks->iKeys[i] > 0)
				pks->iRow = pks->iKeys[i];
			++ pwc;
			continue;
		}
		parse_fraction(pks->iNumerator, pks->iDenominator, pwc);
		if(GEM_SECTION == *pwc && pks->iDenominator < 0) {
			++ pwc;
			if('0' <= *pwc && *pwc <= '9')
				jSection = *pwc ++ - '0',
				pks->jSection = jSection;
			else
			if('(' == *pwc) {
				parse_section(pk, jSection, &pwc);
				continue;
			}
			if(pks->iNumerator >= 0)
				if(iSection < 0)
					pk->Sections[jSection].iRow = pks->iNumerator;
				else
					pks->iRow = pks->iNumerator;
			continue;
		}
		if(GEM_SECTION == *pwc && pks->iNumerator >= 0 && pks->iDenominator < 0) {
			if(iSection < 0 && jSection >= 0) {
				pk->Sections[jSection].iRow = pks->iNumerator;
			} else
				pks->iRow = pks->iNumerator;
			++ pwc;
			continue;
		}
		if(GEM_SECTION_RUN == *pwc && pks->iDenominator < 0 && jSection >= 0) {
			pk->Sections[jSection].iKeyboard = pks->iKeyboard;
			parse_section(pk, jSection);
			++ pwc;
			continue;
		}
		if(GEM_SECTION_UP == *pwc && pks->iDenominator < 0) {
			if(pks->iNumerator > 0)
				pks->iRow = pks->iRow > pks->iNumerator ? pks->iRow - pks->iNumerator : 1;
			else
			if(pks->iRow > 1)
				-- pks->iRow;
			++ pwc;
			continue;
		}
		if(GEM_SECTION_DWN == *pwc && pks->iDenominator < 0) {
			if(pks->iNumerator > 0)
				pks->iRow += pks->iNumerator;
			else
				++ pks->iRow;
			++ pwc;
			continue;
		}
		if(GEM_SHOW_DELAY == *pwc) {
			csbi.dwCursorPosition.X = 10,
			csbi.dwCursorPosition.Y = 1;
			SetConsoleCursorPosition(hStdOut, csbi.dwCursorPosition);
			if(jSection < 0 || pk->Sections[jSection].iLoops < 1)
				wprintf(L"Scenario-row#%-3d %3d%%x%d", pk->Scenario.iRow, iRowLoops * 100 / pk->Scenario.iLoops, pk->Scenario.iLoop);
			else
				wprintf(L"Scenario-row#%-3d %3d%%x%-5d %3d$%d %3d%%x%-5d", pk->Scenario.iRow, iRowLoops * 100 / pk->Scenario.iLoops, iRowLoops, pk->Sections[jSection].iRow, jSection, pk->Sections[jSection].iLoop * 100 / pk->Sections[jSection].iLoops, pk->Sections[jSection].iLoop);
			if(pks->iNumerator < 0) {
				if(pks->iDelayDivider > 0)
					pk->duration += pks->iDelayPeriod * pks->iDelayMultiplier / pks->iDelayDivider,
					Sleep(pks->iDelayPeriod * pks->iDelayMultiplier / pks->iDelayDivider);
				else {
					if(iSection < 0)
						printf("\r\nERROR: Scenario Row %d --- Wrong Delay Interval", iRow);
					else
						printf("\r\nERROR: Paragraph #%d Row %d --- Wrong Delay Interval", iSection, iRow);
					Sleep(1000);
				}
			} else
			if(pks->iDenominator < 0)
				pk->duration += pks->iDelayPeriod * pks->iDelayMultiplier * pks->iNumerator / pks->iDelayDivider,
				Sleep(pks->iDelayPeriod * pks->iDelayMultiplier * pks->iNumerator / pks->iDelayDivider);
			else
				pk->duration += pks->iDelayPeriod * pks->iDelayMultiplier * pks->iNumerator / pks->iDelayDivider / pks->iDenominator,
				Sleep(pks->iDelayPeriod * pks->iDelayMultiplier * pks->iNumerator / pks->iDelayDivider / pks->iDenominator);
/*			if(pks->iNumerator < 0 && pks->iDenominator < 0) {
				if(pks->iDelayDivider > 0)
					Sleep(pks->iDelayPeriod * pks->iDelayMultiplier / pks->iDelayDivider);
				else {
					printf("\r\nERROR: Paragraph #%d Row %d", iSection, iRow);
					Sleep(5000);
				}
			} else
			if(pks->iNumerator == 0 && pks->iDenominator < 0)
				pks->iDelayMultiplier = pk->Scenario.iDelayPeriod,
				pks->iDelayDivider = 1;
			else {
				if(pks->iNumerator > 0)
					pks->iDelayMultiplier = pks->iNumerator,
					pks->iDelayDivider = 1;
				if(pks->iDenominator > 0)
					pks->iDelayDivider = pks->iDenominator;
			}*/
			csbi.dwCursorPosition.X = 10,
			csbi.dwCursorPosition.Y = 0;
			SetConsoleCursorPosition(hStdOut, csbi.dwCursorPosition);
			printf("[%d:%02d:%02d.%03d] - I/O-Errors:%d", pk->duration / 3600000 % 10, pk->duration / 60000 % 60, pk->duration / 1000 % 60, pk->duration % 1000, pk->errors);
			++ pwc;
			continue;
		}
		if(';' == *pwc) {
			++ pwc;
			if(pks->iReps >= 0) {
				i = pks->nReps[pks->iReps];
				if(i > pks->dReps[pks->iReps])
					continue;
				-- pks->nReps[pks->iReps];
				if(i > 1)
					continue;
				i = 0;
				do {
					++ pwc;
					if('(' == *pwc)
						++ i;
					if(')' == *pwc)
						-- i;
				} while(*pwc && i >= 0);
			}
		}
		if(')' == *pwc) {
			if(!!ppwc)
				return !!*(*ppwc = ++ pwc);
			if(pks->iReps >= 0) {
				i = pks->dReps[pks->iReps];
				if(i > 0) {
					if((pks->nReps[pks->iReps] -= i) > 0) {
						pwc = pks->pReps[pks->iReps];
						continue;
					}
				} else
				if(-- pks->nReps[pks->iReps] > 0) {
					pwc = pks->pReps[pks->iReps];
					continue;
				}
				-- pks->iReps;
			}
			++ pwc;
			continue;
		}
		if('(' == *pwc) {
			++ pwc;
			if(pks->iNumerator > 0) {
				++ pks->iReps;
				pks->pReps[pks->iReps] = pwc;
				pks->nReps[pks->iReps] = pks->iNumerator;
				pks->dReps[pks->iReps] = pks->iDenominator;
				continue;
			}
			break;
		}
		if(play_effect(pk, pks, pwc))
			continue;
		if(GEM_STATE_PRINT == *pwc) {
			++ pwc;
			if(pks->iNumerator >= 0 && pks->iNumerator <= 20)
				index = pks->iNumerator;
			else
				index = 0;
			prem = pks->pRems[index];
			plog = pk->log;
			i = 0;
			GetConsoleScreenBufferInfo(hStdOut, &csbi);
			csbi.dwCursorPosition.X = 10,
			csbi.dwCursorPosition.Y = 2;
			SetConsoleCursorPosition(hStdOut, csbi.dwCursorPosition);
			while(*prem && *prem != 0x000D) {
				index = GET_SUB(*prem);
				kSection = -1;
				if(index >= 0)
					kSection = iSection;
				else
					index = GET_PWR(*prem);
				if(index >= 0) {
					if(index == 0)
						index = pk->Sections[kSection].iRow;
					else
					if(index - 1 <= pk->Sections[kSection].iReps) {
						tmp = pk->Sections[kSection].dReps[pk->Sections[kSection].iReps - index + 1];
						index = pk->Sections[kSection].nReps[pk->Sections[kSection].iReps - index + 1];
						if(tmp > 0)
							index /= tmp;
					} else
						index = -1;
					if(index >= 0)
						index = swprintf(plog, L"%d", index);
					else
						index = swprintf(plog, L"?");
					plog += index;
					i += index;
					++ prem;
					continue;
				}
				if(GEM_SECTION == *prem)
					*plog ++ = iSection + '0',
					++ prem;
				else
					*plog ++ = *prem ++;
				++ i;
			}
			WriteConsoleW(hStdOut, pk->log, i, NULL, NULL);
			continue;
		}
		if(GEM_STATE_NULL == *pwc) {
			if(pks->iDenominator < 0 && pks->iNumerator < 0) {
				csbi.dwCursorPosition.X = 10,
				csbi.dwCursorPosition.Y = 2;
				FillConsoleOutputCharacter(hStdOut, ' ', 70, csbi.dwCursorPosition, (PDWORD)&index);
			} else
			if(pks->iDenominator < 0) {
				if(pks->iNumerator >= 1 && pks->iNumerator <= 20) {
					if(pks->pDots[pks->iNumerator])
						*pks->pDots = pks->pDots[pks->iNumerator];
				}
			}
			++ pwc;
			continue;
		}
		if(GEM_DEVICE_SET == *pwc && pks->iDenominator < 0) {
			if(pks->iNumerator >= 1 && pks->iNumerator <= 9)
				pks->iKeyboard = pks->iNumerator;
			else
			if(pks->iNumerator >= 1 && pks->iNumerator >= 11) {
				index = pks->iNumerator;
				pk->order = 0;
				do
					pk->order = pk->order * 10 + index % 10;
				while(index /= 10);
			}
			pwc ++;
			continue;
		}
		if(GEM_SHOW_SPEED == *pwc) {
			if(pks->iNumerator < 0)
				pks->iDelayMultiplier = 1,
				pks->iDelayDivider = 1;
			else
			if(pks->iDenominator < 0)
				pks->iDelayMultiplier = 1,
				pks->iDelayDivider = 1;
			else
				pks->iDelayMultiplier *= pks->iNumerator,
				pks->iDelayDivider *= pks->iDenominator;
			parse_fraction(pks->iDelayMultiplier, pks->iDelayDivider);
/*			if(pks->iNumerator < 0 && pks->iDenominator < 0) {
				if(pks->iDelayDivider > 0)
					Sleep(pks->iDelayPeriod * pks->iDelayMultiplier / pks->iDelayDivider);
				else {
					printf("\r\nERROR: Paragraph #%d Row %d", iSection, iRow);
					Sleep(5000);
				}
			} else
			if(pks->iNumerator == 0 && pks->iDenominator < 0)
				pks->iDelayMultiplier = pk->Scenario.iDelayPeriod,
				pks->iDelayDivider = 1;
			else {
				if(pks->iNumerator > 0)
					pks->iDelayMultiplier = pks->iNumerator,
					pks->iDelayDivider = 1;
				if(pks->iDenominator > 0)
					pks->iDelayDivider = pks->iDenominator;
			}*/
			++ pwc;
			continue;
		}
		if(GEM_LOOP_TIMES == *pwc) {
			++ pwc;
			if(pks->iNumerator > 0 && pks->iDenominator < 0) {
				if(pks->iNumerator > iRowLoops)
					pks->iRow = iRowIndex,
					pks->iLoop = iRowLoops + 1,
					pks->iLoops = pks->iNumerator;
			}
			continue;
		}
		if(GEM_SETUP_MS == *pwc) {
			++ pwc;
			if(iSection < 0) {
				if(pks->iNumerator > 0 && pks->iDenominator > 0)
					pks->iDelayMultiplier = pks->iNumerator,
					pks->iDelayDivider = pks->iDenominator;
				else
				if(pks->iNumerator > 0)
					pks->iDelayPeriod = pks->iNumerator,
					pks->iDelayMultiplier = 1,
					pks->iDelayDivider = 1;
				else
					pks->iDelayMultiplier = 1,
					pks->iDelayDivider = 1;
				continue;
			} else
			if(pks->iNumerator > 0 && pks->iDenominator > 0)
				pks->iDelayMultiplier = pks->iNumerator,
				pks->iDelayDivider = pks->iDenominator;
			else
			if(pks->iNumerator > 0)
				pks->iDelayPeriod = pks->iNumerator,
				pks->iDelayMultiplier = 1,
				pks->iDelayDivider = 1;
			else
				pks->iDelayPeriod = pk->Scenario.iDelayPeriod * pk->Scenario.iDelayMultiplier / pk->Scenario.iDelayDivider,
				pks->iDelayMultiplier = 1,
				pks->iDelayDivider = 1;
			continue;
		}
		if(GEM_SETUP_HZ == *pwc && pks->iNumerator >= 0 && pks->iDenominator < 0) {
			pk->speed = pks->iNumerator;
			++ pwc;
			continue;
		}
		++ pwc;
	};
	return true;
}

void	parse_scenario(PKLEDS pk, PWCHAR pwc) {
	static	WCHAR	wcProcs[16][4096] = {0};
	static	WCHAR	wcBuffer[6553];
	//
	PKLEDS_SECTION	pks;
	//
	long	iSection;
	//
	int		iTempo = 0;
	int		iSpeed = 0;
	int		index, iRow;
	long unsigned	size;
	char	buffer[83] = {81};
	//
	_setmode(_fileno(stdout), _O_U16TEXT);
//	sprintfex(wcBuffer, "%d %^d %_d", 123, 456, 789);
//	wprintf(L"%25s", wcBuffer);
	pk->duration = 0;
	if(*pwc == GAP_UNICODE)
		++ pwc;
	do {
		iRow = 0;
		if(GEM_SECTION == pwc[0] && '0' <= pwc[1] && pwc[1] <= '9')
			iSection = *++ pwc - '0',
			++ pwc;
		else
			iSection = -1;
		pks = &pk->Sections[iSection];
		pks->iRow = 0;
		pks->pSection = pwc;
		for(index = 0; index <= 20; ++ index)
			pks->pDots[index] = 0,
			pks->pRems[index] = 0,
			pks->iTags[index] = 0;
		for(index = 0; index <= 46; ++ index)
			pks->iKeys[index] = 0;
		pks->iLoop = 0;
		pks->jSection = -1;
		do {
			if(GEM_SECTION == *pwc)
				break;
//			if(GEM_SECTION == pwc[1] || pwc[2] == GEM_SECTION)
//				size = 0,
//				iRow = 0;
			size = 0;
			index = GET_KEY(*pwc);
			if(index > 0)
				pks->iKeys[index] = iRow,
				++ pwc,
				++ size;
			index = GET_TAG(*pwc);
			if(index < 0)
				index = 0;
			else
				++ pwc,
				++ size;
			if(GET_LED(*pwc))
				pks->pDots[index] = pwc,
				pks->pDots[0] = pwc;
			else
			if(GET_TAB(*pwc))
				pks->iTags[index] = iRow,
				pks->iTags[0] = iRow,
				++ iRow;
			else
				pks->pRems[index] = pwc,
				pks->pRems[0] = pwc;
			index = GET_KEY(*pwc);
			if(index > 0)
				pks->iKeys[index] = iRow,
				++ pwc,
				++ size;
			while(*pwc && 0x000D != *pwc && *pwc != 0x000A)
				++ pwc,
				++ size;
			if((0x000D == pwc[0] && pwc[1] == 0x000A)
			|| (0x000A == pwc[0] && pwc[1] == 0x000D)) {
				if(size == 0)
					*pwc ++ = 0,
					*pwc ++ = 0;
				else
					pwc += 2;
			} else
			if(0x000D == pwc[0] || pwc[0] == 0x000A) {
				if(size == 0)
					*pwc ++ = 0;
				else
					++ pwc;
			}
		} while(size > 0);
	} while(*pwc && (iSection >= 0 || GEM_SECTION == *pwc));
}

////////////////////////////////////////////////////////////////////////////////

void	PrintFile(HANDLE hFile) {
	union {
		char	szText[4096];
		WCHAR	wcText[2048];
	};
	DWORD		size, nChars;
	PWCHAR		pwc;
	bool		is_unicode	= false;
	//
	if(INVALID_HANDLE_VALUE != hFile)
		do {
			size = sizeof szText;
			ReadFile(hFile, szText, size, &size, NULL);
			pwc = wcText;
			nChars = size;
			if(GAP_UNICODE == *wcText && size > 0)
				is_unicode = true,
				pwc ++,
				nChars --;
			if(is_unicode)
				nChars /= sizeof *wcText;
			if(nChars > 0)
				if(is_unicode)
					WriteConsoleW(hStdOut, pwc, nChars, NULL, NULL);
				else
					WriteConsoleA(hStdOut, szText, nChars, NULL, NULL);
		} while(size == sizeof szText);
}

//	section_access(pk)			- create scenario
//	section_access(pk, n)		- print section
//	section_access(pk, n, "")	- create section
//	section_access(pk, n, s)	- append section
///
//	section_access(pk, n, bits)	- create scenario
///
//	section_access(pk, n)		- print section n	- section_access(pk, 0..10)
//	section_access(pk, n, 0)	- create section n	- section_access(pk, 0..10, 0)
//	section_access(pk, n, s)	- append section n	- section_access(pk, 0..10, "...")
//	section_access(pk)			- print scenario
//	section_access(pk, 10, mask)- create scenario
//	section_acesss(pk, 10, s)	- append scenario

#define	Section_Print(index)\
	section_access(pk, index)
#define	Section_Create(index)\
	section_access(pk, index, NULL)
#define	Section_Append(index, string)\
	section_access(pk, index, string)
#define	Script_


void	section_access(PKLEDS pk, long index = -1, PWCHAR pwc = (PWCHAR)-1) {
	union {
		char		szFile[4096];
		WCHAR		wcFile[2048];
	};
	DWORD			size, total, err;
	long			bits = (long)pwc;
	HANDLE			hFile, hFiles;
	//
	if(index < 0 && bits < (1 << 10)) {	//	Linking all sections
		sprintf(szFile, GAS_SCRIPT, 1);
		hFile = CreateFile(szFile, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
		err = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
		*wcFile = GAP_UNICODE;
		size = sizeof *wcFile;
		WriteFile(hFile, wcFile, size, &size, NULL);
		total = 0;
		sprintf(szFile, GAS_READ_ME, 1);
		for(index = -1; index <= 10; ++ index) {
			if(index > 9) {
				sprintf(szFile, GAS_SCENARIO, 1);
				if(total > 0) {
					wcFile[0] = 0x000D;
					wcFile[1] = 0x000A;
					WriteFile(hFile, wcFile, 4, &size, NULL);
				}
			} else
			if((bits & (1 << index)) == 0)
				continue;
			else
			if(index >= 0)
				sprintf(szFile, GAS_SECTIONS, 1, index);
			hFiles = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
			do {
				ReadFile(hFiles, wcFile, sizeof wcFile, &size, NULL);
				pwc = wcFile;
				if(size > 2 && *pwc == GAP_UNICODE)
					size -= 2,
					++ pwc;
				if(size > 0)
					WriteFile(hFile, pwc, size, &size, NULL);
				total += size;
			} while(size == sizeof wcFile);
			CloseHandle(hFiles);
		}
		SetEndOfFile(hFile);
		CloseHandle(hFile);
	} else {
		if(index < 10)			//	Append section
			sprintf(szFile, GAS_SECTIONS, 1, index);
		else					//	Append scenario
		if(index > 10)
			sprintf(szFile, GAS_SCRIPT, 1);
		else
			sprintf(szFile, GAS_SCENARIO, 1);
		hFile = CreateFile(szFile, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
		if(INVALID_HANDLE_VALUE != hFile) {
			if(bits < 0 || index == 10) {	//	Print scenario/section
				do {
					ReadFile(hFile, wcFile, sizeof wcFile, &size, NULL);
					if(size > 1 && *wcFile == GAP_UNICODE)
						WriteConsoleW(hStdOut, wcFile + 1, (size - 1) / sizeof *wcFile, NULL, NULL);
					else
					if(size > 0)
						WriteConsoleW(hStdOut, wcFile, size / sizeof *wcFile, NULL, NULL);
				} while(size == sizeof wcFile);
			} else
			if(!pwc) {			//	Create section
				pwc[0] = GAP_UNICODE;
				err = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
				WriteFile(hFile, pwc, wcslen(pwc) * sizeof *pwc, &size, NULL);
				SetEndOfFile(hFile);
			} else {			//	Append section
				err = SetFilePointer(hFile, 0, NULL, FILE_END);
				//if(INVALID_SET_FILE_POINTER != err)
//				do {
//					ReadFile(hFile, wcFile, sizeof wcFile, &size, NULL);
//				} while(size == sizeof wcFile);
				WriteFile(hFile, pwc, wcslen(pwc) * sizeof *pwc, &size, NULL);
			}
			CloseHandle(hFile);
		}
	}
}

////
//	OK:$1
//		print the section #1
////
//	OK:$1
//		print the section #1
//	... append string ...
////
void	parse_command(PKLEDS pk, char **pString) {
	int const		SZ_BUFFER_SIZE	= 4096;
	int const		WC_BUFFER_SIZE	= SZ_BUFFER_SIZE / sizeof WCHAR;
	char static		szFile[SZ_BUFFER_SIZE];
	WCHAR static	wcCmd[WC_BUFFER_SIZE]	= {GAP_UNICODE};
	//
	HANDLE			hFile;
	INPUT_RECORD	ir[32];
	PWCHAR			pwc, ptr;
	long unsigned	size;
	long unsigned	remains;
	long			index;
	long			iSection;
	long			biSections;	//	Binary flags of used sections: Scenario §9 §8 §7 §6 §5 §4 §3 §2 §1 §0
	bool			is_user;
	bool			is_paste;
	bool			is_shift;
	//_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode((int)hStdOut, _O_U16TEXT);
	do {
		system("CLS");
		SetConsoleTextAttribute(hStdOut, 0x8E);
		printf("\t  Glazeur-console\r\n");
		printf("\t  Version 1.00.000\r\n");
		printf("\t  (C)2017 by Alikberov");
		SetConsoleTextAttribute(hStdOut, 0x0E);
		printf("\r\nSession#%d\r\nOK:\r\n", iSession);
		is_user = true;
		is_paste = false;
		iSection = -1;
		biSections = 0;
		sprintf(szFile, GAS_READ_ME, iSession);
		hFile = CreateFile(szFile, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
		while(INVALID_HANDLE_VALUE != hFile && (is_user || is_paste)) {
			pwc = &wcCmd[1];
			size = WC_BUFFER_SIZE;
			ReadConsoleW(hStdIn, pwc, size, &size, 0);
			PeekConsoleInputW(hStdIn, ir, sizeof ir / sizeof *ir, &remains);
			pwc[size] = 0;
			pwc[size + 1] = 0;
			if(is_paste && size >= 2)
				WriteFile(hFile, pwc, size * sizeof *pwc, &size, NULL);
			if(KEY_EVENT == ir[1].EventType && remains > 2)
				is_paste |= is_user && !is_paste;
			else
			if(is_paste)
				break;
			if(is_paste && is_user) {
				CloseHandle(hFile);
				sprintf(szFile, GAS_SCRIPT, iSession);
				hFile = CreateFile(szFile, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
				SetEndOfFile(hFile);
				if(size > 2)
					WriteFile(hFile, wcCmd, (size + 1) * sizeof *wcCmd, &size, NULL);
				is_user = false;
				continue;
			}
			is_user &= !is_paste && size >= 2;
			is_shift = GetKeyState(VK_SHIFT) < 0;
			index = -1;
			if(GEM_SECTION == *pwc || *pwc == '$')	//	Section prefix	- § or $
				if('0' <= pwc[1] && pwc[1] <= '9')	///	Section index	-  0..9
					index = pwc[1] - '0';
				else
				if('$' == pwc[1] && is_user)		//	Scenario prefix	-   $$
					index = 10;
			if(is_user) {							//	User prompt
				if(size <= 2 || (size == 3 && (':' == *pwc || *pwc == '.')))
					if(!is_shift)
						break;
				if(size <= 2 && is_shift) {
					PrintFile(hFile);
					continue;
				}
				//
				if(index >= 0) {					//	Switch to section/scenario
					CloseHandle(hFile);
					iSection = index;
					if(iSection < 10)
						sprintf(szFile, GAS_SECTIONS, 1, iSection);
					else
						sprintf(szFile, GAS_SCENARIO, 1);
					hFile = CreateFile(szFile, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
					SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
					if(size <= 2)
						PrintFile(hFile);
					 else {
						WriteFile(hFile, wcCmd, (size + 1) * sizeof *wcCmd, &size, NULL);
						SetEndOfFile(hFile);
					}
				} else
				if(size > 2)
					WriteFile(hFile, pwc, size * sizeof *pwc, &size, NULL);
			}
		}
/*		if(iSection == 10 && pwc[0] != '.') {//	Execute scenario
					if(biSections >= 0)
						section_access(pk, -1, (PWCHAR)biSections);
					biSections = -1;
				CloseHandle(hFile);
			sprintf(szFile, GAS_SCRIPT, iSession, 0);
			hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ*0 | FILE_SHARE_WRITE*0, NULL, OPEN_ALWAYS, 0, NULL);
		}*/
		if(is_paste || pwc[0] == ':') {
			if(!is_paste) {
				CloseHandle(hFile);
				sprintf(szFile, GAS_SCRIPT, iSession);
				hFile = CreateFile(szFile, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
			}
			size = GetFileSize(hFile, &size);
			ptr = (PWCHAR)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
			if(!!ptr) {
				SetFilePointer(hFile, 0l, NULL, FILE_BEGIN);
				ReadFile(hFile, ptr, size, &size, NULL);
			}
			if(hFile)
				CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			if(!!ptr) {
				system("CLS");
				printf("RUN");
				SetConsoleTextAttribute(hStdOut, 0x8E);
				parse_scenario(pk, ptr);
				while(GetKeyState(VK_ESCAPE) >= 0) {
					if(!parse_section(pk, -1))
						break;
					else
						for(index = 9; index >= 0; -- index) {
							if(GetKeyState('0' + index) < 0) {
								index = pk->Scenario.iKeys[(index + 9) % 10 + 1];
								if(index > 0) {
									pk->Scenario.iRow = index;
									break;
								}
							}
						}
					if(index < 0)
						for(index = 9; index >= 0; -- index) {
							if(GetKeyState(VK_NUMPAD0 + index) < 0) {
								index = pk->Scenario.iKeys[(index + 9) % 10 + 11];
								if(index > 0) {
									pk->Scenario.iRow = index;
									break;
								}
							}
						}
					if(index < 0)
						for(index = 25; index >= 0; -- index) {
							if(GetKeyState('A' + index) < 0) {
								index = pk->Scenario.iKeys[index + 21];
								if(index > 0) {
									pk->Scenario.iRow = index;
									break;
								}
							}
						}
				}
				VirtualFree(ptr, 0, MEM_FREE);
				FlushConsoleInputBuffer(hStdIn);
			}
		} else
		if('?' == *pwc) {
			printf("\r\n\t$0..$9\tView/append the paragraph");
			printf("\r\n\t$$\tView/append the basic scenario");
			printf("\r\n\t$\tView/append and compile whole scenario (ESC - break)");
			printf("\r\n\t.\rExit...");
		}
		if(INVALID_HANDLE_VALUE != hFile)
			CloseHandle(hFile);
	} while('.' != *pwc);
}

int main(int argc, char* argv[]) {
HANDLE		hScene;
DWORD		lScene;
PCHAR		pScene;
PCHAR		pAsc;
int			nLines;
int			i, n;

	SetConsoleCP(65001);
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
/*	if(argc < 2) {
		printf("Usage: Glazeur your_scenario_file\r\n");
		printf("Options: Glazeur --rate:<Hz> --queue:<indexes> --cmd");
		return 0;
	}*/
	GetKeyboardSymbolics();
	for(i = 0, n = 0; i < nKeyboards; ++ i) {
		hKbd[n] = CreateFileW(szKeyboards[i], GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if(INVALID_HANDLE_VALUE != hKbd[n])
			++ n;
	}
	nKeyboards = n;
	iSession = 1;
	play_cycle = play_null;
	play_cycle = parse_command;
	leds_cycle = ShowLEDs_Thread;
	for(i = 1; i < argc; ++ i) {
		if(argv[i] == strstr(argv[i], (pAsc = "--session::")))
			sscanf(argv[i] + strlen(pAsc), "%d", &iSession);
		else
		if(argv[i] == strstr(argv[i], (pAsc = "--queue:")))
			sscanf(argv[i] + strlen(pAsc), "%d", &LEDs.order);
		else
		if(argv[i] == strstr(argv[i], (pAsc = "--rate:")))
			sscanf(argv[i] + strlen(pAsc), "%d", &LEDs.speed);
		else
/*		if(argv[i] == strstr(argv[i], (pAsc = "--piano...")))
			play_cycle = play_piano;
		else
*/		if(argv[i] == strstr(argv[i], (pAsc = "--user")))
			;//play_cycle = parse_user;
		else
		if(argv[i] == strstr(argv[i], (pAsc = "--web:")))
			sscanf(argv[i] + strlen(pAsc), "%d", &port),
			play_cycle = parse_socket;
		else
		if(argv[i] == strstr(argv[i], (pAsc = "--cmd")))
			play_cycle = parse_command;
		else
		if(argv[i] == strstr(argv[i], (pAsc = "--new_leds")))
			leds_cycle = ShowLEDs_Thread;
		else
		if(argv[i] == strstr(argv[i], (pAsc = "--neo_leds")))
			leds_cycle = ShowLEDs_Thread;
		else
		if(argv[i] == strstr(argv[i], (pAsc = "--play:"))) {
//			play_cycle = play_once;
			pScene = argv[i] + strlen(pAsc);
		}
	}
	LEDs.speed = 1000;
	LEDs.order = 1524*0;
	LEDs.errors = 0;
	LEDs.leds[0] = 000076325410;
	LEDs.leds[1] = 000000011111;
	LEDs.leds[2] = 000002222200;
	LEDs.leds[3] = 000333330000;
	LEDs.leds[4] = 004444000004;
	LEDs.leds[5] = 005500000555;
	LEDs.leds[6] = 000002222200;
	LEDs.leds[7] = 000333330000;
	LEDs.leds[8] = 004444000004;
	LEDs.leds[9] = 005500000555;
	for(i = -1; i < 10; ++ i) {
		LEDs.Sections[i].pSection = &wszMeasures[i][0];
	}
	if(play_cycle != play_null) {
		hThread = CreateThread(0, 0, leds_cycle, (LPVOID)&LEDs, 0, 0);// (LPVOID)&aLEDs, 0, 0);
		play_cycle(&LEDs, &pScene);
		LEDs.speed = -1;
		WaitForSingleObject(hThread, INFINITE);
	} else {
		printf("Number of keyboards is %d", nKeyboards);
		hScene = CreateFile(argv[argc - 1], GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if(INVALID_HANDLE_VALUE == hScene) {
			printf("Error: Can't openning %s", argv[argc - 1]);
			return 1;
		}
		lScene = GetFileSize(hScene, &lScene);
		pScene = (PCHAR)VirtualAlloc(NULL, lScene, MEM_COMMIT, PAGE_READWRITE);
		if(!!pScene)
			ReadFile(hScene, pScene, lScene, &lScene, NULL);
		CloseHandle(hScene);
		if(!pScene) {
			printf("Error: Can't allocate for %d bytes", lScene);
			return 2;
		}
		pAsc = pScene;
		nLines = 1;
		while(!!pAsc[0]) {
			if(*pAsc >= ' ' && pAsc ++ && nLines > 1)
				continue;
			if(*pAsc == 0xD && nLines ++)
				if(* ++ pAsc == 0xA)
					++ pAsc;
			if(pAsc == strstr(pAsc, "Glazeur")) {
			}
		}
		VirtualFree(pScene, 0, MEM_FREE);
	}
	while(nKeyboards --)
		CloseHandle(hKbd[nKeyboards]);
	return 0;
}