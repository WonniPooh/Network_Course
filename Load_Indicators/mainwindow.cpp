#include "mainwindow.h"
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    column = 1;
    row = 1;
    request_sender = new RequestSender;
    layout = new QGridLayout;
    assets_file.setFileName(QString("assets.txt"));

    QScrollArea* scroll = new QScrollArea;
    QVBoxLayout* vbox =  new QVBoxLayout;
    QWidget* scroll_widget = new QWidget;
    QLabel* title = new QLabel(QString("<H1 align = \"center\"> Assets Indicators </H1>"));

    parseAssets();
    scroll_widget->setLayout(layout);
    scroll->setWidget(scroll_widget);
    vbox->addWidget(title);
    vbox->addWidget(scroll);
    this->setLayout(vbox);
    QObject::connect(this, &MainWindow::startActions, request_sender, &RequestSender::sendRequests);
    this->show();
//    emit startActions();
}

MainWindow::~MainWindow()
{
    assets_file.open(QIODevice::WriteOnly);

    QList<int> list = assets.values();
    qSort(list.begin(), list.end());

    for(int i = 0; i < assets.size(); i++)
    {
        assets_file.write((QString::number(list[i]) + QString(",") +  assets.key(list[i])->text() + QString (", ") + QString::number((int)assets.key(list[i])->isChecked())+ QString ("\n")).toLatin1());
    }

    assets_file.close();
}

void MainWindow::parseAssets()
{
    assets_file.open(QIODevice::ReadWrite);
    char buf[128] = {};

    while(!assets_file.atEnd())
    {
        assets_file.readLine(buf, qint64(128));

        QString data(buf);
        QStringList list = data.split(QString(','), QString::SkipEmptyParts);

        QCheckBox* box = new QCheckBox(list[1]);
        box->setMinimumWidth(200);

        if(list[2].toInt())
        {
            box->setChecked(true);
            request_sender->addQuote(list[0].toInt());
        }

        layout->addWidget(box, row, column);
        assets.insert(box, list[0].toInt());
        column+=3;

        QObject::connect(box, &QCheckBox::clicked, this, &MainWindow::slotChecked);

        if(column == 10)
        {
            QLabel* row_num = new QLabel(QString::number(row));
            layout->addWidget(row_num, row, 0);

            column = 1;
            row++;
        }

    }
    assets_file.close();
}

void MainWindow::slotChecked()
{
    QCheckBox* box_checked = (QCheckBox*)sender();

    if(box_checked->isChecked())
        request_sender->addQuote(assets.value(box_checked));
    else
        request_sender->removeQuote(assets.value(box_checked));
}
