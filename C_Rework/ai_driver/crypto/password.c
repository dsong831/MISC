#include "password.h"
#include "sha256.h"
#include <string.h>

// 간단한 랜덤 생성 (실제로는 RNG 사용!)
static void make_salt(uint8_t *salt)
{
    extern uint32_t HAL_GetTick(void);
    uint32_t tick = HAL_GetTick();
    
    for (int i = 0; i < 16; i++) {
        salt[i] = (tick >> (i % 4 * 8)) ^ i;
        tick = tick * 1103515245 + 12345;  // LCG
    }
}

// 비밀번호 설정
void password_set(const char *pwd, password_t *cred)
{
    struct tc_sha256_state_struct s;
    
    // Salt 생성
    make_salt(cred->salt);
    
    // SHA-256 계산
    tc_sha256_init(&s);
    tc_sha256_update(&s, (const uint8_t *)pwd, strlen(pwd));
    tc_sha256_update(&s, cred->salt, 16);
    tc_sha256_final(cred->hash, &s);
}

// 비밀번호 확인
bool password_check(const char *pwd, const password_t *cred)
{
    struct tc_sha256_state_struct s;
    uint8_t hash[32];
    
    // 같은 방식으로 계산
    tc_sha256_init(&s);
    tc_sha256_update(&s, (const uint8_t *)pwd, strlen(pwd));
    tc_sha256_update(&s, cred->salt, 16);
    tc_sha256_final(hash, &s);
    
    // 비교 (타이밍 공격 방지)
    uint8_t diff = 0;
    for (int i = 0; i < 32; i++) {
        diff |= hash[i] ^ cred->hash[i];
    }
    
    return (diff == 0);
}