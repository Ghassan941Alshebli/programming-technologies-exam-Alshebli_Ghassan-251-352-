#include "mytcpserver.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QByteArray>

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    // المنفذ 65432 كما هو مطلوب في المهمة الأولى
    if (!mTcpServer->listen(QHostAddress::Any, 65432))
    {
        qDebug() << "Server is not started";
    }
    else
    {
        qDebug() << "Server is started on port 65432";
        // توليد رقم سري عشوائي بين 1 و 100
        secretNumber = QRandomGenerator::global()->bounded(1, 101);
    }
}

void MyTcpServer::slotNewConnection()
{
    QTcpSocket *clientSocket = mTcpServer->nextPendingConnection();

    // التحقق من الحد الأقصى (5 عملاء)
    if (mTcpSockets.size() >= 5)
    {
        clientSocket->write("The server is busy. Please connect later.\r\n");
        clientSocket->disconnectFromHost();
        return;
    }

    mTcpSockets.push_back(clientSocket);
    connect(clientSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

    // رسالة الترحيب
    clientSocket->write("Hello! Welcome to the game 'Guess the Number'.\r\n");

    // إرسال عدد العملاء الحالي لجميع المتصلين
    QString info = QString("Current number of connected clients: %1\r\n").arg(mTcpSockets.size());
    for (QTcpSocket *socket : mTcpSockets)
    {
        socket->write(info.toUtf8());
    }
}

void MyTcpServer::slotServerRead()
{
    QTcpSocket *clientSocket = (QTcpSocket *)sender();
    QByteArray data = clientSocket->readAll();

    // تحويل مدخلات العميل إلى رقم
    int guess = data.trimmed().toInt();

    if (guess == 0)
        return; // تجاهل الإدخالات الفارغة

    if (guess < secretNumber)
    {
        clientSocket->write("More than \r\n"); // الرقم السري أكبر
    }
    else if (guess > secretNumber)
    {
        clientSocket->write("Less than \r\n"); // الرقم السري أصغر
    }
    else
    {
        // تم تخمين الرقم بشكل صحيح
        for (QTcpSocket *socket : mTcpSockets)
        {
            socket->write(" Guessed right \r\n");
            socket->disconnectFromHost(); // قطع الاتصال مع الجميع
        }
        mTcpSockets.clear();
        // بدء لعبة جديدة وتوليد رقم سري جديد
        secretNumber = QRandomGenerator::global()->bounded(1, 101);
    }
}

void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket *clientSocket = (QTcpSocket *)sender();
    mTcpSockets.removeOne(clientSocket);
    clientSocket->deleteLater();
}