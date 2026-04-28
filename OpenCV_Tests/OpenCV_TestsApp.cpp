//  OpenCV_TestsApp.cpp
/*
 ***************************************************************
 * Name:      OpenCV_Tester.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-21
 * Copyright: Sergi E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 *************************************************************
*/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif
#include <wx/wx.h>
#include <wx/bitmap.h>
#include <wx/image.h> // Обязательно для JPEG/PNG
//#include <wx/gdicmn.h>

#include "OpenCV_TestsApp.h"
#include "OpenCV_TestsMain.h"
#include "OpenCV_Tester.h"

IMPLEMENT_APP(OpenCV_TestsApp);

bool OpenCV_TestsApp::OnInit()
{
    wxImage::AddHandler(new wxJPEGHandler);
    OpenCV_Tester_Window* frame = new OpenCV_Tester_Window(wxT("OpenCV Tester Demo"));
    frame->SetIcon(wxIcon("MyAvatar.jpg", wxBITMAP_TYPE_JPEG)); // To Set App Icon
    frame->Show();

    return true;
}
