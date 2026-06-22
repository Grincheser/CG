# Как добавить функцию и тесты

## Структура

```
s21_string.h         API библиотеки
s21_<func>.c         реализация функции
tests.h              инклюды + прототипы всех Suite
main.c               один main, гоняет все Suite
test_s21_<func>.c    тесты функции
```

Сейчас плоско в корне репо. В основном проекте переедем в `src/`.

---

## Шаг 1. Реализация

**1.1.** В `s21_string.h` добавить прототип перед `#endif`.

**1.2.** Создать `s21_<func>.c`:
```c
#include "s21_string.h"

тип s21_<func>(...) {
    /* реализация */
}
```

Правила:
- Только `#include "s21_string.h"`. Никаких `<string.h>` в либе.
- Helper-функции внутри `.c` — `static`.
- Локалки инициализируем (`int i = 0;`).
- ≤1 return, ≤1 break, вложенность ≤4, функция ≤50 строк.

---

## Шаг 2. Тесты

**2.1.** В `tests.h` добавить:
```c
Suite *s21_<func>_suite(void);
```

**2.2.** В `main.c` в массив `suites[]` добавить **перед NULL**:
```c
s21_<func>_suite(),
```

**2.3.** Создать `test_s21_<func>.c`:
```c
#include "tests.h"

static void check_<func>(/* параметры */) {
    /* сравнить s21_<func> с эталоном через ck_assert_*_eq */
}

START_TEST(test_<func>_<case>) {
    check_<func>(/* данные */);
}
END_TEST

/* ещё 2-3 теста */

Suite *s21_<func>_suite(void) {
    Suite *s = suite_create("s21_<func>");
    TCase *tc = tcase_create("Basic");
    tcase_add_test(tc, test_<func>_<case>);
    /* остальные tcase_add_test */
    suite_add_tcase(s, tc);
    return s;
}
```

Правила:
- `<string.h>` в тестах **разрешён** (для эталона).
- 2-4 теста на функцию. По числу веток кода, не по числу входных данных.
- На каждую функцию — отдельный Suite + один TCase `"Basic"`.

---

## ck_assert_* шпаргалка

| Что сравниваем | Макрос |
|---|---|
| `s21_size_t` (unsigned) | `ck_assert_uint_eq` |
| `int` (signed) | `ck_assert_int_eq` |
| Указатели | `ck_assert_ptr_eq` |
| Строки до `\0` | `ck_assert_str_eq` |
| N байт памяти | `ck_assert_mem_eq(p1, p2, n)` |

---

## Шаг 3. Сборка и запуск

```bash
gcc -Wall -Werror -Wextra -std=c11 \
    s21_*.c test_s21_*.c main.c \
    -lcheck -lm -lpthread -lsubunit \
    -o test_runner
./test_runner
```

Ожидается `Failures: 0, Errors: 0` на каждой Suite.

---

## Шаг 4. Коммит

```bash
git add s21_string.h s21_<func>.c test_s21_<func>.c tests.h main.c
git commit -m "Реализовал s21_<func> + тесты"
git push origin develop
```

Правила коммитов:
- Русский, простыми словами.
- Без `feat:`/`fix:`/`refactor:`.
- Без многоуровневых буллетов.

**Не коммитим:** `test_runner`, `*.o`, `*.a`, `*.gcno`, `*.gcda`, `.vscode/`.

---

## Перед коммитом — чек-лист

- [ ] `gcc -Wall -Werror -Wextra -std=c11 -c s21_<func>.c` без варнингов
- [ ] `./test_runner` все зелёные
- [ ] `git pull origin develop` (взял свежие коммиты команды)

---

## Частые ошибки

| Симптом | Причина |
|---|---|
| Конфликт линковки между `test_*.c` | Забыл `static` у helper |
| Ошибка в макросах | Забыл `END_TEST` |
| Ругань Vertner | `<string.h>` в коде либы |
| Варнинг signed/unsigned | `ck_assert_int_eq` вместо `ck_assert_uint_eq` для `s21_size_t` |

---

## Если что-то не работает

- Check не установлен → `sudo apt-get install check libsubunit-dev`
- Не на той ветке → `git checkout develop`
- Мердж-конфликт → `git pull origin develop` перед началом работы и перед push