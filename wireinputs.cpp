#include "wireinputs.h"
#include "ui_wireinputs.h"
#include <fstream>
#include <iostream>

void wireinputs::show_str() {
    // QObject* source = QObject::sender();
    // bool btn_found = false;
    // for(int i = 0; i < sizeof(analogbtn)/sizeof(analogbtn[0]) && btn_found == false; i++) {
    //     if (analogbtn[i] == source) {
    //         this->lastButton = analogbtn[i];
    //         btn_found = true;
    //         break;
    //     }
    // }
    //ui->textBrowser->setText(QString::fromStdString(this->lastButton->displayInfo()));

    ui->textBrowser->setText("hello");

}

wireinputs::wireinputs(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::wireinputs)
{
    ui->setupUi(this);

    ui->scrollArea->setWidget(ui->pinbtnwidget);

    std::fstream SaveFile;
    SaveFile.open("pininputs.txt");
    SaveFile.close();


    // On Button Press, change the text of the text browser to the text saved in the pinInfo class for the button
    for(QPushButton* btn : ui->pinbtnwidget->findChildren<QPushButton *>()) {
        btnInfoVec.push_back(pinInfo("btn1|0|power|"));
        connect(btn, &QPushButton::clicked, this, &wireinputs::show_str);
    }
}
