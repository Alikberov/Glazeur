# Glazeur
Lots keyboard illumination automat

<details>
  <summaryКраткое описание синтаксиса языка сценариев</summary>
  
Сигнатура | Краткое описание назначения и действия | Пример использования | Область действия¹
--------- | -------------------------------------- | -------------------- | ----------------
i⃣	| Активизация параграфа эффектов					| 0⃣ …9⃣			| Сценарий
§i	| Активизация параграфа эффектов					| §0…§9			| Сценарий
n§	| Выбор строки активного параграфа				| 0§…99§		| Сценарий/Параграф
n§i	| Выбор строки с активизацией параграфа				| 99§0…45§6…0§9		| Сценарий
i⌨	| Выборка базовой (первой слева) клавиатуры²			| 1⌨…9⌨			| Сценарий/Параграф
n⌨	| Задание логического порядка клавиатур²			| 12⌨…987654321⌨	| Сценарий
n㎐	| Задание частоты обновления индикаторов³			| 1㎐…1000㎐		| Сценарий/Параграф
n㎳	| Установка базового интервала задержки сценария		| 1㎳…1000㎳		| Сценарий
0㎳	| Сброс коэффициентов интервала задержки			| 0㎳			| Сценарий/Параграф
n⁄d㎳	| Задание коэффициентов интервала задержки			| 2⅛㎳…3⅘⅞㎳		| Сценарий/Параграф
n⁄d⏱	| Организация паузы в «проигрывании»				| ⏱…1⏱…2⅛⏱…3⅘⅞⏱		| Сценарий/Параграф
n⤴	| Переход строками выше в активном параграфе			| ⤴…1⤴…99⤴		| Сценарий/Параграф
n⤵	| Переход строками ниже в активном параграфе			| ⤵…1⤵…99⤵		| Сценарий/Параграф
↝	| Запуск «проигрывания» эффекта активной строки параграфа	| ↝			| Сценарий
n(…)	| Организация зацикливания n-раз				| 1(↝)…99(↝⤵)		| Сценарий/Параграф
§(…)	| Управление выбранным параграфом				| §(0⏱⅘⏱⤵)		| Сценарий
⠿	| Управление индикаторами клавиатуры				| 5(⠪⠝⏱⠿⠽⏱)		| Сценарий/Параграф
⇡/⇣	| Управление яркостью⁴ активных индикаторов			| 10(⇣⠿⠿⠿⏱⇡⠿⠿⠿⏱)		| Сценарий/Параграф
ⁿ	| Доступ к аргументу⁵ итерации вызываемого цикла		| Line #⁰ Lap #¹	| Строка отладки
ᵢ	| Доступ к аргументу⁵ итерации текущей строки параграфа		| Line #₀ Lap #₁	| Строка отладки
§	| Доступ к индексу⁵ активного параграфа				| Effect#§ Line #₀	| Строка отладки
⎚/⎙	| Очистка/Печать⁵ строки форматированной отладки		| 10(⎙⏱⎚⏱)		| Сценарий/Параграф

¹- действие эффекта может несколько отличаться в параграфе и за его пределами

²- игнорируется графической симуляцией, но имеет существенную необходимость для физических клавиатур

³- игнорируется графической симуляцией, но управляет качеством индикации на физических клавиатурах

⁴- не все модели клавиатур поддерживают ШИМ-алгоритм и могут иметь непредсказуемое моргание

⁵- аргументы используются любой ремаркой, которая предшествует оператору печати отладочной информации
</details>

<details>
  <summary>The scenarios syntax short descriptions</summary>
  
