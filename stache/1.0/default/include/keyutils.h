#include <sys/syscall.h>

typedef int32_t key_serial_t;

#define KEYCTL_GET_KEYRING_ID		0	/* ask for a keyring's ID */
#define KEY_SPEC_SESSION_KEYRING	-3	/* current session keyring */

key_serial_t add_key(const char *type, const char *description,
                     const void *payload, size_t plen, key_serial_t ringid) {
    return syscall(__NR_add_key, type, description, payload, plen, ringid);
}

key_serial_t keyctl_get_keyring_ID(key_serial_t id, int create) {
    return syscall(__NR_keyctl, KEYCTL_GET_KEYRING_ID, id, create);
}
