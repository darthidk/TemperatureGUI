#include "tempui.h"
#include "savedialog.h"
#include "wireinputs.h"
#include "ArduSerial.cpp"
#include <fstream>
#include <iostream>
#include <string>

void readSetupTempSpinBox(QSpinBox* spin1, QSpinBox* spin2, std::string in_str) {
    int i = 2;
    std::string val;
    while(in_str[i] != '|') {
        val += in_str[i];
        i++;
    }
    spin1->setValue(std::stoi(val));
    i += 1;
    val.clear();
    while(in_str[i] != '|') {
        val += in_str[i];
        i++;
    }
    spin2->setValue(std::stoi(val));
}

bool readFileCheckSaved(QSpinBox* spin1, QSpinBox* spin2, std::string in_str) {
    int i = 2;
    std::string val;
    while(in_str[i] != '|') {
        val += in_str[i];
        i++;
    }
    if (std::stoi(val) != spin1->value()) {
        return false;
    }
    i += 1;
    val.clear();
    while(in_str[i] != '|') {
        val += in_str[i];
        i++;
    }
    if (std::stoi(val) != spin2->value()) {
        return false;
    }
    return true;
}

tempUI::tempUI(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.pushSaveButton, &QPushButton::clicked, this, &tempUI::saveSettings);
    connect(ui.pushUploadButton, &QPushButton::clicked, this, &tempUI::uploadSettings);
    connect(ui.pushDefaultButton, &QPushButton::clicked, this, &tempUI::setDefaultSettings);
    connect(ui.pushButton, &QPushButton::clicked, this, &tempUI::savecheck);
    connect(ui.pushPinButton,&QPushButton::clicked, this, &tempUI::openPinScreen);
    connect(ui.pushRescan, &QPushButton::clicked, this, &tempUI::displayCOMconnected);
    connect(ui.comConnectedCheckBox, &QCheckBox::clicked, this, &tempUI::resetCOMCheckBox);

    std::ifstream settingsfile;
    settingsfile.open("settings.txt");
    std::string input;
    while (getline(settingsfile, input)) {
        switch (input[0]) {
        case 'L':
        {
            readSetupTempSpinBox(ui.spinBox_1, ui.spinBox_2, input);
            break;
        }
        case 'M':
        {
            readSetupTempSpinBox(ui.spinBox_3, ui.spinBox_4, input);
            break;
        }
        case 'H':
        {
            readSetupTempSpinBox(ui.spinBox_5, ui.spinBox_6, input);
            break;
        }
        case 'P':
            ui.checkLEDBox->setChecked((int)(input[2] - '0'));
            ui.checkLockBox->setChecked((int)(input[4] - '0'));
            ui.checkPinBox->setChecked((int)(input[6] - '0'));
            break;
        }
    }
    settingsfile.close();
}

void tempUI::saveSettings() {
    int progress_delay = 100/6;

    std::fstream SaveFile;
    ui.saveProgressBar->setValue(0);
    SaveFile.open("settings.txt");

    if (SaveFile.is_open() != true) {
        SaveDialog *n = new SaveDialog(DialogSetup::SaveFail);
        n->show();
        n->exec();
        return;
    }

    ui.saveProgressBar->setValue(progress_delay);
    SaveFile << "L|" << ui.spinBox_1->value() << "|" << ui.spinBox_2->value() << "|\n";
    ui.saveProgressBar->setValue(progress_delay * 2);
    SaveFile << "M|" << ui.spinBox_3->value() << "|" << ui.spinBox_4->value() << "|\n";
    ui.saveProgressBar->setValue(progress_delay * 3);
    SaveFile << "H|" << ui.spinBox_5->value() << "|" << ui.spinBox_6->value() << "|\n";
    ui.saveProgressBar->setValue(progress_delay * 4);
    SaveFile << "P|" << ui.checkLEDBox->isChecked() << "|"
             << ui.checkLockBox->isChecked() << "|"
             << ui.checkPinBox->isChecked() << "|\n;";
    ui.saveProgressBar->setValue(progress_delay * 5);
    SaveFile.close();
    ui.saveProgressBar->setValue(100);

    SaveDialog *n = new SaveDialog(DialogSetup::SaveSuccess);
    n->show();
    n->exec();
}


