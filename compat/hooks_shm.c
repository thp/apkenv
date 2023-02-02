/*
 * Copyright (c) 2012 Carsten Munk <carsten.munk@gmail.com>
 * Copyright (c) 2013 Christophe Chapuis <chris.chapuis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "hooks_shm.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>

#ifdef APKENV_DEBUG
#  define SHM_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#  define SHM_DEBUG_PRINTF(...)
#endif

/* Debug */
#define LOGD(message, ...) SHM_DEBUG_PRINTF(message "\n", ##__VA_ARGS__)

#define APKENV_DATA_SIZE    4000
#define APKENV_SHM_MASK     0xFF000000UL
#define APKENV_SHM_PATH     "/apkenv_shm_data"

/* Structure of a shared memory region */
typedef struct _apkenv_shm_data_t {
    pthread_mutex_t access_mutex;
    int current_offset;
    int max_offset;
    unsigned char data;
} apkenv_shm_data_t;

/* A helper to switch between the size of the data and the size of the shm object */
const int APKENV_SHM_DATA_HEADER_SIZE = (sizeof(apkenv_shm_data_t) - sizeof(unsigned char));

/* pointer to the shared memory region */
static apkenv_shm_data_t *_apkenv_shm_data = NULL;

/* the SHM mem_id of the shared memory region */
static int _apkenv_shm_fd = -1;

/* the size of the shared memory region that is currently mmap'ed to this process */
static int apkenv__current_mapped_size = 0;

/* forward-declare the internal static methods */
static void apkenv__release_shm(void);
static void apkenv__sync_mmap_with_shm(void);
static void _apkenv_shm_init(void);
static void _apkenv_shm_extend_region(void);

/*
 * Detach the allocated memory region, and mark it for deletion
 */
static void apkenv__release_shm(void)
{
    if (_apkenv_shm_data) {
        munmap(_apkenv_shm_data, apkenv__current_mapped_size); /* unmap from this process */
        _apkenv_shm_data = NULL; /* pointer is no more valid */
    }
    if (_apkenv_shm_fd >= 0) {
        close(_apkenv_shm_fd);   /* close the shm file descriptor */
        _apkenv_shm_fd = -1;
    }
    shm_unlink(APKENV_SHM_PATH);  /* request the deletion of the shm region */
}

/*
 * Synchronize the size of the mmap with the size of the shm region
 */
static void apkenv__sync_mmap_with_shm()
{
    if (_apkenv_shm_fd >= 0 && _apkenv_shm_data) {
        if (apkenv__current_mapped_size < _apkenv_shm_data->max_offset + APKENV_SHM_DATA_HEADER_SIZE) {
            /* Note that mremap may change the address pointed by _apkenv_shm_data.
             * But as we never point directly into _apkenv_shm_data, it's fine.
             * */
            _apkenv_shm_data = (apkenv_shm_data_t *)mremap( _apkenv_shm_data, apkenv__current_mapped_size,
                                  _apkenv_shm_data->max_offset + APKENV_SHM_DATA_HEADER_SIZE,
                                  MREMAP_MAYMOVE );

            apkenv__current_mapped_size = _apkenv_shm_data->max_offset + APKENV_SHM_DATA_HEADER_SIZE;
        }
    }
}

/*
 * Initialize the shared memory region for hybris, in order to store
 * pshared mutex, condition and rwlock
 */
