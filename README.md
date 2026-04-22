# Mussels-OpenCV-wxWingets-CodeBlocks
Исследование поведения мидий в рамках динамического предупреждения загрязнения морских акваторий с использованием библиотек OpenCV и wxWingets в каркасе IDE CoderBlocks.
##  * Authors:    
  Sergi E. Heckel (Solar_Wind1956@yahoo.com)
           with insightful support from Gemini (AI)
                 "Дорогу осилит идущий, а вдвоем идти спорее"

## Источники и вдохновение / Bibliography

Проект опирается на классические труды в области компьютерного зрения и обработки изображений:

* **Теоретический фундамент:** *Р. Гонсалес, Р. Вудс. «Цифровая обработка изображений»*.
    > Эта книга помогла понять математическую суть фильтрации и морфологии, что критически важно для выделения контуров объектов в биологических средах.
    
* **Практическая реализация:** *А. Кэлер, Г. Брэдски. «Изучаем OpenCV 3» (или 4)*.
    > Незаменимое руководство по эффективному использованию функций библиотеки и управлению структурами данных в C++.                 
    > Гарсия,  Суареса и др.  "Обработка изображений с помощью OpenCV".
  
* **Проектирование GUI:** *Jan Bodnar. "wxWidgets C++ tutorial" (ZetCode)*.
    > Благодаря его четким и понятным урокам удалось выстроить логичную систему сайзеров и создать удобную панель управления фильтрами.
                 
## 📂 Структура проекта
OpenCV_Tests/
│
├── OpenCV_TestsApp.cpp				# 
├── OpenCV_Tester.cpp
├── OpenCV_Tester.h
│
├── images/
│   ├── blue_mussels.jpg     		# 
│   └── ...
│
├── videos/
│   ├── about_mussels_silent.mp4	# 
│   └── ...							# 
│
├── include/
│   ├── color_output.hpp       		# ANSI-цвета для красивого терминального вывода
│   ├── ...
│   └── others.hpp
│   
├── control_panel_src/
│	├── KernelMatrixCtrl.cpp
│	├── KernelMatrixCtrl.h
│	├── ZoomSliderCtrl.cpp
│	├── ZoomSliderCtrl.h
│ 
├── opencv_filters_src/
│	├── BlurTestFrame.cpp
│	├── BlurTestFrame.h
│	├── medianBlurTestFrame.cpp
│	├── medianBlurTestFrame.h
│   ├── ...
│
├── auxiliary_src/
│	├── AuxiliaryFreeFunctions.cpp
│	├── AuxiliaryFreeFunctions.h
│   ├── ...
│
└── README.md

