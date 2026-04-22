//  BlurTestFrame.cpp
/*
 ***************************************************************
 * Name:      BlurTestFrame.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergi E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-22
 * Copyright: Sergi E. Heckel (www.Solar_Wind1956.com)
 * License:
 *************************************************************
*/
#include "BlurTestFrame.h"
/*
    Класс разнообразного тестирования BlurTestFrame
*/
//	-----------------------------------------------------------------------------------------------------
/*
    Конструктор выполняет следующие блоки операций:
    I. 		Создание панели для предварительного просмотра и прокручивающихся окон
    II.		Создание системы сайзеров
    III.	Привязка обработчиков событий
    IV.		Считывание изображения из дискового файла с использование стандартного диалога открытия файлов
    V.     	Фильтрация изображения OpenCV
    VI.		Прорисовка изображений OpenCV в формате библиотеки wxVidgets
*/
BlurTestFrame::BlurTestFrame    ( wxWindow  *   parent)
                        : wxPanel   (   parent          //  Инициализируем как панель
                                    ,   wxID_ANY
                                    )
{
    //  I.  ------------------------------------------------------------------------------------------------
    //  Создадим панель для предварительного просмотра оригинального изображения и
    //  прокручивающиеся окна для изображений
    informPanel                 =   new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1));
    informPanel->SetMinSize(wxSize(-1, 50));

    informPanel->SetBackgroundColour(*wxWHITE); // Чтобы визуально отделить
    informPanel->SetBackgroundColour(*wxGREEN); // Чтобы визуально отделить
    m_filter_name_text          =   new wxStaticText(informPanel, -1, wxT("Blur Filter Test") , wxPoint(0, 0));
    wxFont      m_filter_name_text_font(24, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    m_filter_name_text->SetFont(m_filter_name_text_font);
    m_filter_name_text->SetForegroundColour(wxColour(255, 255, 0));
    //m_filter_name_text->SetBackgroundColour(*wxGREEN);

    previewPanel                =   new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1));
    previewPanel->SetMinSize(wxSize(200, -1));
    previewPanel->SetBackgroundColour(*wxLIGHT_GREY); // Чтобы визуально отделить
    staticPreviewBitmap         =   new wxStaticBitmap(previewPanel, wxID_ANY, wxNullBitmap);

    scrolled_wind_original      =   new wxScrolledWindow(this);
    scrolled_wind_filtered      =   new wxScrolledWindow(this);

    //  II.    --------------------------------------------------------------------------------------------
    //  Сайзеры
    //  II.1. Сайзеры для отображения различной спомогательной инофрмации о тестировании
    wxBoxSizer *            informHSizer       =   new wxBoxSizer(wxHORIZONTAL);
    informHSizer->Add(informPanel, 1, wxCENTER | wxALL, 5);
    //
    //  II.2. Сайзеры для отображения различных вариантов исследуемого изображения
    //  Используем сайзер, чтобы центрировать превью внутри панели
    wxBoxSizer *            previewSizer    =   new wxBoxSizer(wxVERTICAL);
    previewSizer->Add(staticPreviewBitmap, 1, wxCENTER | wxALL, 5);
    previewPanel->SetSizer(previewSizer);

    wxBoxSizer *            imagesHSizer       =   new wxBoxSizer(wxHORIZONTAL);
    //  Добавляем в главный сайзер превью-панель
    imagesHSizer->Add(previewPanel, 0, wxEXPAND | wxALL, 5);               // Фикс
    imagesHSizer->Add(scrolled_wind_original, 1, wxEXPAND | wxALL, 5);       // Растягивается
    imagesHSizer->Add(scrolled_wind_filtered, 1, wxEXPAND | wxALL, 5);     // Растягивается

    //  ------------------------------------------------------------------------------------------------------
    //  Подготовка панели управления отображением изображений
    zoom_slider_ctrl            =   new ZoomSliderCtrl(this);
    kernel_matrix_ctrl          =   new KernelMatrixCtrl(this, wxID_ANY, wxPoint(0, 0), wxSize(100, 100));
    kernel_matrix_ctrl->SetMinSize(wxSize(100, 100));           // Фиксируем минимальное присутствие

    //  II.3. Сайзеры для панели настроек и управления
    wxBoxSizer*         controlPanelHSizer   = new wxBoxSizer(wxHORIZONTAL);
    controlPanelHSizer->SetMinSize(wxSize(-1, 150)); // -1 значит "любая ширина", 150 - фиксированная высота

    // Слайдер масштаба в отдельном столбике
    wxBoxSizer*         zoomCol             = new wxBoxSizer(wxVERTICAL);
    zoomCol->Add(new wxStaticText(this, -1, "Масштаб"), 0, wxALL, 5);
    zoomCol->Add(zoom_slider_ctrl, 0, wxEXPAND | wxALL, 5);

    // Матрица ядра в другом
    wxBoxSizer*         kernelCol           = new wxBoxSizer(wxVERTICAL);
    kernelCol->Add(new wxStaticText(this, -1, "Ядро"), 0, wxALL, 5);
    kernelCol->Add(kernel_matrix_ctrl, 0, wxALL, 5);

    controlPanelHSizer->Add(zoomCol, 0, wxEXPAND | wxALL, 10);
    controlPanelHSizer->Add(kernelCol, 0, wxEXPAND | wxALL, 10);

    //  II.4. Объединяем сайзеры отображения изображений и панели управления отображением
    //  И наконец добавляем горизонтальные сайзеры в главный вертикальный сайзер
    wxBoxSizer *            mainSizer    =   new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(informHSizer, 0, wxEXPAND | wxALL, 5);       // Панель только столько, сколько ей нужно
    mainSizer->Add(imagesHSizer, 1, wxEXPAND | wxALL, 5);       // Картинки забирают всё свободное место
    mainSizer->Add(controlPanelHSizer, 0, wxEXPAND | wxALL, 5); // Панель только столько, сколько ей нужно

    this->SetSizer(mainSizer);

    //  III.     ------------------------------------------------------------------------------------------------
    //  Привязка обработчиков событий
    //  В конструкторе теперь только "регистрация"
    zoom_slider_ctrl->Bind(wxEVT_SLIDER, &BlurTestFrame::OnZoomSlider, this);
    kernel_matrix_ctrl->Bind(EVT_KERNEL_CHANGED, &BlurTestFrame::OnKernelChanged, this);

    Bind(wxEVT_SIZE, &OnResize, this);

    //  IV. -----------------------------------------------------------------------------------------------------

    //  На случай сбоя при чтении файла cоздаем черное изображение 400x400, тип CV_8UC3 (8 бит, 3 канала RGB)
    m_cv_original_img = cv::Mat::zeros(400, 400, CV_8UC3);

    // По желанию: рисуем на нем текст, чтобы было понятно, что файл не загружен
    cv::putText (   m_cv_original_img
                ,   "No Image Loaded"
                ,   cv::Point(50, 200)
                ,   cv::FONT_HERSHEY_SIMPLEX
                ,   1.0
                ,   cv::Scalar(255, 255, 255) // Белый цвет
                ,   2
                );
    //  Выбираем файл с изображением для считывания
    wxString    fullFileName;               //  Переменная для получения полноо имени файла

    if (!getFullFileName(fullFileName)){
        cout << bright_red << "Ошибка получения полного имени файла. Завершение работы." << reset << endl;
    } else {
        // 	Считываем изображение из файла
        cv::Mat  tempRGB = cv::imread(std::string(fullFileName), cv::COLOR_GRAY2RGB);   //  cv::IMREAD_GRAYSCALE
        if(tempRGB.empty()){
            cout << bright_red << std::string(fullFileName) + "Ошибка загрузки изображения. Завершение работы." << reset << endl;
		} else {
            cv::cvtColor(tempRGB, m_cv_original_img, cv::COLOR_BGR2RGB);
		}
	};

	//  V. --------------------------------------------------------------------------------------------------------------------
    //  Фильтрация оригинального изображения OpenCV
    ApplyFilters();

    //  VI. ----------------------------------------------------------------------------------------------------------------
    //  Отображение всех изображений
    UpdateAllViews();

    //  ----------------------------------------------------------------------------------------------------------------
    Center () ;
}

