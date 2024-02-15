**Необходимо реализовать под управлением ОС Unix интерактивный командный
интерпретатор (некоторый аналог shell), осуществляющий в цикле считывание командной
строки со стандартного ввода, ее анализ и исполнение соответствующих действий.**

* *Условия*


В командной строке могут присутствовать следующие операции:
указаны в порядке убывания приоритетов (на одной строке приоритет одинаков)


1) | , > , >> , <

2) && , ||

3) ; , &

Допустимы также круглые скобки, которые позволяют изменить порядок выполнения
операций.
В командной строке допустимо также произвольное количество пробелов между
составляющими ее словами

* *Разбор командной строки осуществляется Shell-oм по следующим правилам:
 
<Команда Shellа > →

< Команда с условным выполнением >{ [; | &] < Команда Shellа>}{ ; |&}

<Команда с условным выполнением > →

<Команда> { [&& | || ] <Команда с условным выполнением>}

<Команда> → {<перенаправление ввода/вывода>}<Конвейер> |

<Конвейер>{<перенаправление ввода/вывода>} | ( <Команда Shellа>)

<перенаправление ввода/вывода> →

{<перенаправление ввода > } <перенаправление вывода> |

{<перенаправление вывода>}<перенаправление ввода >

<перенаправление ввода > → ‘<’ файл

<перенаправление вывода> → ‘>’ файл | ‘>>’ файл

<Конвейер>→ <Простая команда> {‘|’ <Конвейер>}

<Простая команда>→ <имя команды><список аргументов>*

{X} – означает, что X может отсутствовать;

[x|y] – значит, что должен присутствовать один из вариантов : x либо y

| - в описании правил то же, что «ИЛИ»

pr1 | …| prN – конвейер.

Каждая команда выполняется каксамостоятельный процесс. Shell ожидаетзавершения последней команды для проверки ее статуса в команде с условным выполнением.
Код завершения конвейера = код завершения последней команды конвейера. Остальные команды
конвейера также не должны стать «зомби».**

* *Пример*

***com1 && (com2; com3)***

Здесь команды com2 и com3 будут выполнены только при успешном завершении com1.

***(com1; com2) &***

В фоновом режиме будет выполняться последовательность команд com1 и com2.
Круглые скобки (), кроме выполнения функции группировки, выполняют и функцию вызова
нового экземпляра интерпретатора Shell

***ps; ls; (cd namedir; ls; ps) && ls && ps***

Первый и последний ls отработают в текущем каталоге, ls в скобках будет выполнен в каталоге
namedir, ps в скобках должен показать наличие дополнительного процесса Shell.