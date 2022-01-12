# elog

## 简介

elog 是一个用于嵌入式设备的微小 log 输出插件，代码量极小，非常适合用于资源受限的嵌入式设备，当然也同样支持多线程设备。



## 使用方法

1. 注册并初始化 elog，在初始化的同时会自动创建一个默认输出终端，默认名称 “TERM”，默认输出所有日志，日志输出等级为 LOG_TRACE；

```c
elog_init(LOG_TRACE, buff, sizeof(buff), elog_user_output1);
```

2. 设置全局 log 输出等级，大于这个等级的日志输出项，将不会被输出到显示终端；

```c
elog_set_global_level(LOG_INFO);
```

3. 你也可以添加新的日志打印模块，目前最大支持 8 个，可以通过宏来修改个数；

```c
elog_add_terminal("lux1", LOG_NOTICE, elog_user_output2);
```

4. 同时支持指定模块输出；

```c
    ELOG_WARN("hello world5"); 
    ELOG_MDU_WARN("lux1","hello world7");
```



## Examples

```c
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
    elog_set_global_level(LOG_INFO);
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
```