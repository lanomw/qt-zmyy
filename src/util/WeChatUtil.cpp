//
// Created by arc on 2022/1/19.
//

#include "WeChatUtil.h"

bool WeChatUtil::InjectDLL(const QString& pName, QString dllPath) {
    /*
     * dll注入流程
     * 1. 获取微信进程句柄
     *      - 通过微信进程名称查找微信pid(进程id)，然后通过pid打开微信获取进程句柄
     * 2. 在微信内部申请内存来存放dll
     * 3. 写入dll路径，然后通过远程线程执行函数去执行`loadLibrary`这个函数来加载写入的dll
     */

    dllPath = QDir::toNativeSeparators(dllPath);
    char * cDLLPath = dllPath.toLatin1().data();
    qDebug() << "========================================= InjectDLL Start ==============================================";
    qDebug() << "InjectDLL args:  ==== " << pName << " --- " << dllPath;

    // 取得进程id
    int pid = findPid(pName);
    if (!pid) {
        qDebug() << "InjectDLL ==== " << "未找到进程";
        return false;
    }

    if (!setPrivilege()) {
        qDebug() << "InjectDLL ==== " << "权限提升失败";
    }

    // 打开进程
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (!process) {
        qDebug() << "InjectDLL ==== " << "进程打开失败。可能是权限不足或关闭了应该";
        return false;
    }

    // 申请内存空间
    LPVOID pDLLPath = VirtualAllocEx(process, nullptr, dllPath.length(), MEM_COMMIT, PAGE_READWRITE);
    if (!pDLLPath) {
        qDebug() << "InjectDLL ==== " << "内存分配失败";
        CloseHandle(process);
        return false;
    }
    qDebug() << "InjectDLL: dll address:" << pDLLPath;

    // dll路径写入内存
    if (!WriteProcessMemory(process, pDLLPath, cDLLPath, dllPath.length(), nullptr)) {
        qDebug() << "InjectDLL ==== " << "DLL路径写入失败";
        CloseHandle(process);
        CloseHandle(pDLLPath);
        return false;
    }

    // 获取LoadLibrary函数地址
    FARPROC loadLibrary = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!loadLibrary) {
        qDebug() << "InjectDLL ==== " << "library地址获取失败";
        CloseHandle(process);
        CloseHandle(pDLLPath);
        return false;
    }

    // 在目标进程执行LoadLibrary 注入指定的线程
    HANDLE tHandle = CreateRemoteThread(process, nullptr, 0, (LPTHREAD_START_ROUTINE)loadLibrary, pDLLPath, 0, nullptr);
    if (!tHandle) {
        qDebug() << "InjectDLL ==== " << "线程创建失败";
        CloseHandle(process);
        CloseHandle(pDLLPath);
        return false;
    }

    qDebug() << "InjectDLL ==== " << "DLL注入中...";
    WaitForSingleObject(tHandle, INFINITE);
    CloseHandle(process);
    CloseHandle(pDLLPath);
    CloseHandle(tHandle);
    qDebug() << "InjectDLL ==== " << "DLL注入完成";
    qDebug() << "========================================= InjectDLL End ==============================================";

    return true;
}

