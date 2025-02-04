#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include "ui_savedialog.h"
#include <QDialog>

enum DialogSetup {
    SaveSuccess,
    SaveFail,
    UploadSuccess,
    UploadFail
};

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDialog(DialogSetup setup, QDialog *parent = nullptr);

private slots:
    void closeWindow();

private:
    Ui::SaveDialog ui;
};

#endif // SAVEDIALOG_H
