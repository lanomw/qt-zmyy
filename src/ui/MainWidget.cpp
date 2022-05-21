//
// Created by arc on 2022/2/11.
//

#include "MainWidget.h"

MainWidget::MainWidget() {
    setWindowTitle("知苗易约");
    resize(1200, 700);

    // 左边
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(createAreaLayout());
    leftLayout->addLayout(createCateLayout());
    leftLayout->addWidget(createHospitalLayout(), 1);
    leftLayout->addWidget(createProductLayout(), 1);
    leftLayout->addWidget(createLogLayout());

    // 右边
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(createInfoLayout(), 1);
    rightLayout->addWidget(createCookieLayout(), 1);
    rightLayout->addWidget(createSubLayout(), 2);

    // 最外层容器
    QHBoxLayout *container = new QHBoxLayout(this);
    container->addLayout(leftLayout, 1);
    container->addLayout(rightLayout);

    // 逻辑部分移入新线程执行
    qThread = new QThread(this);
    mainService = new MainService;
    mainService->moveToThread(qThread);
    qThread->start();

    // 注册传输数据类型
    qRegisterMetaType<QList<Hospital>>("QList<Hospital>");
    qRegisterMetaType<QList<Hospital>>("QList<Hospital>&");
    qRegisterMetaType<QList<Product>>("QList<Product>");
    qRegisterMetaType<QList<Product>>("QList<Product>&");
    qRegisterMetaType<QList<Cate>>("QList<Cate>");
    qRegisterMetaType<QList<Cate>>("QList<Cate>&");
    qRegisterMetaType<LogType>("LogType");

    // 通信关联
    connect(this, &MainWidget::fetchCate, mainService, &MainService::fetchCate); // 获取分类
    connect(this, &MainWidget::fetchHospital, mainService, &MainService::fetchHospital); // 获取医院列表
    connect(this, &MainWidget::fetchProduct, mainService, &MainService::fetchProduct); // 获取产品列表
    connect(this, &MainWidget::getStorage, mainService, &MainService::getStorage); // 获取Storage数据
    connect(this, &MainWidget::setCookie, mainService, &MainService::setCookie); // 设置cookie
    connect(this, &MainWidget::saveStorageByCity, mainService, &MainService::saveStorageByCity); // 保存Storage数据
    connect(this, &MainWidget::getUser, mainService, &MainService::getUser); // 获取用户信息
    connect(this, &MainWidget::enableTask, mainService, &MainService::enableTask); // 定时任务

    // ui关联service
    connect(mainService, &MainService::logger, this, &MainWidget::logger); // 日志输出
    connect(mainService, &MainService::widgetDisable, this, &MainWidget::widgetDisable); // 控件启用/禁用
    connect(mainService, &MainService::renderCate, this, &MainWidget::renderCate); // 渲染分类
    connect(mainService, &MainService::renderHospital, this, &MainWidget::renderHospital); // 渲染医院
    connect(mainService, &MainService::renderProduct, this, &MainWidget::renderProduct); // 渲染产品
    connect(mainService, &MainService::renderStorage, this, &MainWidget::renderStorage); // 渲染本地存储
    connect(mainService, &MainService::renderUser, this, &MainWidget::renderUser); // 渲染用户信息
    //关闭窗口的时候回收线程资源
    connect(this, &QWidget::destroyed, this, [=](){
        if (qThread->isRunning()) {
            qThread->quit();
            qThread->wait();
        }
    });

    logger(LogType::INFO, "========== 主界面构建完成 ==========");

    // 数据请求
    emit fetchCate(0);
    emit getStorage();
}

MainWidget::~MainWidget() {
    areaBox = nullptr;
    if (qThread->isRunning()) {
        qThread->quit();
        qThread->wait();
    }
    delete mainService;
}

