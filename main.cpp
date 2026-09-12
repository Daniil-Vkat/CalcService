#include "libmath.h"
#include <cstdio>
#include <cstdlib> 
#include <cstring> 

bool parseNumber(const char* s, long long& out) {
    if (s == nullptr || s[0] == '\0') return false;
    
    char* end = nullptr;
    long long v = strtoll(s, &end, 10);
    
    if (end == s) return false;
    
    if (*end == '\0') {
        out = v;
        return true;
    }
    
    return false; 
}

bool parseNumberWithTrailingFactorial(const char* s, long long& out) {
    if (s == nullptr || s[0] == '\0') return false;
    
    size_t len = strlen(s);
    if (len < 1) return false;

    if (s[len - 1] == '!') {
        char buf[256];
        if (len - 1 >= sizeof(buf)) return false; // Защита от переполнения
        
        memcpy(buf, s, len - 1);
        buf[len - 1] = '\0';
        
        return parseNumber(buf, out);
    }
    
    return parseNumber(s, out);
}
 
bool parse(int argc, char** argv, Task& task) {
    if (argc == 4) {
        if (!parseNumber(argv[1], task.value1)) {
            task.status = Status::InvalidNumber;
            return false;
        }
        
        if (strlen(argv[2]) != 1) {
            task.status = Status::UnknownOperation;
            return false;
        }
        task.operation = argv[2][0];
        
        if (!parseNumber(argv[3], task.value2)) {
            task.status = Status::InvalidNumber;
            return false;
        }
        return true;
    }
    
    if (argc == 2) {
        size_t len = strlen(argv[1]);
        if (len > 0 && argv[1][len - 1] == '!') {
            if (!parseNumberWithTrailingFactorial(argv[1], task.value1)) {
                task.status = Status::InvalidNumber;
                return false;
            }
            task.operation = '!';
            task.value2 = 0; 
            return true;
        }
        
        task.status = Status::BadArgumentCount;
        return false;
    }

    task.status = Status::BadArgumentCount;
    return false;
}

void output(const Task& task) {
    switch (task.status) {
        case Status::Ok:
            if (task.operation == '!')
                printf("%lld! = %lld\n", (long long)task.value1, (long long)task.result);
            else
                printf("%lld %c %lld = %lld\n", (long long)task.value1, task.operation, (long long)task.value2, (long long)task.result);
            break;
        case Status::DivisionByZero:
            printf("Error! Division by zero!\n"); break;
        case Status::UnknownOperation:
            printf("Error! Unknown operation!\n"); break;
        case Status::BadArgumentCount:
            printf("Error! Usage: <num1> <op> <num2> OR <num1>!\n"); break;
        case Status::InvalidNumber:
            printf("Error! Invalid number!\n"); break;
        case Status::Overflow:
            printf("Error! Overflow!\n"); break;
        case Status::NegativeExponent:
            printf("Error! Negative exponent is not supported!\n"); break;
        case Status::NegativeFactorial:
            printf("Error! Factorial of a negative number is undefined!\n"); break;
        default:
            printf("Error! Unknown error.\n"); break;
    }
}
 
// ---------- Запуск ----------
 
void run(int argc, char** argv) {
    Task task;
    if (parse(argc, argv, task)) {
        calculate(task);
    }
    output(task);
}

int main(int argc, char** argv) {
    run(argc, argv);
    return 0;
}