void tempUI::uploadSettings() {
    if (this->checkSettingsSaved() == false) {
        this->checkSettingsSaved();
    }
    WindowsSerial serial_input = WindowsSerial(ui.comPortSpinBox->value());
    serial_input.begin(9600, ui.comPortSpinBox->value());

    if (serial_input.connected()) {
        serial_input.print("S");
        std::fstream SaveFile;
        SaveFile.open("settings.txt");
        std::string new_settings;
        std::string next_line;
        while(getline(SaveFile, next_line)) {
            new_settings += next_line;
        }
        serial_input.write(new_settings.c_str(), sizeof(new_settings));
        SaveFile.close();

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

void tempUI::setDefaultSettings() {
    ui.spinBox_1->setValue(0);
    ui.spinBox_2->setValue(23);
    ui.spinBox_3->setValue(18);
    ui.spinBox_4->setValue(31);
    ui.spinBox_5->setValue(25);
    ui.spinBox_6->setValue(33);
    ui.checkLEDBox->setChecked(true);
    ui.checkLockBox->setChecked(false);
    ui.checkPinBox->setChecked(true);
    this->saveSettings();
}

bool tempUI::checkSettingsSaved() {
    bool saved = true;
    std::ifstream settingsfile;
    settingsfile.open("settings.txt");
    std::string input;
    while (getline(settingsfile, input)) {
        switch (input[0]) {
        case 'L':
        {
            saved = readFileCheckSaved(ui.spinBox_1, ui.spinBox_2, input);
            break;
        }
        case 'M':
        {
            saved = readFileCheckSaved(ui.spinBox_3, ui.spinBox_4, input);
            break;
        }
        case 'H':
        {
            saved = readFileCheckSaved(ui.spinBox_5, ui.spinBox_6, input);
            break;
        }
        case 'P':
            if (ui.checkLEDBox->isChecked() != (int)(input[2] - '0')) {
                saved = false;
            }
            if (ui.checkLockBox->isChecked() != (int)(input[4] - '0')) {
                saved = false;
            }
            if (ui.checkLockBox->isChecked() != (int)(input[6] - '0')) {
                saved = false;
            }
            break;
        }
        if (saved == false) {
            break;
        }
    }
    settingsfile.close();
    return saved;
}

void tempUI::savecheck() {
    if (this->checkSettingsSaved() == false) {
        SaveDialog *n = new SaveDialog(DialogSetup::UploadFail);
        n->show();
        n->exec();
    }
}

void tempUI::openPinScreen() {
    wireinputs *n = new wireinputs;
    connect(n, SIGNAL(getCOMport()), this, SLOT(getCOMport()));
    n->show();
    n->exec();
}

int tempUI::getCOMport() {
    return ui.comPortSpinBox->value();
}

bool tempUI::comConnected() {
    WindowsSerial serial_input = WindowsSerial(ui.comPortSpinBox->value());
    serial_input.begin(9600, ui.comPortSpinBox->value());
    bool result = serial_input.connected();
    serial_input.end();
    std::cout << result;
    return result;
}

void tempUI::displayCOMconnected() {
    // WindowsSerial serial_input = WindowsSerial(ui.comPortSpinBox->value());
    // ui.comConnectedCheckBox->setChecked(serial_input.connected());
    ui.comConnectedCheckBox->setChecked(tempUI::comConnected());
}

void tempUI::resetCOMCheckBox() {
    ui.comConnectedCheckBox->setChecked(!ui.comConnectedCheckBox->isChecked());
}