Signature | Common description for signature action | Variant using sample | Using restriction¹
--------- | --------------------------------------- | -------------------- | ------------------
i⃣	| Select active paragraph with effects				| 0⃣ …9⃣			| Scenario
§i	| Select active paragraph with effects				| §0…§9			| Scenario
n§	| Select effects row in active paragraph			| 0§…99§		| Scenario/Paragraph
n§i	| Select active paragraph with row				| 99§0…45§6…0§9		| Scenario
i⌨	| Select for basic (left-side) keyboard²			| 1⌨…9⌨			| Scenario/Paragraph
n⌨	| Set up the logic order for keyboards²				| 12⌨…987654321⌨	| Scenario
n㎐	| Set up the indicators refresh frequency³			| 1㎐…1000㎐		| Scenario/Paragraph
n㎳	| Set up the basic delay interval				| 1㎳…1000㎳		| Scenario
0㎳	| Reset the delay coefficients					| 0㎳			| Scenario/Paragraph
n⁄d㎳	| Define the delay coefficient					| 2⅛㎳…3⅘⅞㎳		| Scenario/Paragraph
n⁄d⏱	| Waiting for coefficient interval				| ⏱…1⏱…2⅛⏱…3⅘⅞⏱		| Scenario/Paragraph
n⤴	| Go to row over in active paragraph				| ⤴…1⤴…99⤴		| Scenario/Paragraph
n⤵	| Go to next row in active paragraph				| ⤵…1⤵…99⤵		| Scenario/Paragraph
↝	| «Play» for active paragraph row				| ↝			| Scenario
n(…)	| Set looping for n-times					| 1(↝)…99(↝⤵)		| Scenario/Paragraph
§(…)	| Particulary run in active paragraph				| §(0⏱⅘⏱⤵)		| Scenario
⠿	| Draw over the keyboards indicators				| 5(⠪⠝⏱⠿⠽⏱)		| Scenario/Paragraph
⇡/⇣	| The bright modulation⁴ control of indicators			| 10(⇣⠿⠿⠿⏱⇡⠿⠿⠿⏱)		| Scenario/Paragraph
ⁿ	| Using of iteration argument⁵ of parent row cycle		| Line #⁰ Lap #¹	| Debugging string
ᵢ	| Using of iteration argument⁵ of current row cycle		| Line #₀ Lap #₁	| Debugging string
§	| Using of active paragraph index⁵				| Effect#§ Line #₀	| Debugging string
⎚/⎙	| Clear/Print⁵ the formated logging string			| 10(⎙⏱⎚⏱)		| Scenario/Paragraph

¹- the action can have a differences inside or outside the paragraphs

²- ignoring by online web-simulation, but strongly required in console utility

³- ignoring by online web-simulation, but controling for indication fidelity 

⁴- not all keyboards devices supports for Pulse-Width Modulation then can show noise

⁵- any previous line remark or commentary using as output format
</details>