/*----------------------- 界面主要模块 -------------------------*/
// 区域
QHBoxLayout *MainWidget::createAreaLayout() {
    QHBoxLayout *areaLayout = new QHBoxLayout;

    chooseArea = new QPushButton("未选择地区");
    chooseArea->setStyleSheet("QPushButton {border: none;}");
    chooseArea->setToolTip("点击选择地区");

    areaLayout->addWidget(new QLabel("当前地区："));
    areaLayout->addWidget(chooseArea);
    areaLayout->addStretch();

    // 打开区域选择窗口
    connect(chooseArea, &QPushButton::clicked, this, [this] {
        if (!areaBox) {
            areaBox = new AreaBox;
            // 信号监听
            connect(areaBox, &AreaBox::confirm, this, [this](double lat, double lng, const QString &cityName) {
                chooseArea->setText(cityName.isEmpty() ? "未选择地区" : cityName);
                emit saveStorageByCity(lat, lng, cityName);
            });
        }
        areaBox->show();
    });

    return areaLayout;
}

// 分类
QHBoxLayout *MainWidget::createCateLayout() {
    QHBoxLayout *cateLayout = new QHBoxLayout;
    oneCate = new QComboBox;
    oneCate->setMinimumWidth(200);
    twoCate = new QComboBox;
    twoCate->setMinimumWidth(200);
    oneCate->addItem("暂无数据", 0);
    twoCate->addItem("暂无数据", 0);
    cateConfirm = new QPushButton("查询医院");

    cateLayout->addWidget(new QLabel("一级分类："));
    cateLayout->addWidget(oneCate);
    cateLayout->addWidget(new QLabel("二级分类："));
    cateLayout->addWidget(twoCate);
    cateLayout->addWidget(cateConfirm);
    cateLayout->addStretch();

    // 一级分类切换
    connect(oneCate, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this] {
        int id = oneCate->currentData().value<int>();
        if (id) {
            twoCate->clear();
            twoCate->addItem("暂无数据", 0);
            emit fetchCate(id);
        }
    });
    // 分类下的医院
    connect(cateConfirm, &QPushButton::clicked, this, [this]() {
        hTableWidget->clearContents();
        hTableWidget->setRowCount(1);
        hTableWidget->setItem(0, 3, new QTableWidgetItem("加载中..."));
        emit fetchHospital(twoCate->currentData().value<int>());
    });

    return cateLayout;
}

// 医院
QGroupBox *MainWidget::createHospitalLayout() {
    hTableWidget = new QTableWidget;
    hTableWidget->setShowGrid(false);
    hTableWidget->setColumnCount(5);
    hTableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "医院名称" << "地址" << "联系电话" << "通知");
    QHeaderView *headerView = hTableWidget->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch);
    headerView->setSectionResizeMode(0, QHeaderView::Custom);
    headerView->setSectionResizeMode(3, QHeaderView::Custom);
    hTableWidget->setColumnWidth(0, 60);
    hTableWidget->setColumnWidth(3, 120);
    hTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    hTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    hTableWidget->setRowCount(1);
    hTableWidget->setItem(0, 3, new QTableWidgetItem("暂无数据"));

    QGroupBox *box = new QGroupBox("医院列表");
    QHBoxLayout *qhBoxLayout = new QHBoxLayout;
    qhBoxLayout->addWidget(hTableWidget);
    box->setLayout(qhBoxLayout);

    return box;
}

// 产品
QGroupBox *MainWidget::createProductLayout() {
    pTableWidget = new QTableWidget;
    pTableWidget->setShowGrid(false);
    pTableWidget->setColumnCount(8);
    pTableWidget->setHorizontalHeaderLabels(
            QStringList() << "ID" << "名称" << "价格/针次" << "警告" << "标签" << "预约时间" << "状态" << "备注");
    QHeaderView *headerView = pTableWidget->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch);
    headerView->setSectionResizeMode(0, QHeaderView::Custom);
    pTableWidget->setColumnWidth(0, 60);
    pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pTableWidget->setRowCount(1);
    pTableWidget->setItem(0, 4, new QTableWidgetItem("暂无数据"));

    QGroupBox *box = new QGroupBox("产品列表");
    QHBoxLayout *qhBoxLayout = new QHBoxLayout;
    qhBoxLayout->addWidget(pTableWidget);
    box->setLayout(qhBoxLayout);

    return box;
}

