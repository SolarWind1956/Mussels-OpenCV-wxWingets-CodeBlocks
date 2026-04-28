//  OpenCV_Tester.cpp
/*
 ***************************************************************
 * Name:      OpenCV_Tester.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-21
 * Copyright: Sergej E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 *************************************************************
*/
#include "OpenCV_Tester.h"
//	-----------------------------------------------------------------------------------------------------
/*
    Классы для тестирования библиотеки OpenCV
*/
//  -----------------------------------------------------------------------------------------------------

//	-----------------------------------------------------------------------------------------------------
//  Главный класс тестера
//	-----------------------------------------------------------------------------------------------------
OpenCV_Tester_Window::OpenCV_Tester_Window  (const wxString &   title)
                                                : wxFrame   (   nullptr
                                                            ,   wxID_ANY
                                                            ,   title
                                                            ,   wxDefaultPosition
                                                            ,   wxSize(1920, 840)
                                                            )
{
    main_menu_bar           =   new wxMenuBar;

    menu_Filters            =   new wxMenu;
    menu_Morphology         =   new wxMenu;
    menu_GeneralTransforms  =   new wxMenu;

    //  Добавляем пункты `выпадающих` меню в объект-cтроку главного `горизонтального` меню
    main_menu_bar->Append(menu_Filters, wxT("&Filters"));
    main_menu_bar->Append(menu_Morphology, wxT("&Morphology"));
    main_menu_bar->Append(menu_GeneralTransforms, wxT("&General Inage Transforms"));

    //  Этот метод принадлежит `виджету` `wxFrame` и устанавливает `строку горизнонтального меню`.
    SetMenuBar(main_menu_bar);

    //  ---------------------------------------------------------------------------------------------------
    //  Добавляем пункты в выпадающие меню
    //  ---------------------------------------------------------------------------------------------------
    //  Filters and Convolution   -   Ядра, фильтры и свертки
    menu_item_Blur  =   new wxMenuItem  (menu_Filters,   wxID_ANY,   wxT("Blur"));
    menu_Filters->Append(menu_item_Blur);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &OpenCV_Tester_Window::On_Blur, this, menu_item_Blur->GetId());

    menu_item_medianBlur  =   new wxMenuItem  (menu_Filters,   wxID_ANY,   wxT("medianBlur"));
    menu_Filters->Append(menu_item_medianBlur);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &OpenCV_Tester_Window::On_medianBlur, this, menu_item_medianBlur->GetId());

    menu_item_GaussianBlur  =   new wxMenuItem  (menu_Filters,   wxID_ANY,   wxT("GaussianBlur"));
    menu_Filters->Append(menu_item_GaussianBlur);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &OpenCV_Tester_Window::On_GaussianBlur, this, menu_item_GaussianBlur->GetId());

    menu_item_threshold  =   new wxMenuItem  (menu_Filters,   wxID_ANY,   wxT("threshold"));
    menu_Filters->Append(menu_item_threshold);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &OpenCV_Tester_Window::On_threshold, this, menu_item_threshold->GetId());

    //  ---------------------------------------------------------------------------------------------------

    menu_item_erode  =   new wxMenuItem  (menu_Morphology,   wxID_ANY,   wxT("erode"));
    menu_Morphology->Append(menu_item_erode);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &OpenCV_Tester_Window::On_erode, this, menu_item_erode->GetId());

    menu_item_dilate  =   new wxMenuItem  (menu_Morphology,   wxID_ANY,   wxT("dilate"));
    menu_Morphology->Append(menu_item_dilate);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &OpenCV_Tester_Window::On_dilate, this, menu_item_dilate->GetId());

    menu_item_erodeAfterThreshold  =   new wxMenuItem  (menu_Morphology,   wxID_ANY,   wxT("erodeAfterThreshold"));
    menu_Morphology->Append(menu_item_erodeAfterThreshold);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &OpenCV_Tester_Window::On_erodeAfterThreshold, this,
                                                                            menu_item_erodeAfterThreshold->GetId());

    menu_item_erodeAfterAdaptive  =   new wxMenuItem  (menu_Morphology,   wxID_ANY,   wxT("erodeAfterAdaptive"));
    menu_Morphology->Append(menu_item_erodeAfterAdaptive);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &OpenCV_Tester_Window::On_erodeAfterAdaptive, this,
                                                                            menu_item_erodeAfterAdaptive->GetId());

    //  ---------------------------------------------------------------------------------------------------
    menu_item_resize  =   new wxMenuItem  (menu_GeneralTransforms,   wxID_ANY,   wxT("resize"));
    menu_GeneralTransforms->Append(menu_item_resize);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &OpenCV_Tester_Window::On_resize, this, menu_item_resize->GetId());

    m_frame_panel = new wxPanel(this);

    // ВАЖНО: Главный сайзер для ФРЕЙМА, чтобы панель занимала всё место
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(m_frame_panel, 1, wxEXPAND);
    this->SetSizer(main_sizer);

    Centre();
}
//	-----------------------------------------------------------------------------------------------------
//  Filters and Convolution   -   Ядра, фильтры и свертки

