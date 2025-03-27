# Лабораторна робота № 2

## Завдання 1

### Результат роботи програми:
```
time_t bits (1 bit +- sign): 64
Max time_t is 2^63: 9223372036854775807
++time1 result: -9223372036854775808
Negative value - OVERFLOWED
```

Для 64-бітної (поточної) архітектури максимальне значення time_t є 2^63
На 32-бітній - це 2^31, або 2 147 483 648

### Сегменти виконуваного файлу наступні:
```
   text    data     bss     dec     hex filename
   1263     552      16    1831     727 1
```

## Завдання 2

### Результат виконання ls -l:
```
-rwxr-xr-x  1 max max 8184 Mar 26 20:58 2
```

### Результат виконання size:
```
  text   data   bss    dec     hex   filename
  1077    552    16   1645   0x66d   2
```

### Після оголошення глобального масиву на 1000 int:
```
-rwxr-xr-x  1 max max 8216 Mar 26 21:01 2
```
```
  text   data    bss    dec      hex   filename
  1077    552   4032   5661   0x161d   2
```

### Додано початкове значення в оголошення масиву:
```
-rwxr-xr-x  1 max max 12232 Mar 26 21:09 2
```
```
  text   data   bss    dec      hex   filename
  1077   4568    16   5661   0x161d   2
```

### В main() додано оголошення великого масиву з цніціалізатором:
```
-rwxr-xr-x  1 max max 12296 Mar 26 21:17 2
```
```
  text   data   bss    dec      hex   filename
  1117   4568    16   5701   0x1645   2
```

Якщо масив не ініціалізовано, він зберігається у сегменті bss
Після ініціалізації, він переміщується у data

Для налагодження файли стають більші через менш оптимізований код та сегмент debug
Для оптимізації файли роблються менші для швидкодії, наприклад за допомогою inline функцій

## Завдання 3

### Результат виконання програми:
```
Text segment, aka main(), is at: 0x400663
Global int (data segment) is at: 0x401ad8
Heap is at: 0x21dff6208008
The stack top is near: 0x820f004d4
Func called
Now stack top is near: 0x820eff13c
```

## Завдання 4

### Результати дослідження за допомогою gdb:
Перше вікно терміналу:
```
max@aspz:~/c/2 $ ./4
In function                 main; &localvar = 0x8213804bc
In function                  foo; &localvar = 0x82138048c
In function                  bar; &localvar = 0x82138046c
In function    bar_is_now_closed; &localvar = 0x82138044c

 Now blocking on pause()...
Terminated
```

Друге вікно терміналу:
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

### Недоступність функції gstack на FreeBSD:
Використання функції gstack виявилося неможливим через її відсутність у системі та неможливість її завантаження
Така ситуація була передбачена умовою, тому використано лише gdb, без порівняння з gstack

## Завдання 5

### Результат виконання:
```
Before func
Inside func
After func
```

### Пояснення питання:
У даному прикладі демонструється використання функції у середині коду. 
Тут виклик func за нормальних обставин:
- поточний IP (адреса інструкції після виклику func) на стеку як адреса повернення
- IP змінюється і вказує на першу інструкцію у func(), завершується func(), адреса повернення переходить зі стеку в IP
- далі програма продовжує з наступної інструкції після func()

ПРОБЛЕМА:
- Якщо на вершині стека IP, то нема місця для адреси повернення, через що при завершенні func()
- процесор не знатиме, куди повернутися і скоріш за все звернеться до несправжньої адреси або зробить щось непотрібне
- виклик func() призведе до збою програми, бо процесор забуде що робити після func()

## Завдання 6
### Результат виконання:
Перше вікно терміналу:
```
max@aspz:~/c/2 $ ./4
In function                 main; &localvar = 0x8205d06fc
In function                  foo; &localvar = 0x8205d06cc
In function                  bar; &localvar = 0x8205d06ac
In function    bar_is_now_closed; &localvar = 0x8205d068c

 Now blocking on pause()...
Terminated
```

Друге вікно терміналу:
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

### Пояснення рішення:
До програми у командній строці надаються 2 аргументи:
- PID, знайдений за допомогою ps aux | grep ./4
- адреса, за якою будуть зчитані дані

За "інший процес" взято програму з завдання 4, бо вона спиняється
За виводом програми 4, беремо адресу, наприклад localvar з foo, що має значення 5
Результат 0x5 - це hex 5, тобто дані зчитано вірно 
