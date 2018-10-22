#ifndef __CCFLOG_H
#define __CCFLOG_H

//todo: Adjust vim to c++ style
using std::string = string;

class CCLog {
    pirvate:
        string proc_name;
        int    level;

    void add_header(string level);
};

void CCLog::add_header(string level) {

    string padding = "";

    if (level.size() == 4) {
        padding = " ";
    }
}

#endif