void BlurTestFrame::OnResize(wxSizeEvent& event)
{
    // Проверяем, созданы ли уже нужные объекты
    if (!staticPreviewBitmap || !originalBitmap.IsOk()) {
        event.Skip();
        return;
    }

    UpdateAllViews();

    Layout();

    event.Skip();
}

void BlurTestFrame::OnZoomSlider(wxCommandEvent& event) {
    zoom_slider_ctrl->m_zoom = event.GetInt();
    UpdateAllViews();
}

void BlurTestFrame::OnKernelChanged(wxCommandEvent& event) {
    // 1. Извлекаем размер ядра, который мы положили в SetInt() в контроле
    int newSize = event.GetInt();

    // 2. Обновляем переменную класса
    m_kernel_size = newSize;

    // 3. Запускаем обработку OpenCV
    ApplyFilters();

    // 4. Обновляем картинки
    UpdateAllViews();

    // 5. Если матрица поменяла размер (например, стала 7x7 вместо 3x3),
    // заставляем сайзеры пересчитать всё окно
    Layout();
}

// Универсальная функция, которая знает про оба окна
void BlurTestFrame::UpdateAllViews() {
    //  Визуализация оригинального и отфильтрованного изображения (выводим оба состояния)

    //  Преобразования для вывода оригнинального изображения OpenCV в формате библиотеки wxVidgets
    UpdateDisplay   (   m_wx_img
                    ,   m_cv_original_img
                    ,   originalBitmap
                    ,   staticOriginalBitmap
                    ,   scrolled_wind_original
                    ,   zoom_slider_ctrl->m_zoom
                    );

    //  Преобразования для вывода отфильтрованного изображения OpenCV в формате библиотеки wxVidgets
    UpdateDisplay   (   m_wx_img
                    ,   m_cv_filtered_img
                    ,   filteredBitmap
                    ,   staticFilteredBitmap
                    ,   scrolled_wind_filtered
                    ,   zoom_slider_ctrl->m_zoom
                    );
    //  Внутри панели создаем битмэп и статический битмап для вывода предварительного оригинального изображения
    int     previewW    = previewPanel->GetSize().x;

    previewBitmap       = GetScaledBitmap(originalBitmap, previewW, previewW);
    staticPreviewBitmap->SetBitmap(previewBitmap);
    previewPanel->Layout();
    previewPanel->Refresh();
}
//  -----------------------------------------------------------------------------------------------------
//  Экосистема OpenCV
void BlurTestFrame::ApplyFilters() {
    // Вся "кухня" OpenCV здесь
    cv::blur    (   m_cv_original_img
                ,   m_cv_filtered_img
                ,   cv::Size(m_kernel_size, m_kernel_size)
                );
}

