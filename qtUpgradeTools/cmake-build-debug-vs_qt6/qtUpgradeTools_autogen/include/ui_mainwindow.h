/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QGroupBox *portComboBox;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *serial_port_comboBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *baud_rate_comboBox;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *open_pushButton;
    QPushButton *scan_port_pushButton;
    QPushButton *send_pushButton;
    QLineEdit *firm_path_lineEdit;
    QTextBrowser *tx_msg_textBrowser;
    QPushButton *load_file_pushButton;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1003, 627);
        portComboBox = new QGroupBox(MainWindow);
        portComboBox->setObjectName("portComboBox");
        portComboBox->setGeometry(QRect(20, 10, 291, 271));
        gridLayout = new QGridLayout(portComboBox);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(portComboBox);
        label->setObjectName("label");
        label->setMinimumSize(QSize(100, 40));
        label->setMaximumSize(QSize(100, 40));

        horizontalLayout->addWidget(label);

        serial_port_comboBox = new QComboBox(portComboBox);
        serial_port_comboBox->setObjectName("serial_port_comboBox");
        serial_port_comboBox->setMinimumSize(QSize(100, 40));
        serial_port_comboBox->setMaximumSize(QSize(100, 40));

        horizontalLayout->addWidget(serial_port_comboBox);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_2 = new QLabel(portComboBox);
        label_2->setObjectName("label_2");
        label_2->setMinimumSize(QSize(100, 40));
        label_2->setMaximumSize(QSize(100, 40));

        horizontalLayout_2->addWidget(label_2);

        baud_rate_comboBox = new QComboBox(portComboBox);
        baud_rate_comboBox->addItem(QString());
        baud_rate_comboBox->addItem(QString());
        baud_rate_comboBox->addItem(QString());
        baud_rate_comboBox->addItem(QString());
        baud_rate_comboBox->setObjectName("baud_rate_comboBox");
        baud_rate_comboBox->setMinimumSize(QSize(100, 40));
        baud_rate_comboBox->setMaximumSize(QSize(100, 40));

        horizontalLayout_2->addWidget(baud_rate_comboBox);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        open_pushButton = new QPushButton(portComboBox);
        open_pushButton->setObjectName("open_pushButton");
        open_pushButton->setMinimumSize(QSize(100, 40));
        open_pushButton->setMaximumSize(QSize(100, 40));

        horizontalLayout_3->addWidget(open_pushButton);

        scan_port_pushButton = new QPushButton(portComboBox);
        scan_port_pushButton->setObjectName("scan_port_pushButton");
        scan_port_pushButton->setMinimumSize(QSize(100, 40));
        scan_port_pushButton->setMaximumSize(QSize(100, 40));

        horizontalLayout_3->addWidget(scan_port_pushButton);


        verticalLayout->addLayout(horizontalLayout_3);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        send_pushButton = new QPushButton(MainWindow);
        send_pushButton->setObjectName("send_pushButton");
        send_pushButton->setGeometry(QRect(160, 330, 91, 31));
        firm_path_lineEdit = new QLineEdit(MainWindow);
        firm_path_lineEdit->setObjectName("firm_path_lineEdit");
        firm_path_lineEdit->setGeometry(QRect(10, 370, 321, 31));
        tx_msg_textBrowser = new QTextBrowser(MainWindow);
        tx_msg_textBrowser->setObjectName("tx_msg_textBrowser");
        tx_msg_textBrowser->setGeometry(QRect(50, 420, 201, 161));
        load_file_pushButton = new QPushButton(MainWindow);
        load_file_pushButton->setObjectName("load_file_pushButton");
        load_file_pushButton->setGeometry(QRect(30, 330, 81, 31));

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        portComboBox->setTitle(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\351\205\215\347\275\256:", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\347\253\257\345\217\243\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207\357\274\232", nullptr));
        baud_rate_comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "115200", nullptr));
        baud_rate_comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "921600", nullptr));
        baud_rate_comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "1500000", nullptr));
        baud_rate_comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "2000000", nullptr));

        open_pushButton->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200", nullptr));
        scan_port_pushButton->setText(QCoreApplication::translate("MainWindow", "\346\211\253\346\217\217", nullptr));
        send_pushButton->setText(QCoreApplication::translate("MainWindow", "\344\270\213\350\275\275", nullptr));
        load_file_pushButton->setText(QCoreApplication::translate("MainWindow", "\345\212\240\350\275\275BIN\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
