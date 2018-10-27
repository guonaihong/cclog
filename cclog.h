#ifndef __CCFLOG_H
#define __CCFLOG_H

#include <string>
#include <mutex>
#include <math.h>
#include <stdarg.h>
#include <sys/time.h>

//todo: Adjust vim to c++ style
using string = std::string;

enum class Level {Debug, Info, Warn, Error};

class CCLog {
    public:

        CCLog(enum Level level, string proc_name);


        void errorf(const char *format, ...);

        void warnf(const char *format, ...);

        void infof(const char *format, ...);

        void debugf(const char *format, ...);
    private:

        string     proc_name;

        string     buf;

        enum Level level;

        std::mutex m;

        void format_header(string level);

        void output();
};

CCLog::CCLog(enum Level level, string proc_name) {
    this->proc_name = proc_name;
    this->level     = level;
}

void CCLog::output() {
    std::cout<< this->buf;
}

#define cclog_printf(level, format, ap) {                                 \
    std::lock_guard<std::mutex> l(this->m);                               \
    this->buf.clear();                                                    \
                                                                          \
    this->format_header(level);                                           \
                                                                          \
    int old_size      =  this->buf.size();                                \
                                                                          \
    int need_space    = vsnprintf((char *)"", 0, format, ap);             \
    va_end(ap);                                                           \
    need_space++;                                                         \
                                                                          \
    this->buf.resize(old_size + need_space);                              \
                                                                          \
    va_start(ap, format);                                                 \
    vsnprintf((char *)this->buf.c_str()+old_size, need_space, format, ap);\
    va_end(ap);                                                           \
                                                                          \
    this->output();                                                       \
}

void CCLog::errorf(const char *format, ...) {

    if (Level::Error < this->level) {
        return ;
    }

    va_list ap;
    va_start(ap, format);
    cclog_printf("error", format, ap);
}

void CCLog::warnf(const char *format, ...) {

    if (Level::Warn < this->level) {
        return ;
    }

    va_list ap;
    va_start(ap, format);
    cclog_printf("warn", format, ap);
}

void CCLog::infof(const char *format, ...) {

    if (Level::Info < this->level) {
        return ;
    }

    va_list ap;
    va_start(ap, format);
    cclog_printf("info", format, ap);
}

void CCLog::debugf(const char *format, ...) {

    if (Level::Debug < this->level) {
        return ;
    }

    va_list ap;
    va_start(ap, format);
    cclog_printf("debug", format, ap);
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
    buf.push_back('['), buf.append(level);

    if (level.size() == 4) {
        buf.append(" ");
    }
    buf.append("] ");

}

#endif
