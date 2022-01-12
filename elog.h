/*
 * @Author: Lux1206
 * @Email: 862215606@qq.com
 * @LastEditors: Please set LastEditors
 * @FileName: &file name&
 * @Description: 
 * @Copyright: Copyright 2022 Lux1206. All Rights Reserved.
 * @Date: 2022-01-10 20:40:17
 * @LastEditTime: 2022-01-11 21:05:02
 */

#ifndef __ELOG_H_
#define __ELOG_H_

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


#define LOG_USE_COLOR
#define LOG_OUTPUT_MAX      8

typedef void (*elog_output_f)(const char *str);
typedef void (*elog_lock_f)(bool lock, void* argv);

enum { 
  LOG_ERROR, 
  LOG_WARN, 
  LOG_NOTICE, 
  LOG_INFO, 
  LOG_DEBUG, 
  LOG_TRACE,
  LOG_LEVEL_MAX,
};

void elog_print(int level, const char *file, int line, const char *fmt, ...);
void elog_module_print(char* name, int level, const char *file, int line, const char *fmt, ...);

// elog output without module
#define ELOG_TRACE(...)         elog_print(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define ELOG_DEBUG(...)         elog_print(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define ELOG_INFO(...)          elog_print(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define ELOG_NOTICE(...)        elog_print(LOG_NOTICE, __FILE__, __LINE__, __VA_ARGS__)
#define ELOG_WARN(...)          elog_print(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define ELOG_ERROR(...)         elog_print(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

// elog output without module
#define ELOG_MDU_TRACE(module, ...)     elog_module_print(module, LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define ELOG_MDU_DEBUG(module, ...)     elog_module_print(module, LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define ELOG_MDU_INFO(module, ...)      elog_module_print(module, LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define ELOG_MDU_NOTICE(module, ...)    elog_module_print(module, LOG_NOTICE, __FILE__, __LINE__, __VA_ARGS__)
#define ELOG_MDU_WARN(module, ...)      elog_module_print(module, LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define ELOG_MDU_ERROR(module, ...)     elog_module_print(module, LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)


/**
 * @brief elog init
 * @note: this API will create a default output terminal of LOG_TRACE level
 * 
 * @param[in] level: global log output level
 * @param[in] buff:  point to print buff
 * @param[in] len:   buff len
 * @param[in] callback: elog print callback
 * @return 0:sucess, others:fail
 */
int elog_init(int level, uint8_t *buff, int len, elog_output_f callback);

/**
 * @brief elog lock set
 * @note: callback will tall mutex lock or unlock
 * 
 * @param[in] argv: lock callback argv
 * @param[in] callback: point to lock callback
 * @return none
 */
void elog_set_lock(void* argv, elog_lock_f callback);

/**
 * @brief elog terminal delete
 * 
 * @param[in] name: point to terminal name(name string must less than 8bytes)
 * @return 0:sucess, others:fail
 */
int elog_delete_terminal(char *name);

/**
 * @brief elog terminal add
 * 
 * @param[in] name: point to terminal name(string must be less than 8 bytes)
 * @param[in] level: globle output level(logs larger than this level will not be output)
 * @param[in] callback: point to terminal output callback
 * @return 0:sucess, others:fail
 */
int elog_add_terminal(char *name, int level, elog_output_f callback);

/**
 * @brief elog get global output level
 * 
 * @param[in] none
 * @return output level
 */
int elog_get_global_level(void);

/**
 * @brief elog set global output level
 * 
 * @param[in] level: globle output level(logs larger than this level will not be output)
 * @return none
 */
void elog_set_global_level(int level);

/**
 * @brief elog set module output level
 * 
 * @param[in] name: point to terminal name(string must be less than 8 bytes)
 * @param[in] level: module output level(module logs larger than this level will not be output)
 * @return none
 */
int elog_set_module_level(char *name, int level);

/**
 * @brief elog get module output level
 * 
 * @param[in] name: point to terminal name(string must be less than 8 bytes)
 * @param[out] level: point to module output level
 * @return 0:sucess, others:fail
 */
int elog_get_module_level(char *name, int *level);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
