#include <iostream>
#include <windows.h>
#include <vector>
#include <sstream>
#include <string>
#include <cwchar>  // Pentru wstring ?i wchar_t

// Func?ie pentru verificarea numerelor prime
bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

int main() {
    const int numProcesses = 10;
    const int range = 1000;
    const int maxNumber = 10000;

    HANDLE pipes[numProcesses][2]; // Pipe-uri pentru fiecare proces
    PROCESS_INFORMATION processInfo[numProcesses]; // Informa?ii pentru procese
    STARTUPINFO startupInfo[numProcesses]; // Informa?ii pentru procesul p?rinte

    // Crearea pipe-urilor
    for (int i = 0; i < numProcesses; ++i) {
        if (!CreatePipe(&pipes[i][0], &pipes[i][1], NULL, 0)) {
            std::cerr << "Error creating pipe " << i << std::endl;
            return 1;
        }
    }

    // Crearea proceselor copil
    for (int i = 0; i < numProcesses; ++i) {
        ZeroMemory(&startupInfo[i], sizeof(STARTUPINFO));
        startupInfo[i].cb = sizeof(STARTUPINFO);

        ZeroMemory(&processInfo[i], sizeof(PROCESS_INFORMATION));

        // Crearea comenzii care va fi executat? de procesul copil
        std::wostringstream command;  // Folosim wostringstream pentru a construi comanda wide
        command << L"child.exe " << i * range + 1 << L" " << (i + 1) * range;

        // Conversia comenzii de la std::wstring la LPWSTR (wchar_t*)
        std::wstring commandStr = command.str();
        LPWSTR commandLPWSTR = &commandStr[0];

        // Crearea unui proces copil
        if (!CreateProcess(
            NULL, // Aplica?ia sau comanda de executat
            commandLPWSTR, // Comanda
            NULL, // Atributele procesului
            NULL, // Atributele thread-ului
            TRUE, // Permite comunicarea prin pipe
            0,    // Op?iuni de creare
            NULL, // Mediu (folosit pentru variabile de mediu)
            NULL, // Directorul curent
            &startupInfo[i], // Informa?iile despre proces
            &processInfo[i]  // Informa?iile despre procesul creat
        )) {
            std::cerr << "Error creating process " << i << std::endl;
            return 1;
        }

        // Închide cap?tul de citire al pipe-ului în procesul p?rinte
        CloseHandle(pipes[i][0]);
    }

    // Procesul p?rinte colecteaz? ?i afi?eaz? rezultatele
    for (int i = 0; i < numProcesses; ++i) {
        // Citirea rezultatelor din pipe
        DWORD bytesRead;
        char buffer[4096] = { 0 };
        if (ReadFile(pipes[i][1], buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            std::cout << "Proces " << i + 1 << ": " << buffer << std::endl;
        }
        CloseHandle(pipes[i][1]); // Închide cap?tul de scriere
    }

    // A?teapt? finalizarea tuturor proceselor copil
    for (int i = 0; i < numProcesses; ++i) {
        WaitForSingleObject(processInfo[i].hProcess, INFINITE);
        CloseHandle(processInfo[i].hProcess);
        CloseHandle(processInfo[i].hThread);
    }

    return 0;
}
