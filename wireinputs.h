#ifndef WIREINPUTS_H
#define WIREINPUTS_H

#include "ui_wireinputs.h"
#include <QDialog>
#include <string>
#include <vector>

const std::vector<std::string> serialmap = {
    "Temperature Sensor",
    "Settings Display Button",
    "LED On/Off Button",
    "LED Red",
    "LED Green",
    "LED Blue"
};


class pinInfo {
public:
    QPushButton* btn;
    std::string outputType;
    bool used;
    bool send_to_arduino;
    std::string btnName;

    explicit pinInfo(std::string input, QPushButton* in_btn) {
        btn = in_btn;
        int i = -1;
        std::string cur_str;
        int num_param = 0;
        while((unsigned long long)i != input.length()) {
            i++;
            if (input[i] == '|') {
                switch (num_param) {
                case 0:
                    btnName = cur_str;
                    break;
                case 1:
                    if (cur_str.at(0) == '1') {
                        send_to_arduino = true;
                    } else {
                        send_to_arduino = false;
                    }
                    break;
                case 2:
                    if (cur_str.at(0) == '1') {
                        used = true;
                    } else {
                        used = false;
                    }
                    break;
                case 3:
                    outputType = cur_str;
                    break;
                default:
                    break;
                }
                num_param++;
                cur_str.clear();
            } else {
                cur_str += input[i];
            }
        }
    }
    
    std::string saveInfo() {
        // File Save, "btn1|0|1|power|"
        std::string output = btnName + "|" + std::to_string(send_to_arduino) + "|" + std::to_string(used) + "|" + outputType + "|";
        return output;
    }

    std::string sendInfo() {
        unsigned long long i = 0;
        for(i = 0; i < serialmap.size(); i++) {
            if (outputType == serialmap[i]) {
                break;
            }
        }
        std::string output = btnName + "|" + std::to_string(i) + "|";
        return output;
    }

    QString displayInfo() {
        QString out_txt = "Pin: ";
        out_txt.append(btnName);
        out_txt.append("\nUse: ");
        if (used == false) {
            out_txt += "Unused";
        } else {
            out_txt += QString::fromStdString(outputType);
        }
        return out_txt;
    }
};

class wireinputs : public QDialog
{
    Q_OBJECT

public:
    QVector<pinInfo> btnInfoVec;

    int lastPinIndex;

    explicit wireinputs(QWidget *parent = nullptr);

public slots:
    void show_str();
    void save_to_file();
    void upload_settings();
    void update_send_setting();

signals:
    int getCOMport();

private:
    Ui::wireinputs *ui;
};

#endif // WIREINPUTS_H
