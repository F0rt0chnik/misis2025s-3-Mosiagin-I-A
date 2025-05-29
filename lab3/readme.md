Задача
Реализовать функцию автоконтрастирования для

Серых изображений (8 bpp)

Цветных изображений (наивный вариант)

Провести тестирование и визуализацию гистограмм и результатов


Результаты на серых изображениях

Исходное изображение:
<center><img src="./test3/greycat.jpg" alt="drawing" width="350"/></center>

Гистограмма до автоконтраста:

<center><img src="./report-imgs/grey/cat-orig-hists.jpg" alt="drawing" width="350"/></center>

Изображение после автоконтраста:

<center><img src="./report-imgs/grey/cat-good-contrasted.jpg" alt="drawing" width="350"/></center>

Гистограмма после автоконтраста:

<center><img src="./report-imgs/grey/cat-good-contr-hists.jpg" alt="drawing" width="350"/></center>


Результаты на цветных изображениях (наивная версия)

Исходное изображение:
<center><img src="./test3/bibizan.jpg" alt="drawing" width="350"/></center>

Гистограмма до автоконтраста:

<center><img src="./report-imgs/autocontrast/bibizan-orig-hists.jpg" alt="drawing" width="350"/></center>

Изображение после автоконтраста:

<center><img src="./report-imgs/autocontrast/bibizan-good-contrasted.jpg" alt="drawing" width="350"/></center>

Гистограмма после автоконтраста:

<center><img src="./report-imgs/autocontrast/bibizan-good-contr-hists.jpg" alt="drawing" width="350"/></center>

Ограниченное автоконтрастирование цветных изображений

Исходное изображение:
<center><img src="./test3/bibizan.jpg" alt="drawing" width="350"/></center>

Гистограмма до автоконтраста:

<center><img src="./report-imgs/rgb-results/bibizan-orig-hists.jpg" alt="drawing" width="350"/></center>

Изображение после автоконтраста:

<center><img src="./report-imgs/rgb-results/bibizan-good-contrasted.jpg" alt="drawing" width="350"/></center>

Гистограмма после автоконтраста:

<center><img src="./report-imgs/rgb-results/bibizan-good-contr-hists.jpg" alt="drawing" width="350"/></center>

Выводы
На серых изображениях автоконтрастирование улучшает детализацию и распределение яркости

На цветных изображениях наивный автоконтраст порой вызывает цветовые искажения

Ограниченное автоконтрастирование сохраняет естественный цвет, при этом улучшая контраст

Представленные визуализации и гистограммы подтверждают качественную работу алгоритмов

Все визуализации и результаты сохранены в каталоге lab3/report-imgs.
