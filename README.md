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

Программа использует POSIX-стиль аргументов командной строки.

### Синтаксис

```bash
./main -a <число1> -o <операция> -b <число2>
./main -a <число> -o '!'
./main -h
```

### Опции

| Опция | Аргумент | Описание |
| :--- | :--- | :--- |
| `-a` | `<число>` | Первый операнд (или число для факториала) |
| `-o` | `<знак>` | Операция: `+`, `-`, `*`, `/`, `^`, `!` |
| `-b` | `<число>` | Второй операнд (не требуется для `!`) |
| `-h` | — | Вывод справки по использованию |

### Примеры

1. **Сложение:**
   ```bash
   ./main -a 10 -o + -b 5
   # Вывод: 10 + 5 = 15
   ```

2. **Факториал:**
   ```bash
   ./main -a 5 -o '!'
   # Вывод: 5! = 120
   ```

3. **Справка:**
   ```bash
   ./main -h
   ```

> **Примечание:** Символы операций, такие как `*` и `!`, рекомендуется заключать в одинарные кавычки, чтобы избежать их интерпретации оболочкой bash/zsh.

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

