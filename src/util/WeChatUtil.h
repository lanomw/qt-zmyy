//
// Created by arc on 2022/1/19.
//

#ifndef INJECTDLL_WECHATUTIL_H
#define INJECTDLL_WECHATUTIL_H

#include <QDebug>
#include <QString>
#include <QList>
#include <QDir>
#include <windows.h>
#include <tlhelp32.h>

class WeChatUtil {
public:
    /**
     * 注入DLL
     * @param pName - 进程名称。区分大小写
     * @param dllPath - DLL完整路径
     * @return
     */
    static bool InjectDLL(const QString& pName, QString dllPath);

    /**
     * 卸载DLL
     * @param pName - 进程名称。区分大小写
     * @param dllPath - DLL完整路径
     * @return
     */
    static bool EjectDLL(const QString& pName, QString dllPath);

private:

    /**
     * 根据进程名字查找进程id
     * @param pName  - 进程名称
     * @return
     */
    static int findPid(const QString& pName);

    /**
     * 根据DLL路径获取DLL名称
     * @param dllPath  - 进程名称
     * @return
     */
    static QString getDLLName(const QString& dllPath);

    /**
     * 权限提升
     */
    static bool setPrivilege();

};


#endif //INJECTDLL_WECHATUTIL_H
