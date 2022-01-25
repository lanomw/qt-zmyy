//
// Created by 74079 on 2022/1/8.
//

#include "AreaBox.h"

std::list<AreaCate> areaCate;

AreaBox::AreaBox() {
    setWindowTitle("区域选择");
    resize(300, 500);

    QVBoxLayout *qvBoxLayout = new QVBoxLayout(this);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    lineEdit = new QLineEdit;
    lineEdit->setPlaceholderText("输入内容搜索");
    searchBtn = new QPushButton("搜索");

    searchLayout->addWidget(lineEdit);
    searchLayout->addWidget(searchBtn);

    QHBoxLayout *listLayout = new QHBoxLayout;
    listWidget = new QListWidget;
    areaList = new QList<QListWidgetItem *>();
    listWidget->addItem("加载中...");
    navWidget = new QListWidget;
    navList = new QList<QListWidgetItem *>();
    navWidget->setFixedWidth(30);
    navWidget->addItem("#");
    listLayout->addWidget(listWidget);
    listLayout->addWidget(navWidget, 1, Qt::AlignRight);

    confirmBtn = new QPushButton("确定");

    qvBoxLayout->addLayout(searchLayout);
    qvBoxLayout->addLayout(listLayout, 1);
    qvBoxLayout->addWidget(confirmBtn);

    // 监听信号
    connect(lineEdit, &QLineEdit::returnPressed, this, &AreaBox::searchArea);
    connect(searchBtn, &QPushButton::clicked, this, &AreaBox::searchArea);
    connect(confirmBtn, &QPushButton::clicked, this, &AreaBox::confirmHandler);

    connect(navWidget, &QListWidget::itemClicked, this, &AreaBox::navClick);

    loadList();
}

// 导航列表选中
void AreaBox::navClick() {
    int index = navWidget->currentIndex().row();
    listWidget->scrollToItem(areaList->at(index), QListWidget::PositionAtTop);
}

// 列表数据
void AreaBox::loadList() {
    loadList("");
}

// 列表数据
void AreaBox::loadList(const QString &keyword = "") {
    if (areaCate.size() == 0) {
        QFile file(area_file);
        file.open(QFile::ReadOnly | QFile::Text);
        QByteArray byteArray = file.readAll();
        string str = byteArray.toStdString();

        xpack::json::decode(str, areaCate);

        file.close();
    }

    listWidget->clear();
    navWidget->clear();
    for (const auto &item: areaCate) {
        navWidget->setHidden(!keyword.isEmpty());

        if (keyword.isEmpty()) {
            // 导航
            QListWidgetItem *label = new QListWidgetItem(item.name);
            label->setFlags(Qt::ItemIsUserCheckable);
            label->setBackground(QBrush("#ededef"));
            label->setFont(QFont("", 24, 50));
            listWidget->addItem(label);
            areaList->append(label);

            // 导航快捷按钮
            QListWidgetItem *nav = new QListWidgetItem(item.name);
            navWidget->addItem(nav);
            navList->append(nav);
        }

        // 地区
        for (const auto &area: item.children) {
            QString name = area.name;
            if (!keyword.isEmpty() && !name.contains(keyword, Qt::CaseInsensitive)) {
                continue;
            }

            QListWidgetItem *listWidgetItem = new QListWidgetItem(name);
            listWidgetItem->setToolTip(area.center);
            listWidget->addItem(listWidgetItem);
        }
    }
}

// 搜索
void AreaBox::searchArea() {
    QString keyword = lineEdit->text();
    loadList(keyword);
}

// 确定
void AreaBox::confirmHandler() {
    QListWidgetItem *item = listWidget->currentItem();

    QList<QString> arr = item->toolTip().split(",");
    double lng = arr[0].trimmed().toDouble();
    double lat = arr[1].trimmed().toDouble();

    emit confirm(lat, lng, item->text());
    close();
}