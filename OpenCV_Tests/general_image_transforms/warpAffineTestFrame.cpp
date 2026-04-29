//  warpAffineTestFrame.cpp
/*
 ***************************************************************
 * Name:      warpAffineTestFrame.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-29
 * Copyright: Sergi E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 *************************************************************
*/
#include "warpAffineTestFrame.h"
/*
    Класс тестирования преобрзования `warpAffineTestFrame`
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
warpAffineTestFrame::warpAffineTestFrame  ( wxWindow  *   parent, const wxString& title)
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
                                ,   wxT("`warpAffine`\nGeneral \nAffine Transform")
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


    //  5.  Угол поворота alpha (вокруг оси Z)
    m_alpha_slider_ctrl      =   new     wxSlider   (   this
                                                    ,   wxID_ANY
                                                    ,   1              //  Начальное значение
                                                    ,   -45              //  Минимальное значение
                                                    ,   45              //  Максимальное значение
                                                    ,   wxPoint(10, 10)
                                                    ,   wxSize(100, -1)         //  горизонтальный слайдер
                                                    ,   wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS
                                                    );

    //  6.  Угол поворота beta (вокруг оси X)
    m_beta_slider_ctrl       =   new     wxSlider   (   this
                                                    ,   wxID_ANY
                                                    ,   1              //  Начальное значение
                                                    ,   -45              //  Минимальное значение
                                                    ,   45              //  Максимальное значение
                                                    ,   wxPoint(10, 10)
                                                    ,   wxSize(100, -1)         //  горизонтальный слайдер
                                                    ,   wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS
                                                    );

    //  7.  Угол поворота gamma (вокруг оси Y)
    m_gamma_slider_ctrl       =   new     wxSlider  (   this
                                                    ,   wxID_ANY
                                                    ,   1              //  Начальное значение
                                                    ,   -45              //  Минимальное значение
                                                    ,   45              //  Максимальное значение
                                                    ,   wxPoint(10, 10)
                                                    ,   wxSize(100, -1)         //  горизонтальный слайдер
                                                    ,   wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS
                                                    );


    //  8.  Метод интерполяции
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

    //  9.  Тип  экстраполяции границы (borderType)
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
    "Превью", "Оригинальное изображение", "Изображение после афинных преобразований");
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
    //  2.
    wxBoxSizer*         zoomTransfCol             = new wxBoxSizer(wxVERTICAL);
    zoomTransfCol->Add(new wxStaticText(this, -1, "Масштаб изменения"), 0, wxALL, 5);
    zoomTransfCol->Add(m_zoom_transf_slider_ctrl, 0, wxEXPAND | wxALL, 5);

    //  3.  Новое значение для ширины изображеия
    wxBoxSizer*         newWidth_Col       = new wxBoxSizer(wxVERTICAL);
    newWidth_Col->Add(new wxStaticText(this, -1, "Новая ширина"), 0, wxALL, 5);
    newWidth_Col->Add(m_spin_newWidth_ctrl, 0, wxALL, 5);

    //  4.  Новое значение для высоты изображеия
    wxBoxSizer*         newHeight_Col       = new wxBoxSizer(wxVERTICAL);
    newHeight_Col->Add(new wxStaticText(this, -1, "Новая высота"), 0, wxALL, 5);
    newHeight_Col->Add(m_spin_newHeight_ctrl, 0, wxALL, 5);

    //  5.  Угол поворота alpha (вокруг оси Z)
    wxBoxSizer*         alpha_Col       = new wxBoxSizer(wxVERTICAL);
    alpha_Col->Add(new wxStaticText(this, -1, "alpha"), 0, wxALL, 5);
    alpha_Col->Add(m_alpha_slider_ctrl, 0, wxALL, 5);

    //  6.  Угол поворота beta (вокруг оси X)
    wxBoxSizer*         beta_Col       = new wxBoxSizer(wxVERTICAL);
    beta_Col->Add(new wxStaticText(this, -1, "beta"), 0, wxALL, 5);
    beta_Col->Add(m_beta_slider_ctrl, 0, wxALL, 5);

    //  7.  Угол поворота gamma (вокруг оси Y)
    wxBoxSizer*         gamma_Col       = new wxBoxSizer(wxVERTICAL);
    gamma_Col->Add(new wxStaticText(this, -1, "gamma"), 0, wxALL, 5);
    gamma_Col->Add(m_gamma_slider_ctrl, 0, wxALL, 5);

    //  8.  Выпадающий список для выбора метода интерполяции
    wxBoxSizer*         interpolationCol       = new wxBoxSizer(wxVERTICAL);
    interpolationCol->Add(new wxStaticText(this, -1, "Метод интерполяции"), 0, wxALL, 5);
    interpolationCol->Add(m_interpolation_chois_ctrl, 0, wxALL, 5);

    //  8.  Выпадающий список для выбора типа границ
    wxBoxSizer*         borderTypeCol       = new wxBoxSizer(wxVERTICAL);
    borderTypeCol->Add(new wxStaticText(this, -1, "Тип границ"), 0, wxALL, 5);
    borderTypeCol->Add(m_borderType_chois_ctrl, 0, wxALL, 5);

    //  -------------------------------------------------------------------------------------------------
    //  Объединяем элементы управления в один горизонтальный сайзер
    //  1.
    controlPanelHSizer->Add(zoomOrigCol, 0, wxEXPAND | wxALL, 5);
    //  2.
    controlPanelHSizer->Add(zoomTransfCol, 0, wxEXPAND | wxALL, 5);
    //  3.
    controlPanelHSizer->Add(newWidth_Col, 0, wxEXPAND | wxALL, 5);
    //  4.
    controlPanelHSizer->Add(newHeight_Col, 0, wxEXPAND | wxALL, 5);
    //  5.
    controlPanelHSizer->Add(alpha_Col, 0, wxEXPAND | wxALL, 5);
    //  6.
    controlPanelHSizer->Add(beta_Col, 0, wxEXPAND | wxALL, 5);
    //  6.
    controlPanelHSizer->Add(gamma_Col, 0, wxEXPAND | wxALL, 5);
    //  8.
    controlPanelHSizer->Add(interpolationCol, 0, wxEXPAND | wxALL, 5);
    //  9.
    controlPanelHSizer->Add(borderTypeCol, 0, wxEXPAND | wxALL, 5);

    //  II.4. Объединяем сайзеры отображения изображений и панели управления отображением
    //  И наконец добавляем горизонтальные сайзеры в главный вертикальный сайзер
    wxBoxSizer *            mainSizer    =   new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_informPanel, 0, wxEXPAND | wxALL, 5);       // Панель только столько, сколько ей нужно
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
    m_zoom_orig_slider_ctrl->Bind(wxEVT_SLIDER, &warpAffineTestFrame::On_zoomOrigSliderChanged, this);
    //  2.
    m_zoom_transf_slider_ctrl->Bind(wxEVT_SLIDER, &warpAffineTestFrame::On_zoomTransfSliderChanged, this);
    //  3.
    m_spin_newWidth_ctrl->Bind(wxEVT_SPINCTRL, &warpAffineTestFrame::On_newWidthChanged, this);
    //  4.
    m_spin_newHeight_ctrl->Bind(wxEVT_SPINCTRL, &warpAffineTestFrame::On_newHeightChanged, this);
    //  5.
    m_alpha_slider_ctrl->Bind(wxEVT_SLIDER, &warpAffineTestFrame::On_alphaChanged, this);
    //  6.
    m_beta_slider_ctrl->Bind(wxEVT_SLIDER, &warpAffineTestFrame::On_betaChanged, this);
    //  7.
    m_gamma_slider_ctrl->Bind(wxEVT_SLIDER, &warpAffineTestFrame::On_gammaChanged, this);
    //  8.
    m_interpolation_chois_ctrl->Bind(wxEVT_CHOICE, &warpAffineTestFrame::On_interpolationChanged, this);
    //  9.
    m_borderType_chois_ctrl->Bind(wxEVT_CHOICE, &warpAffineTestFrame::On_borderTypeChanged, this);

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

void warpAffineTestFrame::OnResize(wxSizeEvent& event)
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
void warpAffineTestFrame::On_zoomOrigSliderChanged(wxCommandEvent& event) {
    // 1. Извлекаем масшаб
    m_zoom_orig_slider_ctrl->m_zoom = event.GetInt();
    // 2. Запускаем обработку OpenCV
    UpdateAllViews();

    Layout();
}
//  2.
void warpAffineTestFrame::On_zoomTransfSliderChanged(wxCommandEvent& event) {
    // 1. Извлекаем масшаб
    m_zoom_transf_slider_ctrl->m_zoom = event.GetInt();
    // 2. Запускаем обработку OpenCV
    UpdateAllViews();

    Layout();
}

//  3.
void warpAffineTestFrame::On_newWidthChanged(wxSpinEvent& event) {
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
void warpAffineTestFrame::On_newHeightChanged(wxSpinEvent& event) {
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
void warpAffineTestFrame::On_alphaChanged(wxCommandEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"On_alphaChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем коэффициет масштабирования по высоте изображения
    m_alpha  =   event.GetInt();
    #if 0
    std::wstring msg = L"\nУгол поворота alpha: " + std::to_wstring(m_alpha);
    m_debugInfo->AppendText(msg);
    #endif
    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  6.
void warpAffineTestFrame::On_betaChanged(wxCommandEvent& event) {
     #if 0
    MessageBoxW(NULL, L"Работает", L"On_betaChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем коэффициет масштабирования по высоте изображения
    m_beta  =   event.GetInt();

    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  7.
void warpAffineTestFrame::On_gammaChanged(wxCommandEvent& event) {
     #if 0
    MessageBoxW(NULL, L"Работает", L"On_gammaChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем коэффициет масштабирования по высоте изображения
    m_gamma  =   event.GetInt();

    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}

//  8.
void warpAffineTestFrame::On_interpolationChanged(wxCommandEvent& event) {
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
//  9.
void warpAffineTestFrame::On_borderTypeChanged(wxCommandEvent& event) {
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
void warpAffineTestFrame::UpdateAllViews() {
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
void warpAffineTestFrame::ApplyTransformation() {
    #if 0
    MessageBoxW(NULL, L"Работает", L"ApplyTransformation()", MB_OK | MB_ICONINFORMATION);
    #endif
    // Вся "кухня" OpenCV здесь

    cv::warpAffine  (   m_cv_original_img
                    ,   m_cv_transformed_img
                    ,   get_M(m_alpha, m_beta, m_gamma)
                    ,   cv::Size(m_newWidth, m_newHeight)
                    ,   m_interpolation
                    ,   m_borderType
                    );
}

cv::Mat      warpAffineTestFrame::get_M(int angleXY, int angleYZ, int angleZX){

    src =   m_cv_original_img.clone();
    //	Будем создавать матрицу афинного преобразования
	//	Точки исходного изображения
	cv::Point2f 	srcTri[] = {
		cv::Point2f(0.f, 0.f),                  						// 	левый верхний угол `src`
		cv::Point2f((float)src.cols - 1, 0.f),        				// 	правый верхний угол `src`
		cv::Point2f(0.f, (float)src.rows - 1.f)         				// 	левый нижний угол `src`
	};
    // 	1. Углы (из слайдеров)
	double 			alpha 	= angleXY * CV_PI / 180.0; 		// 	Наклон по X
	double 			beta  	= angleYZ * CV_PI / 180.0; 		// 	Наклон по Y
	double 			gamma 	= angleZX * CV_PI / 180.0; 		// 	Вращение в плоскости

	// 	2. Рассчитываем смещения для имитации наклона (Shear)
	float 			shearX 	= tan(alpha) * src.rows / 2.0f;
	float 			shearY 	= tan(beta)  * src.cols / 2.0f;

	// 	3. Целевые точки 	(Параллелограмм со скосом относительно центра прямоугольника изображения)
	cv::Point2f 		center	(	src.cols / 2.0f
                                , 	src.rows / 2.0f
                                );

	cv::Point2f 		dstTri[3];		//	Координаты верхних и левого нижнего углов параллелограмма

	// 	ЛВ 	(Левый Верхний угол)
	dstTri[0] 	= 	cv::Point2f	(	-src.cols / 2.0f + shearX
								, 	-src.rows / 2.0f + shearY
								);
	// 	ПВ 	(Правый Верхний угол)
	dstTri[1] 	= 	cv::Point2f	( 	 src.cols / 2.0f + shearX
								,  	-src.rows / 2.0f - shearY
								);
	// 	ЛН 	(Левый Нижний угол)
	dstTri[2] 	= 	cv::Point2f	(  	-src.cols / 2.0f - shearX
								,  	 src.rows / 2.0f + shearY
								);

	// 	4. 	Вращаем эти точки вокруг оси OZ (на угол gamma)
	float 	sin_gamma = sin(gamma);
	float 	cos_gamma = cos(gamma);
	//	Для всех углов параллелограмма пересчитаем координаты
	for(int i = 0; i < 3; ++i) {
		float x = dstTri[i].x;
		float y = dstTri[i].y;
		dstTri[i].x = x * cos_gamma - y * sin_gamma + center.x;
		dstTri[i].y = x * sin_gamma + y * cos_gamma + center.y;
	}

	warp_mat = cv::getAffineTransform(srcTri, dstTri);
    return          warp_mat.clone();
}

std::string warpAffineTestFrame::getSignatureText(){
    std::string signature_text =

    "void cv::warpAffine("
    "\n,    cv::InputArray  src"
    "\n,    cv::OutputArray dst"
    "\n,    cv::InputArray  cv::getAffineTransform( const cv::Point2f*"
    "\n,                                          , const cv::Point2f*)"
    "\n,    cv::Size dsize"
    "\n,    int flags = cv::INTER_LINEAR"
    "\n,    int borderMode = cv::BORDER_CONSTANT"
    "\n,    const cv::Scalar& borderValue = cv::Scalar()"
    "\n);"
    "\n"
    "\ncv::getAffineTransform("
    "\n,     const cv::Point2f*"
    "\n,     const cv::Point2f*"
    "\n);"
    ;

    return signature_text;
#if 0
void cv::warpAffine (   cv::InputArray                  src             //  входное изображение
                    ,   cv::OutputArray                 dst             //  выходное изображение
                    ,   cv::InputArray                  cv::getAffineTransform(const cv::Point2f*, const cv::Point2f*)
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
cv::Mat cv::getAffineTransform  (   const cv::Point2f*  src             //  координаты *трех* вершин
                                ,   const cv::Point2f*  dst             //  координаты трех соответственных вершин
                                );
#endif
}
