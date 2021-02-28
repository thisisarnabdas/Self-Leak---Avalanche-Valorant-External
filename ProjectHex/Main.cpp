#include "Main.h"
#include "others/xor.h"
#include "others/auth.h"

uint64_t add[16];

bool k_f5 = 0;
bool k_f6 = 0;
bool k_f7 = 0;
bool k_f8 = 0;

bool IsKeyDown(int vk)
{
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

int aim_key = VK_RBUTTON;

std::string userid = (_xor_("37").c_str());
std::string ProgramID = (_xor_("jI2FOMD8p34E").c_str());

#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define BROWN			6
#define LIGHTGRAY		7
#define DARKGRAY		8
#define LIGHTBLUE		9
#define LIGHTGREEN		10
#define LIGHTCYAN		11
#define LIGHTRED		12
#define LIGHTMAGENTA	13
#define YELLOW			14
#define WHITE			15

bool IsValid = false;

static const char allan[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZbcdefghijklmnopqrstuvwxyz1234567890";
int strLength = sizeof(allan) - 1;

char randomization2()
{
	return allan[rand() % strLength];
}

void Randomization()
{
	std::string Str;
	srand(time(0));
	for (unsigned int i = 0; i < 14; ++i)
	{
		Str += randomization2();
	}
	SetConsoleTitleA(Str.c_str());
}


HWND consoleWindowHandle = GetConsoleWindow();
void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

void SetColor(unsigned short color)
{
	HANDLE con = 0;
	con = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(con, color);
}

void log(const char* text)
{
	SetColor(WHITE);
	std::cout << (_xor_(" [").c_str());
	SetColor(GREEN);
	std::cout << (_xor_("+").c_str());
	SetColor(WHITE);
	std::cout << (_xor_("] ").c_str());
	SetColor(WHITE);
	std::cout << (text);
}

void log2(const char* text)
{
	SetColor(WHITE);
	std::cout << (_xor_(" [").c_str());
	SetColor(GREEN);
	std::cout << (_xor_(">").c_str());
	SetColor(WHITE);
	std::cout << (_xor_("] ").c_str());
	SetColor(WHITE);
	std::cout << (text);
}

void done(const char* text)
{
	SetColor(WHITE);
	std::cout << (_xor_(" [").c_str());
	SetColor(GREEN);
	std::cout << (text);
	SetColor(WHITE);
	std::cout << (_xor_("] ").c_str());
}

bool processexists(const wchar_t* processname)
{
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		return false;
	}

	PROCESSENTRY32W entry = { 0 };
	entry.dwSize = sizeof(entry);
	if (Process32First(snapshot, &entry)) {
		do {
			if (_wcsicmp(entry.szExeFile, processname) == 0) {
				return true;
				break;
			}
		} while (Process32Next(snapshot, &entry));
	}

	CloseHandle(snapshot);

	return false;
}

int main()
{
	std::cout << std::endl;
	log(_xor_("Initializing").c_str());
	std::cout << std::endl;
	system(_xor_("taskkill /f /im EpicGamesLauncher.exe >nul 2>&1").c_str());
	system(_xor_("taskkill /f /im FortniteClient-Win64-Shipping.exe >nul 2>&1").c_str());
	system(_xor_("taskkill /f /im OneDrive.exe >nul 2>&1").c_str());
	system(_xor_("taskkill /f /im RustClient.exe >nul 2>&1").c_str());
	system(_xor_("taskkill /f /im Origin.exe >nul 2>&1").c_str());
	system(_xor_("taskkill /f /im r5apex.exe >nul 2>&1").c_str());
	system(_xor_("taskkill /f /im vanguard.exe >nul 2>&1").c_str());
	Randomization();

	log(_xor_("Enter License: ").c_str());
	std::string i2;
	std::cin >> i2;

	log(_xor_("Status: Subscription Active (Valorant)").c_str());
	Sleep(1525);
	system("cls");
	std::cout << std::endl;
	log2(_xor_("Start Valorant-Win64-Shipping.exe").c_str());
	while (true)
	{
		Sleep(200);
		if (processexists(L"VALORANT-Win64-Shipping.exe"))
			break;
	}
	done(_xor_("ok").c_str());
	std::cout << std::endl;
	std::cout << std::endl;
	log(_xor_("Checking Files").c_str());
	LoadProtectedFunctions();
	Protect(LoadProtectedFunctions);
	Unprotect(Driver::initialize);
	Unprotect(CheckDriverStatus);

	if (!Driver::initialize() || !CheckDriverStatus()) {
		wchar_t VarName[] = { 'F','a','s','t','B','o','o','t','O','p','t','i','o','n','\0' };
		UNICODE_STRING FVariableName = UNICODE_STRING();
		FVariableName.Buffer = VarName;
		FVariableName.Length = 28;
		FVariableName.MaximumLength = 30;
		myNtSetSystemEnvironmentValueEx(
			&FVariableName,
			&DummyGuid,
			0,
			0,
			ATTRIBUTES);
		memset(VarName, 0, sizeof(VarName));
		Beep(600, 1000);
		log(_xor_("Driver not Loaded").c_str());
		ProtectedSleep(3000);
		exit(1);
		return 1;
	}
	Protect(Driver::initialize);
	Protect(CheckDriverStatus);
	Sleep(1000);
	done(_xor_("ok").c_str());
	std::cout << "" << std::endl;
	log(_xor_("Setting Up").c_str());
	SetupWindow();
	DirectXInit(MyWnd);
	verify_game();
	HANDLE hdl = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(cache), nullptr, NULL, nullptr);
	CloseHandle(hdl);
	done(_xor_("ok").c_str());
	std::cout << "" << std::endl;
	std::cout << "" << std::endl;
	Beep(500, 500);
	log(_xor_("Injection: Success").c_str());

	while (TRUE)
	{
		MainLoop();
	}
	log("how are you here?");
	Sleep(10000);
	return 0;
}






