static void _apkenv_shm_init()
{
    if (_apkenv_shm_fd < 0) {
        const size_t size_to_map = APKENV_SHM_DATA_HEADER_SIZE + APKENV_DATA_SIZE; /* 4000 bytes for the data, plus the header size */

        /* initialize or get shared memory segment */
        _apkenv_shm_fd = shm_open(APKENV_SHM_PATH, O_RDWR, 0660);
        if (_apkenv_shm_fd >= 0) {
            /* Map the memory object */
            _apkenv_shm_data = (apkenv_shm_data_t *)mmap( NULL, size_to_map,
                                             PROT_READ | PROT_WRITE, MAP_SHARED,
                                             _apkenv_shm_fd, 0 );
            apkenv__current_mapped_size = size_to_map;

            apkenv__sync_mmap_with_shm();
        }
        else {
            LOGD("Creating a new shared memory segment.");

            _apkenv_shm_fd = shm_open(APKENV_SHM_PATH, O_RDWR | O_CREAT, 0660);
            if (_apkenv_shm_fd >= 0) {
                if (ftruncate( _apkenv_shm_fd, size_to_map ) != 0) {
                    fprintf(stderr, "Could not resize shared memory.\n");
                    exit(1);
                }
                /* Map the memory object */
                _apkenv_shm_data = (apkenv_shm_data_t *)mmap( NULL, size_to_map,
                                             PROT_READ | PROT_WRITE, MAP_SHARED,
                                             _apkenv_shm_fd, 0 );
                if (_apkenv_shm_data == MAP_FAILED) {
                    printf("ERROR: mmap failed: %s\n", strerror(errno));
                    apkenv__release_shm();
                }
                else {
                    apkenv__current_mapped_size = size_to_map;
                    /* Initialize the memory object */
                    memset((void*)_apkenv_shm_data, 0, size_to_map);
                    _apkenv_shm_data->max_offset = APKENV_DATA_SIZE;

                    pthread_mutexattr_t attr;
                    pthread_mutexattr_init(&attr);
                    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
                    pthread_mutex_init(&_apkenv_shm_data->access_mutex, &attr);
                    pthread_mutexattr_destroy(&attr);

                    atexit(apkenv__release_shm);
                }
            }
            else {
                printf("ERROR: Couldn't create shared memory segment !");
            }
        }
    }
}

/*
 * Extend the SHM region's size
 */
static void _apkenv_shm_extend_region()
{
    if (ftruncate( _apkenv_shm_fd, apkenv__current_mapped_size + APKENV_DATA_SIZE ) != 0) {
        fprintf(stderr, "Could not resize shared memory.\n");
        exit(1);
    }
    _apkenv_shm_data->max_offset += APKENV_DATA_SIZE;

    apkenv__sync_mmap_with_shm();
}

/************ public functions *******************/

 /*
  * Determine if the pointer that has been extracted by hybris is
  * pointing to an address in the shared memory
  */
int apkenv_is_pointer_in_shm(void *ptr)
{
    if ((unsigned int)ptr >= APKENV_SHM_MASK)
        return 1;

    return 0;
}

/*
 * Convert this offset pointer to the shared memory to an
 * absolute pointer that can be used in user space
 */
void *apkenv_get_shmpointer(apkenv_shm_pointer_t handle)
{
    void *realpointer = NULL;
    if (apkenv_is_pointer_in_shm((void*)handle)) {
        if (_apkenv_shm_fd < 0) {
            /* if we are not yet attached to any shm region, then do it now */
            _apkenv_shm_init();
        }

        pthread_mutex_lock(&_apkenv_shm_data->access_mutex);

        apkenv__sync_mmap_with_shm();  /* make sure our mmap is sync'ed */

        if (_apkenv_shm_data != NULL) {
            unsigned int offset = handle & (~APKENV_SHM_MASK);
            realpointer = &(_apkenv_shm_data->data) + offset;

            /* Be careful when activating this trace: this method is called *a lot* !
            LOGD("handle = %x, offset  = %d, realpointer = %x)", handle, offset, realpointer);
             */
        }

        pthread_mutex_unlock(&_apkenv_shm_data->access_mutex);
    }

    return realpointer;
}

/*
 * Allocate a space in the shared memory region of hybris
 */
apkenv_shm_pointer_t apkenv_shm_alloc(size_t size)
{
    apkenv_shm_pointer_t location = 0;

    if (_apkenv_shm_fd < 0) {
        /* if we are not yet attached to any shm region, then do it now */
        _apkenv_shm_init();
    }

    pthread_mutex_lock(&_apkenv_shm_data->access_mutex);

    /* Make sure our mmap is sync'ed */
    apkenv__sync_mmap_with_shm();

    if (_apkenv_shm_data == NULL || _apkenv_shm_fd < 0)
        return 0;

    if (_apkenv_shm_data->current_offset + size >= _apkenv_shm_data->max_offset) {
        /* the current buffer if full: extend it a little bit more */
        _apkenv_shm_extend_region();
        apkenv__sync_mmap_with_shm();
    }

    /* there is now enough place in this pool */
    location = _apkenv_shm_data->current_offset | APKENV_SHM_MASK;
    LOGD("Allocated a shared object (size = %d, at offset %d)", size, _apkenv_shm_data->current_offset);

    _apkenv_shm_data->current_offset += size;

    pthread_mutex_unlock(&_apkenv_shm_data->access_mutex);

    return location;
}
