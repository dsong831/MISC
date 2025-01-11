#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// 상수 정의
#define ARRAY_SIZE 400
#define TOTAL_ARRAY_SIZE (ARRAY_SIZE + 1)  // 오염된 배열 포함
#define MAX_NUMBER 399
#define INVALID_INDEX 0xFFFFFFFF

// 오염 타입 열거형
typedef enum {
    CORRUPTION_CHECKSUM_MISMATCH = 1,    // checksum과 number가 불일치
    CORRUPTION_DUPLICATE_NUMBER,          // 중복된 number
    CORRUPTION_INVALID_NUMBER_MATCH,      // 유효범위 초과 number (checksum 일치)
    CORRUPTION_INVALID_NUMBER_MISMATCH    // 유효범위 초과 number (checksum 불일치)
} CorruptionType;

// 배열 요소 구조체
typedef struct {
    uint16_t number;    // 0~399 범위의 숫자
    uint16_t checksum;  // number와 동일해야 하는 검증값
} ArrayElement;

// 검증 결과 구조체
typedef struct {
    bool isCorrupted;           // 오염 여부
    uint32_t corruptedIndex;    // 오염된 위치
    CorruptionType type;        // 오염 타입
    int startIndex;             // 0번 number가 시작되는 인덱스
} ValidationResult;

// 함수 프로토타입
static bool isNumberValid(uint16_t number);
static bool isChecksumValid(const ArrayElement* element);
static int findStartIndex(const ArrayElement* array);
static ValidationResult validateArray(const ArrayElement* array);
static void copyValidArraySorted(const ArrayElement* source, ArrayElement* dest, int startIndex);

/**
 * number가 유효한 범위 내에 있는지 검사
 */
static bool isNumberValid(uint16_t number) {
    return number <= MAX_NUMBER;
}

/**
 * checksum이 number와 일치하는지 검사
 */
static bool isChecksumValid(const ArrayElement* element) {
    return element->number == element->checksum;
}

/**
 * 0번 number가 시작되는 인덱스 찾기
 * @return 시작 인덱스 또는 실패시 -1
 */
static int findStartIndex(const ArrayElement* array) {
    for (int i = 0; i < TOTAL_ARRAY_SIZE; i++) {
        if (array[i].number == 0 && isChecksumValid(&array[i])) {
            return i;
        }
    }
    return -1;
}

/**
 * 배열의 유효성 검사 수행
 */
static ValidationResult validateArray(const ArrayElement* array) {
    ValidationResult result = {false, INVALID_INDEX, 0, -1};
    bool numberUsed[ARRAY_SIZE] = {false};
    
    // 1. 기본 유효성 검사
    for (int i = 0; i < TOTAL_ARRAY_SIZE; i++) {
        // number 범위 검사
        if (!isNumberValid(array[i].number)) {
            result.isCorrupted = true;
            result.corruptedIndex = i;
            result.type = isChecksumValid(&array[i]) ? 
                         CORRUPTION_INVALID_NUMBER_MATCH : 
                         CORRUPTION_INVALID_NUMBER_MISMATCH;
            return result;
        }
        
        // checksum 검사
        if (!isChecksumValid(&array[i])) {
            result.isCorrupted = true;
            result.corruptedIndex = i;
            result.type = CORRUPTION_CHECKSUM_MISMATCH;
            return result;
        }
    }
    
    // 2. 순차 검사
    result.startIndex = findStartIndex(array);
    if (result.startIndex == -1) {
        result.isCorrupted = true;
        return result;
    }
    
    int currentIdx = result.startIndex;
    int expectedNumber = 0;
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (numberUsed[array[currentIdx].number]) {
            result.isCorrupted = true;
            result.corruptedIndex = currentIdx;
            result.type = CORRUPTION_DUPLICATE_NUMBER;
            return result;
        }
        
        if (array[currentIdx].number != expectedNumber) {
            result.isCorrupted = true;
            result.corruptedIndex = currentIdx;
            result.type = CORRUPTION_CHECKSUM_MISMATCH;
            return result;
        }
        
        numberUsed[array[currentIdx].number] = true;
        expectedNumber++;
        currentIdx = (currentIdx + 1) % TOTAL_ARRAY_SIZE;
    }
    
    return result;
}

/**
 * 정상 배열을 number 순서대로 정렬하여 복사
 * number가 0인 데이터부터 시작하여 오름차순으로 저장
 */
static void copyValidArraySorted(const ArrayElement* source, ArrayElement* dest) {
    // number 값을 인덱스로 사용하여 정렬된 형태로 직접 저장
    ArrayElement tempArray[ARRAY_SIZE];
    int currentIdx = 0;
    
    // source 배열에서 유효한 데이터를 number를 인덱스로 하여 임시 배열에 저장
    for (int i = 0; i < TOTAL_ARRAY_SIZE; i++) {
        if (isNumberValid(source[i].number) && isChecksumValid(&source[i])) {
            tempArray[source[i].number] = source[i];
        }
    }
    
    // 임시 배열에서 dest 배열로 순차적으로 복사
    memcpy(dest, tempArray, sizeof(ArrayElement) * ARRAY_SIZE);
}

/**
 * 오염된 배열을 찾고 정상 배열 복사
 * @param source 검사할 원본 배열 (401개 요소)
 * @param destination 복사될 정상 배열 (400개 요소)
 * @return 검증 결과
 */
ValidationResult processArray(const ArrayElement* source, ArrayElement* destination) {
    ValidationResult result = validateArray(source);
    
    if (!result.isCorrupted) {
        copyValidArraySorted(source, destination);
    }
    
    return result;
}

/**
 * 테스트용 배열 초기화
 */
void initializeTestArray(ArrayElement* array, int startIndex, int corruptedIndex, 
                        CorruptionType corruptionType) {
    // 정상 데이터 초기화
    for (int i = 0; i < TOTAL_ARRAY_SIZE; i++) {
        int numberValue = ((i - startIndex + TOTAL_ARRAY_SIZE) % TOTAL_ARRAY_SIZE) % ARRAY_SIZE;
        array[i].number = numberValue;
        array[i].checksum = numberValue;
    }
    
    // 오염 데이터 삽입
    switch (corruptionType) {
        case CORRUPTION_CHECKSUM_MISMATCH:
            array[corruptedIndex].checksum = (array[corruptedIndex].number + 1) % ARRAY_SIZE;
            break;
            
        case CORRUPTION_DUPLICATE_NUMBER:
            array[corruptedIndex].number = array[(corruptedIndex + ARRAY_SIZE) % TOTAL_ARRAY_SIZE].number;
            array[corruptedIndex].checksum = array[corruptedIndex].number;
            break;
            
        case CORRUPTION_INVALID_NUMBER_MATCH:
            array[corruptedIndex].number = MAX_NUMBER + 100;
            array[corruptedIndex].checksum = array[corruptedIndex].number;
            break;
            
        case CORRUPTION_INVALID_NUMBER_MISMATCH:
            array[corruptedIndex].number = MAX_NUMBER + 100;
            array[corruptedIndex].checksum = array[corruptedIndex].number + 1;
            break;
    }
}

/**
 * 배열 내용 출력 (디버깅용)
 */
void printArray(const ArrayElement* array, int size) {
    for (int i = 0; i < size; i++) {
        printf("Index %d: number = %d, checksum = %d\n", 
               i, array[i].number, array[i].checksum);
    }
}