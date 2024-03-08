/*
*    Copyright (c) 2024 Tim Hong All Rights Reserved.
*
*           File : main.cpp
*         Author : Tim Hong(hotize@163.com)
*           Date : 2024-01-01 00:00
*        Version : OpenCASCADE7.3.0 & Qt5.12.12
*
*    Description : OpenCASCADE with Qt demo.
*/


#include "occQt.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    occQt w;
    w.show();

    return a.exec();
}