// 用户信息
QGroupBox *MainWidget::createInfoLayout() {
    QGroupBox *box = new QGroupBox("用户信息");
    box->setFixedWidth(260);
    QVBoxLayout *vBoxLayout = new QVBoxLayout;

    Name = new QLabel;
    Sex = new QLabel;
    Idcard = new QLabel;
    Tel = new QLabel;
    getUserBtn = new QPushButton("刷新用户信息");

    QFormLayout *formLayoutTop = new QFormLayout;
    formLayoutTop->addRow("姓名", Name);
    formLayoutTop->addRow("性别", Sex);
    formLayoutTop->addRow("身份证号码", Idcard);
    formLayoutTop->addRow("电话", Tel);

    vBoxLayout->addLayout(formLayoutTop);
    vBoxLayout->addWidget(getUserBtn);

    box->setLayout(vBoxLayout);

    // 刷新用户信息
    connect(getUserBtn, &QPushButton::clicked, this, [this] {
        emit getUser();
    });

    return box;
}

// Cookie填写
QGroupBox *MainWidget::createCookieLayout() {
    QGroupBox *box = new QGroupBox("Cookie填写");
    box->setFixedWidth(260);
    QVBoxLayout *vBoxLayout = new QVBoxLayout;

    Cookie = new QLineEdit;
    Signature = new QLineEdit;
    Signature->setReadOnly(true);
    Signature->setStyleSheet("QLineEdit {color: #777;outline:none;}");
    saveBtn = new QPushButton("设置cookie");

    QFormLayout *formLayoutTop = new QFormLayout;
    formLayoutTop->setRowWrapPolicy(QFormLayout::WrapAllRows);
    formLayoutTop->addRow("cookie", Cookie);
    formLayoutTop->addRow("cookie解析到的signature", Signature);

    vBoxLayout->addLayout(formLayoutTop);
    vBoxLayout->addWidget(saveBtn);

    box->setLayout(vBoxLayout);

    // 保存信息
    connect(saveBtn, &QPushButton::clicked, this, [this] {
        emit setCookie(Cookie->text());
    });

    return box;
}

// 预约产品
QGroupBox *MainWidget::createSubLayout() {
    QGroupBox *box = new QGroupBox("预约产品");
    box->setFixedWidth(260);
    QVBoxLayout *vBoxLayout = new QVBoxLayout;

    QFormLayout *formLayout = new QFormLayout;
    formLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

    Sub_Product = new QLabel("暂无");
    Sub_Price = new QLabel("暂无");
    Sub_Num = new QComboBox;
    killDate = new QDateTimeEdit;
    killDate->setDate(QDate::currentDate());
    Sub_Date = new QLabel("暂无");
    Sub_Remark = new QLabel("暂无");
    subDateList = new QTextEdit;
    subDateList->setPlaceholderText("多个则用英文逗号或换行进行分割(日期格式:yyyy-MM-dd。例：2022-01-27,2022-01-28)。填写后将跳过预约日期列表API");

    formLayout->addRow("名称:", Sub_Product);
    formLayout->addRow("价格:", Sub_Price);
    formLayout->addRow("时间:", Sub_Date);
    formLayout->addRow("备注:", Sub_Remark);
    formLayout->addRow("针次:", Sub_Num);
    formLayout->addRow("秒杀时间", killDate);
    formLayout->addRow("预约日期", subDateList);

    QGridLayout *gridLayout = new QGridLayout;
    QPushButton *exitBtn = new QPushButton("退出");
    logBtn = new QPushButton("隐藏日志");
    subBtn = new QPushButton("开始");

    gridLayout->addWidget(subBtn, 0, 0);
    gridLayout->addWidget(logBtn, 0, 1);
    gridLayout->addWidget(exitBtn, 0, 2);

    vBoxLayout->addLayout(formLayout);
    vBoxLayout->addLayout(gridLayout);

    // 显示/隐藏日志
    connect(logBtn, &QPushButton::clicked, this, [this] {
        logOut->setHidden(showLog);
        logBtn->setText(showLog ? "查看日志" : "隐藏日志");
        showLog = !showLog;
    });
    // 启动定时器
    connect(subBtn, &QPushButton::clicked, this, [this] {
        emit enableTask(subProduct.id, killDate->dateTime(), Sub_Num->currentData().value<int>(), subDateList->toPlainText());
    });
    // 退出程序
    connect(exitBtn, &QPushButton::clicked, this, &MainWidget::close);

    box->setLayout(vBoxLayout);

    return box;
}

