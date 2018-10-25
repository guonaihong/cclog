#ifndef __CCFLOG_H
#define __CCFLOG_H

#include <string>
#include <math.h>
#include <sys/time.h>

//todo: Adjust vim to c++ style
using string = std::string;

enum class Level {Debug, Info, Warn, Error};

class CCLog {
    public:

        CCLog(enum Level level, string proc_name);

        string print_buf();

        void errorf(const char *format, ...);
        void warnf(const char *format, ...);
        void infof(const char *format, ...);
        void debugf(const char *format, ...);
    private:

        void printf(string level, const char *format, va_list ap);
        string     proc_name;
        string     buf;
        enum Level level;

    void format_header(string level);
};

CCLog::CCLog(enum Level level, string proc_name) {
    this->proc_name = proc_name;
    this->level     = level;
}

string CCLog::print_buf() {
    format_header("debug");
    return buf;
}

void CCLog::errorf(const char *format, ...) {
    if (Level::Error < this->level) {
        return ;
    }
}

void CCLog::warnf(const char *format, ...) {
    if (Level::Warn < this->level) {
    }
}

void CCLog::infof(const char *format, ...) {
    if (Level::Info < this->level) {
    }
}

void CCLog::debugf(const char *format, ...) {
    if (Level::Debug < this->level) {
    }
}

void CCLog::format_header(string level) {

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

        // YYYY-MM-DD hh:mm:ss
        pos = strftime(time, time_size, "%Y-%m-%d %H:%M:%S", &result);
        snprintf(time + pos, time_size - pos, ".%03d", millisec);
        return 0;
    };

    time_add(time, sizeof(time));

    buf.push_back('['), buf.append(this->proc_name), buf.append("] ");
    buf.push_back('['), buf.append(time), buf.append("] ");
    buf.push_back('['), buf.append(level), buf.append("] ");

    if (level.size() == 4) {
        buf.append(" ");
    }

}

#endif
