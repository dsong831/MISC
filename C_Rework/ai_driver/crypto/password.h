#ifndef PASSWORD_H
#define PASSWORD_H

#include <stdint.h>
#include <stdbool.h>

// Flash에 저장할 구조체
typedef struct {
    uint8_t salt[16];
    uint8_t hash[32];
} password_t;

// 비밀번호 설정
void password_set(const char *pwd, password_t *cred);

// 비밀번호 확인
bool password_check(const char *pwd, const password_t *cred);

#endif