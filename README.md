# CalcService

Консольный калькулятор на C++17 без использования STL (freestanding-like). Поддерживает базовые арифметические операции и факториал.

## Сборка

Для сборки проекта требуется:
- Компилятор C++ с поддержкой стандарта C++17 (GCC 8+, Clang 7+).
- Система сборки CMake (версия 3.14 или выше).

### Шаги сборки

1. Создайте директорию для сборки (из корня репозитория):
   ```bash
   mkdir -p build && cd build
   ```

2. Сконфигурируйте проект (с генерацией `compile_commands.json` для анализаторов):
   ```bash
   cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
   ```

3. Соберите исполняемый файл:
   ```bash
   make
   ```

Исполняемый файл [main](file:///home/daniil/CalcService/build/main) появится в папке `build`.

## Запуск

Программа принимает аргументы командной строки в позиционном формате.

### Синтаксис

```bash
./main <число1> <операция> <число2>
./main <число>!
```

### Поддерживаемые операции

| Операция | Описание | Пример |
| :--- | :--- | :--- |
| `+` | Сложение | `./main 5 + 3` |
| `-` | Вычитание | `./main 10 - 2` |
| `*` | Умножение | `./main 5 '*' 3` |
| `/` | Деление | `./main 10 / 2` |
| `^` | Возведение в степень | `./main 2 ^ 3` |
| `!` | Факториал (унарная) | `./main 5'!'` |

> **Важно:** При использовании в bash/zsh специальные символы (`*`, `!`) необходимо заключать в одинарные кавычки или экранировать обратным слэшем, чтобы оболочка не интерпретировала их самостоятельно.

### Коды ошибок

В случае ошибки программа выводит сообщение в `stderr` или `stdout` с префиксом `Error!`:
- `Division by zero` — деление на ноль.
- `Invalid number` — передан нечисловой аргумент.
- `Overflow` — результат не помещается в `long long`.
- `Unknown operation` — передан неподдерживаемый знак операции.

## Форматирование и статический анализ

В проекте настроены цели CMake для автоматического форматирования и статического анализа кода с помощью `clang-format` и `clang-tidy`.

### Установка инструментов

**Ubuntu/Debian:**
```bash
sudo apt install clang-format clang-tidy
```



# Обычная сборка — без линтеров
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Только форматирование
cmake -S . -B build -DENABLE_CLANG_FORMAT=ON
cmake --build build --target format          # отформатировать
cmake --build build --target format-check    # только проверить (CI)

# Только clang-tidy
cmake -S . -B build -DENABLE_CLANG_TIDY=ON
cmake --build build --target tidy            # диагностика
cmake --build build --target tidy-fix        # автоисправления

# Всё сразу + анализ при каждой компиляции main
cmake -S . -B build \
      -DENABLE_CLANG_FORMAT=ON \
      -DENABLE_CLANG_TIDY=ON \
      -DENABLE_CLANG_TIDY_ON_BUILD=ON
cmake --build build

# Выключить обратно (опции кэшируются — нужен явный OFF)
cmake -S . -B build -DENABLE_CLANG_FORMAT=OFF -DENABLE_CLANG_TIDY=OFF \
      -DENABLE_CLANG_TIDY_ON_BUILD=OFF

