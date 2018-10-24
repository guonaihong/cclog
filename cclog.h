#ifndef __CCFLOG_H
#define __CCFLOG_H

#include <string>
#include <math.h>
#include <sys/time.h>

//todo: Adjust vim to c++ style
using string = std::string;

class CCLog {
    public:
        CCLog(string level, string proc_name);
        string print_buf();
    private:
        string proc_name;
        string buf;
        string level;

    void format_header();
};

CCLog::CCLog(string level, string proc_name) {
    this->level      = level;
    this->proc_name = proc_name;
}

string CCLog::print_buf() {
    format_header();
    return buf;
}

void CCLog::format_header() {

    // Like 2018-10-23 10:00:46.356974 only needs 28 byte
    // So 64 + 1 '\0' bytes is definitely enough
    char time[65] = "";
    auto time_add = [](char *time, int time_size) -> int{
        struct timeval tv;
        struct tm result;
        int    pos, millisec;

        if (gettimeofday(&tv, nullptr) != 0) {
            return errno;
        }

        // Round to nearest millisec
        millisec = lrint(tv.tv_usec/1000.0);
        // Allow for rounding up to nearest second
        if (millisec >= 1000) {
            millisec -= 1000;
            tv.tv_sec++;
        }

        localtime_r(&tv.tv_sec, &result);

        /* YYYY-MM-DD hh:mm:ss */
        pos = strftime(time, time_size, "%Y-%m-%d %H:%M:%S", &result);
        snprintf(time + pos, time_size - pos, ".%03d", millisec);
        return 0;
    };

    string padding = "";

    if (this->level.size() == 4) {
        padding = " ";
    }

    time_add(time, sizeof(time));

    buf.push_back('['), buf.append(this->proc_name), buf.append("] ");
    buf.push_back('['), buf.append(time), buf.push_back(']');

}

#endif
