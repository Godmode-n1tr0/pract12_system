#include <windows.h>
#include <iostream>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"Global\\MyPrinterMutex");
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Global\\MyPrinterEvent");

    if (!hMutex || !hEvent)
    {
        cout << "Принтер не найден!\n";
        Sleep(2000);
        return 1;
    }

    cout << "Выберите приоритет:\n";
    cout << "1 - Низкий\n2 - Нормальный\n3 - Высокий\n";

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1: SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS); break;
    case 3: SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS); break;
    default: SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
    }

    cout << "Отправка задания на печать...\n";

    WaitForSingleObject(hMutex, INFINITE);

    SetEvent(hEvent);  

    ReleaseMutex(hMutex);

    cout << "Ожидание завершения печати...\n";

    WaitForSingleObject(hMutex, INFINITE);
    ReleaseMutex(hMutex);

    cout << "Печать завершена. Клиент закрывается.\n";

    CloseHandle(hMutex);
    CloseHandle(hEvent);

    Sleep(2000);
    return 0;
}