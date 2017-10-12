# Glazeur
Lots keyboard illumination automat
## Description (ru)

Сигнатура | Краткое описание назначения и действия | Пример использования | Область действия¹
--------- | -------------------------------------- | -------------------- | ----------------
i⃣	| Выборка активного параграфа эффектов				| 0⃣ …9⃣			| Сценарий
§i	| Выборка активного параграфа эффектов				| §0…§9			| Сценарий
n§	| Выбор строки эффектов в активном параграфе			| 0§…99§		| Сценарий/Параграф
n§i	| Выбор параграфа эффектов					| 99§0…45§6…0§9		| Сценарий
i⌨	| Выборка базовой (первой слева) клавиатуры			| 1⌨…9⌨			| Сценарий/Параграф
n⌨	| Задание логического порядка клавиатур				| 12⌨…987654321⌨	| Сценарий
n♻	| Задание частоты обновления индикаторов (Hz)			| 1♻…1000♻		| Сценарий/Параграф
n⏱	| Установка базового интервала задержки сценария		| 1⏱…1000⏱		| Сценарий
0⏱	| Установка базового интервала задержки в параграфе		| 0⏱			| Параграф
n⁄d⏱	| Организация паузы в параграфе					| 1⏱…2⅛⏱…3⅘⅞⏱		| Сценарий/Параграф
⏱	| Организация паузы «проигрывания» эффекта			| ⏱			| Сценарий/Параграф
n⤴	| Переход строками выше в активном параграфе			| ⤴…1⤴…99⤴		| Сценарий/Параграф
n⤵	| Переход строками ниже в активном параграфе			| ⤵…1⤵…99⤵		| Сценарий/Параграф
↝	| Запуск «проигрывания» эффекта активной строки параграфа	| ↝			| Сценарий
n(…)	| Организация зацикливания n-раз				| 1(↝)…99(↝⤵)		| Сценарий/Параграф
§(…)	| Управление выбранным параграфом				| §(0⏱⅘⏱⤵)		| Сценарий
⠿	| Управление индикаторами клавиатуры				| 5(⠪⠝⏱⠿⠽⏱)		| Сценарий/Параграф
⇡/⇣	| Управление яркостью² индикаторов				| 10(⇣⠿⠿⠿⏱⇡⠿⠿⠿⏱)		| Сценарий/Параграф

¹- действие эффекта может несколько отличаться в параграфе и за его пределами

²- не все модели клавиатур поддерживают ШИМ-алгоритм

## Sample (ru)
<pre>
1⃣ 0⏱⅒⏱⤵⇥Clockwise curve rotation
⇥	⠑⠄⏱⠘⠄⏱⠈⠆⏱⠀⠇⏱⠠⠃⏱⠰⠁⏱⤵⇥	Goto down
⇥	⠔⠁⏱⠔⠂⏱⠤⠂⏱⠤⠄⏱⠢⠄⏱⠒⠄⏱⤵⇥	Goto down
⇥	⠑⠄⏱⠱⠀⏱⠣⠀⏱⠇⠀⏱⠎⠀⏱⠜⠀⏱⤵⇥	Goto down
⇥	⠔⠁⏱⠒⠁⏱⠊⠁⏱⠉⠁⏱⠉⠂⏱⠑⠂⏱1⃣⇥	Goto up to row #1
2⃣ 0⏱⅒⏱⤵⇥Bidirectional line rotation
⇥	⠑⠄⏱⠸⠀⏱⠔⠁⏱⠒⠂⏱⇥	Clockwise cycle
⇥	⠑⠄⏱⠒⠂⏱⠔⠁⏱⠸⠀⏱⇥	Anticlockwise cycle
Here is beginning
Set keyboards order to 4251 and select #4 as first/left
⇥	1524⌨1⌨
Set 50Hz of delay and use 100Hz of refresh
⇥	50⏱100♻
Use paragraph #2 and initialize to 4/5 of 50Hz delay.
Loop 5 times for 4 times of «anticlockwise» and 3 times of «clockwise» rotation FX
⇥	2⃣ §(0⏱⅘⏱⤵) 5(2§ 4(↝)1§ 3(↝))
⇥	2⃣ 0§↝ 5(2§ 4(↝)1§ 4(↝))⇥	Goto head-row of paragraph #2 and loop
⇥	1⃣ 5(4(↝))⇥	⇥	⇥	Loop for 5 over 4 times of paragraph #1 FX
</pre>

## Demonstration
[![Utility in work demonstration](https://img.youtube.com/vi/BoI2meUvO4Y/0.jpg)](http://youtu.be/BoI2meUvO4Y)