// 日志
QTextEdit *MainWidget::createLogLayout() {
    logOut = new QTextEdit();
    logOut->setReadOnly(true);
    logOut->setFixedHeight(120);
    logOut->setStyleSheet("QTextEdit {background-color:#031025;}");
    return logOut;
}

/*----------------------- 槽函数 -------------------------*/
// 日志输出
void MainWidget::logger(LogType type, const QString &msg) {
    QString prefix;
    switch (type) {
        case LogType::INFO:
            prefix = "INFO";
            logOut->setTextColor("#02b340");
            break;
        case LogType::WARN:
            prefix = "WARNING";
            logOut->setTextColor("#faed4b");
            break;
        case LogType::ERR:
            prefix = "ERROR";
            logOut->setTextColor("#ff531a");
            break;
    }
    logOut->append(
            QString("[%1] %2 %3 \r\n").arg(QDateTime::currentDateTime().toString("HH:mm:ss zzz")).arg(prefix).arg(
                    msg));
}

// 渲染医院数据
void MainWidget::renderHospital(const QList<Hospital> &list) {
    hospitalList = list;

    // 更新表格内容前，要断开与 cellChanged 信号关联的所有槽，否则会导致程序闪退
    disconnect(hTableWidget, &QTableWidget::doubleClicked, 0, 0);

    if (list.empty()) {
        hTableWidget->setRowCount(1);
        hTableWidget->setItem(0, 3, new QTableWidgetItem("暂无数据"));
    } else {
        hTableWidget->setRowCount(0);
    }
    for (const auto &item: list) {
        hTableWidget->setRowCount(hTableWidget->rowCount() + 1);
        hTableWidget->setItem(hTableWidget->rowCount() - 1, 0,
                              new QTableWidgetItem(QString::number(item.id)));
        QTableWidgetItem *cname = new QTableWidgetItem(item.cname);
        cname->setToolTip(item.cname);
        hTableWidget->setItem(hTableWidget->rowCount() - 1, 1, cname);
        QTableWidgetItem *addr = new QTableWidgetItem(item.addr);
        addr->setToolTip(item.addr);
        hTableWidget->setItem(hTableWidget->rowCount() - 1, 2, addr);
        hTableWidget->setItem(hTableWidget->rowCount() - 1, 3, new QTableWidgetItem(item.tel));
        QTableWidgetItem *notice = new QTableWidgetItem(item.notice);
        notice->setToolTip(item.notice);
        hTableWidget->setItem(hTableWidget->rowCount() - 1, 4, notice);
    }

    // 完成表格更新后，重新关联 cellChanged 相关的槽
    connect(hTableWidget, &QTableWidget::doubleClicked, this, [this] {
        int idx = hTableWidget->currentIndex().row();
        Hospital item = hospitalList[idx];
        if (!(item.id && item.lat && item.lng)) {
            QMessageBox::warning(nullptr, "警告", "参数错误");
            return;
        }

        pTableWidget->clearContents();
        pTableWidget->setRowCount(1);
        pTableWidget->setItem(0, 4, new QTableWidgetItem("加载中..."));

        emit fetchProduct(item.id, item.lat, item.lng);
    });

}

