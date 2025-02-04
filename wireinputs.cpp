#include "wireinputs.h"
#include "ui_wireinputs.h"
#include <fstream>
#include <vector>

void wireinputs::show_str() {
    // QObject* source = QObject::sender();
    // bool btn_found = false;
    // for(int i = 0; i < sizeof(analogbtn)/sizeof(analogbtn[0]) && btn_found == false; i++) {
    //     if (analogbtn[i]->btn == source) {
    //         this->lastButton = analogbtn[i];
    //         btn_found = true;
    //         break;
    //     }
    // }
    // for(int i = 0; i < sizeof(analogextra)/sizeof(analogextra[0]) && btn_found == false; i++) {
    //     if (analogextra[i]->btn == source) {
    //         this->lastButton = analogextra[i];
    //         btn_found = true;
    //         break;
    //     }
    // }
    // for(int i = 0; i < sizeof(digbtn)/sizeof(digbtn[0]) && btn_found == false; i++) {
    //     if (digbtn[i]->btn == source) {
    //         this->lastButton = digbtn[i];
    //         btn_found = true;
    //         break;
    //     }
    // }
    // for(int i = 0; i < sizeof(digextra)/sizeof(digextra[0]) && btn_found == false; i++) {
    //     if (digextra[i]->btn == source) {
    //         this->lastButton = digextra[i];
    //         btn_found = true;
    //         break;
    //     }
    // }
    // ui->textBrowser->setText(QString::fromStdString(this->lastButton->displayInfo()));

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

    //analogbtn.reserve(sizeof(pinInfo) * 6);

    // pinInfo A5;
    // A5.initialise("A5|1|Temp sensor|", ui->A5Button);

    // for(int i = 0; i < numbtn; i++) {
    //     analogbtn[i] = A5;
    // }

    // On Button Press, change the text of the text browser to the text saved in the pinInfo class for the button
    connect(ui->A5Button, &QPushButton::clicked, this, &wireinputs::show_str);
}

// wireinputs::~wireinputs()
// {
//     delete ui;
// }
