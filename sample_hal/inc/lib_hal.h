/*# Copyright (C) 2021 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
*/
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "pthread.h"
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <log/log.h>
#include <dlfcn.h>
#include "platform_common.h"

#define LIB_VERSION "version:1.0.0.0"
#define if_err(a, b, fmt, ...); if(a) { ALOGE("%s %d" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); b}
#define PREFIX(s) f_##s
#define DLSYM(f) dlsym(dev->handle, #f); \
        if (dlerror() != NULL){ \
            ALOGE("dlsym err %s", #f); \
            return GENERIC_ERR; \
        }

struct tee_client_device
{
    int (*tee_init)(void);
    int (*tee_cmd)(struct tee_client_device *, struct tee_in_buf *, struct tee_out_buf *);
    void (*tee_exit)(struct tee_client_device *);
    pthread_mutex_t mutex;
    void *handle;
};

struct tee_performance_record
{
    uint32_t cmd_run_times;
    uint32_t cmd_cost_max_time;
    uint32_t cmd_cost_total_time;
};

int gp_client_open(struct tee_client_device *dev);
int qsee_client_open(struct tee_client_device *dev);
int isee_client_open(struct tee_client_device *dev);
int trusty_client_open(struct tee_client_device *dev);
int kinibi_client_open(struct tee_client_device *dev);