// 渲染产品数据
void MainWidget::renderProduct(const QList<Product> &list) {
    productList = list;

    // 更新表格内容前，要断开与 cellChanged 信号关联的所有槽，否则会导致程序闪退
    disconnect(pTableWidget, &QTableWidget::doubleClicked, nullptr, nullptr);

    if (productList.empty()) {
        pTableWidget->setRowCount(1);
        pTableWidget->setItem(0, 4, new QTableWidgetItem("暂无数据"));
    } else {
        pTableWidget->setRowCount(0);
    }

    for (const auto &item: productList) {
        pTableWidget->setRowCount(pTableWidget->rowCount() + 1);
        pTableWidget->setItem(pTableWidget->rowCount() - 1, 0,
                              new QTableWidgetItem(QString::number(item.id)));
        QTableWidgetItem *text = new QTableWidgetItem(item.text);
        text->setToolTip(item.text);
        pTableWidget->setItem(pTableWidget->rowCount() - 1, 1, text);
        pTableWidget->setItem(pTableWidget->rowCount() - 1, 2,
                              new QTableWidgetItem(
                                      QString("%1元 / %2针").arg(item.price).arg(item.NumbersVaccine.length())));
        QTableWidgetItem *warn = new QTableWidgetItem(item.warn);
        warn->setToolTip(item.warn);
        pTableWidget->setItem(pTableWidget->rowCount() - 1, 3, warn);
        QTableWidgetItem *tags = new QTableWidgetItem(item.tags.join("、"));
        tags->setToolTip(item.tags.join("、"));
        pTableWidget->setItem(pTableWidget->rowCount() - 1, 4, tags);
        pTableWidget->setItem(pTableWidget->rowCount() - 1, 5,
                              new QTableWidgetItem(item.date));
        pTableWidget->setItem(pTableWidget->rowCount() - 1, 6,
                              new QTableWidgetItem(item.enable ? "可预约" : "暂未开启"));
        QTableWidgetItem *remarks = new QTableWidgetItem(item.remarks);
        remarks->setToolTip(item.remarks);
        pTableWidget->setItem(pTableWidget->rowCount() - 1, 7, remarks);
    }

    // 完成表格更新后，重新关联 cellChanged 相关的槽
    connect(pTableWidget, &QTableWidget::doubleClicked, this, [this] {
        int idx = pTableWidget->currentIndex().row();
        subProduct = productList[idx];

        // 显示选中数据
        Sub_Product->setText(subProduct.text);
        Sub_Price->setText(QString("%1元/针次").arg(subProduct.price));
        Sub_Date->setText(subProduct.date);
        Sub_Remark->setText(subProduct.remarks);
        Sub_Num->clear();
        QList<NumbersVaccine> list = subProduct.NumbersVaccine;
        for (const auto &item: list) {
            Sub_Num->addItem(item.cname, item.value);
        }

        // 自动截取秒杀时间。仅当秒杀时间未过才有效
        bool isSet = false;
        if (subProduct.date.length() > 11) {
            QString dateTime = QString("%1-%2").arg(QDate::currentDate().year()).arg(subProduct.date.mid(0, 11));
            QDateTime qDate = QDateTime::fromString(dateTime, "yyyy-MM-dd HH:mm");
            if (qDate > QDateTime::currentDateTime()) {
                isSet = true;
                killDate->setDateTime(qDate);
            }
        }
        // 重置未当天凌晨
        if (!isSet) {
            QString str = QString("%1 00:00").arg(QDate::currentDate().toString("yyyy-MM-dd"));
            killDate->setDateTime(QDateTime::fromString(str, "yyyy-MM-dd HH:mm"));
        }
    });
}

/**
 * 渲染分类数据
 * @param list 分类列表
 * @param isOne isOne - 为true则为一级分类，否则为二级分类
 */
void MainWidget::renderCate(const QList<Cate> &list, bool isOne) {
    QComboBox *cate = isOne ? oneCate : twoCate;
    cate->clear();
    if (list.empty()) {
        cate->addItem("暂无数据", 0);
    }
    if (isOne) {
        cate->addItem("全部", 0);
    }

    for (const auto &item: list) {
        cate->addItem((isOne ? item.cname : item.alias), item.id);
    }
}

// 渲染Storage数据
void MainWidget::renderStorage(const QString &cityName, const QString &cookie, const QString &signature) {
    Signature->setText(signature);
    Cookie->setText(cookie);

    chooseArea->setText(cityName.isEmpty() ? "未选择地区" : cityName);
}

// 渲染用户信息
void MainWidget::renderUser(const QString &name, int sex, const QString &idcard, const QString &tel) {
    Name->setText(name);
    Sex->setText(sex == 1 ? "男" : "女");
    Idcard->setText(idcard);
    Tel->setText(tel);
}

// 启用/禁用控件
void MainWidget::widgetDisable(bool enable) {
    subBtn->setText(enable ? "停止" : "开始");
    oneCate->setDisabled(enable);
    twoCate->setDisabled(enable);
    Sub_Num->setDisabled(enable);
    killDate->setDisabled(enable);
    chooseArea->setDisabled(enable);
    cateConfirm->setDisabled(enable);
    hTableWidget->setDisabled(enable);
    pTableWidget->setDisabled(enable);
    subDateList->setDisabled(enable);
}
