# Glazeur / Глазурь
The Windows simple console application for control up the indicators of all keyboards, connected to system. You can making any interested illuminations with many keyboards through the «glazeur-script» - simple language for LEDs control, using the «[Glazeurist](http://htmlpreview.github.io/?http://github.com/Alikberov/Glazeur/blob/master/Glazeur/Glazeurist.html)» - useful web-editor.

Простая консольная программа управления индикаторами на нескольких клавиатурах, доступных в системе Windows. Вы можете создавать любые интересные иллюминации на всех своих клавиатурах, используя «глазурь-скрипт» - простой язык управления индикацией клавиатур, для редактирования и отладки которого был разработан также специальный онлайн-редактор - «[Глазурист](http://htmlpreview.github.io/?http://github.com/Alikberov/Glazeur/blob/master/Glazeur/Glazeurist.html)».

# Program / Программа
Download the project and start the Glazeur.exe for test. ([VirusTotal](http://virustotal.com/ru/file/67f75ecf453a4fe658ebdb154320c5f8c120219e68c95278e846e571a1d4798b/analysis/1509709285/))
Type ":" (colon) to start active session script;
Type "." (comma) to leave active session;
Press ESC-key in any moment to break.
Or copy-paste the our scenario from Glazeurist to command line window.

Скачайте проект и запустите Glazeur.exe для теста. ([VirusTotal](http://virustotal.com/ru/file/67f75ecf453a4fe658ebdb154320c5f8c120219e68c95278e846e571a1d4798b/analysis/1509709285/))
Введите ":" (двоеточие) для исполнения скрипта активной сессии;
Введите "." (точку) для завершения текущей сессии;
Жмите ESC в любой момент для прерывания сценария.
Или вставьте собственный сценарий из Глазуриста в окно консоли программы.

<details>
<summary>The «glazeur-scenarios» short syntax descriptions</summary>
  
Signature | Common description for signature action | Variant using sample | Using restriction¹
--------- | --------------------------------------- | -------------------- | ------------------
§i	| Select active paragraph with effects				| §0…§9			| Scenario
n§	| Select effects row in active paragraph			| 0§…99§		| Scenario/Paragraph
n§i	| Select active paragraph with row				| 99§0…45§6…0§9		| Scenario
n㎳	| Set up the basic delay interval				| 1㎳…1000㎳		| Scenario
0㎳	| Reset the delay coefficients					| 0㎳			| Scenario/Paragraph
n⁄d㎳	| Define the delay coefficient					| 2⅛㎳…3⅘⅞㎳		| Scenario/Paragraph
n⁄d⏱	| Waiting for coefficient interval				| ⏱…1⏱…2⅛⏱…3⅘⅞⏱		| Scenario/Paragraph
n⤴	| Go to row over in active paragraph				| ⤴…1⤴…99⤴		| Scenario/Paragraph
n⤵	| Go to next row in active paragraph				| ⤵…1⤵…99⤵		| Scenario/Paragraph
➰	| «Play» for active paragraph row				| ➰			| Scenario
n(…)	| Set looping for n-times					| 1(➰)…99(➰⤵)		| Scenario/Paragraph
n♻	| Set row-looping for n-times					| 1♻…99♻		| Сценарий/Параграф
§(…)	| Particulary run in active paragraph				| §(0⏱⅘⏱⤵)		| Scenario
⠿	| Immediate draw over the keyboards indicators			| 5(⠪⠝⏱⠿⠽⏱)		| Scenario/Paragraph
∧/⊕/∨	| Logical operation prefix for indicators			| 10(⊕⠪⠝⠽⏱∧⠽⠝⠪⏱)		| Сценарий/Параграф
▲/▶/▼/◀	| Scroll the indication buffer in signed dirrection		| 9(◀⏱)			| Scenario/Paragraph
⒑	| Definition of local label					| ⒈…⒛			| Scenario/Paragraph
Ⓞ	| Define the «interactive label»² in scenario			| ①…⑳ / Ⓐ…Ⓩ		| Scenario
i⃣	| Define the «interactive label»² in scenario			| 0⃣ …9⃣			| Scenario
i⌨	| Select for basic (left-side) keyboard³			| 1⌨…9⌨			| Scenario/Paragraph
n⌨	| Set up the logic order for keyboards³				| 12⌨…987654321⌨	| Scenario
n㎐	| Set up the indicators refresh frequency⁴			| 1㎐…1000㎐		| Scenario/Paragraph
n⁄d(…)	| Set «fractional»⁵ looping					| 1¾(…;…;…;…)		| Scenario/Paragraph
⇡/⇣	| The bright modulation⁶ control of indicators			| 10(⇣⠿⠿⠿⏱⇡⠿⠿⠿⏱)		| Scenario/Paragraph
ⁿ	| Using of iteration argument⁷ of parent row cycle		| Line #⁰ Lap #¹	| Debugging string
ᵢ	| Using of iteration argument⁷ of current row cycle		| Line #₀ Lap #₁	| Debugging string
§	| Using of active paragraph index⁷				| Effect#§ Line #₀	| Debugging string
⎚/⎙	| Clear/Print⁷ the formated logging string			| 10(⎙⏱⎚⏱)		| Scenario/Paragraph
⣀	| Patterned⁸ draw over the keyboards indicators			| 7(⣀◀⏱)		| Scenario/Paragraph

¹- the action can have a differences inside or outside the paragraphs

²- the linear parsing can be controlled by numbers key of keyboard

³- ignoring by online web-simulation, but strongly required in console utility

⁴- ignoring by online web-simulation, but controling for indication fidelity 

⁵- the cycle inside operations separated by «;» for ignoring by numerator range

⁶- not all keyboards devices supports for Pulse-Width Modulation then can show noise

⁷- any previous line remark or commentary using as output format

⁸- as pattern using any previous signed remark
</details>

<details>
<summary>Краткое описание синтаксиса «глазурь-сценариев»</summary>
  
Сигнатура | Краткое описание назначения и действия | Пример использования | Область действия¹
--------- | -------------------------------------- | -------------------- | ----------------
§i	| Активизация параграфа эффектов					| §0…§9			| Сценарий
n§	| Выбор строки активного параграфа				| 0§…99§		| Сценарий/Параграф
n§i	| Выбор строки с активизацией параграфа				| 99§0…45§6…0§9		| Сценарий
n㎳	| Установка базового интервала задержки сценария		| 1㎳…1000㎳		| Сценарий
0㎳	| Сброс коэффициентов интервала задержки			| 0㎳			| Сценарий/Параграф
n⁄d㎳	| Задание коэффициентов интервала задержки			| 2⅛㎳…3⅘⅞㎳		| Сценарий/Параграф
n⁄d⏱	| Организация паузы в «проигрывании»				| ⏱…1⏱…2⅛⏱…3⅘⅞⏱		| Сценарий/Параграф
n⤴	| Переход строками выше в активном параграфе			| ⤴…1⤴…99⤴		| Сценарий/Параграф
n⤵	| Переход строками ниже в активном параграфе			| ⤵…1⤵…99⤵		| Сценарий/Параграф
➰	| Запуск «проигрывания» эффекта активной строки параграфа	| ➰			| Сценарий
n(…)	| Организация зацикливания n-раз				| 1(➰)…99(➰⤵)		| Сценарий/Параграф
n♻	| Организация зацикливания строки n-раз				| 1♻…99♻		| Сценарий/Параграф
§(…)	| Управление выбранным параграфом				| §(0⏱⅘⏱⤵)		| Сценарий
⠿	| Непосредственное управление индикаторами клавиатур		| 5(⠪⠝⏱⠿⠽⏱)		| Сценарий/Параграф
∧/⊕/∨	| Префикс управления логической операцией к индикаторам		| 10(⊕⠪⠝⠽⏱∧⠽⠝⠪⏱)		| Сценарий/Параграф
▲/▶/▼/◀	| Скроллинг буфера индикации в соответствующем направлении	| 9(◀⏱)			| Сценарий/Параграф
⒑	| Определение локальной метки					| ⒈…⒛			| Сценарий/Параграф
Ⓞ	| Определение «интерактивной метки»² сценария			| ①…⑳ / Ⓐ…Ⓩ		| Сценарий
i⃣	| Определение «интерактивной метки»² сценария			| 0⃣ …9⃣			| Сценарий
i⌨	| Выборка базовой (первой слева) клавиатуры³			| 1⌨…9⌨			| Сценарий/Параграф
n⌨	| Задание логического порядка клавиатур³			| 12⌨…987654321⌨	| Сценарий
n㎐	| Задание частоты обновления индикаторов⁴			| 1㎐…1000㎐		| Сценарий/Параграф
n⁄d(…)	| Организация «дробного»⁵ зацикливания				| 1¾(…;…;…;…)		| Сценарий/Параграф
⇡/⇣	| Управление яркостью⁶ активных индикаторов			| 10(⇣⠿⠿⠿⏱⇡⠿⠿⠿⏱)		| Сценарий/Параграф
ⁿ	| Доступ к аргументу⁷ итерации вызываемого цикла		| Line #⁰ Lap #¹	| Строка отладки
ᵢ	| Доступ к аргументу⁷ итерации текущей строки параграфа		| Line #₀ Lap #₁	| Строка отладки
§	| Доступ к индексу⁷ активного параграфа				| Effect#§ Line #₀	| Строка отладки
⎚/⎙	| Очистка/Печать⁷ строки форматированной отладки		| 10(⎙⏱⎚⏱)		| Сценарий/Параграф
⣀	| Шаблонное⁸ управление индикаторами клавиатур			| 7(⣀◀⏱)		| Сценарий/Параграф

¹- действие эффекта может несколько отличаться в параграфе и за его пределами

²- клавишами цифровой части клавиатуры можно в реальном времени переключать «программу»

³- игнорируется графической симуляцией, но имеет существенную необходимость для физических клавиатур

⁴- игнорируется графической симуляцией, но управляет качеством индикации на физических клавиатурах

⁵- операторы цикла разделяются символом «;» и заключающей итерацией игнорируются выпадающие за индекс числителя

⁶- не все модели клавиатур поддерживают ШИМ-алгоритм и могут иметь непредсказуемое моргание

⁷- аргументы используются любой ремаркой, которая предшествует оператору печати отладочной информации

⁸- за шаблон принимается любая соответствующая ремарка
</details>

## Samples / Примеры
<details>
  <summary>Scenario v0.01α - (0:01:40)</summary>

Try [web-preview](http://htmlpreview.github.io/?http://github.com/Alikberov/Glazeur/blob/master/Glazeur/Glazeurist.html?script=Paragraphes%20declaration%20started%20at%20here%0A1%25u20E3%20%25u33B3%25u2150%25u33B3%25u2935%25u21E5Clockwise%20curve%20rotation%0ALOG%3AParagraph%23%A7%20Row%23%25u2080%20-%20Call%20from%20line%23%25u2070%20%B2%28%B9%28...%29%29%0A%25u21E5%09%25u2399%25u2811%25u2804%25u23F1%25u2818%25u2804%25u23F1%25u2808%25u2806%25u23F1%25u2800%25u2807%25u23F1%25u2820%25u2803%25u23F1%25u2830%25u2801%25u23F1%25u2935%25u21E5%09Goto%20down%0A%25u21E5%09%25u2399%25u2814%25u2801%25u23F1%25u2814%25u2802%25u23F1%25u2824%25u2802%25u23F1%25u2824%25u2804%25u23F1%25u2822%25u2804%25u23F1%25u2812%25u2804%25u23F1%25u2935%25u21E5%09Goto%20down%0A%25u21E5%09%25u2399%25u2811%25u2804%25u23F1%25u2831%25u2800%25u23F1%25u2823%25u2800%25u23F1%25u2807%25u2800%25u23F1%25u280E%25u2800%25u23F1%25u281C%25u2800%25u23F1%25u2935%25u21E5%09Goto%20down%0A%25u21E5%09%25u2399%25u2814%25u2801%25u23F1%25u2812%25u2801%25u23F1%25u280A%25u2801%25u23F1%25u2809%25u2801%25u23F1%25u2809%25u2802%25u23F1%25u2811%25u2802%25u23F11%A7%25u21E5%09Goto%20up%20to%20row%20%231%0A2%25u20E3%20%25u33B3%25u2152%25u33B3%25u2935%25u21E5Bidirectional%20line%20rotation%0ALOG%3AParagraph%23%A7%20Row%23%25u2080%20-%20Call%20from%20line%23%25u2070%20%B2%28%B9%28...%29%29%0A%25u21E5%09%25u2399%25u2814%25u2801%25u23F1%25u2812%25u2802%25u23F1%25u2811%25u2804%25u23F1%25u2838%25u2800%25u23F1%25u21E5%09Clockwise%20cycle%0A%25u21E5%09%25u2399%25u2811%25u2804%25u23F1%25u2812%25u2802%25u23F1%25u2814%25u2801%25u23F1%25u2838%25u2800%25u23F1%25u21E5%09Anticlockwise%20cycle%0A%0AHere%20is%20beginning%20the%20main%20body%20of%20scenario%0ASet%20keyboards%20order%20to%204251%20and%20select%20%234%20as%20first/left%0A%25u21E5%091524%25u23281%25u2328%0ASet%201000ms%20of%20delay%20and%20use%20500Hz%20of%20refresh%0A%25u21E5%091000%25u33B3500%25u3390%0ACountdown%20to%20start%20%25u2081%0A%25u21E5%093%28%25u2399%25u23F1%25u239A%25u23F1%29%0AUse%20paragraph%20%232%20and%20initialize%20to%201/12%20of%201000ms%20delay%0ALoop%20for%205%20times%20%ABclockwise/anticlockwise%BB%20turns%20with%20ping-pong%0A%25u21E5%096%28%25u2399%BE%25u23F1%25u239A%BC%25u23F1%29%0A%25u21E5%09%A72%20%A7%28%25u33B3%25u2153%BC%25u33B3%25u2935%29%204%BD%281%A7%207%28%25u27B0%25u21E1%25u2328%29%3B2%A7%207%28%25u21E3%25u2328%25u27B0%29%29%0AUse%20paragraph%20%232%20and%20initialize%20to%201/25%20of%201000ms%20delay%0ALoop%20for%205%20times%20%ABclockwise/anticlockwise%BB%20speedy%20ping-pong%20runs%0A%25u21E5%096%28%25u2399%BE%25u23F1%25u239A%BC%25u23F1%29%0A%25u21E5%09%A72%20%A7%28%25u33B3%25u2155%25u2155%25u33B3%25u2935%29%205%BD%281%A7%207%28%25u27B0%25u21E1%25u2328%29%3B2%A7%207%28%25u21E3%25u2328%25u27B0%29%29%0ALoop%20for%205%20times%20of%20%ABcurved%20rotations%BB%0A%25u21E5%096%28%25u2399%BE%25u23F1%25u239A%BC%25u23F1%29%0A%25u21E5%09%A71%205%284%28%25u27B0%29%29%0AThank%20You%20for%20watch%21%20%3A%29%0A%25u21E5%09%25u215B%25u33B310%28%25u2887%25u21E1%25u2328%25u23F1%29%0A%25u21E5%09%25u215B%25u33B310%28%25u23995%28%25u21E3%25u2833%25u21E1%25u2328%25u23F1%29%25u239A5%28%25u21E3%25u281E%25u21E1%25u2328%25u23F1%29%29%0AHalt%20this%20demo%0A%25u28C0%25u2887%25u2882%25u2887%25u2880%25u2887%25u2884%25u2881%25u2887%25u2881%0A%25u21E5%099%25u23289%28%25u25C0%25u239A%BC%25u23F1%25u2399%25u28C0%25u23F1%29%0A%25u21E5%09%25u23995%289%28%25u21E1%25u283F%25u283F%25u283F%25u283F%25u2807%25u23F1%299%28%25u21E3%25u283F%25u283F%25u283F%25u283F%25u2807%25u23F1%29%29) in Glazeurist
<pre>
Paragraphes declaration started at here
1⃣ ㎳⅐㎳⤵⇥Clockwise curve rotation
LOG:Paragraph#§ Row#₀ - Call from line#⁰ ²(¹(...))
⇥	⎙⠑⠄⏱⠘⠄⏱⠈⠆⏱⠀⠇⏱⠠⠃⏱⠰⠁⏱⤵⇥	Goto down
⇥	⎙⠔⠁⏱⠔⠂⏱⠤⠂⏱⠤⠄⏱⠢⠄⏱⠒⠄⏱⤵⇥	Goto down
⇥	⎙⠑⠄⏱⠱⠀⏱⠣⠀⏱⠇⠀⏱⠎⠀⏱⠜⠀⏱⤵⇥	Goto down
⇥	⎙⠔⠁⏱⠒⠁⏱⠊⠁⏱⠉⠁⏱⠉⠂⏱⠑⠂⏱1§⇥	Goto up to row #1
2⃣ ㎳⅒㎳⤵⇥Bidirectional line rotation
LOG:Paragraph#§ Row#₀ - Call from line#⁰ ²(¹(...))
⇥	⎙⠔⠁⏱⠒⠂⏱⠑⠄⏱⠸⠀⏱⇥	Clockwise cycle
⇥	⎙⠑⠄⏱⠒⠂⏱⠔⠁⏱⠸⠀⏱⇥	Anticlockwise cycle

Here is beginning the main body of scenario
Set keyboards order to 4251 and select #4 as first/left
⇥	1524⌨1⌨
Set 1000ms of delay and use 500Hz of refresh
⇥	1000㎳500㎐
Countdown to start ₁
⇥	3(⎙⏱⎚⏱)
Use paragraph #2 and initialize to 1/12 of 1000ms delay
Loop for 5 times «clockwise/anticlockwise» turns with ping-pong
⇥	6(⎙¾⏱⎚¼⏱)
⇥	§2 §(㎳⅓¼㎳⤵) 4½(1§ 7(➰⇡⌨);2§ 7(⇣⌨➰))
Use paragraph #2 and initialize to 1/25 of 1000ms delay
Loop for 5 times «clockwise/anticlockwise» speedy ping-pong runs
⇥	6(⎙¾⏱⎚¼⏱)
⇥	§2 §(㎳⅕⅕㎳⤵) 5½(1§ 7(➰⇡⌨);2§ 7(⇣⌨➰))
Loop for 5 times of «curved rotations»
⇥	6(⎙¾⏱⎚¼⏱)
⇥	§1 5(4(➰))
Thank You for watch! :)
⇥	⅛㎳10(⢇⇡⌨⏱)
⇥	⅛㎳10(⎙5(⇣⠳⇡⌨⏱)⎚5(⇣⠞⇡⌨⏱))
Halt this demo
⣀⢇⢂⢇⢀⢇⢄⢁⢇⢁
⇥	9⌨9(◀⎚¼⏱⎙⣀⏱)
⇥	⎙5(9(⇡⠿⠿⠿⠿⠇⏱)9(⇣⠿⠿⠿⠿⠇⏱))</pre>
</details>

## Demonstration / Демонстрация
[![Utility in work demonstration](https://img.youtube.com/vi/BoI2meUvO4Y/0.jpg)](http://youtu.be/BoI2meUvO4Y)
