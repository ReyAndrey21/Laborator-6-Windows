#include <iostream>
#include <windows.h>
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

int main(int argc, wchar_t* argv[]) {
    if (argc < 3) {
        std::wcerr << L"Error: Missing arguments" << std::endl;
        return 1;
    }

    int start = std::stoi(argv[1]);
    int end = std::stoi(argv[2]);

    std::wostringstream result;
    for (int num = start; num <= end; ++num) {
        if (isPrime(num)) {
            result << num << L" ";
        }
    }

    // Scrierea rezultatelor în stdout (care va fi redirec?ionat? în pipe)
    std::wcout << result.str();

    return 0;
}
