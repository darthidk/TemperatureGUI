#ifndef WIREINPUTS_H
#define WIREINPUTS_H

#include "ui_wireinputs.h"
#include <QDialog>
#include <string>
#include <algorithm>

class pinInfo {
public:
    QPushButton* btn;
    std::string outputType;
    bool used;
    std::string btnName;
    
    std::string saveInfo() {
        // File Save, "btn1|0|power|"
        std::string output = btnName + "|" + std::to_string(used) + "|" + outputType + "|";
        return output;
    }
    
    void initialise(std::string input, QPushButton *in_btn) {
        btn = in_btn;
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

    std::string displayInfo() {
        std::string out_txt = "Pin: ";
        out_txt.append(btnName);
        out_txt.append("\nUse: ");
        if (used == false) {
            out_txt = "Not used";
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
    //std::vector<pinInfo> analogbtn;

    static const int numbtn = 31;
    // pinInfo analogbtn[numbtn];


    // //static const int analogextrasize = 7;
    // pinInfo* analogextra[7];

    // //static const int digbtnsize = 14;
    // pinInfo* digbtn[14];

    // //static const int digextrasize = 4;
    // pinInfo* digextra[4];

    pinInfo* lastButton = nullptr;

    explicit wireinputs(QWidget *parent = nullptr);

private slots:
    void show_str();

private:
    Ui::wireinputs *ui;
};

#endif // WIREINPUTS_H
