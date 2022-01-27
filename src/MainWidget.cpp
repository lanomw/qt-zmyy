//
// Created by arc on 2022/1/4.
//

#include "MainWidget.h"

using namespace std;

/**
 * 主界面
 */
MainWidget::MainWidget() {
    setWindowTitle("知苗易约");
    resize(1200, 700);

    // cookie设置
    QFile file(storage_file);
    file.open(QFile::ReadWrite);
    QString str = file.readAll();
    if (!str.isEmpty()) {
        xpack::json::decode(str.toStdString(), storage);
        Http::setCookie(storage.cookie);
    }
    file.close();

    // 左边
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(createAreaLayout());
    leftLayout->addLayout(createCateLayout());
    leftLayout->addWidget(createHospitalLayout(), 1);
    leftLayout->addWidget(createProductLayout(), 1);
    leftLayout->addWidget(createLogLayout());

    // 右边
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(createInfoLayout(), 2);
    rightLayout->addWidget(createSubLayout(), 1);

    // 最外层容器
    QHBoxLayout *container = new QHBoxLayout(this);
    container->addLayout(leftLayout, 1);
    container->addLayout(rightLayout);
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

    connect(chooseArea, &QPushButton::clicked, this, &MainWidget::openAreaBox);

    return areaLayout;
}

// 分类
QHBoxLayout *MainWidget::createCateLayout() {
    QHBoxLayout *cateLayout = new QHBoxLayout;
    oneCate = new QComboBox;
    oneCate->setMinimumWidth(200);
    twoCate = new QComboBox;
    twoCate->setMinimumWidth(200);
    twoCate->addItem("暂无数据", 0);
    cateConfirm = new QPushButton("查询医院");

    cateLayout->addWidget(new QLabel("一级分类："));
    cateLayout->addWidget(oneCate);
    cateLayout->addWidget(new QLabel("二级分类："));
    cateLayout->addWidget(twoCate);
    cateLayout->addWidget(cateConfirm);
    cateLayout->addStretch();

    connect(oneCate, SIGNAL(currentIndexChanged(int)), this, SLOT(cateChange()));
    connect(cateConfirm, &QPushButton::clicked, this, &MainWidget::fetchHospital);

    // 数据填充 - 一级分类
    oneCate->addItem("加载中...", 0);
    Http(BASE_URL)
            .param("act", "GetCat1")
            .success([=](const QString &response, int code) {
                try {
                    ResponseData<Cate> res;
                    xpack::json::decode(response.toStdString(), res);
                    QList<Cate> cateList = res.list;
                    oneCate->clear();
                    oneCate->addItem("全部", 0);
                    for (const auto &item: cateList) {
                        oneCate->addItem(item.cname, item.id);
                    }
                } catch (const exception &e) {
                    logger(LogType::ERR, QString("【异常-一级分类】 %1").arg(e.what()));
                }
            })
            .fail([=](const QString &response, int code) {
                logger(LogType::ERR, QString("【一级分类】 %1  %2").arg(code).arg(response));
            })
            .get();

    return cateLayout;
}

// 医院
QGroupBox *MainWidget::createHospitalLayout() {
    hTableWidget = new QTableWidget;
    hTableWidget->setShowGrid(false);
    hTableWidget->setColumnCount(4);
    hTableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "医院名称" << "地址" << "联系电话");
    QHeaderView *headerView = hTableWidget->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch);
    headerView->setSectionResizeMode(0, QHeaderView::Custom);
    headerView->setSectionResizeMode(3, QHeaderView::Custom);
    hTableWidget->setColumnWidth(0, 60);
    hTableWidget->setColumnWidth(3, 120);
    hTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    hTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    hTableWidget->setRowCount(1);
    hTableWidget->setItem(0, 2, new QTableWidgetItem("暂无数据"));

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
            QStringList() << "ID" << "名称" << "价格" << "警告" << "标签" << "次数" << "状态" << "备注");
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

