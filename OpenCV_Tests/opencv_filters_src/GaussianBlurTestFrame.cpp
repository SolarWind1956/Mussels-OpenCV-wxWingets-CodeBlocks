//  GaussianBlurTestFrame.cpp
/*
 ***************************************************************
 * Name:      GaussianBlurTestFrame.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-23
 * Copyright: Sergi E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 *************************************************************
*/
#include "GaussianBlurTestFrame.h"
/*
    Класс тестирования преобрзования `GaussianBlurTestFrame`
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
GaussianBlurTestFrame::GaussianBlurTestFrame  ( wxWindow  *   parent, const wxString& title)
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
            new wxStaticText(m_filtering_namePanel, -1, wxT("GaussianBlur filteration"), wxPoint(0, 0));
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

    //  -------------------------------------------------------------------------------------------------
    m_previewPanel                =   new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1));
    m_previewPanel->SetMinSize(wxSize(200, -1));
    m_previewPanel->SetBackgroundColour(*wxLIGHT_GREY); // Чтобы визуально отделить
    m_staticPreviewBitmap         =   new wxStaticBitmap(m_previewPanel, wxID_ANY, wxNullBitmap);

    //  -------------------------------------------------------------------------------------------------
    m_scrolled_wind_original      =   new wxScrolledWindow(this);
    m_scrolled_wind_filtered      =   new wxScrolledWindow(this);

    //  II.    --------------------------------------------------------------------------------------------
    //  Сайзеры
    //  II.1. Сайзеры для отображения различной вспомогательной инофрмации о тестировании
    //  Добавляем в сайзер: (объект, пропорция, флаги, отступ)
    wxBoxSizer *            informHSizer       =   new wxBoxSizer(wxHORIZONTAL);
    informHSizer->SetMinSize(wxSize(-1, -1)); // -1 значит "любая ширина", -1 значит "любая высота"
    informHSizer->Add(m_filtering_namePanel, 1, wxCENTER | wxALL, 5);
    informHSizer->Add(m_signaturePanel, 1, wxCENTER | wxALL, 5);
    m_informPanel->SetSizer(informHSizer);

    //
    //  II.2. Сайзеры для отображения различных вариантов исследуемого изображения
    //  Используем сайзер, чтобы центрировать превью внутри панели
    wxBoxSizer *            previewSizer    =   new wxBoxSizer(wxVERTICAL);
    previewSizer->Add(m_staticPreviewBitmap, 1, wxCENTER | wxALL, 5);
    m_previewPanel->SetSizer(previewSizer);

    wxBoxSizer *            imagesHSizer       =   new wxBoxSizer(wxHORIZONTAL);
    //  Добавляем в главный сайзер превью-панель
    imagesHSizer->Add(m_previewPanel, 0, wxEXPAND | wxALL, 5);               // Фикс
    imagesHSizer->Add(m_scrolled_wind_original, 1, wxEXPAND | wxALL, 5);       // Растягивается
    imagesHSizer->Add(m_scrolled_wind_filtered, 1, wxEXPAND | wxALL, 5);     // Растягивается

    //  ------------------------------------------------------------------------------------------------------
    //  Подготовка панели управления отображением изображений
    m_zoom_slider_ctrl      =   new ZoomSliderCtrl(this);

    m_kernel_width_ctrl     =   new KernelMatrixCtrl(this, wxID_ANY, wxPoint(0, 0), wxSize(100, 100));
    m_kernel_width_ctrl->SetMinSize(wxSize(100, 100));           // Фиксируем минимальное присутствие

    //  Сигмы нормального распределения (полуширину на уровне полувысоты) в направлении оси X и оси Y

    m_spin_sigmaX_ctrl      =   new wxSpinCtrlDouble    (   this
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
    m_spin_sigmaX_ctrl->SetDigits(2);

    m_spin_sigmaY_ctrl      =   new wxSpinCtrlDouble    (   this
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
    m_spin_sigmaY_ctrl->SetDigits(2);

    //  Тип границы (borderType)
    wxString    TypesStrings[] =    {   wxT("cv::BORDER_CONSTANT")  //  заполнение цветом (по умолчанию)
                                    ,   wxT("cv::BORDER_REPLICATE") //  повтор последнего пикселя (а-а-а-а)
                                    ,   wxT("cv::BORDER_REFLECT")   //  зеркало (абв|вба)
                                    //,   wxT("cv::BORDER_WRAP")      //  обертка (как в старых играх:
                                                                    //  вышел слева — зашел справа)
                                    };

    m_border_chois_ctrl     =   new wxChoice     (   this
                                                 ,   wxID_ANY
                                                 ,   wxDefaultPosition
                                                 ,   wxSize(180, -1)
                                                 ,   WXSIZEOF(TypesStrings)
                                                 ,   TypesStrings
                                                 ,   0
                                                 );
    m_border_chois_ctrl->SetStringSelection(wxT("cv::BORDER_CONSTANT"));

    //  II.3. Сайзеры для панели настроек и управления
    wxBoxSizer*         controlPanelHSizer   = new wxBoxSizer(wxHORIZONTAL);
    controlPanelHSizer->SetMinSize(wxSize(-1, 150)); // -1 значит "любая ширина", 150 - фиксированная высота

    // Слайдер масштаба в отдельном столбике
    wxBoxSizer*         zoomCol             = new wxBoxSizer(wxVERTICAL);
    zoomCol->Add(new wxStaticText(this, -1, "Масштаб"), 0, wxALL, 5);
    zoomCol->Add(m_zoom_slider_ctrl, 0, wxEXPAND | wxALL, 5);

    // Матрица для ширины ядра
    wxBoxSizer*         kernelHor           = new wxBoxSizer(wxHORIZONTAL);
    kernelHor->Add(new wxStaticText(this, -1, " Ширина\n вадратного\n ядра"), 0, wxALL, 5);
    kernelHor->Add(m_kernel_width_ctrl, 0, wxALL, 5);

    // Спины для выбора сигм
    wxBoxSizer*         sigmaX_Col       = new wxBoxSizer(wxVERTICAL);
    sigmaX_Col->Add(new wxStaticText(this, -1, "sigmaX"), 0, wxALL, 5);
    sigmaX_Col->Add(m_spin_sigmaX_ctrl, 0, wxALL, 5);

    wxBoxSizer*         sigmaY_Col       = new wxBoxSizer(wxVERTICAL);
    sigmaY_Col->Add(new wxStaticText(this, -1, "sigmaY"), 0, wxALL, 5);
    sigmaY_Col->Add(m_spin_sigmaY_ctrl, 0, wxALL, 5);

    // Выпадающий список для выбора типа границ
    wxBoxSizer*         typeBodersCol       = new wxBoxSizer(wxVERTICAL);
    typeBodersCol->Add(new wxStaticText(this, -1, "Типы границ"), 0, wxALL, 5);
    typeBodersCol->Add(m_border_chois_ctrl, 0, wxALL, 5);

    //  Объединяем элементы управления в один горизонтальный сайзер
    controlPanelHSizer->Add(zoomCol, 0, wxEXPAND | wxALL, 10);
    controlPanelHSizer->Add(kernelHor, 0, wxEXPAND | wxALL, 10);
    controlPanelHSizer->Add(sigmaX_Col, 0, wxEXPAND | wxALL, 10);
    controlPanelHSizer->Add(sigmaY_Col, 0, wxEXPAND | wxALL, 10);
    controlPanelHSizer->Add(typeBodersCol, 0, wxEXPAND | wxALL, 10);

    //  II.4. Объединяем сайзеры отображения изображений и панели управления отображением
    //  И наконец добавляем горизонтальные сайзеры в главный вертикальный сайзер
    wxBoxSizer *            mainSizer    =   new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_informPanel, 0, wxEXPAND | wxALL, 5);       // Панель только столько, сколько ей нужно
    mainSizer->Add(imagesHSizer, 1, wxEXPAND | wxALL, 5);       // Картинки забирают всё свободное место
    mainSizer->Add(controlPanelHSizer, 0, wxEXPAND | wxALL, 5); // Панель только столько, сколько ей нужно

    this->SetSizer(mainSizer);

    //  III.     ------------------------------------------------------------------------------------------------
    //  Привязка обработчиков событий
    //  В конструкторе теперь только "регистрация"
    m_zoom_slider_ctrl->Bind(wxEVT_SLIDER, &GaussianBlurTestFrame::OnZoomSliderChanged, this);
    m_kernel_width_ctrl->Bind(EVT_KERNEL_CHANGED, &GaussianBlurTestFrame::OnKernelWidthChanged, this);
    m_spin_sigmaX_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &GaussianBlurTestFrame::OnSigmaXChanged, this);
    m_spin_sigmaY_ctrl->Bind(wxEVT_SPINCTRLDOUBLE, &GaussianBlurTestFrame::OnSigmaYChanged, this);
    m_border_chois_ctrl->Bind(wxEVT_CHOICE, &GaussianBlurTestFrame::OnBorderExtrapolChanged, this);

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

void GaussianBlurTestFrame::OnResize(wxSizeEvent& event)
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

void GaussianBlurTestFrame::OnZoomSliderChanged(wxCommandEvent& event) {
    m_zoom_slider_ctrl->m_zoom = event.GetInt();
    UpdateAllViews();
}

void GaussianBlurTestFrame::OnKernelWidthChanged(wxCommandEvent& event) {
    // 1. Извлекаем размер ядра, который мы положили в SetInt() в контроле
    int newSize = event.GetInt();

    // 2. Обновляем переменную класса
    m_kernel_width = newSize;

    // 3. Запускаем обработку OpenCV
    ApplyFiltering();

    // 4. Обновляем картинки
    UpdateAllViews();

    // 5. Если матрица поменяла размер (например, стала 7x7 вместо 3x3),
    // заставляем сайзеры пересчитать всё окно
    Layout();
}

void GaussianBlurTestFrame::OnSigmaXChanged(wxSpinDoubleEvent& event) {
    // 1. Извлекаем sigmaX
    m_sigmaX = event.GetValue();

    // 3. Запускаем обработку OpenCV
    ApplyFiltering();

    // 4. Обновляем картинки
    UpdateAllViews();

    // 5. Если матрица поменяла размер (например, стала 7x7 вместо 3x3),
    // заставляем сайзеры пересчитать всё окно
    Layout();
}

void GaussianBlurTestFrame::OnSigmaYChanged(wxSpinDoubleEvent& event) {
    // 1. Извлекаем sigmaY
    m_sigmaY = event.GetValue();

    // 3. Запускаем обработку OpenCV
    ApplyFiltering();

    // 4. Обновляем картинки
    UpdateAllViews();

    // 5. Если матрица поменяла размер (например, стала 7x7 вместо 3x3),
    // заставляем сайзеры пересчитать всё окно
    Layout();
}
void GaussianBlurTestFrame::OnBorderExtrapolChanged(wxCommandEvent& event) {
    // 1. Обновляем переменную типа границы
    m_border_extrapolation  =   event.GetInt();
    #if 0
    std::wstring msg = L"Значение: " + std::to_wstring(m_border_extrapolation);

    //  Выводим в MessageBox. .c_str() преобразует wstring в нужный формат const wchar_t*
    MessageBoxW(NULL, msg.c_str(), L"Результат", MB_OK | MB_ICONINFORMATION);
    #endif
    // 2. Запускаем обработку OpenCV
    ApplyFiltering();

    // 3. Обновляем картинки
    UpdateAllViews();

    // 4. Если матрица поменяла размер (например, стала 7x7 вместо 3x3),
    // заставляем сайзеры пересчитать всё окно
    Layout();
}

// Универсальная функция, которая знает про оба окна
void GaussianBlurTestFrame::UpdateAllViews() {
    //  Визуализация оригинального и отфильтрованного изображения (выводим оба состояния)

    //  Преобразования для вывода оригнинального изображения OpenCV в формате библиотеки wxVidgets
    UpdateDisplay   (   m_wx_img
                    ,   m_cv_original_img
                    ,   m_originalBitmap
                    ,   m_staticOriginalBitmap
                    ,   m_scrolled_wind_original
                    ,   m_zoom_slider_ctrl->m_zoom
                    );

    //  Преобразования для вывода отфильтрованного изображения OpenCV в формате библиотеки wxVidgets
    UpdateDisplay   (   m_wx_img
                    ,   m_cv_filtered_img
                    ,   m_filteredBitmap
                    ,   m_staticFilteredBitmap
                    ,   m_scrolled_wind_filtered
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
void GaussianBlurTestFrame::ApplyFiltering() {
    // Вся "кухня" OpenCV здесь

    cv::GaussianBlur    (   m_cv_original_img                                   // входное изображение
                        ,   m_cv_filtered_img                                   // выходное изображение
                        ,   cv::Size(m_kernel_width, m_kernel_width)            // размер ядра
                        ,   m_sigmaX                                            // полуширина по оси X
                        ,   m_sigmaY = 0.0                                      // полуширина по оси Y
                        ,   m_border_extrapolation                              // экстраполяция границы
                        );
}

std::string GaussianBlurTestFrame::getSignatureText(){
    std::string signature_text =

    "void cv::GaussianBlur("
    "\n     cv::InputArray src"
    "\n,    cv::OutputArray dst"
    "\n,    cv::Size ksize"
    "\n,    double sigmaX"
    "\n,    double sigmaY = 0.0"
    "\n,    int borderType = cv::BORDER_CONSTANT"
    "\n );"

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



