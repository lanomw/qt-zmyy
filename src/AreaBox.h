//
// Created by 74079 on 2022/1/8.
//

#ifndef QT_ZMYY_AREABOX_H
#define QT_ZMYY_AREABOX_H

#include <QDebug>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QPushButton>
#include <QFile>
#include <QCoreApplication>
#include <QByteArray>
#include <QMessageBox>
#include <QString>
#include "Config.h"
#include "lib/xpack/json.h"
#include "model/AreaCate.h"

using namespace std;

class AreaBox : public QDialog {
Q_OBJECT

public:
    AreaBox();

signals:

    // 关闭窗口
    void confirm(double lat, double lng, const QString area);

private slots:

    // 搜索
    void searchArea();

    // 确定
    void confirmHandler();

    // 列表数据
    void loadList();

    void loadList(const QString &keyword);

    // 导航列表选中
    void navClick();

/*----------------------- 控件 -------------------------*/
private:
    QLineEdit *lineEdit;
    QPushButton *searchBtn;
    QListWidget *listWidget;
    QListWidget *navWidget;
    QList<QListWidgetItem *> *areaList;
    QList<QListWidgetItem *> *navList;
    QPushButton *confirmBtn;
};


#endif //QT_ZMYY_AREABOX_H