// 资料填写
QGroupBox *MainWidget::createInfoLayout() {

    QGroupBox *box = new QGroupBox("资料");
    box->setFixedWidth(260);
    QVBoxLayout *vBoxLayout = new QVBoxLayout;

    QFormLayout *formLayout = new QFormLayout;
    formLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    Name = new QLineEdit;
    Idcard = new QLineEdit;
    Tel = new QLineEdit;
    Tel->setMaxLength(11);
    Signature = new QLineEdit;
    Cookie = new QLineEdit;

    // 单选按钮组
    Sex = new QButtonGroup(this);
    Boy = new QRadioButton("男");
    Girl = new QRadioButton("女");
    Sex->addButton(Boy, 1);
    Sex->addButton(Girl, 2);
    QHBoxLayout *sexLayout = new QHBoxLayout;
    sexLayout->setAlignment(Qt::AlignLeft);
    sexLayout->addWidget(Boy);
    sexLayout->addWidget(Girl);

    formLayout->addRow("姓名", Name);
    formLayout->addRow("性别", sexLayout);
    formLayout->addRow("身份证号码", Idcard);
    formLayout->addRow("电话", Tel);
    formLayout->setSpacing(10);
    formLayout->addRow("signature", Signature);
    formLayout->addRow("cookie", Cookie);

    saveBtn = new QPushButton("保存");

    vBoxLayout->addLayout(formLayout);
    vBoxLayout->addWidget(saveBtn);

    box->setLayout(vBoxLayout);

    // 按钮事件
    connect(saveBtn, &QPushButton::clicked, this, &MainWidget::save);

    refreshStorage();

    return box;
}

// 预约产品
QGroupBox *MainWidget::createSubLayout() {
    QGroupBox *box = new QGroupBox("预约产品");
    box->setFixedWidth(260);
    QVBoxLayout *vBoxLayout = new QVBoxLayout;

    QFormLayout *formLayout = new QFormLayout;
    formLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

    Sub_Product = new QLabel("---");
    Sub_Price = new QLabel("---");
    Sub_Num = new QComboBox;
    killDate = new QDateTimeEdit;
    killDate->setDate(QDate::currentDate());
    Sub_Date = new QLabel("---");
    Sub_Remark = new QLabel("---");

    formLayout->addRow("名称:", Sub_Product);
    formLayout->addRow("价格:", Sub_Price);
    formLayout->addRow("时间:", Sub_Date);
    formLayout->addRow("备注:", Sub_Remark);
    formLayout->addRow("针次:", Sub_Num);
    formLayout->addRow("秒杀时间", killDate);

    QGridLayout *gridLayout = new QGridLayout;
    QPushButton *exitBtn = new QPushButton("退出");
    logBtn = new QPushButton(showLog ? "隐藏日志" : "查看日志");
    subBtn = new QPushButton(isStart ? "停止" : "开始");

    gridLayout->addWidget(subBtn, 0, 0);
    gridLayout->addWidget(logBtn, 0, 1);
    gridLayout->addWidget(exitBtn, 0, 2);

    vBoxLayout->addLayout(formLayout);
    vBoxLayout->addLayout(gridLayout);

    // 按钮事件
    connect(logBtn, &QPushButton::clicked, this, &MainWidget::toggleLog);
    connect(subBtn, &QPushButton::clicked, this, &MainWidget::toggleTimer);
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

/*----------------------- 交互 -------------------------*/
// 刷新信息
void MainWidget::refreshStorage() {

    QFile file(storage_file);
    file.open(QFile::ReadWrite);
    QString str = file.readAll();
    if (!str.isEmpty()) {
        xpack::json::decode(str.toStdString(), storage);
        Name->setText(storage.cname);
        if (storage.sex == 1) {
            Boy->setChecked(true);
        } else {
            Girl->setChecked(true);
        }
        Idcard->setText(storage.idcard);
        Tel->setText(storage.tel);
        Signature->setText(storage.signature);
        Cookie->setText(storage.cookie);

        chooseArea->setText(storage.cityName.isEmpty() ? "未选择地区" : storage.cityName);
        if (!storage.cityName.isEmpty()) {
            fetchHospital();
        }
    } else {
        // 文件不存在则初始化
        storage.doctype = 1;
        storage.sex = 0;
        storage.lat = 0;
        storage.lng = 0;
        file.write(xpack::json::encode(storage).c_str());
    }

    file.close();
}

// 刷新分类 - 二级
void MainWidget::refreshCate(int pid = 0) {
    Http(BASE_URL)
            .param("act", "GetCat2")
            .param("id", pid)
            .success([=](const QString &response, int code) {
                try {
                    ResponseData<Cate> res;
                    xpack::json::decode(response.toStdString(), res);
                    QList<Cate> cateList = res.list;
                    twoCate->clear();
                    if (cateList.empty()) {
                        twoCate->addItem("暂无数据", 0);
                    }
                    for (const auto &item: cateList) {
                        twoCate->addItem(item.alias, item.id);
                    }
                } catch (const exception &e) {
                    logger(LogType::ERR, QString("【异常-二级分类】 %1").arg(e.what()));
                }
            })
            .fail([=](const QString &response, int code) {
                logger(LogType::ERR, QString("【二级分类】 %1  %2").arg(code).arg(response));
            })
            .get();
}

// 切换分类 - 二级
void MainWidget::cateChange() {
    int id = oneCate->currentData().value<int>();
    if (id > 0) {
        twoCate->clear();
        twoCate->addItem("加载中...", 0);
        refreshCate(id);
    }
}

// 保存
void MainWidget::save() {
    QFile file(storage_file);
    file.open(QFile::WriteOnly);

    storage.cname = Name->text();
    storage.sex = Sex->checkedButton()->text() == "男" ? 1 : 2;
    // 证件类型固定为身份证
    storage.doctype = 1;
    storage.idcard = Idcard->text();
    QString year = Idcard->text().mid(6, 4);
    QString month = Idcard->text().mid(10, 2);
    QString day = Idcard->text().mid(12, 2);
    storage.birthday = QString("%1-%2-%3").arg(year).arg(month).arg(day);
    storage.tel = Tel->text();
    storage.signature = Signature->text();
    storage.cookie = Cookie->text();
    Http::setCookie(Cookie->text());

    file.write(xpack::json::encode(storage).c_str());
    file.close();
}

// 选择区域
void MainWidget::openAreaBox() {
    areaBox = new AreaBox;

    // 信号监听
    connect(areaBox, &AreaBox::confirm, this, &MainWidget::showCity);

    areaBox->exec();
}

// 显示/隐藏日志
void MainWidget::toggleLog() {
    logOut->setHidden(showLog);
    logBtn->setText(showLog ? "查看日志" : "隐藏日志");
    showLog = !showLog;
}

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
            QString("[%1] %2 %3 \r\n\r\n").arg(QDateTime::currentDateTime().toString("HH:mm:ss zzz")).arg(prefix).arg(msg));
}

