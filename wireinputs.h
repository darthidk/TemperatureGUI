#ifndef WIREINPUTS_H
#define WIREINPUTS_H

#include "ui_wireinputs.h"
#include <QDialog>
#include <string>

class pinInfo {
public:
    std::string outputType;
    bool used;
    std::string btnName;

    explicit pinInfo(std::string input) {
        int i = 0;
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
                    used = 1;/*std::stoi(cur_str);*/
                    break;
                case 2:
                    outputType = cur_str;
                    break;
                default:
                    break;
                }
                num_param++;
                cur_str.clear();
                i++;
            } else {
                cur_str += input[i];
            }
        }
    }
    
    std::string saveInfo() {
        // File Save, "btn1|0|power|"
        std::string output = btnName + "|" + std::to_string(used) + "|" + outputType + "|";
        return output;
    }

    std::string displayInfo() {
        std::string out_txt = "Pin: ";
        out_txt.append(btnName);
        out_txt.append("\nUse: ");
        if (used == false) {
            out_txt = "Nothing";
        } else {
            out_txt = outputType;
        }
        return out_txt;
    }
};

class wireinputs : public QDialog
{
    Q_OBJECT

public:
    QVector<pinInfo> btnInfoVec;

    static const int numbtn = 31;

    explicit wireinputs(QWidget *parent = nullptr);

public slots:
    void show_str();

private:
    Ui::wireinputs *ui;
};

#endif // WIREINPUTS_H
