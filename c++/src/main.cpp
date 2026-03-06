#include <iostream>
#include <windows.h>   // 解决中文乱码
#include "LibraryManager.h"

int main() {
    // 设置控制台输出为UTF-8
    SetConsoleOutputCP(CP_UTF8);

    LibraryManager system;
    system.run();

    return 0;
}