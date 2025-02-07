#include "wireinputs.h"
#include "ui_wireinputs.h"
#include <fstream>
#include <iostream>
#include "ArduSerial.h"
#include "savedialog.h"

void wireinputs::show_str() {
    QObject* source = QObject::sender();
    QString display_str;
    for(int i = 0; i < btnInfoVec.size(); i++) {
        if (btnInfoVec.at(i).btn == source) {
            display_str = btnInfoVec[i].displayInfo();
            lastPinIndex = i;
            break;
        }
    }
    ui->textBrowser->setText(display_str);
    ui->comboBox->setCurrentText(QString::fromStdString(btnInfoVec.at(lastPinIndex).outputType));
}

void wireinputs::save_to_file() {
    std::fstream SaveFile;
    SaveFile.open("pininputs.txt");
    btnInfoVec[lastPinIndex].outputType = ui->comboBox->currentText().toStdString();
    btnInfoVec[lastPinIndex].used = true;
    btnInfoVec[lastPinIndex].btn->click();
    if (btnInfoVec[lastPinIndex].outputType == "Unused") {
        btnInfoVec[lastPinIndex].used = false;
    }
    for(int i = 0; i < btnInfoVec.size(); i++) {
        SaveFile << btnInfoVec[i].saveInfo() << "\n";
    }
    SaveFile.close();
}

void wireinputs::upload_settings() {
    WindowsSerial serial_input = WindowsSerial(wireinputs::getCOMport());
    serial_input.begin(9600, wireinputs::getCOMport());

    if (serial_input.connected()) {
        SaveDialog *n = new SaveDialog(DialogSetup::UploadSuccess);
        n->show();
        n->exec();
    } else {
        SaveDialog *n = new SaveDialog(DialogSetup::UploadFail);
        n->show();
        n->exec();
    }
    serial_input.end();
}

wireinputs::wireinputs(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::wireinputs)
{
    ui->setupUi(this);

    ui->scrollArea->setWidget(ui->pinbtnwidget);
    connect(ui->pushButtonSetter, &QPushButton::clicked, this, &wireinputs::save_to_file);
    connect(ui->uploadButton, &QPushButton::clicked, this, &wireinputs::upload_settings);

    // On Button Press, change the text of the text browser to the text saved in the pinInfo class for the button

    std::fstream SaveFile;
    SaveFile.open("pininputs.txt");
    for(QPushButton* btn : ui->pinbtnwidget->findChildren<QPushButton *>()) {
        std::string in_str;
        std::getline(SaveFile, in_str);
        btnInfoVec.push_back(pinInfo(in_str, btn));
        connect(btn, &QPushButton::clicked, this, &wireinputs::show_str);
    }
    SaveFile.close();
}
