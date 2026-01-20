#include "password.h"
#include <stdio.h>

password_t stored;  // 실제로는 Flash에 저장

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_USART1_UART_Init();
    
    // === 초기 설정 (최초 1회) ===
    printf("비밀번호 설정...\n");
    password_set("hello123", &stored);
    printf("완료!\n\n");
    
    // === 인증 테스트 ===
    printf("테스트 1: 올바른 비밀번호\n");
    if (password_check("hello123", &stored)) {
        printf("✓ 성공!\n\n");
    } else {
        printf("✗ 실패\n\n");
    }
    
    printf("테스트 2: 틀린 비밀번호\n");
    if (password_check("wrong", &stored)) {
        printf("✓ 성공!\n\n");
    } else {
        printf("✗ 실패 (정상)\n\n");
    }
    
    while (1) {
    }
}