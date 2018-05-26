#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QCheckBox>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QMap>
#include <QVector>
#include <QFile>
#include "requestsender.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void parseAssets();

signals:
    void startActions();

public slots:
    void slotChecked();

private:
    RequestSender* request_sender;
    QGridLayout* layout;
    QFile assets_file;
    int row;
    int column;
    QMap<QCheckBox*, int> assets;
};

#endif // MAINWINDOW_H
