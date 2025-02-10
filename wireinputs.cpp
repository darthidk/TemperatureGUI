#include "wireinputs.h"
#include "ui_wireinputs.h"
#include <fstream>
#include <iostream>
#include "ArduSerial.h"
#include "savedialog.h"

void wireinputs::show_str() {
    if (btnInfoVec[lastPinIndex].send_to_arduino == true) {
        btnInfoVec[lastPinIndex].btn->setStyleSheet("background-color: green");
    } else if (btnInfoVec[lastPinIndex].used == true) {
        btnInfoVec[lastPinIndex].btn->setStyleSheet("background-color: orange");
    } else {
        btnInfoVec[lastPinIndex].btn->setStyleSheet("background-color: ");
    }
    QObject* source = QObject::sender();
    QString display_str;
    for(int i = 0; i < btnInfoVec.size(); i++) {
        if (btnInfoVec.at(i).btn == source) {
            display_str = btnInfoVec[i].displayInfo();
            ui->checkBox->setChecked(btnInfoVec[i].send_to_arduino);
            lastPinIndex = i;
            btnInfoVec[i].btn->setStyleSheet("background-color: red");
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
    btnInfoVec[lastPinIndex].send_to_arduino = ui->checkBox->isChecked();
    if (btnInfoVec[lastPinIndex].outputType == "Unused") {
        btnInfoVec[lastPinIndex].used = false;
        btnInfoVec[lastPinIndex].send_to_arduino = false;
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
        serial_input.print("P");
        for(int i = 0; i < btnInfoVec.size(); i++) {
            if (btnInfoVec[i].send_to_arduino == true) {
                std::string write_str = btnInfoVec[i].sendInfo();
                std::cout << write_str.c_str();
                serial_input.write(write_str.c_str(), write_str.size());
            }
        }
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

void wireinputs::update_send_setting() {
    btnInfoVec[lastPinIndex].send_to_arduino = ui->checkBox->isChecked();
}

wireinputs::wireinputs(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::wireinputs)
{
    ui->setupUi(this);

    ui->scrollArea->setWidget(ui->pinbtnwidget);
    connect(ui->pushButtonSetter, &QPushButton::clicked, this, &wireinputs::save_to_file);
    connect(ui->uploadButton, &QPushButton::clicked, this, &wireinputs::upload_settings);
    connect(ui->checkBox, &QCheckBox::clicked, this, &wireinputs::update_send_setting);

    std::fstream SaveFile;
    SaveFile.open("pininputs.txt");
    for(QPushButton* btn : ui->pinbtnwidget->findChildren<QPushButton *>()) {
        std::string in_str;
        std::getline(SaveFile, in_str);
        btnInfoVec.push_back(pinInfo(in_str, btn));
        connect(btn, &QPushButton::clicked, this, &wireinputs::show_str);
        if (btnInfoVec.last().send_to_arduino == true) {
            btn->setStyleSheet("background-color: green");
        } else if (btnInfoVec.last().used == true) {
            btn->setStyleSheet("background-color: orange");
        }
    }
    SaveFile.close();
}
