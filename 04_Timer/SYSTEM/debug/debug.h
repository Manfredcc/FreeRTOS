#ifndef __DEBUG_H__
#define __DEBUG_H__

typedef enum {
    EMERG,
    ERR,
    WARN,
    INFO,
    DEBUG,
    LOG_LEVEL_MAX,
}LOG_LEVEL;

#define SWITCH  1   /* global switch */
#define CUR_LOG_LEVEL   DEBUG

#define pr_emerg(fmt, ...)   \
            pr_log(EMERG, fmt, ##__VA_ARGS__)   \

#define pr_err(fmt, ...)   \
            pr_log(ERR, fmt, ##__VA_ARGS__)   \

#define pr_warn(fmt, ...)   \
            pr_log(WARN, fmt, ##__VA_ARGS__)   \

#define pr_info(fmt, ...)   \
            pr_log(INFO, fmt, ##__VA_ARGS__)   \

#define pr_debug(fmt, ...)   \
            pr_log(DEBUG, fmt, ##__VA_ARGS__)   \

/* This macro should not be called directly */
#define pr_log(level, fmt, ...)      \
    do {                                \
        if (level <= CUR_LOG_LEVEL && SWITCH)      \
            printf("%s:%s " fmt, __FILE__,              \
                __func__, ##__VA_ARGS__);               \
        else                                            \
            ;   \
    } while (0)

#endif /* __DEBUG_H__ */
