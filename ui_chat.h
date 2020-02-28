/********************************************************************************
** Form generated from reading UI file 'chat.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHAT_H
#define UI_CHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QListWidget *listWidget;
    QWidget *tab_2;
    QLineEdit *lineEdit;
    QTextEdit *textEdit_2;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton_2;
    QWidget *tab_3;
    QListWidget *listWidget_2;
    QPushButton *pushButton_3;
    QWidget *tab_4;
    QListWidget *listWidget_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QWidget *tab_5;
    QTextBrowser *textBrowser;
    QTextBrowser *textBrowser_2;
    QTextEdit *textEdit_3;
    QPushButton *pushButton_4;
    QLabel *label_3;
    QLabel *label_4;
    QListWidget *listWidget_3;
    QLabel *label_5;
    QWidget *tab_6;
    QTextEdit *textEdit_4;
    QLabel *label_6;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_8;
    QLabel *label_7;
    QWidget *tab_7;
    QListWidget *listWidget_5;
    QPushButton *pushButton_9;
    QWidget *tab_8;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QPushButton *pushButton_12;
    QListWidget *listWidget_6;
    QWidget *tab_9;
    QTextEdit *textEdit_5;
    QPushButton *pushButton_13;
    QListWidget *listWidget_7;
    QTextBrowser *textBrowser_3;
    QPushButton *pushButton_14;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(667, 400);
        tabWidget = new QTabWidget(Form);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(8, 10, 651, 381));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tab->setContextMenuPolicy(Qt::NoContextMenu);
        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(548, 322, 75, 23));
        textEdit = new QTextEdit(tab);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(21, 232, 601, 81));
        listWidget = new QListWidget(tab);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(21, 10, 601, 211));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        lineEdit = new QLineEdit(tab_2);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(30, 39, 581, 25));
        textEdit_2 = new QTextEdit(tab_2);
        textEdit_2->setObjectName(QStringLiteral("textEdit_2"));
        textEdit_2->setGeometry(QRect(30, 100, 581, 211));
        label = new QLabel(tab_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(291, 12, 61, 20));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(tab_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(291, 70, 61, 20));
        label_2->setAlignment(Qt::AlignCenter);
        pushButton_2 = new QPushButton(tab_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(283, 319, 75, 23));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        listWidget_2 = new QListWidget(tab_3);
        listWidget_2->setObjectName(QStringLiteral("listWidget_2"));
        listWidget_2->setGeometry(QRect(33, 21, 581, 291));
        pushButton_3 = new QPushButton(tab_3);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(284, 323, 75, 23));
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        listWidget_4 = new QListWidget(tab_4);
        listWidget_4->setObjectName(QStringLiteral("listWidget_4"));
        listWidget_4->setGeometry(QRect(34, 20, 581, 271));
        pushButton_5 = new QPushButton(tab_4);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(363, 300, 75, 23));
        pushButton_6 = new QPushButton(tab_4);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(453, 300, 75, 23));
        pushButton_7 = new QPushButton(tab_4);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(541, 300, 75, 23));
        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        textBrowser = new QTextBrowser(tab_5);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(76, 19, 541, 25));
        textBrowser_2 = new QTextBrowser(tab_5);
        textBrowser_2->setObjectName(QStringLiteral("textBrowser_2"));
        textBrowser_2->setGeometry(QRect(76, 53, 541, 91));
        textEdit_3 = new QTextEdit(tab_5);
        textEdit_3->setObjectName(QStringLiteral("textEdit_3"));
        textEdit_3->setGeometry(QRect(76, 271, 541, 51));
        pushButton_4 = new QPushButton(tab_5);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(543, 327, 75, 23));
        label_3 = new QLabel(tab_5);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(13, 20, 61, 20));
        label_3->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(tab_5);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(13, 53, 61, 20));
        label_4->setAlignment(Qt::AlignCenter);
        listWidget_3 = new QListWidget(tab_5);
        listWidget_3->setObjectName(QStringLiteral("listWidget_3"));
        listWidget_3->setGeometry(QRect(76, 152, 541, 111));
        label_5 = new QLabel(tab_5);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(13, 152, 61, 20));
        label_5->setAlignment(Qt::AlignCenter);
        tabWidget->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        textEdit_4 = new QTextEdit(tab_6);
        textEdit_4->setObjectName(QStringLiteral("textEdit_4"));
        textEdit_4->setGeometry(QRect(30, 100, 581, 211));
        label_6 = new QLabel(tab_6);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(291, 70, 61, 20));
        label_6->setAlignment(Qt::AlignCenter);
        lineEdit_2 = new QLineEdit(tab_6);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(30, 39, 581, 25));
        pushButton_8 = new QPushButton(tab_6);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setGeometry(QRect(283, 319, 75, 23));
        label_7 = new QLabel(tab_6);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(291, 12, 61, 20));
        label_7->setAlignment(Qt::AlignCenter);
        tabWidget->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QStringLiteral("tab_7"));
        listWidget_5 = new QListWidget(tab_7);
        listWidget_5->setObjectName(QStringLiteral("listWidget_5"));
        listWidget_5->setGeometry(QRect(31, 21, 581, 291));
        pushButton_9 = new QPushButton(tab_7);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setGeometry(QRect(282, 323, 75, 23));
        tabWidget->addTab(tab_7, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QStringLiteral("tab_8"));
        pushButton_10 = new QPushButton(tab_8);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setGeometry(QRect(363, 300, 75, 23));
        pushButton_11 = new QPushButton(tab_8);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        pushButton_11->setGeometry(QRect(453, 300, 75, 23));
        pushButton_12 = new QPushButton(tab_8);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));
        pushButton_12->setGeometry(QRect(541, 300, 75, 23));
        listWidget_6 = new QListWidget(tab_8);
        listWidget_6->setObjectName(QStringLiteral("listWidget_6"));
        listWidget_6->setGeometry(QRect(34, 20, 581, 271));
        tabWidget->addTab(tab_8, QString());
        tab_9 = new QWidget();
        tab_9->setObjectName(QStringLiteral("tab_9"));
        textEdit_5 = new QTextEdit(tab_9);
        textEdit_5->setObjectName(QStringLiteral("textEdit_5"));
        textEdit_5->setGeometry(QRect(20, 252, 601, 71));
        pushButton_13 = new QPushButton(tab_9);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));
        pushButton_13->setGeometry(QRect(547, 330, 75, 23));
        listWidget_7 = new QListWidget(tab_9);
        listWidget_7->setObjectName(QStringLiteral("listWidget_7"));
        listWidget_7->setGeometry(QRect(20, 50, 601, 191));
        textBrowser_3 = new QTextBrowser(tab_9);
        textBrowser_3->setObjectName(QStringLiteral("textBrowser_3"));
        textBrowser_3->setGeometry(QRect(20, 16, 601, 25));
        pushButton_14 = new QPushButton(tab_9);
        pushButton_14->setObjectName(QStringLiteral("pushButton_14"));
        pushButton_14->setGeometry(QRect(20, 330, 75, 23));
        tabWidget->addTab(tab_9, QString());

        retranslateUi(Form);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Form", "\345\217\221   \351\200\201", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Form", "\345\271\277   \345\234\272", Q_NULLPTR));
        label->setText(QApplication::translate("Form", "\350\257\235\351\242\230\344\270\273\351\242\230", Q_NULLPTR));
        label_2->setText(QApplication::translate("Form", "\350\257\235\351\242\230\350\257\246\346\203\205", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Form", "\345\210\233   \345\273\272", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Form", "\346\226\260\345\273\272\350\257\235\351\242\230", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("Form", "\346\237\245   \347\234\213", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("Form", "\346\211\200\346\234\211\350\257\235\351\242\230", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("Form", "\346\237\245   \347\234\213", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("Form", "\345\210\240   \351\231\244", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("Form", "\345\205\250\351\203\250\345\210\240\351\231\244", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("Form", "\346\210\221\347\232\204\350\257\235\351\242\230", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("Form", "\345\217\221\351\200\201", Q_NULLPTR));
        label_3->setText(QApplication::translate("Form", "\350\257\235\351\242\230\344\270\273\351\242\230", Q_NULLPTR));
        label_4->setText(QApplication::translate("Form", "\350\257\235\351\242\230\350\257\246\346\203\205", Q_NULLPTR));
        label_5->setText(QApplication::translate("Form", "\350\257\235\351\242\230\350\257\204\350\256\272", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("Form", "\350\257\235\351\242\230\350\257\246\346\203\205", Q_NULLPTR));
        label_6->setText(QApplication::translate("Form", "\347\273\204\350\201\212\347\256\200\344\273\213", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("Form", "\345\210\233   \345\273\272", Q_NULLPTR));
        label_7->setText(QApplication::translate("Form", "\347\273\204\350\201\212\345\220\215\347\247\260", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QApplication::translate("Form", "\346\226\260\345\273\272\347\273\204\350\201\212", Q_NULLPTR));
        pushButton_9->setText(QApplication::translate("Form", "\350\277\233   \345\205\245", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_7), QApplication::translate("Form", "\346\211\200\346\234\211\347\273\204\350\201\212", Q_NULLPTR));
        pushButton_10->setText(QApplication::translate("Form", "\350\277\233   \345\205\245", Q_NULLPTR));
        pushButton_11->setText(QApplication::translate("Form", "\345\210\240   \351\231\244", Q_NULLPTR));
        pushButton_12->setText(QApplication::translate("Form", "\345\205\250\351\203\250\345\210\240\351\231\244", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_8), QApplication::translate("Form", "\346\210\221\347\232\204\347\273\204\350\201\212", Q_NULLPTR));
        pushButton_13->setText(QApplication::translate("Form", "\345\217\221   \351\200\201", Q_NULLPTR));
        pushButton_14->setText(QApplication::translate("Form", "\351\200\200\345\207\272\347\273\204\350\201\212", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_9), QApplication::translate("Form", "\347\273\204\350\201\212\350\257\246\346\203\205", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAT_H