## Samples / Примеры
<details>
  <summary>Scenario v0.01α</summary>
  Try [Preview](http://htmlpreview.github.io/?http://github.com/Alikberov/Glazeur/blob/master/Glazeurist.html?script=Paragraphes%20declaration%20started%20at%20here%0A1%25u20E3%20%25u33B3%25u2150%25u33B3%25u2935%25u21E5Clockwise%20curve%20rotation%0ALOG%3AParagraph%23%A7%20Row%23%25u2080%20-%20Call%20from%20line%23%25u2070%20%B2%28%B9%28...%29%29%0A%25u21E5%09%25u2399%25u2811%25u2804%25u23F1%25u2818%25u2804%25u23F1%25u2808%25u2806%25u23F1%25u2800%25u2807%25u23F1%25u2820%25u2803%25u23F1%25u2830%25u2801%25u23F1%25u2935%25u21E5%09Goto%20down%0A%25u21E5%09%25u2399%25u2814%25u2801%25u23F1%25u2814%25u2802%25u23F1%25u2824%25u2802%25u23F1%25u2824%25u2804%25u23F1%25u2822%25u2804%25u23F1%25u2812%25u2804%25u23F1%25u2935%25u21E5%09Goto%20down%0A%25u21E5%09%25u2399%25u2811%25u2804%25u23F1%25u2831%25u2800%25u23F1%25u2823%25u2800%25u23F1%25u2807%25u2800%25u23F1%25u280E%25u2800%25u23F1%25u281C%25u2800%25u23F1%25u2935%25u21E5%09Goto%20down%0A%25u21E5%09%25u2399%25u2814%25u2801%25u23F1%25u2812%25u2801%25u23F1%25u280A%25u2801%25u23F1%25u2809%25u2801%25u23F1%25u2809%25u2802%25u23F1%25u2811%25u2802%25u23F11%A7%25u21E5%09Goto%20up%20to%20row%20%231%0A2%25u20E3%20%25u33B3%25u2152%25u33B3%25u2935%25u21E5Bidirectional%20line%20rotation%0ALOG%3AParagraph%23%A7%20Row%23%25u2080%20-%20Call%20from%20line%23%25u2070%20%B2%28%B9%28...%29%29%0A%25u21E5%09%25u2399%25u2814%25u2801%25u23F1%25u2812%25u2802%25u23F1%25u2811%25u2804%25u23F1%25u2838%25u2800%25u23F1%25u21E5%09Clockwise%20cycle%0A%25u21E5%09%25u2399%25u2811%25u2804%25u23F1%25u2812%25u2802%25u23F1%25u2814%25u2801%25u23F1%25u2838%25u2800%25u23F1%25u21E5%09Anticlockwise%20cycle%0A%0AHere%20is%20beginning%20the%20main%20body%20of%20scenario%0ASet%20keyboards%20order%20to%204251%20and%20select%20%234%20as%20first/left%0A%25u21E5%091524%25u23281%25u2328%0ASet%201000ms%20of%20delay%20and%20use%20500Hz%20of%20refresh%0A%25u21E5%091000%25u33B3500%25u3390%0ACountdown%20to%20start%20%25u2081%0A%25u21E5%093%28%25u2399%25u23F1%25u239A%25u23F1%29%0AUse%20paragraph%20%232%20and%20initialize%20to%201/12%20of%201000ms%20delay%0ALoop%20for%205%20times%20%ABclockwise/anticlockwise%BB%20turns%20with%20ping-pong%0A%25u21E5%096%28%25u2399%BE%25u23F1%25u239A%BC%25u23F1%29%0A%25u21E5%092%25u20E3%20%A7%28%25u33B3%25u2153%BC%25u33B3%25u2935%29%205%281%A7%207%28%25u219D%25u21E1%25u2328%292%A7%207%28%25u21E3%25u2328%25u219D%29%29%0AUse%20paragraph%20%232%20and%20initialize%20to%201/25%20of%201000ms%20delay%0ALoop%20for%205%20times%20%ABclockwise/anticlockwise%BB%20speedy%20ping-pong%20runs%0A%25u21E5%096%28%25u2399%BE%25u23F1%25u239A%BC%25u23F1%29%0A%25u21E5%092%25u20E3%20%A7%28%25u33B3%25u2155%25u2155%25u33B3%25u2935%29%205%281%A7%207%28%25u219D%25u21E1%25u2328%292%A7%207%28%25u21E3%25u2328%25u219D%29%29%0ALoop%20for%205%20times%20of%20%ABcurved%20rotations%BB%0A%25u21E5%096%28%25u2399%BE%25u23F1%25u239A%BC%25u23F1%29%0A%25u21E5%091%25u20E3%205%284%28%25u219D%29%29%0AThank%20You%20for%20watch%21%20%3A%29%0A%25u21E5%09%25u215B%25u33B310%28%25u2887%25u21E1%25u2328%25u23F1%29%0A%25u21E5%09%25u215B%25u33B310%28%25u23995%28%25u21E3%25u2833%25u21E1%25u2328%25u23F1%29%25u239A5%28%25u21E3%25u281E%25u21E1%25u2328%25u23F1%29%29%0AHalt%20this%20demo%0A%25u21E5%091%25u2328%25u2817%25u23F13%25u2328%25u2807%25u23F15%25u2328%25u2827%25u23F17%25u2328%25u2839%25u23F19%25u2328%25u2881%25u23F1%0A%25u21E5%095%289%28%25u21E1%25u283F%25u283F%25u283F%25u283F%25u2807%25u23F1%299%28%25u21E3%25u283F%25u283F%25u283F%25u283F%25u2807%25u23F1%29%29) in Glazeurist
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
⇥	2⃣ §(㎳⅓¼㎳⤵) 5(1§ 7(↝⇡⌨)2§ 7(⇣⌨↝))
Use paragraph #2 and initialize to 1/25 of 1000ms delay
Loop for 5 times «clockwise/anticlockwise» speedy ping-pong runs
⇥	6(⎙¾⏱⎚¼⏱)
⇥	2⃣ §(㎳⅕⅕㎳⤵) 5(1§ 7(↝⇡⌨)2§ 7(⇣⌨↝))
Loop for 5 times of «curved rotations»
⇥	6(⎙¾⏱⎚¼⏱)
⇥	1⃣ 5(4(↝))
Thank You for watch! :)
⇥	⅛㎳10(⢇⇡⌨⏱)
⇥	⅛㎳10(⎙5(⇣⠳⇡⌨⏱)⎚5(⇣⠞⇡⌨⏱))
Halt this demo
⇥	1⌨⠗⏱3⌨⠇⏱5⌨⠧⏱7⌨⠹⏱9⌨⢁⏱
⇥	5(9(⇡⠿⠿⠿⠿⠇⏱)9(⇣⠿⠿⠿⠿⠇⏱))
</pre>
</details>

