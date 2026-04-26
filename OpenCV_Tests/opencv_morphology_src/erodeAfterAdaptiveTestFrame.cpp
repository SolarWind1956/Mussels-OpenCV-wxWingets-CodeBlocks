//  erodeAfterAdaptiveTestFrame.cpp
/*
 ***************************************************************
 * Name:      erodeAfterAdaptiveTestFrame.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-23
 * Copyright: Sergi E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 *************************************************************
*/
#include "erodeAfterAdaptiveTestFrame.h"
/*
    Класс тестирования преобрзования `erodeAfterAdaptiveTestFrame`
*/
//	-----------------------------------------------------------------------------------------------------
/*
    Конструктор выполняет следующие блоки операций:
    I. 		Создание  информационной панели, панели для предварительного просмотра и прокручивающихся окон
    II.		Создание системы сайзеров
    III.	Привязка обработчиков событий
    IV.		Считывание изображения из дискового файла с использование стандартного диалога открытия файлов
    V.     	Фильтрация изображения OpenCV
    VI.		Прорисовка изображений OpenCV в формате библиотеки wxVidgets
*/
erodeAfterAdaptiveTestFrame::erodeAfterAdaptiveTestFrame  ( wxWindow  *   parent)
                                                : wxPanel   (   parent          //  Инициализируем как панель
                                                            ,   wxID_ANY
                                                            )
{
    //  I.  ------------------------------------------------------------------------------------------------
    //  Создадим информационную панель,
    //  панель для предварительного просмотра оригинального изображения и
    //  прокручивающиеся окна для изображений
    m_informPanel                 =   new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_informPanel->SetMinSize(wxSize(-1, -1));
    m_informPanel->SetBackgroundColour(*wxLIGHT_GREY);             // Чтобы визуально отделить

    m_filtering_namePanel         =   new wxPanel(m_informPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_filtering_namePanel->SetBackgroundColour(*wxCYAN);             // Чтобы визуально отделить
    m_filtering_name_text       =
            new wxStaticText    (   m_filtering_namePanel
                                ,   -1
                                ,   wxT("`erode`\nmorphological \ntransform \nafter \n`adaptiveThreshold` filter")
                                ,   wxPoint(0, 0)
                                );
    wxFont      filtering_name_text_font(16, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    m_filtering_name_text->SetFont(filtering_name_text_font);
    m_filtering_name_text->SetForegroundColour(*wxBLACK);

    m_signaturePanel         = new wxPanel(m_informPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_signaturePanel->SetBackgroundColour(*wxWHITE);
    m_signature_name_text  = new wxStaticText(m_signaturePanel, -1, getSignatureText(),
                                              wxPoint(0, 0));
    wxFont      signature_text_font(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    m_signature_name_text->SetFont(signature_text_font);
    m_signature_name_text->SetForegroundColour(*wxBLACK);

    m_debugPanel             = new wxPanel(m_informPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_debugInfo              = new wxTextCtrl   (   m_debugPanel
                                                ,   wxID_ANY
                                                ,   wxT("Отладочный виджет")
                                                ,   wxDefaultPosition
                                                ,   wxSize (880, 330)
                                                ,   wxTE_MULTILINE
                                                );

    //  -------------------------------------------------------------------------------------------------
    m_previewPanel                  =   new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1));
    m_previewPanel->SetMinSize(wxSize(200, -1));
    m_previewPanel->SetBackgroundColour(*wxLIGHT_GREY); // Чтобы визуально отделить
    m_staticPreviewBitmap           =   new wxStaticBitmap(m_previewPanel, wxID_ANY, wxNullBitmap);

    //  -------------------------------------------------------------------------------------------------
    m_scrolled_wind_original        =   new wxScrolledWindow(this);
    m_scrolled_wind_filtered        =   new wxScrolledWindow(this);
    m_scrolled_wind_transformed     =   new wxScrolledWindow(this);

    //  II.    --------------------------------------------------------------------------------------------
    //  Сайзеры
    //  ----------------------------------------------------------------------------------------------------
    //  II.1. Сайзеры для отображения различной вспомогательной инофрмации о тестировании
    //  Добавляем в сайзер: (объект, пропорция, флаги, отступ)
    wxBoxSizer *            informHSizer       =   new wxBoxSizer(wxHORIZONTAL);
    informHSizer->SetMinSize(wxSize(-1, -1)); // -1 значит "любая ширина", -1 значит "любая высота"

    informHSizer->Add(m_filtering_namePanel, 1, wxCENTER | wxALL, 5);
    informHSizer->Add(m_signaturePanel, 1, wxCENTER | wxALL, 5);
    informHSizer->Add(m_debugPanel, 1, wxCENTER | wxALL, 5);

    m_informPanel->SetSizer(informHSizer);


    //  ----------------------------------------------------------------------------------------------------
    //  II.2. Сайзеры для отображения различных вариантов исследуемого изображения
    //  Используем сайзер, чтобы центрировать превью внутри панели
    wxBoxSizer *            previewSizer    =   new wxBoxSizer(wxVERTICAL);
    previewSizer->Add(m_staticPreviewBitmap, 1, wxCENTER | wxALL, 5);
    m_previewPanel->SetSizer(previewSizer);

    wxBoxSizer *            titelSizerHor       =   new wxBoxSizer(wxHORIZONTAL);
    wxString titelStr = wxString::Format("%-150s %-150s %-150s %-150s",
    "Превью", "Оригинальное", "После adaptiveThredshold фильтра", "После Erode трансформации");
    titelSizerHor->Add(new wxStaticText(this, -1, titelStr), 0, wxALL, 5);

    wxBoxSizer *            imagesHSizer       =   new wxBoxSizer(wxHORIZONTAL);
    //  Добавляем в главный сайзер превью-панель
    imagesHSizer->Add(m_previewPanel, 0, wxEXPAND | wxALL, 5);                  // Фикс
    imagesHSizer->Add(m_scrolled_wind_original,     1, wxEXPAND | wxALL, 5);    // Растягивается
    imagesHSizer->Add(m_scrolled_wind_filtered,     1, wxEXPAND | wxALL, 5);    // Растягивается
    imagesHSizer->Add(m_scrolled_wind_transformed,  1, wxEXPAND | wxALL, 5);    // Растягивается

    //  ------------------------------------------------------------------------------------------------------
    //  Подготовка панели управления отображением изображений
    //  1.
    m_zoom_slider_ctrl      =   new ZoomSliderCtrl(this);

    //  2.  Максимальное пороговое значение для adaptiveTthreshold
    m_spin_maxValue_ctrl    =   new wxSpinCtrlDouble    (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   1.0     // Минимум: разрешаем 0 для авторасчета
                                                        ,   255.0    // Максимум:
                                                        ,   255     // Начальное значение
                                                        ,   1.0     // Шаг: золотая середина для визуального теста
                                                        );
    //  Дополнительно количество знаков после запятой
    m_spin_maxValue_ctrl->SetDigits(1);

    //  3.
    //  Метод adaptiveThreshold
    //   Два способа задания адаптивного порога (adaptiveThreshold)
    wxString    adaptiveMethods[] =  {   wxT("cv::ADAPTIVE_THRESH_MEAN_C")        //
                                    ,   wxT("cv::ADAPTIVE_THRESH_GAUSSIAN_C")    //
                                    };

    m_choice_adaptiveMethod_ctrl     =   new wxChoice    (   this
                                                                 ,   wxID_ANY
                                                                 ,   wxDefaultPosition
                                                                 ,   wxSize(180, -1)
                                                                 ,   WXSIZEOF(adaptiveMethods)
                                                                 ,   adaptiveMethods
                                                                 ,   0
                                                                 );
    m_choice_adaptiveMethod_ctrl->SetStringSelection(wxT("cv::ADAPTIVE_THRESH_GAUSSIAN_C"));

    //  4.
    //  Тип операции для adaptiveThreshold (thresholdType)
    wxString    adaptiveTypes[] =  {   wxT("cv::THRESH_BINARY")        //
                                    ,   wxT("cv::THRESH_BINARY_INV")    //
                                    };
    m_choice_adaptiveType_ctrl  =   new wxChoice   (   this
                                                     ,   wxID_ANY
                                                     ,   wxDefaultPosition
                                                     ,   wxSize(180, -1)
                                                     ,   WXSIZEOF(adaptiveTypes)
                                                     ,   adaptiveTypes
                                                     ,   0
                                                     );
    m_choice_adaptiveType_ctrl->SetStringSelection(wxT("cv::THRESH_BINARY"));

    //  5.
    //  Размер блоа
    m_spin_blockSize_ctrl   =   new wxSpinCtrl          (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   1
                                                        ,   100
                                                        ,   11
                                                        );
    //  6.
    //  Смещение
    m_spin_Offset_ctrl    =   new wxSpinCtrlDouble    (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   -10.0     // Минимум: разрешаем 0 для авторасчета
                                                        ,   10.0    // Максимум: 10 достаточно для экспериментов
                                                        ,   3.0     // Начальное значение
                                                        ,   1.     // Шаг: золотая середина для визуального теста
                                                        );
    //  7.
    //  Параметры ядра для морфологической трансформации
    m_kernel_width_ctrl     =   new KernelMatrixCtrl(this, wxID_ANY, wxPoint(0, 0), wxSize(100, 100));
    m_kernel_width_ctrl->SetMinSize(wxSize(100, 100));           // Фиксируем минимальное присутствие
    m_kernel_height_ctrl    =   new KernelMatrixCtrl(this, wxID_ANY, wxPoint(0, 0), wxSize(100, 100));
    m_kernel_height_ctrl->SetMinSize(wxSize(100, 100));           // Фиксируем минимальное присутствие

    //  8.
    //  Форма ядра
    m_kernel_shape_radiobox_ctrl    =   new wxRadioBox  (   this
                                                        ,   wxID_ANY
                                                        ,   wxT("Выбор формы ядра")
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   getKernelShape()
                                                        ,   1
                                                        ,   wxRA_SPECIFY_COLS
                                                        );
    //  9.
    //  Количество итераций
    m_iterations_spin_ctrl  =   new wxSpinCtrl   (   this
                                                 ,   wxID_ANY
                                                 ,   wxEmptyString
                                                 ,   wxDefaultPosition
                                                 ,   wxDefaultSize
                                                 ,   wxSP_ARROW_KEYS
                                                 ,   1
                                                 ,   10
                                                 ,   1
                                                 );
    //  10.
    //  Тип  экстраполяции границыграницы (borderType)
    wxString    borderTypes[] =    {   wxT("cv::BORDER_CONSTANT")  //  заполнение цветом (по умолчанию)
                                    ,   wxT("cv::BORDER_REPLICATE") //  повтор последнего пикселя (а-а-а-а)
                                    ,   wxT("cv::BORDER_REFLECT")   //  зеркало (абв|вба)
                                    };

    m_border_chois_ctrl     =   new wxChoice     (   this
                                                 ,   wxID_ANY
                                                 ,   wxDefaultPosition
                                                 ,   wxSize(180, -1)
                                                 ,   WXSIZEOF(borderTypes)
                                                 ,   borderTypes
                                                 ,   0
                                                 );
    m_border_chois_ctrl->SetStringSelection(wxT("cv::BORDER_CONSTANT"));

    //  11.
    //  Флаг переключения типа морфологической трансформации "Полутона" / "Цвет"
    m_gray_color_radiobox_ctrl  =   new wxRadioBox  (   this
                                                    ,   wxID_ANY
                                                    ,   wxT("Выбор режима 'erode'")
                                                    ,   wxDefaultPosition
                                                    ,   wxDefaultSize
                                                    ,   getGrayOrColor()
                                                    ,   1
                                                    ,   wxRA_SPECIFY_COLS
                                                    );

    //  ----------------------------------------------------------------------------------------------------
    //  II.3. Сайзеры для панели настроек и управления
    wxBoxSizer*         controlPanelHSizer   = new wxBoxSizer(wxHORIZONTAL);
    controlPanelHSizer->SetMinSize(wxSize(-1, 150)); // -1 значит "любая ширина", 150 - фиксированная высота

    // 1. Слайдер масштаба в отдельном столбике
    wxBoxSizer*         zoomCol             = new wxBoxSizer(wxVERTICAL);
    zoomCol->Add(new wxStaticText(this, -1, "Масштаб"), 0, wxALL, 5);
    zoomCol->Add(m_zoom_slider_ctrl, 0, wxEXPAND | wxALL, 5);

    //  Максимальные значения для threshold и для AdaptiveShreshold
    //  2.
    wxBoxSizer*         maxValue_Col       = new wxBoxSizer(wxVERTICAL);
    maxValue_Col->Add(new wxStaticText(this, -1, "maxValue Threshold"), 0, wxALL, 5);
    maxValue_Col->Add(m_spin_maxValue_ctrl, 0, wxALL, 5);

    //  3.
    //  Адаптивный метод для AdaptiveShreshold
    wxBoxSizer*         adaptiveMethodCol       = new wxBoxSizer(wxVERTICAL);
    adaptiveMethodCol->Add(new wxStaticText(this, -1, "Adaptive method"), 0, wxALL, 5);
    adaptiveMethodCol->Add(m_choice_adaptiveMethod_ctrl, 0, wxALL, 5);

    //  4.
    // Типы операции для AdaptiveShreshold
    wxBoxSizer*         adaptiveTypesCol       = new wxBoxSizer(wxVERTICAL);
    adaptiveTypesCol->Add(new wxStaticText(this, -1, "Тип операци Threshold"), 0, wxALL, 5);
    adaptiveTypesCol->Add(m_choice_adaptiveType_ctrl, 0, wxALL, 5);

    //  5.
    //  Размер блока и константа для AdaptiveShreshold
    wxBoxSizer*         blockSizeCol       = new wxBoxSizer(wxVERTICAL);
    blockSizeCol->Add(new wxStaticText(this, -1, "blockSize Adaptive"), 0, wxALL, 5);
    blockSizeCol->Add(m_spin_blockSize_ctrl, 0, wxALL, 5);

    //  6.
    //  Смещение
    wxBoxSizer*         OffsetCol       = new wxBoxSizer(wxVERTICAL);
    OffsetCol->Add(new wxStaticText(this, -1, "Смещение Adaptive"), 0, wxALL, 5);
    OffsetCol->Add(m_spin_Offset_ctrl, 0, wxALL, 5);

    //  7.
    // Матрицы для ширины и высоты ядра
    wxBoxSizer*         kernelHor           = new wxBoxSizer(wxHORIZONTAL);
    kernelHor->Add(new wxStaticText(this, -1, " Ширина\n и высота\n неквадратного\n ядра"), 0, wxALL, 5);
    kernelHor->Add(m_kernel_width_ctrl, 0, wxALL, 5);
    kernelHor->Add(m_kernel_height_ctrl, 0, wxALL, 5);

    //  8.
    // Радиобокс для выбора формы ядра
    wxBoxSizer*         kernelShapeCol       = new wxBoxSizer(wxVERTICAL);
    kernelShapeCol->Add(m_kernel_shape_radiobox_ctrl, 0, wxALL, 5);

    //  9.
    // Спин для выбора количества итераций
    wxBoxSizer*         iterationsCol       = new wxBoxSizer(wxVERTICAL);
    iterationsCol->Add(new wxStaticText(this, -1, "Итерации"), 0, wxALL, 5);
    iterationsCol->Add(m_iterations_spin_ctrl, 0, wxALL, 5);

    //  10.
    // Выпадающий список для выбора типа границ
    wxBoxSizer*         typeBodersCol       = new wxBoxSizer(wxVERTICAL);
    typeBodersCol->Add(new wxStaticText(this, -1, "Типы границ"), 0, wxALL, 5);
    typeBodersCol->Add(m_border_chois_ctrl, 0, wxALL, 5);

    //  11.
    //  Радиобокс для выбора типа морфологической обработки "Полутона/цвет"
    wxBoxSizer*         gray_colorCol       = new wxBoxSizer(wxVERTICAL);
    gray_colorCol->Add(m_gray_color_radiobox_ctrl, 0, wxALL, 5);

    //  -------------------------------------------------------------------------------------------------
    //  Объединяем элементы управления в один горизонтальный сайзер
    //  1.
    controlPanelHSizer->Add(zoomCol, 0, wxEXPAND | wxALL, 5);
    //  2.
    controlPanelHSizer->Add(maxValue_Col, 0, wxEXPAND | wxALL, 5);
    //  3.
    controlPanelHSizer->Add(adaptiveMethodCol, 0, wxEXPAND | wxALL, 5);
    //  4.
    controlPanelHSizer->Add(adaptiveTypesCol, 0, wxEXPAND | wxALL, 5);
    //  5.
    controlPanelHSizer->Add(blockSizeCol, 0, wxEXPAND | wxALL, 5);
    //  6.
    controlPanelHSizer->Add(OffsetCol, 0, wxEXPAND | wxALL, 5);
    //  7.
    controlPanelHSizer->Add(kernelHor, 0, wxEXPAND | wxALL, 10);
    //  8.
    controlPanelHSizer->Add(kernelShapeCol, 0, wxEXPAND | wxALL, 10);
    //  9.
    controlPanelHSizer->Add(iterationsCol, 0, wxEXPAND | wxALL, 10);
    //  10.
    controlPanelHSizer->Add(typeBodersCol, 0, wxEXPAND | wxALL, 10);
    //  11.
    controlPanelHSizer->Add(gray_colorCol, 0, wxEXPAND | wxALL, 10);

    //  II.4. Объединяем сайзеры отображения изображений и панели управления отображением
    //  И наконец добавляем горизонтальные сайзеры в главный вертикальный сайзер
    wxBoxSizer *            mainSizer    =   new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_informPanel, 0, wxEXPAND | wxALL, 5);       // Панель только столько, сколько ей нужно
    mainSizer->Add(titelSizerHor, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(imagesHSizer, 1, wxEXPAND | wxALL, 5);       // Картинки забирают всё свободное место
    mainSizer->Add(controlPanelHSizer, 0, wxEXPAND | wxALL, 5); // Панель только столько, сколько ей нужно

    this->SetSizer(mainSizer);

    //  III.     ------------------------------------------------------------------------------------------------
    //  Привязка обработчиков событий
    //  В конструкторе теперь только "регистрация"
    //  1.
    m_zoom_slider_ctrl->Bind(wxEVT_SLIDER, &erodeAfterAdaptiveTestFrame::OnZoomSliderChanged, this);
    //  2.
    m_spin_maxValue_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &erodeAfterAdaptiveTestFrame::OnMaxValueChanged, this);
    //  3.
    m_choice_adaptiveMethod_ctrl->Bind(wxEVT_CHOICE, &erodeAfterAdaptiveTestFrame::OnAdaptiveMethodChanged, this);
    //  4.
    m_choice_adaptiveType_ctrl->Bind(wxEVT_CHOICE, &erodeAfterAdaptiveTestFrame::OnAdaptiveTypeChanged, this);
    //  5.
    m_spin_blockSize_ctrl->Bind(wxEVT_SPINCTRL, &erodeAfterAdaptiveTestFrame::OnBlockSizeChanged, this);
    //  6.
    m_spin_Offset_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &erodeAfterAdaptiveTestFrame::OnOffsetChanged, this);
    //  7.
    m_kernel_width_ctrl->Bind(EVT_KERNEL_CHANGED, &erodeAfterAdaptiveTestFrame::OnKernelWidthChanged, this);
    m_kernel_height_ctrl->Bind(EVT_KERNEL_CHANGED, &erodeAfterAdaptiveTestFrame::OnKernelHeightChanged, this);
    //  8.
    m_kernel_shape_radiobox_ctrl->Bind(wxEVT_RADIOBOX, &erodeAfterAdaptiveTestFrame::OnKernelShapeChanged, this);
    //  9.
    m_iterations_spin_ctrl->Bind(wxEVT_SPINCTRL, &erodeAfterAdaptiveTestFrame::OnIterationsChanged, this);
    //  10.
    m_border_chois_ctrl->Bind(wxEVT_CHOICE, &erodeAfterAdaptiveTestFrame::OnBorderExtrapolChanged, this);
    //  11.
    m_gray_color_radiobox_ctrl->Bind(wxEVT_RADIOBOX, &erodeAfterAdaptiveTestFrame::OnGrayOrColorChanged, this);

    Bind(wxEVT_SIZE, &OnResize, this);

    //  IV. -----------------------------------------------------------------------------------------------------
    //  Получение матрицы фильтруемого изображения, считанного из дискового файла
    m_cv_original_img   =   getOriginalImage();

	//  V. --------------------------------------------------------------------------------------------------------------------
    //  Морфологическая трансформация оригинального изображения OpenCV
    ApplyMixedTransformation();

    //  VI. ----------------------------------------------------------------------------------------------------------------
    //  Отображение всех изображений
    UpdateAllViews();

    //  ----------------------------------------------------------------------------------------------------------------
    Center () ;
}

void erodeAfterAdaptiveTestFrame::OnResize(wxSizeEvent& event)
{
    // Проверяем, созданы ли уже нужные объекты
    if (!m_staticPreviewBitmap || !m_originalBitmap.IsOk()) {
        event.Skip();
        return;
    }

    UpdateAllViews();

    Layout();

    event.Skip();
}
//  1.
void erodeAfterAdaptiveTestFrame::OnZoomSliderChanged(wxCommandEvent& event) {
    // 1. Извлекаем масшаб
    m_zoom_slider_ctrl->m_zoom = event.GetInt();
    // 2. Запускаем обработку OpenCV
    UpdateAllViews();

    Layout();
}

//  2.
void erodeAfterAdaptiveTestFrame::OnMaxValueChanged(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"OnMaxValueChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1. Извлекаем максимальное значение для threshold
    m_maxValue = event.GetValue();

    // 2. Запускаем обработку OpenCV
    ApplyMixedTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  3.
void erodeAfterAdaptiveTestFrame::OnAdaptiveMethodChanged(wxCommandEvent& event) {
     #if 0
    MessageBoxW(NULL, L"Работает", L"wxCommandEvent()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1. Извлекаем адаптивный метод
    m_adaptiveMethod = event.GetInt();

    // 2. Запускаем обработку OpenCV
    ApplyMixedTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  4.
void erodeAfterAdaptiveTestFrame::OnAdaptiveTypeChanged(wxCommandEvent& event) {
     #if 0
    MessageBoxW(NULL, L"Работает", L"OnThreshold_typeChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем тип ооперации для threshold
    m_adaptiveType  =   event.GetInt();

    // 2. Запускаем обработку OpenCV
    ApplyMixedTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  5.
void erodeAfterAdaptiveTestFrame::OnBlockSizeChanged(wxSpinEvent& event) {
        #if 0
    MessageBoxW(NULL, L"Работает", L"OnBlockSizeChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    int val = event.GetPosition();
    // ЗАЩИТА: OpenCV упадет, если число четное или <= 1
    if (val > 1 && val % 2 != 0) {
        m_blockSize = val;
        if (!m_cv_original_img.empty()) { // ЗАЩИТА от пустой матрицы
            ApplyMixedTransformation();
            UpdateAllViews();
        }
    }

    Layout();
}
//  6.
void erodeAfterAdaptiveTestFrame::OnOffsetChanged(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"OnOffsetChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1. Извлекаем какую-то магическую константу смещения для AdaptiveThreshold
    m_Offset = event.GetInt();

    // 2. Запускаем обработку OpenCV
    ApplyMixedTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  7.
void erodeAfterAdaptiveTestFrame::OnKernelWidthChanged(wxCommandEvent& event) {
    // 1. Извлекаем размер ядра, который мы положили в SetInt() в контроле
    int newSize = event.GetInt();

    // 2. Обновляем переменную класса
    m_kernel_width = newSize;

    // 3. Запускаем обработку OpenCV
    ApplyMixedTransformation();

    // 4. Обновляем картинки
    UpdateAllViews();

    Layout();
}

void erodeAfterAdaptiveTestFrame::OnKernelHeightChanged(wxCommandEvent& event) {
    // 1. Извлекаем размер ядра, который мы положили в SetInt() в контроле
    int newSize = event.GetInt();

    // 2. Обновляем переменную класса
    m_kernel_height = newSize;

    // 3. Запускаем обработку OpenCV
    ApplyMixedTransformation();

    // 4. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  8.
void erodeAfterAdaptiveTestFrame::OnKernelShapeChanged(wxCommandEvent& event) {
    // 1. Запускаем обработку OpenCV
    ApplyMixedTransformation();

    // 2. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  9.
void erodeAfterAdaptiveTestFrame::OnIterationsChanged(wxCommandEvent& event) {
    // 1. Извлекаем количество итераций
    m_iterations = event.GetInt();

    // 3. Запускаем обработку OpenCV
    ApplyMixedTransformation();

    // 4. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  10.
void erodeAfterAdaptiveTestFrame::OnBorderExtrapolChanged(wxCommandEvent& event) {
    // 1. Обновляем переменную типа границы
    m_border_extrapolation  =   event.GetInt();
    #if 0
    std::wstring msg = L"Значение: " + std::to_wstring(m_border_extrapolation);

    //  Выводим в MessageBox. .c_str() преобразует wstring в нужный формат const wchar_t*
    MessageBoxW(NULL, msg.c_str(), L"Результат", MB_OK | MB_ICONINFORMATION);
    #endif
    // 2. Запускаем обработку OpenCV
    ApplyMixedTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  11.
void erodeAfterAdaptiveTestFrame::OnGrayOrColorChanged(wxCommandEvent& event) {
    m_gray_color_idx    =   event.GetInt();
    std::wstring msg = L"\nm_gray_color_idx: " + std::to_wstring(m_gray_color_idx);
    m_debugInfo->AppendText(msg);
    // 1. Запускаем обработку OpenCV
    ApplyMixedTransformation();

    // 2. Обновляем картинки
    UpdateAllViews();

    Layout();
}

// Универсальная функция, которая знает про оба окна
void erodeAfterAdaptiveTestFrame::UpdateAllViews() {
    //  Визуализация оригинального, отфильтрованного и трансформированного изображений

    //  Преобразования для вывода оригинального изображения OpenCV в формате библиотеки wxVidgets
    UpdateDisplay   (   m_wx_img
                    ,   m_cv_original_img
                    ,   m_originalBitmap
                    ,   m_staticOriginalBitmap
                    ,   m_scrolled_wind_original
                    ,   m_zoom_slider_ctrl->m_zoom
                    );

    // Превращаем из GRAY в RGB (чтобы было 3 канала: R=G=B)
    /*
        Чтобы можно было вывести результат адаптивного фильтра на экран,
        мы должны преобразовать его одноканальную черно-белую матрицу (бинарную)
        в трехканальную полутоновую.
    */
    cv::cvtColor(m_cv_filtered_img, m_binary_filtered_img, cv::COLOR_GRAY2RGB);

    UpdateDisplay   (   m_wx_img
                    ,   m_binary_filtered_img
                    ,   m_filteredBitmap
                    ,   m_staticFilteredBitmap
                    ,   m_scrolled_wind_filtered
                    ,   m_zoom_slider_ctrl->m_zoom
                    );

    // Если адаптив выдал 1 канал (а он всегда выдает 1)
    if (m_cv_transformed_img.channels() == 1) {
        // Дублируем канал серого в R, G и B, чтобы wxImage "понял" картинку
        cv::cvtColor(m_cv_transformed_img, m_transformed_img_for_display, cv::COLOR_GRAY2RGB);
    } else {
        m_transformed_img_for_display = m_cv_transformed_img;
    }

    UpdateDisplay   (   m_wx_img
                    ,   m_transformed_img_for_display
                    ,   m_transformedBitmap
                    ,   m_staticTransformeddBitmap
                    ,   m_scrolled_wind_transformed
                    ,   m_zoom_slider_ctrl->m_zoom
                    );

    //  Внутри панели создаем битмэп и статический битмап для вывода предварительного оригинального изображения
    int     previewW    = m_previewPanel->GetSize().x;

    m_previewBitmap       = GetScaledBitmap(m_originalBitmap, previewW, previewW);
    m_staticPreviewBitmap->SetBitmap(m_previewBitmap);
    m_previewPanel->Layout();
    m_previewPanel->Refresh();
}
//  -----------------------------------------------------------------------------------------------------
//  Экосистема OpenCV
void erodeAfterAdaptiveTestFrame::ApplyMixedTransformation() {
    #if 0
    MessageBoxW(NULL, L"Работает", L"ApplyMixedTransformation()", MB_OK | MB_ICONINFORMATION);
    #endif
    // Вся "кухня" OpenCV здесь

    // 1. Создаем ВРЕМЕННУЮ серую копию, не портя оригинал!
    /*
        Почему программа «валится» без предварительного преобразования?

        Ответ кроется в жестких требованиях функции cv::adaptiveThreshold.

        Если заглянуть в документацию, там четко сказано:
        входное изображение должно быть 8-битным одноканальным (CV_8UC1).
    */
    #if 1
    if (m_cv_original_img.channels() == 3)
        cv::cvtColor(m_cv_original_img, m_gray_original_img, cv::COLOR_BGR2GRAY);
    else
        m_gray_original_img = m_cv_original_img.clone();
    #endif
    cv::adaptiveThreshold   (   m_gray_original_img
                            ,   m_cv_filtered_img
                            ,   m_maxValue
                            ,   m_adaptiveMethod
                            ,   m_adaptiveType          //  тип операции
                            ,   m_blockSize
                            ,   m_Offset                //  offset
                            );
    #if 1
    std::wstring msg = L"\nmean(m_cv_filtered_img)[0]: " + std::to_wstring(cv::mean(m_cv_filtered_img)[0]);
    m_debugInfo->AppendText(msg);
    msg = L"\nКоличество каналов после адаптивной фильтрации: " + std::to_wstring(m_cv_filtered_img.channels());
    m_debugInfo->AppendText(msg);
    #endif

    if (1 == m_gray_color_idx) { // Выбран режим "Цвет"
        if (m_cv_filtered_img.channels() == 1) {
            // Маска (m_cv_filtered_img) должна быть того же размера, что и оригинал
            // Операция оставит оригинальные цвета там, где в маске 255, и сделает черным там, где 0
            cv::bitwise_and(m_cv_original_img, m_cv_original_img, m_image_for_transform, m_cv_filtered_img);
            /*
                Адаптив — это всегда про 1 канал и жесткую бинарную маску (даже если она кажется «серой» из-за деталей).

                Маска — это «трафарет».

                С помощью bitwise_and() мы накладываем её на цветной оригинал.

                Морфология — может работать и с маской (для логики),
                и с цветом (для визуальных эффектов или сегментации).

            */
        }
    } else {
        m_image_for_transform = m_cv_filtered_img;
    }

    m_kernel_shape = GetSelectedMorphShape();
    cv::erode   (   m_image_for_transform
                ,   m_cv_transformed_img
                ,   cv::getStructuringElement   (   m_kernel_shape
                                                ,   cv::Size(m_kernel_width, m_kernel_height)
                                                ,   cv::Point(-1, -1)
                                                )
                ,   cv::Point(-1, -1)
                ,   m_iterations
                ,   m_border_extrapolation
                ,   cv::morphologyDefaultBorderValue()
                );

    #if 1
    msg = L"\nmean(m_cv_transformed_img)[0]: " + std::to_wstring(cv::mean(m_cv_transformed_img)[0]);
    m_debugInfo->AppendText(msg);

    msg = L"\nthreshold::m_maxValue: " + std::to_wstring(m_maxValue);
    m_debugInfo->AppendText(msg);
    #endif

}

std::string erodeAfterAdaptiveTestFrame::getSignatureText(){
    std::string signature_text =

    "void cv::adaptiveThreshold("
    "\n,    cv::InputArray src"
    "\n,    cv::OutputArray dst"
    "\n,    double maxValue"
    "\n,    int    adaptiveMethod"
    "\n,    int    thresholdType"
    "\n,    int    blockSize"
    "\n,    double Offset"
    "\n);"
    "void cv::erode("
    "\n     cv::InputArray src"
    "\n,    cv::OutputArray dst"
    "\n,    cv::InputArray element"
    "\n,    cv::Point anchor = cv::Point(-1, -1)"
    "\n,    int iterations = 1"
    "\n,    int borderType = cv::BORDER_CONSTANT"
    "\n,    const v::Scalar& borderValue ="
    "\n             cv::morphologyDefaultBorderValue()"
    "\n );"
    ;

    return signature_text;
}
//  Функция получения строки выбора формы ядра в радиобоксе
wxArrayString   erodeAfterAdaptiveTestFrame::getKernelShape(){
    wxArrayString   strings;
    //  Строки выбора формы ядра
    strings.Add(wxT("&MORPH_RECT"));
    strings.Add(wxT("&MORPH_CROSS"));
    strings.Add(wxT("&MORPH_ELLIPS"));

    return   strings;
}
int erodeAfterAdaptiveTestFrame::GetSelectedMorphShape() {
    // Допустим, m_shapeRadioBox — это ваш контрол с выбором формы
    int selection = m_kernel_shape_radiobox_ctrl->GetSelection();

    switch (selection)
    {
        case 0: m_kernel_shape = cv::MORPH_RECT; break;
        case 1: m_kernel_shape = cv::MORPH_CROSS; break;
        case 2: m_kernel_shape = cv::MORPH_ELLIPSE; break;

        default: m_kernel_shape = cv::MORPH_RECT;
    }
    #if 0
    std::wstring msg = L"Значение: " + std::to_wstring(m_kernel_shape);

    //  Выводим в MessageBox. .c_str() преобразует wstring в нужный формат const wchar_t*
    MessageBoxW(NULL, msg.c_str(), L"Результат", MB_OK | MB_ICONINFORMATION);
    #endif

    return m_kernel_shape;
}


//  Функция получения строки выбора варианта морфологической обработки "Серый/Цветной"
wxArrayString   erodeAfterAdaptiveTestFrame::getGrayOrColor(){
    wxArrayString   strings;
    //  Строки выбора варианта обработки
    strings.Add(wxT("&GRAY"));
    strings.Add(wxT("&COLOR"));

    return   strings;
}