void OpenCV_Tester_Window::On_Blur(wxCommandEvent& event){
    // 1. Просто создаем объект окна
    // Указываем 'this' как родителя, чтобы окно не терялось
    BlurTestFrame * m_BlurPage = new BlurTestFrame(this, wxT("Фильтр размытия (Blur)"));

    // 2. Показываем его
    m_BlurPage->Show(true);
}
void OpenCV_Tester_Window::On_medianBlur(wxCommandEvent& event){
    // 1. Просто создаем объект окна
    // Указываем 'this' как родителя, чтобы окно не терялось
    medianBlurTestFrame * m_medianBlurPage = new medianBlurTestFrame(this, wxT("Фильтр размытия (medianBlur)"));

    // 2. Показываем его
    m_medianBlurPage->Show(true);
}
void OpenCV_Tester_Window::On_GaussianBlur(wxCommandEvent& event){
    // 1. Просто создаем объект окна
    // Указываем 'this' как родителя, чтобы окно не терялось
    GaussianBlurTestFrame * m_GaussianBlurPage = new GaussianBlurTestFrame(this, wxT("Фильтр размытия (GaussianBlur)"));

    // 2. Показываем его
    m_GaussianBlurPage->Show(true);
}
void OpenCV_Tester_Window::On_threshold(wxCommandEvent& event){
// 1. Просто создаем объект окна
    // Указываем 'this' как родителя, чтобы окно не терялось
    thresholdTestFrame * m_thresholdPage = new thresholdTestFrame(this, wxT("Глобальный пороговый фильтр (threshold)"));

    // 2. Показываем его
    m_thresholdPage->Show(true);
}

//  Morphological transformations - морфологические преобразования
void OpenCV_Tester_Window::On_erode(wxCommandEvent& event){
// 1. Просто создаем объект окна
    // Указываем 'this' как родителя, чтобы окно не терялось
    erodeTestFrame * m_erodePage = new erodeTestFrame(this, wxT("Трансформация сжатия (erode)"));

    // 2. Показываем его
    m_erodePage->Show(true);
}
void OpenCV_Tester_Window::On_dilate(wxCommandEvent& event){
    // 1. Просто создаем объект окна
    // Указываем 'this' как родителя, чтобы окно не терялось
    dilateTestFrame * m_dilatePage = new dilateTestFrame(this, wxT("Трансформация расширения (dilate)"));

    // 2. Показываем его
    m_dilatePage->Show(true);
}
void OpenCV_Tester_Window::On_erodeAfterThreshold(wxCommandEvent& event){
    // 1. Просто создаем объект окна
    // Указываем 'this' как родителя, чтобы окно не терялось
    erodeAfterThresholdTestFrame * m_erodeAfterThresholdPage =
        new erodeAfterThresholdTestFrame(this,   wxT("Трансформация erode после глобального порогового фильтра"));

    // 2. Показываем его
    m_erodeAfterThresholdPage->Show(true);
}
void OpenCV_Tester_Window::On_erodeAfterAdaptive(wxCommandEvent& event){
    // 1. Просто создаем объект окна
    // Указываем 'this' как родителя, чтобы окно не терялось
    erodeAfterAdaptiveTestFrame * m_erodeAfterAdaptivePage =
                    new erodeAfterAdaptiveTestFrame(this, wxT("Трансформация erode после адаптива"));

    // 2. Показываем его
    m_erodeAfterAdaptivePage->Show(true);
}

//  General Image Transforms    -   Преобразования изображений общего вида
void OpenCV_Tester_Window::On_resize(wxCommandEvent& event){
    // 1. Просто создаем объект окна
    // Указываем 'this' как родителя, чтобы окно не терялось
    resizeTestFrame * m_resizePage =
                            new resizeTestFrame(this, wxT("Преобразование размера изображения (resize)"));

    // 2. Показываем его
    m_resizePage->Show(true);
}
