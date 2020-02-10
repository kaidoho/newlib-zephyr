/*
 *  Fake crt0 for Zephyr OS
 *
 *  Zephyr OS provides its own crt0 and linker scripts.  Unfortunately
 *  this means that crt0 and the linker script are not available as
 *  each tool is configured.  Without a crt0 and linker script, some
 *  targets do not successfully link "conftest.c" during the configuration 
 *  process.  So this fake crt0.c provides all the symbols required to
 *  successfully link a program.  The resulting program will not run
 *  but this is enough to satisfy the autoconf macro AC_PROG_CC.
 */

#include <sys/lock.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <reent.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <machine/_libatomic.h>

void zephyr_provides_crt0( void ) {}  /* dummy symbol so file always has one */

#define ZEPHYR_STUB(ret, func, body) \
ret zephyr_stub_##func body; \
ret func body


/* Stubs for routines from Zephyr OS <sys/lock.h> */
ZEPHYR_STUB(int,  _z_gthread_yield(void), { return -1; })


/* Stubs for routines from RTEMS <machine/_libatomic.h> */
ZEPHYR_STUB(__uint32_t, _z_libatomic_protect_start(void *ptr), { return 0; });
ZEPHYR_STUB(void, _z_libatomic_protect_end(void *ptr, __uint32_t isr_level), { });
ZEPHYR_STUB(void, _z_libatomic_lock_n(void *ptr, __size_t n), { });
ZEPHYR_STUB(void, _z_libatomic_unlock_n(void *ptr, __size_t n), { });


#if defined(__GNUC__)
/*
 * stubs for libstdc++ Zephyr OS support functions from gcc/gthr-zephyr.h
 */
int rtems_gxx_once() { return -1; }
int rtems_gxx_key_create() { return -1; }
int rtems_gxx_key_delete() { return -1; }
void *rtems_gxx_getspecific() { return 0; }
int rtems_gxx_setspecific() { return -1; }

void rtems_gxx_mutex_init() { }
int rtems_gxx_mutex_lock() { return -1; }
int rtems_gxx_mutex_trylock() { return -1; }
int rtems_gxx_mutex_unlock() { return -1; }

void rtems_gxx_recursive_mutex_init() { }
int rtems_gxx_recursive_mutex_lock() { return -1; }
int rtems_gxx_recursive_mutex_trylock() { return -1; }
int rtems_gxx_recursive_mutex_unlock() { return -1; }
#endif


ZEPHYR_STUB(void, _exit(int status), { while(1); })


#if defined(__arm__)
ZEPHYR_STUB(void, __aeabi_read_tp(void), { })
#endif

ZEPHYR_STUB(void *, __tls_get_addr(const void *ti), { })

