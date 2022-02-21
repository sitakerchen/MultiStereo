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
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPushButton *pushButtonChoose1stereo;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QPushButton *pushButtonSplit2mono;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QPushButton *pushButtonTransmit;
    QTextEdit *textEdit;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QPushButton *pushButtonShowTargetInfo;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1400, 800);
        Widget->setMinimumSize(QSize(1400, 800));
        Widget->setMaximumSize(QSize(1400, 800));
        layoutWidget = new QWidget(Widget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(220, 110, 122, 149));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        pushButtonChoose1stereo = new QPushButton(layoutWidget);
        pushButtonChoose1stereo->setObjectName(QString::fromUtf8("pushButtonChoose1stereo"));
        pushButtonChoose1stereo->setMinimumSize(QSize(120, 120));

        verticalLayout->addWidget(pushButtonChoose1stereo);

        layoutWidget1 = new QWidget(Widget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(519, 110, 122, 149));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetFixedSize);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        pushButtonSplit2mono = new QPushButton(layoutWidget1);
        pushButtonSplit2mono->setObjectName(QString::fromUtf8("pushButtonSplit2mono"));
        pushButtonSplit2mono->setMinimumSize(QSize(120, 120));

        verticalLayout_2->addWidget(pushButtonSplit2mono);

        layoutWidget2 = new QWidget(Widget);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(648, 110, 122, 149));
        verticalLayout_3 = new QVBoxLayout(layoutWidget2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetFixedSize);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_3->addWidget(label_3);

        pushButtonTransmit = new QPushButton(layoutWidget2);
        pushButtonTransmit->setObjectName(QString::fromUtf8("pushButtonTransmit"));
        pushButtonTransmit->setMinimumSize(QSize(120, 120));

        verticalLayout_3->addWidget(pushButtonTransmit);

        textEdit = new QTextEdit(Widget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(20, 440, 1351, 331));
        layoutWidget_2 = new QWidget(Widget);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(370, 110, 122, 149));
        verticalLayout_4 = new QVBoxLayout(layoutWidget_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setSizeConstraint(QLayout::SetFixedSize);
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(layoutWidget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_4->addWidget(label_4);

        pushButtonShowTargetInfo = new QPushButton(layoutWidget_2);
        pushButtonShowTargetInfo->setObjectName(QString::fromUtf8("pushButtonShowTargetInfo"));
        pushButtonShowTargetInfo->setMinimumSize(QSize(120, 120));

        verticalLayout_4->addWidget(pushButtonShowTargetInfo);


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
        label_4->setText(QCoreApplication::translate("Widget", "info", nullptr));
        pushButtonShowTargetInfo->setText(QCoreApplication::translate("Widget", "showTargetInfo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
