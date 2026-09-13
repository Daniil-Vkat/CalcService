#include "libmath.h"
#include <cstdio>
#include <cstdlib> // for strtoll
#include <cstring> // for strlen
#include <unistd.h> // for getopt

using namespace math;

bool parseNumber(const char* s, long long& out) {
    if (s == nullptr || s[0] == '\0') return false;
    
    char* end = nullptr;
    long long v = strtoll(s, &end, 10);
    
    if (end == s || *end != '\0') return false;
    
    out = v;
    return true;
}

void printUsage() {
    printf("Usage:\n");
    printf("  ./calc <num1> <op> <num2>   Calculate expression (e.g., 5 + 3)\n");
    printf("  ./calc <num>!               Calculate factorial (e.g., 5!)\n");
    printf("  ./calc -h                   Show this help message\n");
}
 
bool parse(int argc, char** argv, Task& task) {
    optind = 1;
    opterr = 0;

    int opt;
    while ((opt = getopt(argc, argv, "+h")) != -1) {   // '+' — не переставлять argv, стоп на первом не-опционном
        if (opt == 'h') {
            printUsage();
            task.status = Status::Ok;
            return false;
        }
        task.status = Status::UnknownOperation;          // неизвестный флаг — лучше сообщить, чем проглотить
        return false;
    }

    int remaining_argc = argc - optind;
    char** remaining_argv = argv + optind;

    switch (remaining_argc) {
    case 3: {  // <num1> <op> <num2>
        if (!parseNumber(remaining_argv[0], task.value1)) { task.status = Status::InvalidNumber; return false; }
        if (strlen(remaining_argv[1]) != 1)                { task.status = Status::UnknownOperation; return false; }
        task.operation = remaining_argv[1][0];
        if (!parseNumber(remaining_argv[2], task.value2)) { task.status = Status::InvalidNumber; return false; }
        return true;
    }
    case 2: {  // <num> !
        if (strcmp(remaining_argv[1], "!") != 0)           { task.status = Status::BadArgumentCount; return false; }
        if (!parseNumber(remaining_argv[0], task.value1)) { task.status = Status::InvalidNumber; return false; }
        task.operation = '!';
        task.value2 = 0;
        return true;
    }
    case 1: {  // <num>!
        size_t len = strlen(remaining_argv[0]);
        if (len < 2 || remaining_argv[0][len - 1] != '!') { task.status = Status::BadArgumentCount; return false; }
        char buf[64];
        if (len - 1 >= sizeof(buf))                        { task.status = Status::InvalidNumber; return false; }
        memcpy(buf, remaining_argv[0], len - 1);
        buf[len - 1] = '\0';
        if (!parseNumber(buf, task.value1))                { task.status = Status::InvalidNumber; return false; }
        task.operation = '!';
        task.value2 = 0;
        return true;
    }
    default:
        task.status = Status::BadArgumentCount;
        return false;
    }
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
            printf("Error! Bad argument count.\n"); break;
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