// 获取医院列表
void MainWidget::fetchHospital() {
    if (!(storage.lat && storage.lng)) {
        QMessageBox::warning(this, "警告", "请先选择城市区域");
        return;
    }

    hTableWidget->clearContents();
    hTableWidget->setRowCount(1);
    hTableWidget->setItem(0, 2, new QTableWidgetItem("加载中..."));

    // 这里固定传经纬度，city固定值(主要是不知道数组怎么放入url中，还有url编码)
    QMap<QString, QVariant> params;
    params["act"] = "CustomerList";
    params["city"] = "%5B%22%22%2C%22%22%2C%22%22%5D";
    params["lat"] = storage.lat;
    params["lng"] = storage.lng;
    params["product"] = twoCate->currentData().value<int>();
    params["id"] = 0;

    Http(BASE_URL)
            .params(params)
            .success([=](const QString &response, int code) {
                try {
                    ResponseData<Hospital> res;
                    xpack::json::decode(response.toStdString(), res);
                    hospitalList = res.list;

                    // 更新表格内容前，要断开与 cellChanged 信号关联的所有槽，否则会导致程序闪退
                    disconnect(hTableWidget, &QTableWidget::doubleClicked, 0, 0);

                    if (hospitalList.empty()) {
                        hTableWidget->setRowCount(1);
                        hTableWidget->setItem(0, 2, new QTableWidgetItem("暂无数据"));
                    } else {
                        hTableWidget->setRowCount(0);
                    }
                    for (const auto &item: hospitalList) {
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
                    }

                    // 完成表格更新后，重新关联 cellChanged 相关的槽
                    connect(hTableWidget, &QTableWidget::doubleClicked, this, &MainWidget::fetchProduct);
                } catch (const exception &e) {
                    logger(LogType::ERR, QString("【异常-医院列表】 %1").arg(e.what()));
                }
            })
            .fail([=](const QString &response, int code) {
                logger(LogType::ERR, QString("【医院列表】 %1  %2").arg(code).arg(response));
            })
            .get();
}

