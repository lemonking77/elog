/*
 * @Author: Lux1206
 * @Email: 862215606@qq.com
 * @LastEditors: Please set LastEditors
 * @FileName: &file name&
 * @Description: 
 * @Copyright: Copyright 2022 Lux1206. All Rights Reserved.
 * @Date: 2022-01-10 20:40:17
 * @LastEditTime: 2022-01-11 21:15:28
 */


#include "elog.h"

void elog_user_output1(const char *str)
{
    printf(" %s", str);
}

void elog_user_output2(const char *str)
{
    printf(" %s", str);
}

int main(void)
{
    uint8_t buff[128] = {0};

    elog_init(LOG_TRACE, buff, sizeof(buff), elog_user_output1);
    // elog_set_global_level(LOG_INFO);
    elog_add_terminal("lux1", LOG_NOTICE, elog_user_output2);
    
    ELOG_TRACE("hello world1");
    ELOG_DEBUG("hello world2");
    ELOG_INFO("hello world3"); 
    ELOG_NOTICE("hello world4");
    ELOG_WARN("hello world5"); 
    ELOG_ERROR("hello world6");
    
    ELOG_MDU_WARN("lux1","hello world7");

    return 0;      
}


