//  thresholdTestFrame.cpp
/*
 ***************************************************************
 * Name:      thresholdTestFrame.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-23
 * Copyright: Sergi E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 *************************************************************
*/
#include "thresholdTestFrame.h"
/*
    Класс тестирования преобрзования `thresholdTestFrame`
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
thresholdTestFrame::thresholdTestFrame  ( wxWindow  *   parent, const wxString& title)
                        : wxFrame   (   parent
                                    ,   wxID_ANY
                                    ,   title
                                    ,   wxDefaultPosition
                                    ,   wxSize(800, 600)
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
                                ,   wxT("threshold \nand \nadaptiveThreshold \nfilteration")
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
    m_scrolled_wind_filtered_1      =   new wxScrolledWindow(this);
    m_scrolled_wind_filtered_2      =   new wxScrolledWindow(this);

    //  II.    --------------------------------------------------------------------------------------------
    //  Сайзеры
    //  II.1. Сайзеры для отображения различной вспомогательной инофрмации о тестировании
    //  Добавляем в сайзер: (объект, пропорция, флаги, отступ)
    wxBoxSizer *            informHSizer       =   new wxBoxSizer(wxHORIZONTAL);
    informHSizer->SetMinSize(wxSize(-1, -1)); // -1 значит "любая ширина", -1 значит "любая высота"
    informHSizer->Add(m_filtering_namePanel, 1, wxCENTER | wxALL, 5);
    informHSizer->Add(m_signaturePanel, 1, wxCENTER | wxALL, 5);
    informHSizer->Add(m_debugPanel, 1, wxCENTER | wxALL, 5);
    m_informPanel->SetSizer(informHSizer);

    //
    //  II.2. Сайзеры для отображения различных вариантов исследуемого изображения
    //  Используем сайзер, чтобы центрировать превью внутри панели
    wxBoxSizer *            previewSizer    =   new wxBoxSizer(wxVERTICAL);
    previewSizer->Add(m_staticPreviewBitmap, 1, wxCENTER | wxALL, 5);
    m_previewPanel->SetSizer(previewSizer);

    wxBoxSizer *            titelSizerHor       =   new wxBoxSizer(wxHORIZONTAL);
    wxString titelStr = wxString::Format("%-150s %-150s %-150s %-150s",
    "Превью", "Оригинальное", "После Threshold", "После AdaptiveThreshold");
    titelSizerHor->Add(new wxStaticText(this, -1, titelStr), 0, wxALL, 5);

    wxBoxSizer *            imagesHSizer       =   new wxBoxSizer(wxHORIZONTAL);
    //  Добавляем в главный сайзер превью-панель
    imagesHSizer->Add(m_previewPanel, 0, wxEXPAND | wxALL, 5);               // Фикс
    imagesHSizer->Add(m_scrolled_wind_original, 1, wxEXPAND | wxALL, 5);       // Растягивается
    imagesHSizer->Add(m_scrolled_wind_filtered_1, 1, wxEXPAND | wxALL, 5);     // Растягивается
    imagesHSizer->Add(m_scrolled_wind_filtered_2, 1, wxEXPAND | wxALL, 5);     // Растягивается

    //  ------------------------------------------------------------------------------------------------------
    //  Подготовка панели управления отображением изображений
    //  1.
    m_zoom_slider_ctrl      =   new ZoomSliderCtrl(this);

    //  2.  Пороговое значение для threshold
    m_spin_thresh_ctrl      =   new wxSpinCtrlDouble    (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   1.0     // Минимум: разрешаем 0 для авторасчета
                                                        ,   255.0    // Максимум: 10 достаточно для экспериментов
                                                        ,   127     // Начальное значение
                                                        ,   1.     // Шаг: золотая середина для визуального теста
                                                        );
    //  Дополнительно количество знаков после запятой
    m_spin_thresh_ctrl->SetDigits(1);

    //  Максимальные значения порогов для обоих фильтров
    //  3.
    m_spin_maxValue_1_ctrl  =   new wxSpinCtrlDouble    (   this
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
    m_spin_maxValue_1_ctrl->SetDigits(1);

    //  4.
    m_spin_maxValue_2_ctrl  =   new wxSpinCtrlDouble    (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   1.0     // Минимум: разрешаем 0 для авторасчета
                                                        ,   255.0    // Максимум:
                                                        ,   250     // Начальное значение
                                                        ,   1.0     // Шаг: золотая середина для визуального теста
                                                        );
    //  Дополнительно количество знаков после запятой
    m_spin_maxValue_2_ctrl->SetDigits(1);

    //  Тип операции для threshold (thresholdType)
    //  5.
    wxString    thresholdTypes[] =  {   wxT("cv::THRESH_BINARY")        //
                                    ,   wxT("cv::THRESH_BINARY_INV")    //
                                    ,   wxT("cv::THRESH_TRUNC")         //
                                    ,   wxT("cv::THRESH_TOZERO")        //
                                    ,   wxT("cv::THRESH_TOZERO_INV")    //
                                    };
    m_choice_threshold_type_1_ctrl     =   new wxChoice      (   this
                                                             ,   wxID_ANY
                                                             ,   wxDefaultPosition
                                                             ,   wxSize(180, -1)
                                                             ,   WXSIZEOF(thresholdTypes)
                                                             ,   thresholdTypes
                                                             ,   0
                                                             );
    m_choice_threshold_type_1_ctrl->SetStringSelection(wxT("cv::THRESH_BINARY"));
    //  6.
    wxString    AdaptiveThresholdTypes[] =  {   wxT("cv::THRESH_BINARY")        //
                                            ,   wxT("cv::THRESH_BINARY_INV")    //
                                            };
    m_choice_threshold_type_2_ctrl     =   new wxChoice      (   this
                                                             ,   wxID_ANY
                                                             ,   wxDefaultPosition
                                                             ,   wxSize(180, -1)
                                                             ,   WXSIZEOF(AdaptiveThresholdTypes)
                                                             ,   AdaptiveThresholdTypes
                                                             ,   0
                                                             );
    m_choice_threshold_type_2_ctrl->SetStringSelection(wxT("cv::THRESH_BINARY"));

    //  7.
    //   Два способа задания адаптивного порога (adaptiveThreshold)
    wxString    TypesStringsAT[] =  {   wxT("cv::ADAPTIVE_THRESH_MEAN_C")        //
                                    ,   wxT("cv::ADAPTIVE_THRESH_GAUSSIAN_C")    //
                                    };

    m_choice_AdaptiveThresholdTypes_ctrl     =   new wxChoice    (   this
                                                                 ,   wxID_ANY
                                                                 ,   wxDefaultPosition
                                                                 ,   wxSize(180, -1)
                                                                 ,   WXSIZEOF(TypesStringsAT)
                                                                 ,   TypesStringsAT
                                                                 ,   0
                                                                 );
    m_choice_AdaptiveThresholdTypes_ctrl->SetStringSelection(wxT("cv::ADAPTIVE_THRESH_GAUSSIAN_C"));

    //  8.
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
    //  9.
    m_spin_Offset_ctrl    =   new wxSpinCtrlDouble    (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   -10.0     // Минимум: разрешаем 0 для авторасчета
                                                        ,   50.0    // Максимум: 10 достаточно для экспериментов
                                                        ,   15.0     // Начальное значение
                                                        ,   1.     // Шаг: золотая середина для визуального теста
                                                        );
    //  Дополнительно количество знаков после запятой
    m_spin_Offset_ctrl->SetDigits(1);

    //  II.3. Сайзеры для панели настроек и управления
    wxBoxSizer*         controlPanelHSizer   = new wxBoxSizer(wxHORIZONTAL);
    controlPanelHSizer->SetMinSize(wxSize(-1, 150)); // -1 значит "любая ширина", 150 - фиксированная высота

    // 1. Слайдер масштаба в отдельном столбике
    wxBoxSizer*         zoomCol             = new wxBoxSizer(wxVERTICAL);
    zoomCol->Add(new wxStaticText(this, -1, "Масштаб"), 0, wxALL, 5);
    zoomCol->Add(m_zoom_slider_ctrl, 0, wxEXPAND | wxALL, 5);

    // 2. Пороговое значение для threshold
    wxBoxSizer*         thresholdCol         = new wxBoxSizer(wxVERTICAL);
    thresholdCol->Add(new wxStaticText(this, -1, "Порог для threshold"), 0, wxALL, 5);
    thresholdCol->Add(m_spin_thresh_ctrl, 0, wxALL, 5);

    //  Максимальные значения для threshold и для AdaptiveShreshold
    //  3.
    wxBoxSizer*         maxValue_1_Col       = new wxBoxSizer(wxVERTICAL);
    maxValue_1_Col->Add(new wxStaticText(this, -1, "maxValue Threshold"), 0, wxALL, 5);
    maxValue_1_Col->Add(m_spin_maxValue_1_ctrl, 0, wxALL, 5);
    //  4.
    wxBoxSizer*         maxValue_2_Col       = new wxBoxSizer(wxVERTICAL);
    maxValue_2_Col->Add(new wxStaticText(this, -1, "maxValue Adaptive"), 0, wxALL, 5);
    maxValue_2_Col->Add(m_spin_maxValue_2_ctrl, 0, wxALL, 5);

    // Типы операции для threshold  и для AdaptiveShreshold
    //  5.
    wxBoxSizer*         threshold_type_1_Col       = new wxBoxSizer(wxVERTICAL);
    threshold_type_1_Col->Add(new wxStaticText(this, -1, "Тип операци Threshold"), 0, wxALL, 5);
    threshold_type_1_Col->Add(m_choice_threshold_type_1_ctrl, 0, wxALL, 5);
    //  6.
    wxBoxSizer*         threshold_type_2_Col       = new wxBoxSizer(wxVERTICAL);
    threshold_type_2_Col->Add(new wxStaticText(this, -1, "Тип операци Adaptive"), 0, wxALL, 5);
    threshold_type_2_Col->Add(m_choice_threshold_type_2_ctrl, 0, wxALL, 5);

    //  7. Адаптивный метод только для AdaptiveShreshold
    wxBoxSizer*         adaptiveMethodCol       = new wxBoxSizer(wxVERTICAL);
    adaptiveMethodCol->Add(new wxStaticText(this, -1, "Adaptive method"), 0, wxALL, 5);
    adaptiveMethodCol->Add(m_choice_AdaptiveThresholdTypes_ctrl, 0, wxALL, 5);

    //  Размер блока и константа только для AdaptiveShreshold
    //  8.
    wxBoxSizer*         blockSizeCol       = new wxBoxSizer(wxVERTICAL);
    blockSizeCol->Add(new wxStaticText(this, -1, "blockSize Adaptive"), 0, wxALL, 5);
    blockSizeCol->Add(m_spin_blockSize_ctrl, 0, wxALL, 5);

    //  9.
    wxBoxSizer*         ConstCol       = new wxBoxSizer(wxVERTICAL);
    ConstCol->Add(new wxStaticText(this, -1, "Смещение Adaptive"), 0, wxALL, 5);
    ConstCol->Add(m_spin_Offset_ctrl, 0, wxALL, 5);


    //  Объединяем элементы управления в один горизонтальный сайзер
    //  1.
    controlPanelHSizer->Add(zoomCol, 0, wxEXPAND | wxALL, 5);
    //  2.
    controlPanelHSizer->Add(thresholdCol, 0, wxEXPAND | wxALL, 5);
    //  3.
    controlPanelHSizer->Add(maxValue_1_Col, 0, wxEXPAND | wxALL, 5);
    //  4.
    controlPanelHSizer->Add(maxValue_2_Col, 0, wxEXPAND | wxALL, 5);
    //  5.
    controlPanelHSizer->Add(threshold_type_1_Col, 0, wxEXPAND | wxALL, 5);
    //  6.
    controlPanelHSizer->Add(threshold_type_2_Col, 0, wxEXPAND | wxALL, 5);
    //  7.
    controlPanelHSizer->Add(adaptiveMethodCol, 0, wxEXPAND | wxALL, 5);
    //  8.
    controlPanelHSizer->Add(blockSizeCol, 0, wxEXPAND | wxALL, 5);
    //  9.
    controlPanelHSizer->Add(ConstCol, 0, wxEXPAND | wxALL, 5);

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
    m_zoom_slider_ctrl->Bind(wxEVT_SLIDER, &thresholdTestFrame::OnZoomSliderChanged, this);
    //  2.
    m_spin_thresh_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &thresholdTestFrame::OnThreshChanged, this);
    //  3.
    m_spin_maxValue_1_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &thresholdTestFrame::OnMaxValue_1Changed, this);
    //  4.
    m_spin_maxValue_2_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &thresholdTestFrame::OnMaxValue_2Changed, this);
    //  5.
    m_choice_threshold_type_1_ctrl->Bind(wxEVT_CHOICE, &thresholdTestFrame::OnThreshold_type_1Changed, this);
    //  6.
    m_choice_threshold_type_2_ctrl->Bind(wxEVT_CHOICE, &thresholdTestFrame::OnThreshold_type_2Changed, this);
    //  7.
    m_choice_AdaptiveThresholdTypes_ctrl->
                            Bind(wxEVT_CHOICE, &thresholdTestFrame::OnAdaptiveThresholdTypesChanged, this);
    //  8.
    m_spin_blockSize_ctrl->Bind(wxEVT_SPINCTRL, &thresholdTestFrame::OnBlockSizeChanged, this);
    //  9.
    m_spin_Offset_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &thresholdTestFrame::OnOffsetChanged, this);

    Bind(wxEVT_SIZE, &OnResize, this);

    //  IV. -----------------------------------------------------------------------------------------------------
    //  Получение матрицы фильтруемого изображения, считанного из дискового файла
    m_cv_original_img   =   getOriginalImage();

	//  V. --------------------------------------------------------------------------------------------------------------------
    //  Морфологическая трансформация оригинального изображения OpenCV
    ApplyFiltering();

    //  VI. ----------------------------------------------------------------------------------------------------------------
    //  Отображение всех изображений
    UpdateAllViews();

    //  ----------------------------------------------------------------------------------------------------------------
    Center () ;
}

void thresholdTestFrame::OnResize(wxSizeEvent& event)
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
void thresholdTestFrame::OnZoomSliderChanged(wxCommandEvent& event) {
    // 1. Извлекаем масшаб
    m_zoom_slider_ctrl->m_zoom = event.GetInt();
    // 2. Запускаем обработку OpenCV
    UpdateAllViews();

    Layout();
}

//  2.
void thresholdTestFrame::OnThreshChanged(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"OnMaxValue_1Changed()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1. Извлекаем порог для threshold
    m_thresh = event.GetValue();

    // 2. Запускаем обработку OpenCV
    ApplyFiltering();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}

//  3.
void thresholdTestFrame::OnMaxValue_1Changed(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"OnMaxValue_1Changed()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1. Извлекаем максимальное значение для threshold
    m_maxValue_1 = event.GetValue();

    // 2. Запускаем обработку OpenCV
    ApplyFiltering();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  4.
void thresholdTestFrame::OnMaxValue_2Changed(wxSpinDoubleEvent& event) {
     #if 0
    MessageBoxW(NULL, L"Работает", L"OnMaxValue_2Changed()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1. Извлекаем максимальное значение для AdaptiveThreshold
    //m_maxValue_2 = event.GetInt();
    m_maxValue_2 = event.GetValue();

    // 2. Запускаем обработку OpenCV
    ApplyFiltering();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  5.
void thresholdTestFrame::OnThreshold_type_1Changed(wxCommandEvent& event) {
     #if 0
    MessageBoxW(NULL, L"Работает", L"OnThreshold_type_1Changed()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем тип ооперации для threshold
    m_threshold_type_1  =   event.GetInt();

    // 2. Запускаем обработку OpenCV
    ApplyFiltering();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  6.
void thresholdTestFrame::OnThreshold_type_2Changed(wxCommandEvent& event) {
         #if 0
    MessageBoxW(NULL, L"Работает", L"OnThreshold_type_2Changed()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем тип ооперации для AdaptiveThreshold
    m_threshold_type_2  =   event.GetInt();

    // 2. Запускаем обработку OpenCV
    ApplyFiltering();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  7.
void thresholdTestFrame::OnAdaptiveThresholdTypesChanged(wxCommandEvent& event) {
     #if 0
    MessageBoxW(NULL, L"Работает", L"wxCommandEvent()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1. Извлекаем адаптивный метод
    m_adaptiveMethod = event.GetInt();

    // 2. Запускаем обработку OpenCV
    ApplyFiltering();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  8.
void thresholdTestFrame::OnBlockSizeChanged(wxSpinEvent& event) {
        #if 0
    MessageBoxW(NULL, L"Работает", L"OnBlockSizeChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    int val = event.GetPosition();
    // ЗАЩИТА: OpenCV упадет, если число четное или <= 1
    if (val > 1 && val % 2 != 0) {
        m_blockSize = val;
        if (!m_cv_original_img.empty()) { // ЗАЩИТА от пустой матрицы
            ApplyFiltering();
            UpdateAllViews();
        }
    }

    Layout();
}
//  9.
void thresholdTestFrame::OnOffsetChanged(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"OnOffsetChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1. Извлекаем какую-то магическую константу смещения для AdaptiveThreshold
    m_Offset = event.GetInt();

    // 2. Запускаем обработку OpenCV
    ApplyFiltering();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}

// Универсальная функция, которая знает про оба окна
void thresholdTestFrame::UpdateAllViews() {
    //  Визуализация оригинального и отфильтрованного изображения (выводим оба состояния)

    //  Преобразования для вывода оригинального изображения OpenCV в формате библиотеки wxVidgets
    UpdateDisplay   (   m_wx_img
                    ,   m_cv_original_img
                    ,   m_originalBitmap
                    ,   m_staticOriginalBitmap
                    ,   m_scrolled_wind_original
                    ,   m_zoom_slider_ctrl->m_zoom
                    );

    // Для каждого выходного изображения делаем так:

    // Превращаем из GRAY в RGB (чтобы было 3 канала: R=G=B)
    cv::cvtColor(m_cv_filtered_img_1, out1_for_display, cv::COLOR_GRAY2RGB);

    UpdateDisplay   (   m_wx_img
                    ,   out1_for_display
                    ,   m_filteredBitmap_1
                    ,   m_staticFilteredBitmap_1
                    ,   m_scrolled_wind_filtered_1
                    ,   m_zoom_slider_ctrl->m_zoom
                    );


    //  Преобразования для вывода отфильтрованного изображения OpenCV в формате библиотеки wxVidgets

    // Превращаем из GRAY в RGB (чтобы было 3 канала: R=G=B)
    //cv::cvtColor(m_cv_filtered_img_2, out2_for_display, cv::COLOR_GRAY2RGB);

    cv::Mat mat_for_display;
    // Если адаптив выдал 1 канал (а он всегда выдает 1)
    if (m_cv_filtered_img_2.channels() == 1) {
        // Дублируем канал серого в R, G и B, чтобы wxImage "понял" картинку
        cv::cvtColor(m_cv_filtered_img_2, mat_for_display, cv::COLOR_GRAY2RGB);
    } else {
        mat_for_display = m_cv_filtered_img_2;
    }


    UpdateDisplay   (   m_wx_img
                    ,   mat_for_display
                    ,   m_filteredBitmap_2
                    ,   m_staticFilteredBitmap_2
                    ,   m_scrolled_wind_filtered_2
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
void thresholdTestFrame::ApplyFiltering() {
    // 1. Проверка: загружена ли картинка?
    if (m_cv_original_img.empty()) return;

    // 2. Проверка: параметры адаптива в рамках закона?
    // blockSize должен быть > 1 И нечетным
    if (m_blockSize < 3 || m_blockSize % 2 == 0) {
         // Если значение плохое, просто выходим, не вызывая OpenCV
         return;
    }
    #if 0
    MessageBoxW(NULL, L"Работает", L"ApplyFiltering()", MB_OK | MB_ICONINFORMATION);
    #endif
    // Вся "кухня" OpenCV здесь
    //if (m_cv_original_img.empty()) return;

    // 1. Создаем ВРЕМЕННУЮ серую копию, не портя оригинал!
    cv::Mat gray;
    if (m_cv_original_img.channels() == 3)
        cv::cvtColor(m_cv_original_img, gray, cv::COLOR_BGR2GRAY);
    else
        gray = m_cv_original_img.clone();

    cv::threshold       (   gray               //  входное изображение
                        ,   m_cv_filtered_img_1             //  выходное изображение
                        ,   m_thresh                        //  пороговое значение
                        ,   m_maxValue_1
                        ,   m_threshold_type_1              //  тип операции cv::THRESH_TRUNC //
                        );
    #if 1
    std::wstring msg = L"\nmean(m_cv_filtered_img_1)[0]: " + std::to_wstring(cv::mean(m_cv_filtered_img_1)[0]);
    m_debugInfo->AppendText(msg);
    #endif
    cv::adaptiveThreshold   (   gray
                            ,   m_cv_filtered_img_2
                            ,   m_maxValue_2
                            ,   m_adaptiveMethod
                            ,   m_threshold_type_2          //  тип операции
                            ,   m_blockSize
                            ,   m_Offset                //  offset
                            );
    #if 1
    msg = L"\nmean(m_cv_filtered_img_2)[0]: " + std::to_wstring(cv::mean(m_cv_filtered_img_2)[0]);
    m_debugInfo->AppendText(msg);

    msg = L"\nAdaptiveThreshold::m_blockSize: " + std::to_wstring(m_blockSize);
    m_debugInfo->AppendText(msg);

    msg = L"\nAdaptiveThreshold::m_maxValue_2: " + std::to_wstring(m_maxValue_2);
    m_debugInfo->AppendText(msg);

    #endif

}

std::string thresholdTestFrame::getSignatureText(){
    std::string signature_text =

    "void cv::threshold("
    "\n,    cv::InputArray src"
    "\n,    cv::OutputArray dst"
    "\n,    double thresh"
    "\n,    double maxValue"
    "\n,    int    thresholdType"
    "\n);"
    "\n "
    "\n"
    "void cv::adaptiveThreshold("
    "\n,    cv::InputArray src"
    "\n,    cv::OutputArray dst"
    "\n,    double maxValue"
    "\n,    int    adaptiveMethod"
    "\n,    int    thresholdType"
    "\n,    int    blockSize"
    "\n,    double dblConst"
    "\n);"

    ;

    return signature_text;
}

/*
    -------------------------------------------------------------------------------------------------
    Как были организованы типы границ

    cvdefs.hpp

    enum BorderTypes {
        BORDER_CONSTANT    = 0, //  !< `iiiiii|abcdefgh|iiiiiii`  with some specified `i`
                                //  — заполнение цветом (по умолчанию)
        BORDER_REPLICATE   = 1, //  !< `aaaaaa|abcdefgh|hhhhhhh`
                                //  — повтор последнего пикселя (а-а-а-а).
        BORDER_REFLECT     = 2, //  !< `fedcba|abcdefgh|hgfedcb`
                                //  — зеркало (абв|вба).
        BORDER_WRAP        = 3, //  !< `cdefgh|abcdefgh|abcdefg`
                                //  — обертка (как в старых играх: вышел слева — зашел справа).
        BORDER_REFLECT_101 = 4, //  !< `gfedcb|abcdefgh|gfedcba`
        BORDER_TRANSPARENT = 5, //  !< `uvwxyz|abcdefgh|ijklmno`

        BORDER_REFLECT101  = BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
        BORDER_DEFAULT     = BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
        BORDER_ISOLATED    = 16 //!< do not look outside of ROI
    };
*/