// 获取产品列表
void MainWidget::fetchProduct() {
    int idx = hTableWidget->currentIndex().row();
    Hospital item = hospitalList[idx];
    if (!(item.id && item.lat && item.lng)) {
        QMessageBox::warning(nullptr, "警告", "参数错误");
        return;
    }

    id = item.id;
    lat = item.lat;
    lng = item.lng;
    pTableWidget->clearContents();
    pTableWidget->setRowCount(1);
    pTableWidget->setItem(0, 4, new QTableWidgetItem("加载中..."));

    QMap<QString, QVariant> params;
    params["act"] = "CustomerProduct";
    params["id"] = item.id;
    params["lat"] = item.lat;
    params["lng"] = item.lng;

    Http(BASE_URL)
            .params(params)
            .success([=](const QString &response, int code) {
                try {
                    HospitalDetail res;
                    xpack::json::decode(response.toStdString(), res);
                    productList = res.list;

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
                                              new QTableWidgetItem(item.price + "元/针次"));
                        QTableWidgetItem *warn = new QTableWidgetItem(item.warn);
                        warn->setToolTip(item.warn);
                        pTableWidget->setItem(pTableWidget->rowCount() - 1, 3, warn);
                        QTableWidgetItem *tags = new QTableWidgetItem(item.tags.join("、"));
                        tags->setToolTip(item.tags.join("、"));
                        pTableWidget->setItem(pTableWidget->rowCount() - 1, 4, tags);
                        pTableWidget->setItem(pTableWidget->rowCount() - 1, 5,
                                              new QTableWidgetItem(QString::number(item.NumbersVaccine.size())));
                        pTableWidget->setItem(pTableWidget->rowCount() - 1, 6,
                                              new QTableWidgetItem(item.enable ? "已开启" : "未开启"));
                        QTableWidgetItem *remarks = new QTableWidgetItem(item.remarks);
                        remarks->setToolTip(item.remarks);
                        pTableWidget->setItem(pTableWidget->rowCount() - 1, 7, remarks);
                    }

                    // 完成表格更新后，重新关联 cellChanged 相关的槽
                    connect(pTableWidget, &QTableWidget::doubleClicked, this, &MainWidget::chooseProduct);
                } catch (const exception &e) {
                    logger(LogType::ERR, QString("【异常-产品列表】 %1").arg(e.what()));
                }
            })
            .fail([=](const QString &response, int code) {
                logger(LogType::ERR, QString("【产品列表】 %1  %2").arg(code).arg(response));
            })
            .get();
}

// 开启/关闭定时器
void MainWidget::toggleTimer() {
    if (!isStart && !pid) {
        QMessageBox::warning(nullptr, "警告", "未选择产品");
        return;
    }

    int time = 0;
    if (!isStart) {
        // 毫秒置为0
        QDateTime curDateTime = QDateTime::currentDateTime();
        QDateTime dateTime = killDate->dateTime();

        time = curDateTime.secsTo(dateTime);
        if (time <= 0) {
            QMessageBox::warning(nullptr, "警告", "无效时间");
            logger(LogType::INFO, "无效时间");
            return;
        }
    }

    widgetDisable(!isStart);
    timer = new QTimer();
    timer->setSingleShot(true);

    if (isStart) {
        logger(LogType::INFO, QString("%1秒后开始秒杀").arg(time));
        connect(timer, &QTimer::timeout, this, &MainWidget::secKill);
        timer->start(time * 1000);
    } else {
        logger(LogType::INFO, "秒杀已关闭");
        disconnect(timer, &QTimer::timeout, this, nullptr);
        timer->stop();
    }
}

// 显示选中区域
void MainWidget::showCity(double lat, double lng, const QString &area) {
    if (!area.isEmpty()) {
        storage.cityName = area;
        storage.lat = lat;
        storage.lng = lng;
        chooseArea->setText(storage.cityName.isEmpty() ? "未选择地区" : storage.cityName);

        QFile file(storage_file);
        file.open(QFile::ReadWrite);
        file.write(xpack::json::encode(storage).c_str());
        file.close();
    }
}

// 启用/禁用
void MainWidget::widgetDisable(bool enable) {
    isStart = enable;
    subBtn->setText(enable ? "停止" : "开始");
    Sub_Num->setDisabled(enable);
    killDate->setDisabled(enable);
    saveBtn->setDisabled(enable);
    chooseArea->setDisabled(enable);
    cateConfirm->setDisabled(enable);
    hTableWidget->setDisabled(enable);
    pTableWidget->setDisabled(enable);
}

// 选择产品
void MainWidget::chooseProduct() {
    int idx = pTableWidget->currentIndex().row();
    subProduct = productList[idx];
    pid = subProduct.id;

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
}

