# Завдання 1

## Опис:

У написаній програмі ([код](1.c)) за допомогою визначення розміру time_t (sizeof(time_t)) дізнаємося, що цей тип займає 8 байтів (64 біти).
Це збігається з системою використаною у прикладі (64 бітна), а на 32 бітній системі займала б time_t відповідно 32.
Оскільки time_t може мати і від'ємні значення (як видно у результатах виконання), то один з цих бітів займає знак.
Якщо цей біт 0 - то значення додатнє, якщо 1 - від'ємне, через що на зберігання самого значення залишається 63 біта.
Це видно чітко у результатах виконання, оскільки при інкрементації 2^63 - 1 (-1 через включення до позитивних значень time_t нуля),
отримаємо -2^63 (без -1 бо нуль у від'ємні не входить), оскільки значення 2^63 - максимальне додатнє, тому при інкрементації
відбувається оверфлоу у мінімальне можливе значення, а саме -2^63. Виходить що всього тоді time_t може приймати 2^64 значень, рахуючи
від'ємні, але максимальне 2^63 - 1, а мінімальне -2^63 (у 32 бітній: 2^31 - 1, та -2^31 відповідно).

Обраховані значення:
- 2^63 - 1 = 9 223 372 036 854 775 807
- 2^31 - 1 = 2 147 483 648

## Результат роботи програми:

### Виконання:

```
time_t bits (1 bit +- sign): 64
Max time_t is 2^63: 9223372036854775807
++time1 result: -9223372036854775808
Negative value - OVERFLOWED
```

### Перегляд сегментів за допомогою size:
```
   text    data     bss     dec     hex filename
   1263     552      16    1831     727 1
```

# Завдання 2

## Опис:

Створено програму hello world, до якої поступово внесено зміни, після кожної з яких перевірено загальний розмір виконуваного файлу за
допомогою ls -l, та розміри його сегментів за допомогою size.

## Результат виконання:

### Базова програма ([код](2-1.c)): 

Загальний розмір:
```
-rwxr-xr-x  1 max max 8184 Mar 26 20:58 2
```

Розміри сегментів:
```
  text   data   bss    dec     hex   filename
  1077    552    16   1645   0x66d   2
```

### Після оголошення глобального масиву на 1000 int ([код](2-2.c)): 

Загальний розмір:
```
-rwxr-xr-x  1 max max 8216 Mar 26 21:01 2
```

Розміри сегментів:
```
  text   data    bss    dec      hex   filename
  1077    552   4032   5661   0x161d   2
```

### Ініціалізовано одиницею глобальний масив arr ([код](2-3.c)):

Загальний розмір:
```
-rwxr-xr-x  1 max max 12232 Mar 26 21:09 2
```

Розміри сегментів:
```
  text   data   bss    dec      hex   filename
  1077   4568    16   5661   0x161d   2
```

### В main() додано оголошення великого масиву arr2 та ініціалізовано одиницею ([код](2-4.c)):

Загальний розмір:
```
-rwxr-xr-x  1 max max 12296 Mar 26 21:17 2
```

Розміри сегментів:
```
  text   data   bss    dec      hex   filename
  1117   4568    16   5701   0x1645   2
```

## Відповіді на питання:

Спираючись на результати змін до hello world програми, можна сказати що якщо масив не ініціалізовано,
він зберігається у сегменті bss, а після ініціалізації, він переміщується у data.

Для налагодження файли стають більші через менш оптимізований код та новий сегмент debug, що в такому випадку з'являється.
Для оптимізації файли роблються менші для швидкодії, наприклад за допомогою inline функцій, але код частот стає менш читабельним
та складнішим для відлагодження.

# Завдання 3

## Опис:

Написано програму ([код](3.c)) для визначення адрес сегментів файлу. Для цього виводяться такі значення:
- адреса функції main(), код у якій зберігається у текстовому сегменті, тобто її адреса і є місцезнаходженням цього сегменту
- адреса ініціалізованої глобальної зміної, що зберігається у сегменті даних
- адреса, що зберігається у алокованому через malloc вказівнику p, що лежить на купі (heap)

Також за допомогою зміних i (до збільшення стеку) та i2 (після збільшення стеку) показано переміщення вершини стеку
після збільшення його розміру шляхом оголошення двох великих масивів arr та charr і спостережено значну зміну адрес i та i2.

## Результат виконання програми:

```
Text segment, aka main(), is at: 0x400663
Global int (data segment) is at: 0x401ad8
Heap is at: 0x21dff6208008
The stack top is near: 0x820f004d4
Func called
Now stack top is near: 0x820eff13c
```

# Завдання 4

## Опис:

За допомогою запропонованої програми ([код](4.c)) проведено дослідження стеку процесу.
Використання функції gstack виявилося неможливим через її відсутність у системі та неможливість її завантаження.
Така ситуація була передбачена умовою, тому використано лише gdb, без порівняння з gstack.
Дослідження проведено на FreeBSD з графічним інтерфейсом KDE Plasma, що дозволило легко відкрити 2 окремих вікна термінала.
У першому вікні терміналу запущено надану програму, яка за допомогою pause() переводить поточний процес у сплячий режим, що
дає змогу провести його дослідження з іншого вікна терміналу. У другому вікні отримано pid процесу через ps aux | grep ./4, 
де ./4 - це запущений у іншому вікні процес, що зараз у сплячому режимі, далі використано sudo gdb --quiet для дослідження 
стеку процесу. Після виводу даних процес 2038 (pid ./4) від'єднано та "вбито" (закінчено), що видно на першому терміналі
у вигляді повідомлення "Terminated", що повністю зупинило процес роботи програми.


## Результати дослідження за допомогою gdb:

### Перше вікно терміналу:

```
max@aspz:~/c/2 $ ./4
In function                 main; &localvar = 0x8213804bc
In function                  foo; &localvar = 0x82138048c
In function                  bar; &localvar = 0x82138046c
In function    bar_is_now_closed; &localvar = 0x82138044c

 Now blocking on pause()...
Terminated
```

### Друге вікно терміналу:

```
max@aspz:~ $ ps aux | grep ./4
max        2038   0.0  0.0      13728    2244  3  I+   22:48     0:00.00 ./4
max        2055   0.0  0.0      13836    2500  4  S+   22:49     0:00.00 grep ./4
max@aspz:~ $ sudo gdb --quiet
(gdb) attach 2038
Attaching to process 2038
Reading symbols from /home/max/c/2/4...
Reading symbols from /lib/libc.so.7...
Reading symbols from /usr/lib/debug//lib/libc.so.7.debug...
Reading symbols from /libexec/ld-elf.so.1...
Reading symbols from /usr/lib/debug//libexec/ld-elf.so.1.debug...
_sigsuspend () at _sigsuspend.S:4
warning: 4      _sigsuspend.S: No such file or directory
(gdb) bt
#0  _sigsuspend () at _sigsuspend.S:4
#1  0x0000000822b57c35 in __pause () at /usr/src/lib/libc/gen/pause.c:51
#2  0x000000000040068b in bar_is_now_closed ()
#3  0x00000000004006bd in bar ()
#4  0x00000000004006ef in foo ()
#5  0x0000000000400728 in main ()
(gdb) exit
A debugging session is active.

        Inferior 1 [process 2038] will be detached.

Quit anyway? (y or n) y
Detaching from program: /home/max/c/2/4, process 2038
[Inferior 1 (process 2038) detached]
max@aspz:~ $ kill 2038
```

# Завдання 5

## Опис:

Написано [демонстративний код](5.c) лише для використання його структури для розгляду гіпотетичної ситуації зміни IP на вершину стека.
У даному прикладі демонструється використання функції у середині коду між двома текстовими повідомленнями, як видно у результатах запуску.

Тут виклик func за нормальних обставин:
- поточний IP (адреса інструкції після виклику func) на стеку як адреса повернення
- IP змінюється і вказує на першу інструкцію у func(), завершується func(), адреса повернення переходить зі стеку в IP
- далі програма продовжує з наступної інструкції після func()

Проблема виникає у такому випадку:
- Якщо на вершині стека IP, то нема місця для адреси повернення, через що при завершенні func()
- процесор не знатиме, куди повернутися і скоріш за все звернеться до несправжньої адреси або зробить щось непотрібне
- виклик func() призведе до збою програми, бо процесор забуде що робити після func()

## Результат виконання:
```
Before func
Inside func
After func
```

# Завдання 6 за варіантом 18:

## Опис:

Написано програму ([код](6.c)), що використовує ptrace() для отримання значення з адреси у іншому процесі.
За процес, чию пам'ять переглядатиме програма, у цьому прикладі взято програму з завдання 4 ([код](4.c)), оскільки вона
виводить адреси клітинок пам'яті у ній (localvar у функціях), чиє значення можна буде прочитати з програми цього завдання, а також 
код 4 завдання спиняється на pause(), що знову ж таки дає змогу переглянути pid процесу. Подібно до [методу](Опис-3) визначення pid 
за допомогою 2-х терміналів у 4 завданні, отримується id процесу, який разом із адресою пам'яті (в цьому випадку взято localvar з foo()) 
передається в якості аргумента до цієї програми. Далі програма під'єднує процес, перевіряючи успішність цього, і використовуючи
ptrace з переданими pid та адресою пам'яті, переглядає (для цього аргумент PT_READ_D) та виводить значення у цій пам'яті іншого процесу.
Далі за допомогою ptrace за аргументом PT_DETACH процес від'єднується та успішність цього перевіряється.

## Результат виконання:

### Перше вікно терміналу:

```
max@aspz:~/c/2 $ ./4
In function                 main; &localvar = 0x8205d06fc
In function                  foo; &localvar = 0x8205d06cc
In function                  bar; &localvar = 0x8205d06ac
In function    bar_is_now_closed; &localvar = 0x8205d068c

 Now blocking on pause()...
Terminated
```

### Друге вікно терміналу:

```
max@aspz:~/c/2 $ ps aux | grep ./4
max        1630   0.0  0.0      13836    2344  1  S+   12:02     0:00.00 grep ./4
max        1628   0.0  0.0      13728    2096  2  I+   12:01     0:00.00 ./4
max@aspz:~/c/2 $ ./6 1628 0x8205d06cc
Attached to PID: 1628
Address: 0x8205d06cc, data: 0x5
Detached from PID: 1628
max@aspz:~/c/2 $ kill 1628
```
