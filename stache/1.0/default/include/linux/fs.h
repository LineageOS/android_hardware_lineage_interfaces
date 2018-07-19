#include <stdint.h>
#include <unistd.h>

#define FS_MAX_KEY_SIZE	64

struct fscrypt_key {
    uint32_t mode;
    uint8_t raw[FS_MAX_KEY_SIZE];
    uint32_t size;
} __attribute__((packed));

#define FS_KEY_DESCRIPTOR_SIZE		8
#define FS_KEY_DESCRIPTOR_HEX_SIZE	((2 * FS_KEY_DESCRIPTOR_SIZE) + 1)

// Amount of padding
#define FS_POLICY_FLAGS_PAD_4		0x00
#define FS_POLICY_FLAGS_PAD_8		0x01
#define FS_POLICY_FLAGS_PAD_16		0x02
#define FS_POLICY_FLAGS_PAD_32		0x03
#define FS_POLICY_FLAGS_PAD_MASK	0x03

// Encryption algorithms
#define FS_ENCRYPTION_MODE_INVALID	0
#define FS_ENCRYPTION_MODE_AES_256_XTS	1
#define FS_ENCRYPTION_MODE_AES_256_GCM	2
#define FS_ENCRYPTION_MODE_AES_256_CBC	3
#define FS_ENCRYPTION_MODE_AES_256_CTS	4
#define FS_ENCRYPTION_MODE_AES_128_CBC	5
#define FS_ENCRYPTION_MODE_AES_128_CTS	6

// Policy provided via an ioctl on the topmost directory
struct fscrypt_policy {
    uint8_t version;
    uint8_t contents_encryption_mode;
    uint8_t filenames_encryption_mode;
    uint8_t flags;
    uint8_t master_key_descriptor[FS_KEY_DESCRIPTOR_SIZE];
} __attribute__((packed));

#define FS_IOC_SET_ENCRYPTION_POLICY _IOR('f', 19, struct fscrypt_policy)
#define FS_IOC_GET_ENCRYPTION_POLICY _IOW('f', 21, struct fscrypt_policy)

// Service prefixes for encryption keys
#define FS_KEY_DESC_PREFIX		"fscrypt:"
#define EXT4_KEY_DESC_PREFIX		"ext4:"  // For ext4 before 4.8 kernel
#define F2FS_KEY_DESC_PREFIX		"f2fs:"  // For f2fs before 4.6 kernel
#define MAX_KEY_DESC_PREFIX_SIZE	8
