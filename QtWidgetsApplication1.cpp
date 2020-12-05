#include "QtWidgetsApplication1.h"
#include "ui_QtWidgetsApplication1.h"
#include <QtNetwork>
#include <qfiledialog.h>
#include <qdebug.h>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowTitle(QString::fromUtf8("�����������裬���Ͷ�"));
    ui.label->setText("�����ţ�");
    ui.label_2->setText("�˿ںţ�");
    ui.label_3->setText("δ���ļ�");
    ui.openButton->setText("���ļ�");
    ui.sendButton->setText("�����ļ�");
    tcpClient = new QTcpSocket(this);
    connect(ui.openButton, SIGNAL(clicked(bool)), this, SLOT(openFile()));
    connect(tcpClient, SIGNAL(connected()), this, SLOT(startTransfer()));
    connect(tcpClient, SIGNAL(bytesWritten(qint64)), this, SLOT(updateClientProgress(qint64)));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(ui.sendButton, &QPushButton::clicked, this, &QtWidgetsApplication1::send);
}

void QtWidgetsApplication1::openFile()
{
    ui.progressBar->reset();
    fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        ui.sendButton->setEnabled(true);
        ui.label_3->setText(QString("���ļ� %1 �ɹ�!").arg(fileName));
    }
}

void QtWidgetsApplication1::send()
{
    ui.sendButton->setEnabled(false);
    bytesWritten = 0;
    ui.label_3->setText("������...");
    tcpClient->connectToHost(ui.hostLineEdit->text(), ui.portLineEdit->text().toInt());
}

void QtWidgetsApplication1::startTransfer()
{
    localFile = new QFile(fileName);
    if (!localFile->open(QFile::ReadOnly)) {
        qDebug() << "client��open file error!";
        return;
    }
    totalBytes = localFile->size();
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_7);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
    qDebug() << currentFileName;
    sendOut << qint64(0) << qint64(0) << currentFileName;
    totalBytes += outBlock.size();
    sendOut.device()->seek(0);
    sendOut << totalBytes << qint64(outBlock.size() - sizeof(qint64) * 2);
    bytesToWrite = totalBytes - tcpClient->write(outBlock);
    ui.label_3->setText("������");
    outBlock.resize(0);
}

void QtWidgetsApplication1::updateClientProgress(qint64 x)
{
    bytesWritten += (int)x;
    if (bytesToWrite > 0) {
        outBlock = localFile->read(qMin(bytesToWrite, payloadSize));
        bytesToWrite -= (int)tcpClient->write(outBlock);
        outBlock.resize(0);
    }
    else {
        localFile->close();
    }
    ui.progressBar->setMaximum(totalBytes);
    ui.progressBar->setValue(bytesWritten);

    if (bytesWritten == totalBytes) {
        ui.label_3->setText(QString("�����ļ� %1 �ɹ�").arg(fileName));
        localFile->close();
        tcpClient->close();
    }

}

void QtWidgetsApplication1::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpClient->errorString();
    tcpClient->close();
    ui.progressBar->reset();
    ui.sendButton->setEnabled(true);

}
