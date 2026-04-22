//  OpenCV_Tester.cpp
/*
 ***************************************************************
 * Name:      OpenCV_Tester.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergi E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-21
 * Copyright: Sergi E. Heckel (www.Solar_Wind1956.com)
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
                                                            ,   wxSize(1200, 600)
                                                            )
{
    frame_panel = new wxPanel(this);

    // Теперь XXX_TestFrame — это просто сложный виджет (панель)
    //test_frame = new BlurTestFrame(frame_panel);
    test_frame = new medianBlurTestFrame(frame_panel);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    vbox->Add(test_frame, 1, wxEXPAND | wxALL, 5);

    frame_panel->SetSizer(vbox);

    this->Layout(); // Принудительный расчет всей иерархии
}
//	-----------------------------------------------------------------------------------------------------
