#include "savedialog.h"
#include "ui_savedialog.h"

SaveDialog::SaveDialog(DialogSetup setup, QDialog *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton, &QPushButton::clicked, this, &SaveDialog::deleteLater);

    switch(setup) {
    case DialogSetup::SaveSuccess:
        ui.textBrowser->setText("Settings saved successfully");
        break;
    case DialogSetup::SaveFail:
        ui.textBrowser->setText("Settings not saved");
        break;
    case DialogSetup::UploadSuccess:
        ui.textBrowser->setText("Uploaded settings");
        break;
    case DialogSetup::UploadFail:
        ui.textBrowser->setText("Failed to upload settings");
        break;
    }
}

void SaveDialog::closeWindow()
{

}
