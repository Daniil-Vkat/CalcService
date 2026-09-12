#include "libmath.h" // Предполагаемый заголовок из репозитория calculat
#include <iostream>
using namespace math;
bool parseNumber(const std::string& s, long long& out) {
    try {
        size_t pos = 0;
        long long v = std::stoll(s, &pos);
        if (pos != s.size()) return false;
        out = v;
        return true;
    } catch (...) {
        return false;
    }
}
 
bool parse(int argc, char** argv, Task& task) {
    if (argc != 4) {
        task.status = Status::BadArgumentCount;
        return false;
    }
    if (!parseNumber(argv[1], task.value1)) {
        task.status = Status::InvalidNumber;
        return false;
    }
    if (argv[2][0] == '\0') {
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
void output(const Task& task) {
    switch (task.status) {
        case Status::Ok:
            if (task.operation == '!')
                std::cout << task.value1 << "! = " << task.result << '\n';
            else
                std::cout << task.value1 << ' ' << task.operation << ' '
                          << task.value2 << " = " << task.result << '\n';
            break;
        case Status::DivisionByZero:
            std::cout << "Error! Division by zero!\n"; break;
        case Status::UnknownOperation:
            std::cout << "Error! Unknown operation!\n"; break;
        case Status::BadArgumentCount:
            std::cout << "Error! Usage: <num1> <op> <num2>\n"; break;
        case Status::InvalidNumber:
            std::cout << "Error! Invalid number!\n"; break;
        case Status::Overflow:
            std::cout << "Error! Overflow!\n"; break;
        case Status::NegativeExponent:
            std::cout << "Error! Negative exponent is not supported!\n"; break;
        case Status::NegativeFactorial:
            std::cout << "Error! Factorial of a negative number is undefined!\n"; break;
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

