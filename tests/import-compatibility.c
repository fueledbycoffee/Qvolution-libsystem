#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <spawn.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    assert(argc == 2);
    void *library = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
    if (!library) { fprintf(stderr, "%s\n", dlerror()); return 1; }
    pid_t (*responsible)(pid_t) = dlsym(library, "responsibility_get_pid_responsible_for_pid");
    int (*disclaim)(posix_spawnattr_t *, int) = dlsym(library, "responsibility_spawnattrs_setdisclaim");
    char *(*issue)(const char *, const char *, uint32_t) = dlsym(library, "sandbox_extension_issue_file");
    int64_t (*consume)(const char *) = dlsym(library, "sandbox_extension_consume");
    int (*release)(int64_t) = dlsym(library, "sandbox_extension_release");
    void (*trace)(void *, const char *, uint8_t, const void *, size_t, void (^)(void *)) = dlsym(library, "_os_trace_with_buffer");
    int (*addchdir)(posix_spawn_file_actions_t *, const char *) = dlsym(library, "posix_spawn_file_actions_addchdir");
    assert(responsible && disclaim && issue && consume && release && trace && addchdir);
    assert(addchdir(NULL, "/") == EINVAL);
    posix_spawn_file_actions_t actions;
    assert(posix_spawn_file_actions_init(&actions) == 0);
    assert(addchdir(&actions, "/") == 0);
    assert(posix_spawn_file_actions_destroy(&actions) == 0);
    errno = 0;
    assert(responsible(123) == -1 && errno == ENOTSUP);
    posix_spawnattr_t attributes, before;
    memset(&attributes, 0xa5, sizeof(attributes));
    memcpy(&before, &attributes, sizeof(before));
    assert(disclaim(&attributes, 1) == ENOTSUP);
    assert(memcmp(&attributes, &before, sizeof(before)) == 0);
    assert(disclaim(NULL, 0) == ENOTSUP);
    errno = 0;
    assert(issue("com.apple.app-sandbox.read", "/tmp", 0) == NULL && errno == ENOTSUP);
    errno = 0;
    assert(consume("invalid") == -1 && errno == ENOTSUP);
    errno = 0;
    assert(release(1) == -1 && errno == ENOTSUP);
    errno = EDOM;
    trace(NULL, "ignored", 0, NULL, 0, NULL);
    assert(errno == EDOM);
    dlclose(library);
    puts("PASS libSystem compatibility: 7 exports, errors, attributes, errno");
    return 0;
}