Try [demo-scenario #1](http://htmlpreview.github.io/?http://github.com/Alikberov/Glazeur/blob/master/Glazeurist.html?script=Here%20is%20beginning%0A%25u21E5%091524%25u23281%25u2328500%25u23F11000%25u267B%0A%25u21E5%096%282%25u2152%25u23F12%28%25u2814%25u2881%25u23F1%25u2812%25u2882%25u23F1%25u2811%25u2884%25u23F1%25u2838%25u2880%25u23F11%25u2152%25u23F1%29%25u2887%25u21E1%25u2328%29%0A%25u21E5%094%28%25u21E3%25u2328%25u283F%25u283F%25u283F%25u23F19%28%25u21E3%25u283F%25u283F%25u283F%25u283F%25u2807%25u23F1%29%29%0A%25u21E5%096%289%28%25u21E3%25u283F%25u2807%25u23F1%299%28%25u21E1%25u2815%25u2805%25u23F1%299%28%25u21E3%25u2815%25u2805%25u21E1%25u282A%25u2802%25u23F1%299%28%25u21E1%25u283F%25u2807%25u23F1%29%25u21E1%25u2328%29%0A%25u21E5%099%289%28%25u21E3%25u2801%25u21E1%25u2328%29%25u23F1%299%289%28%25u21E3%25u2802%25u21E1%25u2328%29%25u23F1%299%289%28%25u21E3%25u2804%25u21E1%25u2328%29%25u23F1%29%0A%25u21E5%099%289%28%25u21E1%25u2801%25u21E1%25u2328%29%25u23F1%299%289%28%25u21E1%25u2802%25u21E1%25u2328%29%25u23F1%299%289%28%25u21E1%25u2804%25u21E1%25u2328%29%25u23F1%29)

Try [demo-scenario #2](http://htmlpreview.github.io/?http://github.com/Alikberov/Glazeur/blob/master/Glazeurist.html?script=Paragraphes%20declaration%20started%20at%20here%0A1%25u20E3%200%25u23F1%25u215A%25u23F1%25u2935%25u21E5Clockwise%20curve%20rotation%0ALOG%3AParagraph%23%A7%20Row%23%25u2080%20-%20Call%20from%20line%23%25u2070%20%B2%28%B9%28...%29%29%0A%25u21E5%09%25u2399%25u2811%25u2804%25u23F1%25u2818%25u2804%25u23F1%25u2808%25u2806%25u23F1%25u2800%25u2807%25u23F1%25u2820%25u2803%25u23F1%25u2830%25u2801%25u23F1%25u2935%25u21E5%09Goto%20down%0A%25u21E5%09%25u2399%25u2814%25u2801%25u23F1%25u2814%25u2802%25u23F1%25u2824%25u2802%25u23F1%25u2824%25u2804%25u23F1%25u2822%25u2804%25u23F1%25u2812%25u2804%25u23F1%25u2935%25u21E5%09Goto%20down%0A%25u21E5%09%25u2399%25u2811%25u2804%25u23F1%25u2831%25u2800%25u23F1%25u2823%25u2800%25u23F1%25u2807%25u2800%25u23F1%25u280E%25u2800%25u23F1%25u281C%25u2800%25u23F1%25u2935%25u21E5%09Goto%20down%0A%25u21E5%09%25u2399%25u2814%25u2801%25u23F1%25u2812%25u2801%25u23F1%25u280A%25u2801%25u23F1%25u2809%25u2801%25u23F1%25u2809%25u2802%25u23F1%25u2811%25u2802%25u23F11%A7%25u21E5%09Goto%20up%20to%20row%20%231%0A2%25u20E3%200%25u23F1%25u2152%25u23F1%25u2935%25u21E5Bidirectional%20line%20rotation%0ALOG%3AParagraph%23%A7%20Row%23%25u2080%20-%20Call%20from%20line%23%25u2070%20%B2%28%B9%28...%29%29%0A%25u21E5%09%25u2399%25u2814%25u2801%25u23F1%25u2812%25u2802%25u23F1%25u2811%25u2804%25u23F1%25u2838%25u2800%25u23F1%25u21E5%09Clockwise%20cycle%0A%25u21E5%09%25u2399%25u2811%25u2804%25u23F1%25u2812%25u2802%25u23F1%25u2814%25u2801%25u23F1%25u2838%25u2800%25u23F1%25u21E5%09Anticlockwise%20cycle%0A%0AHere%20is%20beginning%20the%20main%20body%20of%20scenario%0ASet%20keyboards%20order%20to%204251%20and%20select%20%234%20as%20first/left%0A%25u21E5%091524%25u23281%25u2328%0ASet%20250Hz%20of%20delay%20and%20use%20100Hz%20of%20refresh%0A%25u21E5%09250%25u23F1100%25u267B%0ACountdown%20to%20start%20%25u2081%0A%25u21E5%093%28%25u23994%28%25u23F1%29%25u239A4%28%25u23F1%29%29%0AUse%20paragraph%20%232%20and%20initialize%20to%204/5%20of%2050Hz%20delay.%0ALoop%20for%205%20times%20%ABclockwise/anticlockwise%BB%20turns%20with%20ping-pong%0A%25u21E5%093%28%25u23994%28%25u23F1%29%25u239A4%28%25u23F1%29%29%0A%25u21E5%092%25u20E3%20%A7%280%25u23F1%25u2153%25u23F1%25u2935%29%205%281%A7%207%28%25u219D%25u21E1%25u2328%292%A7%207%28%25u21E3%25u2328%25u219D%29%29%0ALoop%20for%205%20times%20%ABclockwise/anticlockwise%BB%20speedy%20ping-pong%20runs%0A%25u21E5%093%28%25u23994%28%25u23F1%29%25u239A4%28%25u23F1%29%29%0A%25u21E5%092%25u20E3%20%A7%280%25u23F1%25u2151%25u23F1%25u2935%29%205%281%A7%207%28%25u219D%25u21E1%25u2328%292%A7%207%28%25u21E3%25u2328%25u219D%29%29%0ALoop%20for%205%20times%20of%20%ABcurved%20rotations%BB%0A%25u21E5%093%28%25u23994%28%25u23F1%29%25u239A4%28%25u23F1%29%29%0A%25u21E5%091%25u20E3%205%284%28%25u219D%29%29%0AThank%20You%20for%20watch%21%20%3A%29%0A%25u21E5%092%25u215C%25u23F110%28%25u2887%25u21E1%25u2328%25u23F1%29%0A%25u21E5%090%25u23F19%28%25u23997%28%25u21E3%25u2833%25u21E1%25u2328%25u23F1%29%25u239A5%28%25u21E3%25u281E%25u21E1%25u2328%25u23F1%29%29)

## Demonstration / Демонстрация
[![Utility in work demonstration](https://img.youtube.com/vi/BoI2meUvO4Y/0.jpg)](http://youtu.be/BoI2meUvO4Y)
