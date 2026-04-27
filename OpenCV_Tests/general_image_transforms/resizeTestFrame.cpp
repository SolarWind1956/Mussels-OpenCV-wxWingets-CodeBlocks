//  resizeTestFrame.cpp
/*
 ***************************************************************
 * Name:      resizeTestFrame.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-23
 * Copyright: Sergi E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 *************************************************************
*/
#include "resizeTestFrame.h"
/*
    Класс тестирования преобрзования `resizeTestFrame`
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
resizeTestFrame::resizeTestFrame  ( wxWindow  *   parent)
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

    m_transforming_namePanel      =   new wxPanel(m_informPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_transforming_namePanel->SetBackgroundColour(*wxCYAN);             // Чтобы визуально отделить
    m_transform_name_text       =
            new wxStaticText    (   m_transforming_namePanel
                                ,   -1
                                ,   wxT("`resize`\nGeneral Transform")
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
    m_scrolled_wind_resized         =   new wxScrolledWindow(this);

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
    "Превью", "Оригинальное изображение", "Изображение после изменения размеров");
    titelSizerHor->Add(new wxStaticText(this, -1, titelStr), 0, wxALL, 5);

    wxBoxSizer *            imagesHSizer       =   new wxBoxSizer(wxHORIZONTAL);
    //  Добавляем в главный сайзер превью-панель
    imagesHSizer->Add(m_previewPanel, 0, wxEXPAND | wxALL, 5);                  // Фикс
    imagesHSizer->Add(m_scrolled_wind_original,     1, wxEXPAND | wxALL, 5);    // Растягивается
    imagesHSizer->Add(m_scrolled_wind_resized,     1, wxEXPAND | wxALL, 5);    // Растягивается

    //  ------------------------------------------------------------------------------------------------------
    //  Подготовка панели управления отображением изображений
    //  1.
    m_zoom_orig_slider_ctrl      =   new ZoomSliderCtrl(this);
    m_zoom_resz_slider_ctrl      =   new ZoomSliderCtrl(this, 10);

    //  2.  Новое значение для ширины изображеия
    m_spin_newWidth_ctrl    =   new wxSpinCtrl          (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   0           // Минимум:
                                                        ,   1920        // Максимум:
                                                        ,   0           // Начальное значение
                                                        );
    //  3.  Новое значение для высоты изображеия
    m_spin_newHeight_ctrl    =   new wxSpinCtrl         (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   0           // Минимум:
                                                        ,   1080        // Максимум:
                                                        ,   0           // Начальное значение
                                                        );

    //  4.  Коэффициент масштабирования по ширине изображения (по оси X)
    m_spin_X_scaling_ctrl    =   new wxSpinCtrlDouble   (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   0.01    // Минимум:
                                                        ,   3.0     // Максимум:
                                                        ,   1.0     // Начальное значение
                                                        ,   0.1     // Шаг: золотая середина для визуального теста
                                                        );
    //  Дополнительно количество знаков после запятой
    m_spin_X_scaling_ctrl->SetDigits(1);

    //  5.  Коэффициент масштабирования по высоте изображения (по оси Y)
    m_spin_Y_scaling_ctrl    =   new wxSpinCtrlDouble   (   this
                                                        ,   wxID_ANY
                                                        ,   wxEmptyString
                                                        ,   wxDefaultPosition
                                                        ,   wxDefaultSize
                                                        ,   wxSP_ARROW_KEYS
                                                        ,   0.01    // Минимум:
                                                        ,   3.0     // Максимум:
                                                        ,   1.0     // Начальное значение
                                                        ,   0.1     // Шаг: золотая середина для визуального теста
                                                        );
    //  Дополнительно количество знаков после запятой
    m_spin_Y_scaling_ctrl->SetDigits(1);


    //  6.  Метод интерполяции
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


    //  ----------------------------------------------------------------------------------------------------
    //  II.3. Сайзеры для панели настроек и управления
    wxBoxSizer*         controlPanelHSizer   = new wxBoxSizer(wxHORIZONTAL);
    controlPanelHSizer->SetMinSize(wxSize(-1, 150)); // -1 значит "любая ширина", 150 - фиксированная высота

    // 1. СлайдерЫ масштаба в отдельных столбиках
    wxBoxSizer*         zoomOrigCol             = new wxBoxSizer(wxVERTICAL);
    zoomOrigCol->Add(new wxStaticText(this, -1, "Масштаб оригинала"), 0, wxALL, 5);
    zoomOrigCol->Add(m_zoom_orig_slider_ctrl, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer*         zoomReszCol             = new wxBoxSizer(wxVERTICAL);
    zoomReszCol->Add(new wxStaticText(this, -1, "Масштаб изменения"), 0, wxALL, 5);
    zoomReszCol->Add(m_zoom_resz_slider_ctrl, 0, wxEXPAND | wxALL, 5);

    //  2.  Новое значение для ширины изображеия
    wxBoxSizer*         newWidth_Col       = new wxBoxSizer(wxVERTICAL);
    newWidth_Col->Add(new wxStaticText(this, -1, "Новая ширина"), 0, wxALL, 5);
    newWidth_Col->Add(m_spin_newWidth_ctrl, 0, wxALL, 5);

    //  3.  Новое значение для высоты изображеия
    wxBoxSizer*         newHeight_Col       = new wxBoxSizer(wxVERTICAL);
    newHeight_Col->Add(new wxStaticText(this, -1, "Новая высота"), 0, wxALL, 5);
    newHeight_Col->Add(m_spin_newHeight_ctrl, 0, wxALL, 5);

    //  4.  Коэффициент масштабирования по ширине изображения (по оси X)
    wxBoxSizer*         X_scaling_Col       = new wxBoxSizer(wxVERTICAL);
    X_scaling_Col->Add(new wxStaticText(this, -1, "Масштаб по X"), 0, wxALL, 5);
    X_scaling_Col->Add(m_spin_X_scaling_ctrl, 0, wxALL, 5);

    //  5.  Коэффициент масштабирования по ширине изображения (по оси Y)
    wxBoxSizer*         Y_scaling_Col       = new wxBoxSizer(wxVERTICAL);
    Y_scaling_Col->Add(new wxStaticText(this, -1, "Масштаб по Y"), 0, wxALL, 5);
    Y_scaling_Col->Add(m_spin_Y_scaling_ctrl, 0, wxALL, 5);

    //  6.  Выпадающий список для выбора метода интерполяции
    wxBoxSizer*         interpolationCol       = new wxBoxSizer(wxVERTICAL);
    interpolationCol->Add(new wxStaticText(this, -1, "Метод интерполяции"), 0, wxALL, 5);
    interpolationCol->Add(m_interpolation_chois_ctrl, 0, wxALL, 5);

    //  -------------------------------------------------------------------------------------------------
    //  Объединяем элементы управления в один горизонтальный сайзер
    //  1.
    controlPanelHSizer->Add(zoomOrigCol, 0, wxEXPAND | wxALL, 5);
    controlPanelHSizer->Add(zoomReszCol, 0, wxEXPAND | wxALL, 5);
    //  2.
    controlPanelHSizer->Add(newWidth_Col, 0, wxEXPAND | wxALL, 5);
    //  3.
    controlPanelHSizer->Add(newHeight_Col, 0, wxEXPAND | wxALL, 5);
    //  4.
    controlPanelHSizer->Add(X_scaling_Col, 0, wxEXPAND | wxALL, 5);
    //  5.
    controlPanelHSizer->Add(Y_scaling_Col, 0, wxEXPAND | wxALL, 5);
    //  6.
    controlPanelHSizer->Add(interpolationCol, 0, wxEXPAND | wxALL, 5);

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
    m_zoom_orig_slider_ctrl->Bind(wxEVT_SLIDER, &resizeTestFrame::OnZoomOrigSliderChanged, this);
    m_zoom_resz_slider_ctrl->Bind(wxEVT_SLIDER, &resizeTestFrame::OnZoomReszSliderChanged, this);
    //  2.
    m_spin_newWidth_ctrl->Bind(wxEVT_SPINCTRL, &resizeTestFrame::OnNewWidthChanged, this);
    //  3.
    m_spin_newHeight_ctrl->Bind(wxEVT_SPINCTRL, &resizeTestFrame::OnNewHeightChanged, this);
    //  4.
    m_spin_X_scaling_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &resizeTestFrame::OnX_scalingChanged, this);
    //  5.
    m_spin_Y_scaling_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &resizeTestFrame::OnY_scalingChanged, this);
    //  6.
    m_interpolation_chois_ctrl->Bind(wxEVT_CHOICE, &resizeTestFrame::OnInterpolationChanged, this);

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

void resizeTestFrame::OnResize(wxSizeEvent& event)
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
void resizeTestFrame::OnZoomOrigSliderChanged(wxCommandEvent& event) {
    // 1. Извлекаем масшаб
    m_zoom_orig_slider_ctrl->m_zoom = event.GetInt();
    // 2. Запускаем обработку OpenCV
    UpdateAllViews();

    Layout();
}
void resizeTestFrame::OnZoomReszSliderChanged(wxCommandEvent& event) {
    // 1. Извлекаем масшаб
    m_zoom_resz_slider_ctrl->m_zoom = event.GetInt();
    // 2. Запускаем обработку OpenCV
    UpdateAllViews();

    Layout();
}

//  2.
void resizeTestFrame::OnNewWidthChanged(wxSpinEvent& event) {
    #if 0
    MessageBoxW(NULL, L"Работает", L"OnNewWidthChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1. Извлекаем новую ширину
    m_newWidth = event.GetInt();

    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  3.
void resizeTestFrame::OnNewHeightChanged(wxSpinEvent& event) {
     #if 0
    MessageBoxW(NULL, L"Работает", L"OnNewHeightChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1. Извлекаем новую высоту
    m_newHeight = event.GetInt();

    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  4.
void resizeTestFrame::OnX_scalingChanged(wxSpinDoubleEvent& event) {
     #if 0
    MessageBoxW(NULL, L"Работает", L"OnX_scalingChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем коэффициет масштабирования по ширине изображения
    m_X_scaling  =   event.GetValue();

    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}
//  5.
void resizeTestFrame::OnY_scalingChanged(wxSpinDoubleEvent& event) {
     #if 0
    MessageBoxW(NULL, L"Работает", L"OnY_scalingChanged()", MB_OK | MB_ICONINFORMATION);
    #endif
    // 1.   Извлекаем коэффициет масштабирования по высоте изображения
    m_Y_scaling  =   event.GetValue();

    // 2. Запускаем обработку OpenCV
    ApplyTransformation();

    // 3. Обновляем картинки
    UpdateAllViews();

    Layout();
}

//  6.
void resizeTestFrame::OnInterpolationChanged(wxCommandEvent& event) {
    #if 0
    std::wstring msg = L"Значение интерполяции: " + std::to_wstring(m_interpolation);

    //  Выводим в MessageBox. .c_str() преобразует wstring в нужный формат const wchar_t*
    MessageBoxW(NULL, msg.c_str(), L"Результат", MB_OK | MB_ICONINFORMATION);
    #endif

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
// Универсальная функция, которая знает про оба окна
void resizeTestFrame::UpdateAllViews() {
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
                    ,   m_cv_resized_img
                    ,   m_resizedBitmap
                    ,   m_staticResizedBitmap
                    ,   m_scrolled_wind_resized
                    ,   m_zoom_resz_slider_ctrl->m_zoom
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
void resizeTestFrame::ApplyTransformation() {
    #if 0
    MessageBoxW(NULL, L"Работает", L"ApplyTransformation()", MB_OK | MB_ICONINFORMATION);
    #endif
    // Вся "кухня" OpenCV здесь

    cv::resize  (   m_cv_original_img
                ,   m_cv_resized_img
                ,   cv::Size(m_newWidth, m_newHeight)
                ,   m_X_scaling
                ,   m_Y_scaling
                ,   m_interpolation
                );
}

std::string resizeTestFrame::getSignatureText(){
    std::string signature_text =

    "void cv::resize("
    "\n,    cv::InputArray  src"
    "\n,    cv::OutputArray dst"
    "\n,    cv::Size dsize"
    "\n,    double fx = 0"
    "\n,    double fy = 0"
    "\n,    int interpolation = CV::INTER_LINEAR"
    "\n);"
    ;

    return signature_text;
#if 0
    void cv::resize(
        cv::InputArray  src,                                // входное изображение
        cv::OutputArray dst,                                // выходное изображение
        cv::Size        dsize,                              // новый размер
        double          fx              = 0,                // коэффициент масштабирования по x
        double          fy              = 0,                // коэффициент масштабирования по y
        int             interpolation   = CV::INTER_LINEAR  // метод интерполяции
    );
#endif

}
