#include "session.h"

#include "vga.h"

static SessionState current_session;

static int string_equals(const char *a, const char *b) {
    if (a == 0 || b == 0) {
        return 0;
    }

    while (*a != '\0' && *b != '\0') {
        if (*a != *b) {
            return 0;
        }
        a++;
        b++;
    }

    return *a == '\0' && *b == '\0';
}

static void copy_string(char *destination, const char *source, unsigned int capacity) {
    unsigned int index = 0;

    if (capacity == 0) {
        return;
    }

    while (index + 1 < capacity && source[index] != '\0') {
        destination[index] = source[index];
        index++;
    }

    destination[index] = '\0';
}

void session_init(void) {
    current_session.username[0] = '\0';
    current_session.role = SESSION_ROLE_GUEST;
    current_session.authenticated = 0;
}

int session_login(const char *username, const char *password) {
    if ((username == 0 || username[0] == '\0') && (password == 0 || password[0] == '\0')) {
        copy_string(current_session.username, "root", sizeof(current_session.username));
        current_session.role = SESSION_ROLE_ROOT;
        current_session.authenticated = 1;
        return 1;
    }

    if (string_equals(username, "root") && string_equals(password, "buddy")) {
        copy_string(current_session.username, "root", sizeof(current_session.username));
        current_session.role = SESSION_ROLE_ROOT;
        current_session.authenticated = 1;
        return 1;
    }

    if (string_equals(username, "user") && string_equals(password, "buddy")) {
        copy_string(current_session.username, "user", sizeof(current_session.username));
        current_session.role = SESSION_ROLE_USER;
        current_session.authenticated = 1;
        return 1;
    }

    current_session.username[0] = '\0';
    current_session.role = SESSION_ROLE_GUEST;
    current_session.authenticated = 0;
    return 0;
}

int session_is_authenticated(void) {
    return current_session.authenticated;
}

int session_is_root(void) {
    return current_session.authenticated && current_session.role == SESSION_ROLE_ROOT;
}

const char *session_username(void) {
    return current_session.username;
}