// 秒杀
void MainWidget::secKill() {
    logger(LogType::INFO, "【----- 秒杀开始 -----】");
    widgetDisable(false);

    QMap<QString, QVariant> params;
    params["act"] = "GetCustSubscribeDateAll";
    params["id"] = id;
    params["pid"] = pid;
    params["month"] = QDate::currentDate().toString("yyyyMM");

    // 获取可预约日期列表
    Http(BASE_URL)
            .params(params)
            .success([=](const QString &response, int code) {
                try {
                    if (response.contains("list")) {
                        ResponseData<SubDate> res;
                        xpack::json::decode(response.toStdString(), res);
                        subDateList = res.list;
                        subDateIndex = 0;

                        if (!res.list.empty()) {
                            getProductDetail();
                        } else {
                            logger(LogType::ERR, "【时间列表为空】");
                        }
                    } else {
                        logger(LogType::ERR, "【时间列表不存在】");
                    }
                } catch (const exception &e) {
                    logger(LogType::ERR, QString("【异常-时间列表】 %1").arg(e.what()));
                }
            })
            .fail([=](const QString &response, int code) {
                logger(LogType::ERR, QString("【时间列表】 %1  %2").arg(code).arg(response));
            })
            .get();
}

// 产品详情
void MainWidget::getProductDetail() {
    if (storage.signature.isEmpty()) {
        QMessageBox::warning(nullptr, "警告", "缺少 signature");
        return;
    }
    if (storage.cookie.isEmpty()) {
        QMessageBox::warning(nullptr, "警告", "缺少 cookie");
        return;
    }

    // 当前日期是否可预约
    while (subDateIndex < subDateList.length() && !subDateList[subDateIndex].enable) {
        subDateIndex++;
    }

    // 退出循环
    if (subDateIndex >= subDateList.length()) {
        logger(LogType::INFO, QString("【产品详情-结束】 %1"));
        return;
    }

    QMap<QString, QVariant> params;
    params["act"] = "GetCustSubscribeDateDetail";
    params["id"] = id;
    params["pid"] = pid;

    QString scdate = subDateList[subDateIndex++].date; // 下次循环索引+1
    params["scdate"] = scdate;
    Http(BASE_URL)
            .params(params)
            .success([=](const QString &response, int code) {
                try {
                    // 302则表示当天的疫苗已经没了
                    if (code == 302) {
                        logger(LogType::ERR, QString("【预约时间详情】 当前没有可预约产品，查询下一条数据"));
                        getProductDetail();
                        return;
                    }

                    // 数据解密
                    QString key = storage.signature.mid(0, 16);
                    string rel = CryptoUtil::decrypt(response.toStdString(), key.toLocal8Bit());
                    logger(LogType::INFO, QString("【产品详情】 %1").arg(QString::fromStdString(rel)));

                    ResponseData<DateDetail> res;
                    xpack::json::decode(rel, res);

                    if (res.list.empty()) {
                        logger(LogType::INFO, QString("【产品详情】 %1 预约已满").arg(scdate));
                        getProductDetail();
                        return;
                    }

                    // 提交订单
                    postOrder(res.list[0].mxid, scdate);
                } catch (const exception &e) {
                    logger(LogType::ERR, QString("【异常-产品详情】 %1").arg(e.what()));
                }
            })
            .fail([=](const QString &response, int code) {
                logger(LogType::ERR, QString("【预约时间详情】 %1  %2").arg(code).arg(response));
            })
            .get();
}

// 提交订单
void MainWidget::postOrder(QString &mxid, const QString& date) {
    OrderPost data;
    data.birthday = storage.birthday;
    data.tel = storage.tel;
    data.sex = storage.sex;
    data.cname = storage.cname;
    data.doctype = storage.doctype;
    data.idcard = storage.idcard;
    data.mxid = mxid;
    data.date = date;
    data.pid = QString("%1").arg(pid);
    data.Ftime = Sub_Num->currentData().value<int>();

    // 加密
    string str = xpack::json::encode(data);
    QString key = storage.signature.mid(0, 16);
    QString rel = QString::fromStdString(CryptoUtil::encrypt(str, key.toLocal8Bit()));
    Http(POST_ORDER_URL)
            .json(rel)
            .success([=](const QString &response, int code) {
                try {
                    Response res;
                    xpack::json::decode(response.toStdString(), res);
                    if (res.status == 200) {
                        logger(LogType::INFO, QString("【提交订单 成功】 %1").arg(res.msg));
                    } else {
                        getProductDetail();
                        logger(LogType::INFO, QString("【提交订单 失败】 %1").arg(res.msg));
                    }
                } catch (const exception &e) {
                    logger(LogType::ERR, QString("【异常-提交订单】 %1").arg(e.what()));
                }
            })
            .fail([=](const QString &response, int code) {
                logger(LogType::ERR, QString("【提交订单】 %1  %2").arg(code).arg(response));
            })
            .post();
}