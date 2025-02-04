#ifndef TEMPUI_H
#define TEMPUI_H

#include "ui_tempui.h"
#include <QWidget>

void readSetupTempSpinBox(QSpinBox* spin1, QSpinBox* spin2, std::string in_str);
bool readFileCheckSaved(QSpinBox* spin1, QSpinBox* spin2, std::string in_str);


class tempUI : public QWidget
{
    Q_OBJECT

public:
    explicit tempUI(QWidget *parent = nullptr);

private slots:
    void saveSettings();
    void uploadSettings();
    void setDefaultSettings();
    bool checkSettingsSaved();
    void savecheck();
    void openPinScreen();

private:
    Ui::tempUI ui;
};

#endif // TEMPUI_H
