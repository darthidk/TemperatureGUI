#include "wireinputs.h"
#include "ui_wireinputs.h"
#include <fstream>
#include <iostream>

void wireinputs::show_str() {
    QObject* source = QObject::sender();
    QString display_str;
    for(int i = 0; i < btnInfoVec.size(); i++) {
        if (btnInfoVec.at(i).btn == source) {
            display_str = btnInfoVec[i].displayInfo();
            std::cout << "hello";
            break;
        }
    }
    ui->textBrowser->setText(display_str);
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
        std::string create_string = btn->text().toStdString() + "|0|power|";
        btnInfoVec.push_back(pinInfo(create_string, btn));
        connect(btn, &QPushButton::clicked, this, &wireinputs::show_str);
    }
}