bool WeChatUtil::EjectDLL(const QString& pName, QString dllPath) {
    /*
     * dll卸载流程
     *
     * 1. 获取微信进程句柄
     *      - 通过微信进程名称查找微信pid(进程id)，然后通过pid打开微信获取进程句柄
     * 2. 在微信内部申请内存来存放dll
     * 3. 写入dll路径，然后通过远程线程执行函数去执行`FreeLibrary`这个函数卸载的dll
     */

    dllPath = QDir::toNativeSeparators(dllPath);
    QString dllName = getDLLName(dllPath);
    qDebug() << "========================================= InjectDLL Start ==============================================";
    qDebug() << "EjectDLL args:  ==== " << pName << " --- " << dllPath;

    // 取得进程id
    int pid = findPid(pName);
    if (!pid) {
        qDebug() << "EjectDLL ==== " << "未找到进程";
        return false;
    }

    if (!setPrivilege()) {
        qDebug() << "InjectDLL ==== " << "权限提升失败";
    }

    // 打开进程
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (!process) {
        qDebug() << "EjectDLL ==== " << "进程打开失败。可能是权限不足或关闭了应该";
        return false;
    }

    // 获取卸载dll的模块句柄
    HANDLE snapHandele = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (INVALID_HANDLE_VALUE == snapHandele) {
        qDebug() << "EjectDLL ==== " << "进程快照获取失败";
        CloseHandle(process);
        return false;
    }

    // 获取dll地址
    HMODULE pDLLPath;
    MODULEENTRY32 entry = { 0 };
    entry.dwSize = sizeof(entry);// 长度必须赋值
    BOOL ret = Module32First(snapHandele, &entry);
    while (ret) {
        qDebug() << "compare ==== " << dllName << " -- " << entry.szModule;
        if (dllName == entry.szModule) {
            pDLLPath = entry.hModule;
            qDebug() << "compare success ==== " << entry.hModule;
            break;
        }
        ret = Module32Next(snapHandele, &entry);
    }
    CloseHandle(snapHandele);
    if (!pDLLPath) {
        qDebug() << "EjectDLL ==== " << "dll未加载";
        CloseHandle(process);
        return false;
    }
    qDebug() << "EjectDLL: dll address:" << pDLLPath;

    // 获取FreeLibrary函数地址
    FARPROC loadLibrary = GetProcAddress(GetModuleHandleA("FreeLibrary"), "FreeLibrary");
    if (!loadLibrary) {
        qDebug() << "EjectDLL ==== " << "library地址获取失败";
        CloseHandle(process);
        return false;
    }

    // 在目标进程执行FreeLibrary 卸载指定的线程
    HANDLE tHandle = CreateRemoteThread(process, nullptr, 0, (LPTHREAD_START_ROUTINE)loadLibrary, pDLLPath, 0, nullptr);
    if (!tHandle) {
        qDebug() << "EjectDLL ==== " << "线程创建失败";
        CloseHandle(process);
        CloseHandle(pDLLPath);
        return false;
    }

    qDebug() << "EjectDLL ==== " << "DLL卸载中...";
    WaitForSingleObject(tHandle, INFINITE);
    CloseHandle(process);
    CloseHandle(pDLLPath);
    CloseHandle(tHandle);
    qDebug() << "EjectDLL ==== " << "DLL卸载完成";
    qDebug() << "========================================= InjectDLL End ==============================================";

    return true;
}

int WeChatUtil::findPid(const QString& pName) {
    // 获取进程列表
    HANDLE processAll = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (processAll == INVALID_HANDLE_VALUE){
        qDebug() << "findPid ==== " << "进程快照创建失败";
        return 0;
    }

    // 对比进程
    PROCESSENTRY32 process = {sizeof(PROCESSENTRY32) };
    do {
        qDebug() << "compare ==== " << QString(process.szExeFile) << " -- " << pName;
        if (QString(process.szExeFile) == pName) {
            qDebug() << "compare success ==== " << process.th32ProcessID;
            CloseHandle(processAll);
            return process.th32ProcessID;
        }
    } while (Process32Next(processAll, &process));

    CloseHandle(processAll);
    return 0;
}

QString WeChatUtil::getDLLName(const QString& dllPath) {
    QList<QString> arr = dllPath.split("/");
    return arr[arr.length() - 1];
}

bool WeChatUtil::setPrivilege() {
    HANDLE token;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) {
        qDebug() << "setPrivilege ==== " << "打开进程令牌失败";
        return false;
    }

    // 获取进程本地唯一ID
    LUID id;
    if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &id)) {
        qDebug() << "setPrivilege ==== " << "获取LUID失败";
        CloseHandle(token);
        return false;
    }

    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1; // 要提升的权限个数
    tp.Privileges[0].Luid = id;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // 调整进程权限
    if (!AdjustTokenPrivileges(token, 0, &tp, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr)) {
        qDebug() << "setPrivilege ==== " << "提权失败";
        CloseHandle(token);
        return false;
    }

    CloseHandle(token);

    return true;
}
