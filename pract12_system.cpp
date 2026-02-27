#include <windows.h>
#include <iostream>
#include <ctime>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");


    HANDLE hMutex = CreateMutex(NULL, FALSE, L"Global\\MyPrinterMutex");
    HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, L"Global\\MyPrinterEvent");

    if (!hMutex || !hEvent)
    {
        cout << "Ошибка создания объектов!\n";
        return 1;
    }

    cout << "Принтер запущен.\n";
    cout << "Ожидание клиента...\n";

    DWORD lastActivity = GetTickCount64();

    while (true)
    {
        DWORD result = WaitForSingleObject(hEvent, 1000);

        if (result == WAIT_OBJECT_0)
        {
            WaitForSingleObject(hMutex, INFINITE);

            cout << "\nКлиент подключён.\n";

            srand((unsigned)time(NULL));
            int printTime = 5 + rand() % 11;
            cout << "Начало печати: " << printTime << " Секунд\n";

            for (int i = 1; i <= printTime; i++)
            {
                cout << "Печать: " << i << " сек...\n";
                Sleep(1000);
            }

            if (printTime > 10)
                cout << "Ошибка печати! (более 10 сек)\n";
            else
                cout << "Печать успешно завершена.\n";

            ReleaseMutex(hMutex);

            cout << "Ожидание клиента...\n";

            lastActivity = GetTickCount64();
        }

        if (GetTickCount64() - lastActivity > 600000)
        {
            cout << "\nНет клиентов 10 минут. Принтер выключается...\n";
            break;
        }
    }

    CloseHandle(hMutex);
    CloseHandle(hEvent);
    return 0;
}