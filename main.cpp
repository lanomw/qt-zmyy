#include <QApplication>
#include <QStyleFactory>
#include "src/ui/MainWidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    // 使用系统自带样式  QT帮助文档搜索“gallery” 获取更多主题样式的介绍
    QApplication::setStyle(QStyleFactory::create("fusion"));

    MainWidget widget;
    widget.show();

    return QApplication::exec();
}
