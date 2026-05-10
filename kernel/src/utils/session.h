#ifndef SESSION_H
#define SESSION_H

typedef enum {
    SESSION_ROLE_GUEST = 0,
    SESSION_ROLE_USER = 1,
    SESSION_ROLE_ROOT = 2,
} SessionRole;

typedef struct {
    char username[32];
    SessionRole role;
    int authenticated;
} SessionState;

void session_init(void);
int session_login(const char *username, const char *password);
int session_is_authenticated(void);
int session_is_root(void);
const char *session_username(void);

#endif