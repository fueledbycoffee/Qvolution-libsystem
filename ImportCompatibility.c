#include <sys/types.h>
#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <spawn.h>

/* GUESS: private responsibility ABI, names from the Spotify import audit.
 * No responsibility service exists, so do not invent a responsible process. */
pid_t responsibility_get_pid_responsible_for_pid(pid_t pid)
{
    errno = ENOTSUP;
    return -1;
}

int responsibility_spawnattrs_setdisclaim(posix_spawnattr_t *attributes, int disclaim)
{
    return ENOTSUP;
}

/* Signatures: vendored WTF/wtf/spi/darwin/SandboxSPI.h. */
char *sandbox_extension_issue_file(const char *extension_class, const char *path, uint32_t flags)
{
    errno = ENOTSUP;
    return NULL;
}

int64_t sandbox_extension_consume(const char *token)
{
    errno = ENOTSUP;
    return -1;
}

int sandbox_extension_release(int64_t handle)
{
    errno = ENOTSUP;
    return -1;
}

/* xnu/libkern/os/trace.h: pre-encoded diagnostic event. There is no trace
 * collector; dropping telemetry must not change the caller's errno. */
void _os_trace_with_buffer(void *dso, const char *message, uint8_t type,
    const void *buffer, size_t buffer_size, void (^payload)(void *))
{
}

/* xnu's published spawn wrapper already implements the same file action. */
extern int posix_spawn_file_actions_addchdir_np(posix_spawn_file_actions_t *, const char *);
int posix_spawn_file_actions_addchdir(posix_spawn_file_actions_t *actions, const char *path)
{
    if (!actions || !path) return EINVAL;
    return posix_spawn_file_actions_addchdir_np(actions, path);
}
