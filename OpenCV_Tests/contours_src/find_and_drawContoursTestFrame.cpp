//  find_and_drawContoursFrame.cpp
/*
 ***************************************************************
 * Name:      find_and_drawContours.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-05-06
 * Copyright: Sergi E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 *************************************************************
*/
#include "find_and_drawContoursTestFrame.h"
/*
    Класс тестирования преобрзования `find_and_drawContoursTestFrame`
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
find_and_drawContoursTestFrame::find_and_drawContoursTestFrame  ( wxWindow  *   parent, const wxString& title)
                                : wxFrame   (   parent
                                            ,   wxID_ANY
                                            ,   title
                                            ,   wxDefaultPosition
                                            ,   wxSize(1800, 1000)
                                            )
{
    //  I.  ------------------------------------------------------------------------------------------------
    //  Создадим информационную панель,
    //  панель для предварительного просмотра оригинального изображения и
    //  прокручивающиеся окна для изображений
    m_informPanel                 =   new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_informPanel->SetMinSize(wxSize(-1, -1));
    m_informPanel->SetBackgroundColour(*wxLIGHT_GREY);             // Чтобы визуально отделить

    m_transforming_namePanel      =   new wxPanel(m_informPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_transforming_namePanel->SetBackgroundColour(*wxCYAN);             // Чтобы визуально отделить
    m_transform_name_text       =
            new wxStaticText    (   m_transforming_namePanel
                                ,   -1
                                ,   wxT("`find and draw contours`\nContours")
                                ,   wxPoint(0, 0)
                                );
    wxFont      m_transform_name_text_font(16, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    m_transform_name_text->SetFont(m_transform_name_text_font);
    m_transform_name_text->SetForegroundColour(*wxBLACK);

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
                                                ,   wxSize (440, 150)
                                                ,   wxTE_MULTILINE
                                                );

    //  -------------------------------------------------------------------------------------------------
    m_previewPanel                  =   new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1));
    m_previewPanel->SetMinSize(wxSize(200, -1));
    m_previewPanel->SetBackgroundColour(*wxLIGHT_GREY); // Чтобы визуально отделить
    m_staticPreviewBitmap           =   new wxStaticBitmap(m_previewPanel, wxID_ANY, wxNullBitmap);

    //  -------------------------------------------------------------------------------------------------
    m_scrolled_wind_original        =   new wxScrolledWindow(this);
    m_scrolled_wind_transformed     =   new wxScrolledWindow(this);

    //  ------------------------------------------------------------------------------------------------------
    //  Подготовка панели управления отображением изображений
    //  1.
    m_zoom_orig_slider_ctrl      =   new ZoomSliderCtrl(this);

    //  2.
    m_zoom_transf_slider_ctrl    =   new ZoomSliderCtrl(this, 10);

    //  3.  Размер квадратного ядра Гауссиана
    m_kernel_width_ctrl     =   new KernelMatrixCtrl(this, wxID_ANY, wxPoint(0, 0), wxSize(100, 100));
    m_kernel_width_ctrl->SetMinSize(wxSize(100, 100));           // Фиксируем минимальное присутствие

    //  4. 5.  Сигмы распределения Гауссиана
    m_sigmaX_spin_ctrl      =   new wxSpinCtrlDouble    (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   0.0     // Минимум: разрешаем 0 для авторасчета
                                                        ,   10.0    // Максимум: 10 достаточно для экспериментов
                                                        ,   0.0     // Начальное значение
                                                        ,   0.1     // Шаг: золотая середина для визуального теста
                                                        );
    //  Дополнительно количество знаков после запятой
    m_sigmaX_spin_ctrl->SetDigits(2);

    m_sigmaY_spin_ctrl      =   new wxSpinCtrlDouble    (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   0.0     // Минимум: разрешаем 0 для авторасчета
                                                        ,   10.0    // Максимум: 10 достаточно для экспериментов
                                                        ,   0.0     // Начальное значение
                                                        ,   0.1     // Шаг: золотая середина для визуального теста
                                                        );
    //  Дополнительно количество знаков после запятой
    m_sigmaY_spin_ctrl->SetDigits(2);

    //  6.  Тип экстраполяции границ Гауссиана (borderType)
    wxString    borderTypes[] =    {   wxT("cv::BORDER_CONSTANT")  //  заполнение цветом (по умолчанию)
                                   ,   wxT("cv::BORDER_REPLICATE") //  повтор последнего пикселя (а-а-а-а)
                                   ,   wxT("cv::BORDER_REFLECT")   //  зеркало (абв|вба)
                                   };

    m_borderType_chois_ctrl =   new wxChoice     (   this
                                                 ,   wxID_ANY
                                                 ,   wxDefaultPosition
                                                 ,   wxSize(150, -1)
                                                 ,   WXSIZEOF(borderTypes)
                                                 ,   borderTypes
                                                 ,   0
                                                 );
    m_borderType_chois_ctrl->SetStringSelection(wxT("cv::BORDER_CONSTANT"));

    //  7.  Пороговое значение глобального порогового фильтра
    m_thresh_sldr_ctrl      =   new     wxSlider   (   this
                                                    ,   wxID_ANY
                                                    ,   100                 //  Начальное значение
                                                    ,   1                   //  Минимальное значение
                                                    ,   255                 //  Максимальное значение
                                                    ,   wxPoint(1, 1)
                                                    ,   wxSize(150, -1)     //  Горизонтальный слайдер
                                                    ,   wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS
                                                    );

    //  8.  Тип операции для порогового фильтра
    wxString    thresholdTypes[] =  {   wxT("cv::THRESH_BINARY")        //
                                    ,   wxT("cv::THRESH_BINARY_INV")    //
                                    ,   wxT("cv::THRESH_TRUNC")         //
                                    ,   wxT("cv::THRESH_TOZERO")        //
                                    ,   wxT("cv::THRESH_TOZERO_INV")    //
                                    };
    m_threshold_type_choice_ctrl     =   new wxChoice      (   this
                                                           ,   wxID_ANY
                                                           ,   wxDefaultPosition
                                                           ,   wxSize(150, -1)
                                                           ,   WXSIZEOF(thresholdTypes)
                                                           ,   thresholdTypes
                                                           ,   0
                                                           );
    m_threshold_type_choice_ctrl->SetStringSelection(wxT("cv::THRESH_BINARY"));

    //  9.  Режим возврата контуров
    wxString    contour_return_mode[] =
                                    {   wxT("cv::RETR_EXTERNAL")
                                    ,   wxT("cv::RETR_LIST")
                                    ,   wxT("cv::RETR_CCOMP")
                                    ,   wxT("cv::RETR_TREE")
                                    };
    m_contour_return_mode_choice_ctrl   = new wxChoice (   this
                                                       ,   wxID_ANY
                                                       ,   wxDefaultPosition
                                                       ,   wxSize(120, -1)
                                                       ,   WXSIZEOF(contour_return_mode)
                                                       ,   contour_return_mode
                                                       ,   0
                                                       );
    m_contour_return_mode_choice_ctrl->SetStringSelection(wxT("cv::RETR_LIST"));

    //  10. Метод апроксимации при поиске контуров
    wxString    approximation_method[] =
                                    {   wxT("cv::CHAIN_APPROX_NONE")
                                    ,   wxT("cv::CHAIN_APPROX_SIMPLE")
                                    ,   wxT("cv::CHAIN_APPROX_TC89_L1")
                                    ,   wxT("cv::CHAIN_APPROX_TC89_KCOS")
                                    };
    m_approximation_method_choice_ctrl     = new wxChoice  (   this
                                                           ,   wxID_ANY
                                                           ,   wxDefaultPosition
                                                           ,   wxSize(180, -1)
                                                           ,   WXSIZEOF(approximation_method)
                                                           ,   approximation_method
                                                           ,   0
                                                           );
    m_approximation_method_choice_ctrl->SetStringSelection(wxT("cv::CHAIN_APPROX_SIMPLE"));

    //  11. Толщина линии контура при прорисовке
    m_thickness_spin_ctrl    =   new wxSpinCtrl (   this
                                                ,   wxID_ANY
                                                ,   wxEmptyString
                                                ,   wxDefaultPosition
                                                ,   wxDefaultSize
                                                ,   wxSP_ARROW_KEYS
                                                ,   1        // Минимум:
                                                ,   5        // Максимум:
                                                ,   1        // Текущее значение
                                                );
    //  12. Максимальный уровень иерархии
    m_maxLevel_spin_ctrl    =   new wxSpinCtrl (   this
                                                ,   wxID_ANY
                                                ,   wxEmptyString
                                                ,   wxDefaultPosition
                                                ,   wxDefaultSize
                                                ,   wxSP_ARROW_KEYS
                                                ,   1        // Минимум:
                                                ,   15       // Максимум:
                                                ,   1        // Текущее значение
                                                );

    //  II.    --------------------------------------------------------------------------------------------
    //  Сайзеры
    //  ----------------------------------------------------------------------------------------------------
    //  II.1. Сайзеры для отображения различной вспомогательной инофрмации о тестировании
    //  Добавляем в сайзер: (объект, пропорция, флаги, отступ)
    wxBoxSizer *            informHSizer       =   new wxBoxSizer(wxHORIZONTAL);
    informHSizer->SetMinSize(wxSize(-1, -1)); // -1 значит "любая ширина", -1 значит "любая высота"

    informHSizer->Add(m_transforming_namePanel, 1, wxCENTER | wxALL, 5);
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
    wxString titelStr = wxString::Format("%-150s %-150s %-150s",
    "Превью", "Оригинальное изображение", "Найденные контуры");
    titelSizerHor->Add(new wxStaticText(this, -1, titelStr), 0, wxALL, 5);

    wxBoxSizer *            imagesHSizer       =   new wxBoxSizer(wxHORIZONTAL);
    //  Добавляем в главный сайзер превью-панель
    imagesHSizer->Add(m_previewPanel, 0, wxEXPAND | wxALL, 5);                  // Фикс
    imagesHSizer->Add(m_scrolled_wind_original,     1, wxEXPAND | wxALL, 5);    // Растягивается
    imagesHSizer->Add(m_scrolled_wind_transformed,  1, wxEXPAND | wxALL, 5);    // Растягивается

    //  ----------------------------------------------------------------------------------------------------
    //  II.3. Сайзеры для панели настроек и управления
    wxBoxSizer*         controlPanelHSizer   = new wxBoxSizer(wxHORIZONTAL);
    controlPanelHSizer->SetMinSize(wxSize(-1, 150)); // -1 значит "любая ширина", 150 - фиксированная высота

    // 1. СлайдерЫ масштаба в отдельных столбиках
    wxBoxSizer*         zoomOrigCol             = new wxBoxSizer(wxVERTICAL);
    zoomOrigCol->Add(new wxStaticText(this, -1, "Масштаб оригинала"), 0, wxALL, 5);
    zoomOrigCol->Add(m_zoom_orig_slider_ctrl, 0, wxEXPAND | wxALL, 5);
    controlPanelHSizer->Add(zoomOrigCol, 0, wxEXPAND | wxALL, 5);

    //  2.
    wxBoxSizer*         zoomTransfCol             = new wxBoxSizer(wxVERTICAL);
    zoomTransfCol->Add(new wxStaticText(this, -1, "Масштаб изменения"), 0, wxALL, 5);
    zoomTransfCol->Add(m_zoom_transf_slider_ctrl, 0, wxEXPAND | wxALL, 5);
    controlPanelHSizer->Add(zoomTransfCol, 0, wxEXPAND | wxALL, 5);

    //  3.  Размер квадратного ядра Гауссиана
    wxBoxSizer*         kernelHor           = new wxBoxSizer(wxHORIZONTAL);
    kernelHor->Add(new wxStaticText(this, -1, " Размер\n квадратного\n ядра"), 0, wxALL, 5);
    kernelHor->Add(m_kernel_width_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(kernelHor, 0, wxEXPAND | wxALL, 5);

    //  4. и 5.  Сигмы распределения Гауссиана
    wxBoxSizer*         sigmas_Col       = new wxBoxSizer(wxVERTICAL);
    sigmas_Col->Add(new wxStaticText(this, -1, "sigmaX"), 0, wxALL, 5);
    sigmas_Col->Add(m_sigmaX_spin_ctrl, 0, wxALL, 5);

    sigmas_Col->Add(new wxStaticText(this, -1, "sigmaY"), 0, wxALL, 5);
    sigmas_Col->Add(m_sigmaY_spin_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(sigmas_Col, 0, wxEXPAND | wxALL, 5);

    //  6.  Тип экстраполяции границ Гауссиана (borderType)
    wxBoxSizer*         typeBodersCol       = new wxBoxSizer(wxVERTICAL);
    typeBodersCol->Add(new wxStaticText(this, -1, "Типы границ"), 0, wxALL, 5);
    typeBodersCol->Add(m_borderType_chois_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(typeBodersCol, 0, wxEXPAND | wxALL, 5);

    //  7.  Пороговое значение глобального порогового фильтра
    wxBoxSizer*         thresholdCol         = new wxBoxSizer(wxVERTICAL);
    thresholdCol->Add(new wxStaticText(this, -1, "Порог для Threshold"), 0, wxALL, 5);
    thresholdCol->Add(m_thresh_sldr_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(thresholdCol, 0, wxEXPAND | wxALL, 5);

    //  8.  Тип операции для порогового фильтра
    wxBoxSizer*         threshold_type_Col       = new wxBoxSizer(wxVERTICAL);
    threshold_type_Col->Add(new wxStaticText(this, -1, "Тип операци Threshold"), 0, wxALL, 5);
    threshold_type_Col->Add(m_threshold_type_choice_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(threshold_type_Col, 0, wxEXPAND | wxALL, 5);

    //  9.  Режим возврата контуров
    wxBoxSizer*         contour_return_mode_Col       = new wxBoxSizer(wxVERTICAL);
    contour_return_mode_Col->Add(new wxStaticText(this, -1, "Режим \nвозврата контуров"), 0, wxALL, 5);
    contour_return_mode_Col->Add(m_contour_return_mode_choice_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(contour_return_mode_Col, 0, wxEXPAND | wxALL, 5);

    //  10. Метод апроксимации при поиске контуров
    wxBoxSizer*         approximation_method_Col       = new wxBoxSizer(wxVERTICAL);
    approximation_method_Col->Add(new wxStaticText(this, -1, "Метод аппроксимации"), 0, wxALL, 5);
    approximation_method_Col->Add(m_approximation_method_choice_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(approximation_method_Col, 0, wxEXPAND | wxALL, 5);

    //  11. Толщина линии прорисовки контуров
    wxBoxSizer*         thickness_Col       = new wxBoxSizer(wxVERTICAL);
    thickness_Col->Add(new wxStaticText(this, -1, "Толщина \nлинии"), 0, wxALL, 5);
    thickness_Col->Add(m_thickness_spin_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(thickness_Col, 0, wxEXPAND | wxALL, 5);

    //  12. Максимальный уровень иерархии
    wxBoxSizer*         maxLevel_Col       = new wxBoxSizer(wxVERTICAL);
    maxLevel_Col->Add(new wxStaticText(this, -1, "Максимальный \nуровень \nиерархии"), 0, wxALL, 5);
    maxLevel_Col->Add(m_maxLevel_spin_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(maxLevel_Col, 0, wxEXPAND | wxALL, 5);

    //  -------------------------------------------------------------------------------------------------

    //  II.4. Объединяем сайзеры отображения изображений и панели управления отображением
    //  И наконец добавляем горизонтальные сайзеры в главный вертикальный сайзер
    wxBoxSizer *         mainSizer          =   new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_informPanel, 0, wxEXPAND | wxALL, 5);      // Панель только столько, сколько ей нужно
    mainSizer->Add(titelSizerHor, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(imagesHSizer, 1, wxEXPAND | wxALL, 5);       // Картинки забирают всё свободное место
    mainSizer->Add(controlPanelHSizer, 0, wxEXPAND | wxALL, 5); // Панель только столько, сколько ей нужно

    this->SetSizer(mainSizer);
    //  ----------------------------------------------------------------------------------------------------------
    //  Принудительно заставляем сайзер родительского окна (или панели) рассчитать размеры
    this->Layout();

    //  III.     ------------------------------------------------------------------------------------------------
    //  Привязка обработчиков событий
    //  1.
    m_zoom_orig_slider_ctrl->Bind(wxEVT_SLIDER, &find_and_drawContoursTestFrame::On_zoomOrigSliderChanged, this);
    //  2.
    m_zoom_transf_slider_ctrl->Bind(wxEVT_SLIDER, &find_and_drawContoursTestFrame::On_zoomTransfSliderChanged, this);
    //  3.
    m_kernel_width_ctrl->Bind(EVT_KERNEL_CHANGED, &find_and_drawContoursTestFrame::On_kernel_widthChanged, this);
    //  4.
    m_sigmaX_spin_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &find_and_drawContoursTestFrame::On_sigmaXChanged, this);
    //  5.
    m_sigmaY_spin_ctrl->Bind(wxEVT_SPINCTRLDOUBLE,
                                        &find_and_drawContoursTestFrame::On_sigmaYChanged, this);
    //  6.
    m_borderType_chois_ctrl->Bind(wxEVT_CHOICE,
                                        &find_and_drawContoursTestFrame::On_borderTypeChanged, this);
    //  7.
    m_thresh_sldr_ctrl->Bind(wxEVT_SLIDER,
                                        &find_and_drawContoursTestFrame::On_threshChanged, this);
    //  8.
    m_threshold_type_choice_ctrl->Bind(wxEVT_CHOICE,
                                        &find_and_drawContoursTestFrame::On_threshold_typeChanged, this);
    //  9.
    m_contour_return_mode_choice_ctrl->Bind(wxEVT_CHOICE,
                                        &find_and_drawContoursTestFrame::On_contour_return_modeChanged, this);
    //  10.
    m_approximation_method_choice_ctrl->Bind(wxEVT_CHOICE,
                                        &find_and_drawContoursTestFrame::On_approximation_methodChanged, this);
    //  11.
    m_thickness_spin_ctrl->Bind(wxEVT_SPINCTRL,
                                        &find_and_drawContoursTestFrame::On_thicknessChanged, this);

    // 12.
    m_maxLevel_spin_ctrl->Bind(wxEVT_SPINCTRL,
                                        &find_and_drawContoursTestFrame::On_maxLevelChanged, this);

    Bind(wxEVT_SIZE, &OnResize, this);

    //  IV. -----------------------------------------------------------------------------------------------------
    //  Получение матрицы фильтруемого изображения, считанного из дискового файла
    m_cv_original_img   =   getOriginalImage();

    //  Готовим полутоновое изображение
    cv::cvtColor(m_cv_original_img, gray, cv::COLOR_BGR2GRAY);

	//  V. --------------------------------------------------------------------------------------------------------------------
    //  Морфологическая трансформация оригинального изображения OpenCV
    ApplyTransformation();

    //  VI. ----------------------------------------------------------------------------------------------------------------
    //  Отображение всех изображений
    UpdateAllViews();

    //  ----------------------------------------------------------------------------------------------------------------
    Center () ;
}

void find_and_drawContoursTestFrame::OnResize(wxSizeEvent& event)
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
void find_and_drawContoursTestFrame::On_zoomOrigSliderChanged(wxCommandEvent& event) {

    m_zoom_orig_slider_ctrl->m_zoom = event.GetInt();

    UpdateAllViews();

    Layout();
}
//  2.
void find_and_drawContoursTestFrame::On_zoomTransfSliderChanged(wxCommandEvent& event) {

    m_zoom_transf_slider_ctrl->m_zoom = event.GetInt();

    UpdateAllViews();

    Layout();
}

//  3.
void find_and_drawContoursTestFrame::On_kernel_widthChanged(wxCommandEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_kernel_widthChanged()", MB_OK | MB_ICONINFORMATION);
    #endif

    m_kernel_width = event.GetInt();

    ApplyTransformation();

    UpdateAllViews();

    Layout();
}
//  4.
void find_and_drawContoursTestFrame::On_sigmaXChanged(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_sigmaXChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    m_sigmaX = event.GetValue();

    ApplyTransformation();

    UpdateAllViews();

    Layout();
}
//  5.
void find_and_drawContoursTestFrame::On_sigmaYChanged(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_sigmaYChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    m_sigmaY  =   event.GetValue();
    #if 0
    std::wstring msg = L"\nm_sigmaY: " + std::to_wstring(m_sigmaY);
    m_debugInfo->AppendText(msg);
    #endif

    ApplyTransformation();

    UpdateAllViews();

    Layout();
}
//  6.
void find_and_drawContoursTestFrame::On_borderTypeChanged(wxCommandEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_borderTypeChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    m_borderType  =   event.GetInt();
    #if 0
    std::wstring msg = L"\n: " + std::to_wstring(m_borderType);
    m_debugInfo->AppendText(msg);
    #endif

    ApplyTransformation();

    UpdateAllViews();

    Layout();
}
//  7.
void find_and_drawContoursTestFrame::On_threshChanged(wxCommandEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_threshChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    m_thresh  =   (double)event.GetInt();
    #if 0
    std::wstring msg = L"\nКоэффициент X нижнего левого угла: " + std::to_wstring(m_thresh);
    m_debugInfo->AppendText(msg);
    #endif

    ApplyTransformation();

    UpdateAllViews();

    Layout();
}
//  8.
void find_and_drawContoursTestFrame::On_threshold_typeChanged(wxCommandEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_threshold_typeChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    m_threshold_type  =   event.GetInt();
    #if 0
    std::wstring msg = L"\n: " + std::to_wstring(m_threshold_type);
    m_debugInfo->AppendText(msg);
    #endif

    ApplyTransformation();

    UpdateAllViews();

    Layout();
}
//  9.
void find_and_drawContoursTestFrame::On_contour_return_modeChanged(wxCommandEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_contour_return_modeChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    m_contour_return_mode = event.GetInt();
    #if 0
    std::wstring msg = L"\n: " + std::to_wstring(m_contour_return_mode);
    m_debugInfo->AppendText(msg);
    #endif

    ApplyTransformation();

    UpdateAllViews();

    Layout();
}
//  10.
void find_and_drawContoursTestFrame::On_approximation_methodChanged(wxCommandEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_approximation_methodChanged()", MB_OK | MB_ICONINFORMATION);
    #endif

    m_approximation_method  =   event.GetInt() + 1;   //  Нумерация флагов начинается с единицы
    #if 0
    std::wstring msg = L"\n: " + std::to_wstring(m_approximation_method);
    m_debugInfo->AppendText(msg);
    #endif

    ApplyTransformation();

    UpdateAllViews();

    Layout();
}
//  11.
void find_and_drawContoursTestFrame::On_thicknessChanged(wxSpinEvent& event) {
    // 1. Обновляем переменную типа границы
    m_thickness  =   event.GetInt();
    #if 1
    std::wstring msg = L"\nТолщина контура: " + std::to_wstring(m_thickness);
    m_debugInfo->AppendText(msg);
    #endif

    ApplyTransformation();

    UpdateAllViews();

    Layout();
}
//  12.
void find_and_drawContoursTestFrame::On_maxLevelChanged(wxSpinEvent& event) {
    // 1. Обновляем переменную типа границы
    m_maxLevel  =   event.GetInt();
    #if 1
    std::wstring msg = L"\nМаксимальный уровень иерархии: " + std::to_wstring(m_maxLevel);
    m_debugInfo->AppendText(msg);
    #endif

    ApplyTransformation();

    UpdateAllViews();

    Layout();
}
// Универсальная функция, которая знает про оба окна
void find_and_drawContoursTestFrame::UpdateAllViews() {
    #if 0
    MessageBoxW(NULL, L"Работает before m_cv_original_img", L"UpdateAllViews()", MB_OK | MB_ICONINFORMATION);
    #endif
    //  ---------------------------------------------------------------------------------------------------------
    //  Вывод оригинального изображения - оно может быть и трехканальным и одноканалным, лишь бы не бинарным
    UpdateDisplay   (   m_wx_img
                    ,   m_cv_original_img
                    ,   m_originalBitmap
                    ,   m_staticOriginalBitmap
                    ,   m_scrolled_wind_original
                    ,   m_zoom_orig_slider_ctrl->m_zoom
                    );

    //  ---------------------------------------------------------------------------------------------------------
    //  Преобразования для вывода бинарного изображения OpenCV в формате библиотеки wxVidgets
    #if 0
    MessageBoxW(NULL, L"Работает before bitwise_and", L"UpdateAllViews()", MB_OK | MB_ICONINFORMATION);
    #endif

    gray_tmp = binary.clone();
    cv::cvtColor(gray_tmp, m_cv_transformed_img, cv::COLOR_GRAY2BGR);

    #if 0
    MessageBoxW(NULL, L"Работает before m_cv_transformed_img", L"UpdateAllViews()", MB_OK | MB_ICONINFORMATION);
    #endif
    UpdateDisplay   (   m_wx_img
                    ,   m_cv_transformed_img
                    ,   m_transformedBitmap
                    ,   m_staticTransformedBitmap
                    ,   m_scrolled_wind_transformed
                    ,   m_zoom_transf_slider_ctrl->m_zoom
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
void find_and_drawContoursTestFrame::ApplyTransformation() {
    #if 0
    MessageBoxW(NULL, L"Работает", L"ApplyTransformation()", MB_OK | MB_ICONINFORMATION);
    #endif
    // Вся "кухня" OpenCV здесь

	cv::GaussianBlur(gray, blurred, cv::Size(m_kernel_width, m_kernel_width), m_borderType);

	cv::threshold	( 	blurred
					, 	binary
					, 	m_thresh
					, 	255.0
					, 	cv::THRESH_BINARY
					);

    contours.clear();
    //hierarchy.clear();
	#if 0
    MessageBoxW(NULL, L"Работает before cv::findContours", L"ApplyTransformation()", MB_OK | MB_ICONINFORMATION);
    #endif
    std::vector<cv::Vec4i> 				hierarchy;

	cv::findContours	(	binary
                        ,	contours
                        ,	hierarchy               //  информация о топологии
                        ,	m_contour_return_mode
                        ,	m_approximation_method
                        );

	binary 	= 	cv::Scalar::all(0);     //  Делается черный фон в бинарном изображении

	#if 0
    MessageBoxW(NULL, L"Работает before cv::drawContours", L"ApplyTransformation()", MB_OK | MB_ICONINFORMATION);
    #endif

    int         		lineType 			= 8;           	// 	связность ('4' или '8')

	cv::drawContours	( 	binary          //  Контуры рисуются на входном изображении
                        , 	contours
                        , 	-1
                        , 	cv::Scalar::all(255)
                        ,   m_thickness
                        ,   lineType
                        ,   hierarchy               //  информация о топологии
                        ,   m_maxLevel
                        );
    #if 0
    MessageBoxW(NULL, L"Работает after cv::drawContours", L"ApplyTransformation()", MB_OK | MB_ICONINFORMATION);
    #endif
}

std::string find_and_drawContoursTestFrame::getSignatureText(){
    std::string signature_text =
    "void cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);"
    "\n"
    "void cv::GaussianBlur(cv::InputArray gray, cv::OutputArray blured"
    "\n,    cv::Size ksize"
    "\n,    double sigmaX = 0.0, double sigmaY = 0.0"
    "\n,    int borderType = cv::BORDER_CONSTANT"
    "\n);"
    "\n"
    "void cv::threshold(cv::InputArray blured, cv::OutputArray binary"
    "\n,    double thresh, double maxValue"
    "\n,    int thresholdType = cv::THRESH_BINARY"
    "\n);"
    "\n"
    "std::vector<std::vector<cv::Point>> 	contours;"
    "\n"
    "void cv::findContours(cv::InputArray binary, cv::OutputArray contours"
    "\n,    cv::OutputArray hierarchy"
    "\n,    int  contour_return_mode  = cv::RETR_LIST"
    "\n,    int  approximation_method = cv::CHAIN_APPROX_SIMPLE"
    "\n,    cv::Point points_offset   = cv::Point()"
    "\n);"
    "void cv::drawContours(cv::InputArray binary, cv::OutputArray contours"
    "\n,    int contourIdx"
    "\n,    const cv::Scalar& contours_color"
    "\n,    int thickness           = 1"
    "\n,    int maxHierarchyLevel   = INT_MAX"
    "\n,    cv::Point points_offset = cv::Point()"
    "\n);"
    ;
    return signature_text;
}
