#include "QtWidgetsApplication1.h"
#include "ui_QtWidgetsApplication1.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.label->setText("�����ţ�");
    ui.label_2->setText("�˿ںţ�");
    ui.label_3->setText("δ���ļ�");
    ui.openButton->setText("���ļ�");
    ui.sendButton->setText("�����ļ�");
}

void QtWidgetsApplication1::openFile()
{

}

void QtWidgetsApplication1::send()
{

}

