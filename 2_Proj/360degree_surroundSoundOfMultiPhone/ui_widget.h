/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPushButton *pushButtonChoose1stereo;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QPushButton *pushButtonSplit2mono;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QPushButton *pushButtonTransmit;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QLabel *label_5;
    QSplitter *splitter;
    QTextBrowser *textBrowserError;
    QTextBrowser *textBrowserOutPut;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1453, 849);
        verticalLayout_4 = new QVBoxLayout(Widget);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        pushButtonChoose1stereo = new QPushButton(Widget);
        pushButtonChoose1stereo->setObjectName(QString::fromUtf8("pushButtonChoose1stereo"));
        pushButtonChoose1stereo->setMinimumSize(QSize(120, 120));

        verticalLayout->addWidget(pushButtonChoose1stereo);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetFixedSize);
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        pushButtonSplit2mono = new QPushButton(Widget);
        pushButtonSplit2mono->setObjectName(QString::fromUtf8("pushButtonSplit2mono"));
        pushButtonSplit2mono->setMinimumSize(QSize(120, 120));

        verticalLayout_2->addWidget(pushButtonSplit2mono);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetFixedSize);
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_3->addWidget(label_3);

        pushButtonTransmit = new QPushButton(Widget);
        pushButtonTransmit->setObjectName(QString::fromUtf8("pushButtonTransmit"));
        pushButtonTransmit->setMinimumSize(QSize(120, 120));

        verticalLayout_3->addWidget(pushButtonTransmit);


        horizontalLayout->addLayout(verticalLayout_3);


        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout_4->addLayout(horizontalLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        label_5 = new QLabel(Widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 0, 1, 1, 1);

        splitter = new QSplitter(Widget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        textBrowserError = new QTextBrowser(splitter);
        textBrowserError->setObjectName(QString::fromUtf8("textBrowserError"));
        splitter->addWidget(textBrowserError);
        textBrowserOutPut = new QTextBrowser(splitter);
        textBrowserOutPut->setObjectName(QString::fromUtf8("textBrowserOutPut"));
        splitter->addWidget(textBrowserOutPut);

        gridLayout->addWidget(splitter, 1, 0, 1, 2);

        gridLayout->setRowStretch(1, 4);

        verticalLayout_4->addLayout(gridLayout);

        verticalLayout_4->setStretch(1, 4);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QCoreApplication::translate("Widget", "2 or 5.1 stereo", nullptr));
        pushButtonChoose1stereo->setText(QCoreApplication::translate("Widget", "choose1stereo", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "2 or 6 mono", nullptr));
        pushButtonSplit2mono->setText(QCoreApplication::translate("Widget", "split2mono", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "TextLabel", nullptr));
        pushButtonTransmit->setText(QCoreApplication::translate("Widget", "transmit", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "ErrorInfo", nullptr));
        label_5->setText(QCoreApplication::translate("Widget", "OutPutInfo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
