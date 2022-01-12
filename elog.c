/*
 * Copyright(c) 2022 Lux1206
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "elog.h"

#define MODULE_NAME_MAX		8
#define INVALID_MD_INDEX    0xFF
#define DEF_OUTPUT_NAME     "TEMR"


typedef struct {
    elog_output_f fn;
    int level;
    char name[MODULE_NAME_MAX];
} elog_t;

static struct {
    elog_lock_f lock;
    void *lock_argv;
    int g_level;
    int len;
    uint8_t *buff;
    elog_t elog[LOG_OUTPUT_MAX];
} elog_mgr;


static const char *level_strs[] = {
    //"ERROR", "WARN", "NOTICE", "INFO", "DEBUG", "TRACE"
    "E", "W", "N", "I", "D","T"
};


#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
  "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif


static void __lock(void)
{
    if (elog_mgr.lock) { 
        elog_mgr.lock(true, elog_mgr.lock_argv);
    }
}


static void __unlock(void)
{
    if (elog_mgr.lock) {
        elog_mgr.lock(false, elog_mgr.lock_argv);
    }
}

uint8_t __find_module(char *name)
{
    uint8_t i;

    for(i = 0; i < LOG_OUTPUT_MAX; i++) {
        if(!strcmp(name, elog_mgr.elog[i].name)) {
            return i;
        }
    }

    return INVALID_MD_INDEX;
}

uint8_t __find_free_module(char *name)
{
    for (int i = 0; i < LOG_OUTPUT_MAX; i++) {
        if (!elog_mgr.elog[i].fn) {
            return i;
        }
    }

    return INVALID_MD_INDEX;
}

int __v_printf(int index, int level, const char *file, int line, const char *fmt, ...)
{
    int cnt = 0;
    elog_t *elog = &elog_mgr.elog[index];
    
    if (level > elog->level) {
        return -1;
    }
    
    va_list ap;
    va_start(ap, fmt);

    #ifdef LOG_USE_COLOR
    cnt = snprintf(elog_mgr.buff, elog_mgr.len, "[%s%s %s\x1b[0m][%s:%d] ", 
                    level_colors[level], elog->name, level_strs[level], file, line);
    if (cnt <= 0 ) {
        return -1;
    }
    #else
    cnt = snprintf(elog_mgr.buff, elog_mgr.len, "[%s %s][%s:%d] ", 
                    elog->name, level_strs[elog->level], file, line);
    if (cnt <= 0 ) {
        return -1;
    }
    #endif

    int len = cnt;
    cnt = vsnprintf(elog_mgr.buff + len, elog_mgr.len - len, fmt, ap);
    if (cnt <= 0) {
        return -1;
    }
    len += cnt;
    if (len > (int)(elog_mgr.len - 3) ) {
        len = elog_mgr.len - 3;
    }
    elog_mgr.buff[len]     = '\r';
    elog_mgr.buff[len + 1] = '\n';
    elog_mgr.buff[len + 2] = 0;

    elog->fn(elog_mgr.buff);

    va_end(ap);

    return 0;
}

void elog_set_lock(void* argv, elog_lock_f callback)
{
    elog_mgr.lock_argv = argv;
    elog_mgr.lock = callback;
}

int elog_add_terminal(char *name, int level, elog_output_f callback)
{
    uint8_t index;

    if(level >= LOG_LEVEL_MAX || callback == NULL || 
       name == NULL || strlen(name) >= MODULE_NAME_MAX) {
       return -1;
    }

    index = __find_module(name);
    if(index == INVALID_MD_INDEX) {
        index = __find_free_module(name);
        if(index != INVALID_MD_INDEX) {
            strcpy(elog_mgr.elog[index].name, name);
        }
        else {
            return -1;
        }
    }
    elog_mgr.elog[index].level = level;
    elog_mgr.elog[index].fn = callback;

    return 0;
}

int elog_delete_terminal(char *name)
{
    uint8_t index;

    if(name == NULL || 
       strlen(name) >= MODULE_NAME_MAX) {
       return -1;
    }
    index = __find_module(name);
    if(index == INVALID_MD_INDEX) {
        return -1;
    }

    elog_mgr.elog[index].fn = NULL;
    elog_mgr.elog[index].level = LOG_TRACE;
    memset(elog_mgr.elog[index].name, 0, MODULE_NAME_MAX);
    
    return 0;
}

void elog_set_global_level(int level)
{
    elog_mgr.g_level = level;
}

int elog_get_global_level(void)
{
    return elog_mgr.g_level;
}

int elog_set_module_level(char *name, int level)
{
    uint8_t index;

    if(level >= LOG_LEVEL_MAX ||
       name == NULL || strlen(name) >= MODULE_NAME_MAX) {
       return -1;
    }

    index = __find_module(name);
    if(index == INVALID_MD_INDEX) {
        return -1;
    }

    elog_mgr.elog[index].level = level;
    
    return 0;
}

int elog_get_module_level(char *name, int *level)
{
    uint8_t index;

    if(level == NULL || name == NULL || 
       strlen(name) >= MODULE_NAME_MAX) {
       return -1;
    }

    index = __find_module(name);
    if(index == INVALID_MD_INDEX) {
        return -1;
    }

    *level = elog_mgr.elog[index].level;

    return 0;
}


void elog_module_print(char* name, int level, const char *file, int line, const char *fmt, ...)
{
    uint8_t index;

    if (level > elog_mgr.g_level) {
        return;
    }

    __lock();
    index = __find_module(name);
    if(index == INVALID_MD_INDEX) {
        goto ext;
    }
    (void)__v_printf(index, level, file, line, fmt);
    ext:
    __unlock();
}

void elog_print(int level, const char *file, int line, const char *fmt, ...)
{
    int ret = 0;
    
    if (level > elog_mgr.g_level) {
        return;
    }

    __lock();
    for (int i = 0; i < LOG_OUTPUT_MAX && elog_mgr.elog[i].fn; i++) {
        ret = __v_printf(i, level, file, line, fmt);
        if(ret != 0) {
            break;
        }
    }

    __unlock();
}

int elog_init(int level, uint8_t *buff, int len, elog_output_f callback)
{
    int ret;

    if(level >= LOG_LEVEL_MAX || callback == NULL || 
       buff == NULL || len == 0) {
       return -1;
    }
    ret = elog_add_terminal(DEF_OUTPUT_NAME, LOG_TRACE, callback);
    if(ret != 0) {
        return ret;
    }
    elog_mgr.buff = buff;
    elog_mgr.len = len;
    elog_mgr.g_level = level;

    return 0;
}





