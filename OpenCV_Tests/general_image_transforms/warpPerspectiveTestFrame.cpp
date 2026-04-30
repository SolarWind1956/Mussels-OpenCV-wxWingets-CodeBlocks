//  warpPerspectiveTestFrame.cpp
/*
 ***************************************************************
 * Name:      warpPerspectiveTestFrame.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-29
 * Copyright: Sergi E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 *************************************************************
*/
#include "warpPerspectiveTestFrame.h"
/*
    Класс тестирования преобрзования `warpPerspectiveTestFrame`
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
warpPerspectiveTestFrame::warpPerspectiveTestFrame  ( wxWindow  *   parent, const wxString& title)
                                : wxFrame   (   parent
                                            ,   wxID_ANY
                                            ,   title
                                            ,   wxDefaultPosition
                                            ,   wxSize(1600, 1000)
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
                                ,   wxT("`warpPerspective`\nGeneral \nPerspective Transform")
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

    //  3.  Новое значение для ширины изображеия
    m_spin_newWidth_ctrl    =   new wxSpinCtrl          (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   0           // Минимум:
                                                        ,   1920        // Максимум:
                                                        ,   0
                                                        );
    //  4.  Новое значение для высоты изображения
    m_spin_newHeight_ctrl    =   new wxSpinCtrl         (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   0           // Минимум:
                                                        ,   1080        // Максимум:
                                                        ,   0
                                                        );


    //  5.  Коэффициенты по верхнему левому углу
    m_spin_top_left_col_ctrl      =
                                new wxSpinCtrlDouble   (   this
                                                       ,   wxID_ANY
                                                       ,   wxEmptyString
                                                       ,   wxDefaultPosition
                                                       ,   wxDefaultSize
                                                       ,   wxSP_ARROW_KEYS
                                                       ,   0.01     //  Минимум:
                                                       ,   0.99      //  Максимум:
                                                       ,   0.05     //  Начальное значение
                                                       ,   0.01     //  Шаг: золотая середина
                                                       );
    //  Дополнительно количество знаков после запятой
    m_spin_top_left_col_ctrl->SetDigits(2);

    m_spin_top_left_row_ctrl      =
                                new wxSpinCtrlDouble   (   this
                                                       ,   wxID_ANY
                                                       ,   wxEmptyString
                                                       ,   wxDefaultPosition
                                                       ,   wxDefaultSize
                                                       ,   wxSP_ARROW_KEYS
                                                       ,   0.01     //  Минимум:
                                                       ,   0.99      //  Максимум:
                                                       ,   0.33     //  Начальное значение
                                                       ,   0.01     //  Шаг: золотая середина
                                                       );
    //  Дополнительно количество знаков после запятой
    m_spin_top_left_row_ctrl->SetDigits(2);

    //  6.  Коэффициенты по верхнему правому углу
        m_spin_top_right_col_ctrl      =
                                new wxSpinCtrlDouble   (   this
                                                       ,   wxID_ANY
                                                       ,   wxEmptyString
                                                       ,   wxDefaultPosition
                                                       ,   wxDefaultSize
                                                       ,   wxSP_ARROW_KEYS
                                                       ,   0.01     //  Минимум:
                                                       ,   0.99      //  Максимум:
                                                       ,   0.9     //  Начальное значение
                                                       ,   0.01     //  Шаг: золотая середина
                                                       );
    //  Дополнительно количество знаков после запятой
    m_spin_top_right_col_ctrl->SetDigits(2);

    m_spin_top_right_row_ctrl      =
                                new wxSpinCtrlDouble   (   this
                                                       ,   wxID_ANY
                                                       ,   wxEmptyString
                                                       ,   wxDefaultPosition
                                                       ,   wxDefaultSize
                                                       ,   wxSP_ARROW_KEYS
                                                       ,   0.01     //  Минимум:
                                                       ,   0.99     //  Максимум:
                                                       ,   0.25     //  Начальное значение
                                                       ,   0.01     //  Шаг: золотая середина
                                                       );
    //  Дополнительно количество знаков после запятой
    m_spin_top_right_row_ctrl->SetDigits(2);

    //  7.  Коэффициенты по нижнему левому углу
    m_spin_bottom_left_col_ctrl      =
                                new wxSpinCtrlDouble   (   this
                                                       ,   wxID_ANY
                                                       ,   wxEmptyString
                                                       ,   wxDefaultPosition
                                                       ,   wxDefaultSize
                                                       ,   wxSP_ARROW_KEYS
                                                       ,   0.01     //  Минимум:
                                                       ,   0.99     //  Максимум:
                                                       ,   0.8     //  Начальное значение
                                                       ,   0.01     //  Шаг: золотая середина
                                                       );
    //  Дополнительно количество знаков после запятой
    m_spin_bottom_left_col_ctrl->SetDigits(2);

    m_spin_bottom_left_row_ctrl      =
                                new wxSpinCtrlDouble   (   this
                                                       ,   wxID_ANY
                                                       ,   wxEmptyString
                                                       ,   wxDefaultPosition
                                                       ,   wxDefaultSize
                                                       ,   wxSP_ARROW_KEYS
                                                       ,   0.01     //  Минимум:
                                                       ,   1.0      //  Максимум:
                                                       ,   0.9     //  Начальное значение
                                                       ,   0.01     //  Шаг: золотая середина
                                                       );
    //  Дополнительно количество знаков после запятой
    m_spin_bottom_left_row_ctrl->SetDigits(2);

    //  7.  Коэффициенты по нижнему правому углу
    m_spin_bottom_right_col_ctrl      =
                                new wxSpinCtrlDouble   (   this
                                                       ,   wxID_ANY
                                                       ,   wxEmptyString
                                                       ,   wxDefaultPosition
                                                       ,   wxDefaultSize
                                                       ,   wxSP_ARROW_KEYS
                                                       ,   0.01     //  Минимум:
                                                       ,   1.0      //  Максимум:
                                                       ,   0.2     //  Начальное значение
                                                       ,   0.01     //  Шаг: золотая середина
                                                       );
    //  Дополнительно количество знаков после запятой
    m_spin_bottom_right_col_ctrl->SetDigits(2);

    m_spin_bottom_right_row_ctrl      =
                                new wxSpinCtrlDouble   (   this
                                                       ,   wxID_ANY
                                                       ,   wxEmptyString
                                                       ,   wxDefaultPosition
                                                       ,   wxDefaultSize
                                                       ,   wxSP_ARROW_KEYS
                                                       ,   0.01     //  Минимум:
                                                       ,   1.0      //  Максимум:
                                                       ,   0.7     //  Начальное значение
                                                       ,   0.01     //  Шаг: золотая середина
                                                       );
    //  Дополнительно количество знаков после запятой
    m_spin_bottom_right_row_ctrl->SetDigits(2);

    //  9.  Метод интерполяции
    wxString    interpolation[] =   {   wxT("cv::INTER_NEAREST")    //  Nearest neighbor
                                    ,   wxT("cv::INTER_LINEAR")     //  Bilinear
                                    ,   wxT("cv::INTER_AREA")       //  Pixel area resampling
                                    ,   wxT("cv::INTER_CUBIC")      //  Bicubic interpolation
                                    ,   wxT("cv::INTER_LANCZOS4")   //  Lanczos interpolation over 8×8 neighborhood
                                    };
    m_interpolation_chois_ctrl     =   new wxChoice      (   this
                                                         ,   wxID_ANY
                                                         ,   wxDefaultPosition
                                                         ,   wxSize(180, -1)
                                                         ,   WXSIZEOF(interpolation)
                                                         ,   interpolation
                                                         ,   0
                                                         );
    m_interpolation_chois_ctrl->SetStringSelection(wxT("cv::INTER_LINEAR"));

    //  10.  Тип  экстраполяции границы (borderType)
    wxString    borderTypes[] =    {   wxT("cv::BORDER_CONSTANT")  //  заполнение цветом (по умолчанию)
                                   ,   wxT("cv::BORDER_REPLICATE") //  повтор последнего пикселя (а-а-а-а)
                                   ,   wxT("cv::BORDER_REFLECT")   //  зеркало (абв|вба)
                                   };

    m_borderType_chois_ctrl =   new wxChoice     (   this
                                                 ,   wxID_ANY
                                                 ,   wxDefaultPosition
                                                 ,   wxSize(180, -1)
                                                 ,   WXSIZEOF(borderTypes)
                                                 ,   borderTypes
                                                 ,   0
                                                 );
    m_borderType_chois_ctrl->SetStringSelection(wxT("cv::BORDER_CONSTANT"));


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
    "Превью", "Оригинальное изображение", "Изображение после перспективных преобразований");
    titelSizerHor->Add(new wxStaticText(this, -1, titelStr), 0, wxALL, 5);

    wxBoxSizer *            imagesHSizer       =   new wxBoxSizer(wxHORIZONTAL);
    //  Добавляем в главный сайзер превью-панель
    imagesHSizer->Add(m_previewPanel, 0, wxEXPAND | wxALL, 5);                  // Фикс
    imagesHSizer->Add(m_scrolled_wind_original,     1, wxEXPAND | wxALL, 5);    // Растягивается
    imagesHSizer->Add(m_scrolled_wind_transformed,     1, wxEXPAND | wxALL, 5);    // Растягивается

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

    //  3.  Новое значение для ширины изображеия
    wxBoxSizer*         newWidth_Col       = new wxBoxSizer(wxVERTICAL);
    newWidth_Col->Add(new wxStaticText(this, -1, "Новая ширина"), 0, wxALL, 5);
    newWidth_Col->Add(m_spin_newWidth_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(newWidth_Col, 0, wxEXPAND | wxALL, 5);

    //  4.  Новое значение для высоты изображеия
    wxBoxSizer*         newHeight_Col       = new wxBoxSizer(wxVERTICAL);
    newHeight_Col->Add(new wxStaticText(this, -1, "Новая высота"), 0, wxALL, 5);
    newHeight_Col->Add(m_spin_newHeight_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(newHeight_Col, 0, wxEXPAND | wxALL, 5);

    // 5. Создаем статический бокс с заголовком для верхнего левого угла
    wxStaticBoxSizer*   top_leftSizer   = new wxStaticBoxSizer(wxVERTICAL, parent, wxEmptyString);
    top_leftSizer->Add(new wxStaticText(this, -1, "Top-Left"), 0, wxALL, 5);
    top_leftSizer->Add(m_spin_top_left_col_ctrl, 0, wxALL, 5);
    top_leftSizer->Add(m_spin_top_left_row_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(top_leftSizer, 0, wxEXPAND | wxALL, 10);

    //  6. Создаем статический бокс с заголовком для верхнего правого угла
    wxStaticBoxSizer*   top_rightSizer   = new wxStaticBoxSizer(wxVERTICAL, parent, wxEmptyString);
    top_rightSizer->Add(new wxStaticText(this, -1, "Top-Right"), 0, wxALL, 5);
    top_rightSizer->Add(m_spin_top_right_col_ctrl, 0, wxALL, 5);
    top_rightSizer->Add(m_spin_top_right_row_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(top_rightSizer, 0, wxEXPAND | wxALL, 10);

    // 7. Создаем статический бокс с заголовком для нижнего левого угла
    wxStaticBoxSizer*   bottom_leftSizer   = new wxStaticBoxSizer(wxVERTICAL, parent, wxEmptyString);
    bottom_leftSizer->Add(new wxStaticText(this, -1, "Bottom-Left"), 0, wxALL, 5);
    bottom_leftSizer->Add(m_spin_bottom_left_col_ctrl, 0, wxALL, 5);
    bottom_leftSizer->Add(m_spin_bottom_left_row_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(bottom_leftSizer, 0, wxEXPAND | wxALL, 10);

    //  8. Создаем статический бокс с заголовком для верхнего правого угла
    wxStaticBoxSizer*   bottom_rightSizer   = new wxStaticBoxSizer(wxVERTICAL, parent, wxEmptyString);
    bottom_rightSizer->Add(new wxStaticText(this, -1, "Bottom-Right"), 0, wxALL, 5);
    bottom_rightSizer->Add(m_spin_bottom_right_col_ctrl, 0, wxALL, 5);
    bottom_rightSizer->Add(m_spin_bottom_right_row_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(bottom_rightSizer, 0, wxEXPAND | wxALL, 10);

    //  9.  Выпадающий список для выбора метода интерполяции
    wxBoxSizer*         interpolationCol       = new wxBoxSizer(wxVERTICAL);
    interpolationCol->Add(new wxStaticText(this, -1, "Метод интерполяции"), 0, wxALL, 5);
    interpolationCol->Add(m_interpolation_chois_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(interpolationCol, 0, wxEXPAND | wxALL, 10);

    //  10.  Выпадающий список для выбора типа границ
    wxBoxSizer*         borderTypeCol       = new wxBoxSizer(wxVERTICAL);
    borderTypeCol->Add(new wxStaticText(this, -1, "Тип границ"), 0, wxALL, 5);
    borderTypeCol->Add(m_borderType_chois_ctrl, 0, wxALL, 5);
    controlPanelHSizer->Add(borderTypeCol, 0, wxEXPAND | wxALL, 10);

    //  -------------------------------------------------------------------------------------------------

    //  II.4. Объединяем сайзеры отображения изображений и панели управления отображением
    //  И наконец добавляем горизонтальные сайзеры в главный вертикальный сайзер
    wxBoxSizer *         mainSizer          =   new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_informPanel, 0, wxEXPAND | wxALL, 5);      // Панель только столько, сколько ей нужно
    mainSizer->Add(titelSizerHor, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(imagesHSizer, 1, wxEXPAND | wxALL, 5);       // Картинки забирают всё свободное место
    mainSizer->Add(controlPanelHSizer, 0, wxEXPAND | wxALL, 5); // Панель только столько, сколько ей нужно

    this->SetSizer(mainSizer);
    //  ----------------------------------------------------------------------------------------------------------
    //  Именно после настройки сайзеров можно установить начальные значения на спин-контролах ширины и высоты
    //  окна преобразованного изображения, только я пока не заню, как
    // Получаем текущий размер окна для трансформированного изображения
    // 1. Принудительно заставляем сайзер родительского окна (или панели) рассчитать размеры
    this->Layout();

    // 2. Спрашиваем размер клиентской области (без рамок и заголовков)
    wxSize dstSize = m_scrolled_wind_transformed->GetClientSize();
    m_spin_newWidth_ctrl->SetValue(dstSize.x);
    m_spin_newHeight_ctrl->SetValue(dstSize.y);
    //  III.     ------------------------------------------------------------------------------------------------
    //  Привязка обработчиков событий
    //  В конструкторе теперь только "регистрация"
    //  1.
    m_zoom_orig_slider_ctrl->Bind(wxEVT_SLIDER, &warpPerspectiveTestFrame::On_zoomOrigSliderChanged, this);
    //  2.
    m_zoom_transf_slider_ctrl->Bind(wxEVT_SLIDER, &warpPerspectiveTestFrame::On_zoomTransfSliderChanged, this);
    //  3.
    m_spin_newWidth_ctrl->Bind(wxEVT_SPINCTRL, &warpPerspectiveTestFrame::On_newWidthChanged, this);
    //  4.
    m_spin_newHeight_ctrl->Bind(wxEVT_SPINCTRL, &warpPerspectiveTestFrame::On_newHeightChanged, this);
    //  5.
    m_spin_top_left_col_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &warpPerspectiveTestFrame::On_top_left_col_Changed, this);
    m_spin_top_left_row_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &warpPerspectiveTestFrame::On_top_left_row_Changed, this);
    //  6.
    m_spin_top_right_col_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &warpPerspectiveTestFrame::On_top_right_col_Changed, this);
    m_spin_top_right_row_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &warpPerspectiveTestFrame::On_top_right_row_Changed, this);
    //  7.
    m_spin_bottom_left_col_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &warpPerspectiveTestFrame::On_bottom_left_col_Changed, this);
    m_spin_bottom_left_row_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &warpPerspectiveTestFrame::On_bottom_left_row_Changed, this);
    //  8.
    m_spin_bottom_right_col_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &warpPerspectiveTestFrame::On_bottom_right_col_Changed, this);
    m_spin_bottom_right_row_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &warpPerspectiveTestFrame::On_bottom_right_row_Changed, this);
    //  9.
    m_interpolation_chois_ctrl->Bind(wxEVT_CHOICE, &warpPerspectiveTestFrame::On_interpolationChanged, this);
    //  10.
    m_borderType_chois_ctrl->Bind(wxEVT_CHOICE, &warpPerspectiveTestFrame::On_borderTypeChanged, this);

    Bind(wxEVT_SIZE, &OnResize, this);

    //  IV. -----------------------------------------------------------------------------------------------------
    //  Получение матрицы фильтруемого изображения, считанного из дискового файла
    m_cv_original_img   =   getOriginalImage();

	//  V. --------------------------------------------------------------------------------------------------------------------
    //  Морфологическая трансформация оригинального изображения OpenCV
    ApplyTransformation();

    //  VI. ----------------------------------------------------------------------------------------------------------------
    //  Отображение всех изображений
    UpdateAllViews();

    //  ----------------------------------------------------------------------------------------------------------------
    Center () ;
}

void warpPerspectiveTestFrame::OnResize(wxSizeEvent& event)
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
void warpPerspectiveTestFrame::On_zoomOrigSliderChanged(wxCommandEvent& event) {
    // 1. Извлекаем масшаб
    m_zoom_orig_slider_ctrl->m_zoom = event.GetInt();
    // 2. Запускаем обработку OpenCV
    UpdateAllViews();

    Layout();
}
//  2.
void warpPerspectiveTestFrame::On_zoomTransfSliderChanged(wxCommandEvent& event) {
    // 1. Извлекаем масшаб
    m_zoom_transf_slider_ctrl->m_zoom = event.GetInt();
    // 2. Запускаем обработку OpenCV
    UpdateAllViews();

    Layout();
}

//  3.
void warpPerspectiveTestFrame::On_newWidthChanged(wxSpinEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_newWidthChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1. Извлекаем новую ширину
    m_newWidth = event.GetInt();

    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  4.
void warpPerspectiveTestFrame::On_newHeightChanged(wxSpinEvent& event) {
     #if 0
    MessageBoxW(NULL, L"Работает", L"On_newHeightChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1. Извлекаем новую высоту
    m_newHeight = event.GetInt();

    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  5.
void warpPerspectiveTestFrame::On_top_left_col_Changed(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_top_left_col_Changed()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем коэффициет
    m_top_left_col  =   event.GetValue();
    #if 0
    std::wstring msg = L"\nКоэффициент X верхнего левого угла: " + std::to_wstring(m_top_left_col);
    m_debugInfo->AppendText(msg);
    #endif
    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
void warpPerspectiveTestFrame::On_top_left_row_Changed(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_top_left_row_Changed()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем коэффициет
    m_top_left_row  =   event.GetValue();
    #if 0
    std::wstring msg = L"\nКоэффициент Y верхнего левого угла: " + std::to_wstring(m_top_left_row);
    m_debugInfo->AppendText(msg);
    #endif
    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}

//  6.
void warpPerspectiveTestFrame::On_top_right_col_Changed(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_top_right_col_Changed()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем коэффициет
    m_top_right_col  =   event.GetValue();
    #if 0
    std::wstring msg = L"\nКоэффициент X верхнего правого угла: " + std::to_wstring(m_top_right_col);
    m_debugInfo->AppendText(msg);
    #endif
    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
void warpPerspectiveTestFrame::On_top_right_row_Changed(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_top_right_row_Changed()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем коэффициет
    m_top_right_row  =   event.GetValue();
    #if 0
    std::wstring msg = L"\nКоэффициент Y верхнего правого угла: " + std::to_wstring(m_top_right_row);
    m_debugInfo->AppendText(msg);
    #endif
    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  7.
void warpPerspectiveTestFrame::On_bottom_left_col_Changed(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_bottom_left_col_Changed()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем коэффициет
    m_bottom_left_col  =   event.GetValue();
    #if 0
    std::wstring msg = L"\nКоэффициент X нижнего левого угла: " + std::to_wstring(m_bottom_left_col);
    m_debugInfo->AppendText(msg);
    #endif
    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
void warpPerspectiveTestFrame::On_bottom_left_row_Changed(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_bottom_left_row_Changed()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем коэффициет
    m_bottom_left_row  =   event.GetValue();
    #if 0
    std::wstring msg = L"\nКоэффициент Y нижнего левого угла: " + std::to_wstring(m_bottom_left_row);
    m_debugInfo->AppendText(msg);
    #endif
    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  8.
void warpPerspectiveTestFrame::On_bottom_right_col_Changed(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_bottom_right_col_Changed()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем коэффициет
    m_bottom_right_col  =   event.GetValue();
    #if 0
    std::wstring msg = L"\nКоэффициент X нижнего правого угла: " + std::to_wstring(m_bottom_right_col);
    m_debugInfo->AppendText(msg);
    #endif
    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
void warpPerspectiveTestFrame::On_bottom_right_row_Changed(wxSpinDoubleEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_bottom_right_row_Changed()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем коэффициет
    m_bottom_right_row  =   event.GetValue();
    #if 0
    std::wstring msg = L"\nКоэффициент Y нижнего правого угла: " + std::to_wstring(m_bottom_right_row);
    m_debugInfo->AppendText(msg);
    #endif
    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}

//  9.
void warpPerspectiveTestFrame::On_interpolationChanged(wxCommandEvent& event) {
    // 1. Обновляем переменную типа границы
    m_interpolation  =   event.GetInt();
    #if 1
    std::wstring msg = L"\nИндекс строки интерполяции: " + std::to_wstring(m_interpolation);
    m_debugInfo->AppendText(msg);
    #endif

    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  10.
void warpPerspectiveTestFrame::On_borderTypeChanged(wxCommandEvent& event) {
    // 1. Обновляем переменную типа границы
    m_borderType  =   event.GetInt();
    #if 1
    std::wstring msg = L"\nИндекс строки типа границ: " + std::to_wstring(m_borderType);
    m_debugInfo->AppendText(msg);
    #endif

    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
// Универсальная функция, которая знает про оба окна
void warpPerspectiveTestFrame::UpdateAllViews() {
    //  Визуализация оригинального, отфильтрованного и трансформированного изображений

    //  Преобразования для вывода оригинального изображения OpenCV в формате библиотеки wxVidgets
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
void warpPerspectiveTestFrame::ApplyTransformation() {
    #if 0
    MessageBoxW(NULL, L"Работает", L"ApplyTransformation()", MB_OK | MB_ICONINFORMATION);
    #endif
    // Вся "кухня" OpenCV здесь

    cv::warpPerspective (   m_cv_original_img
                        ,   m_cv_transformed_img
                        ,   get_PerspectiveTransfMatrix()
                        ,   cv::Size(m_newWidth, m_newHeight)
                        ,   m_interpolation
                        ,   m_borderType
                        );
}
//  Функция вычисление матрицы аффинного преобразования
cv::Mat      warpPerspectiveTestFrame::get_PerspectiveTransfMatrix(){

    src =   m_cv_original_img.clone();

    cv::Point2f 		srcQuad[] = {
		cv::Point2f(0,0),                     		// 	левый верхний угол `src`
		cv::Point2f(src.cols - 1, 0),           		// 	правый верхний угол `src`
		cv::Point2f(src.cols - 1, src.rows - 1),  	// 	правый нижний угол `src`
		cv::Point2f(0, src.rows-1)            		// 	левый нижний угол `src`
	};

	cv::Point2f         dstQuad[] = {
		cv::Point2f(src.cols * m_top_left_col,      src.rows * m_top_left_row),
		cv::Point2f(src.cols * m_top_right_col,     src.rows * m_top_right_row),
		cv::Point2f(src.cols * m_bottom_left_col,   src.rows * m_bottom_left_row),
		cv::Point2f(src.cols * m_bottom_right_col,  src.rows * m_bottom_right_row)
	};

	warp_mat = cv::getPerspectiveTransform(srcQuad, dstQuad);
    return          warp_mat.clone();
}

std::string warpPerspectiveTestFrame::getSignatureText(){
    std::string signature_text =

    "void cv::warpPerspective("
    "\n,    cv::InputArray  src"
    "\n,    cv::OutputArray dst"
    "\n,    cv::InputArray  cv::getPerspectiveTransform( const cv::Point2f*"
    "\n,                                          , const cv::Point2f*)"
    "\n,    cv::Size dsize"
    "\n,    int flags = cv::INTER_LINEAR"
    "\n,    int borderMode = cv::BORDER_CONSTANT"
    "\n,    const cv::Scalar& borderValue = cv::Scalar()"
    "\n);"
    "\n"
    "\ncv::getPerspectiveTransform("
    "\n,     const cv::Point2f*"
    "\n,     const cv::Point2f*"
    "\n);"
    ;

    return signature_text;
#if 0
void cv::warpPerspective (   cv::InputArray                  src             //  входное изображение
                    ,   cv::OutputArray                 dst             //  выходное изображение
                    ,   cv::InputArray                  cv::getPerspectiveTransform(const cv::Point2f*, const cv::Point2f*)
                                                                        //  матрица преобразования 2×3
                    ,   cv::Size                        dsize           //  размер выходного изображения
                    ,   int                             flags       =   cv::INTER_LINEAR
                                                                        //  интерполяция, обращение
                    ,   int                             borderMode  =   cv::BORDER_CONSTANT
                                                                        //  экстраполяция пикселей
                    ,   const cv::Scalar&               borderValue =   cv::Scalar()
                                                                        //  для константных рамок
                    );
//  возвращает матрицу 2×3
cv::Mat cv::getPerspectiveTransform  (   const cv::Point2f*  src             //  координаты *трех* вершин
                                ,   const cv::Point2f*  dst             //  координаты трех соответственных вершин
                                );
#endif
}
