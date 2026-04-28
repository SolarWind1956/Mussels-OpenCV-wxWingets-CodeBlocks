//  SpinCtrl.h
/*
 **************************************************************
 * Name:      SpinCtrl.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-23
 * Copyright: Sergej E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 **************************************************************
*/
#ifndef SPINCTRLCTRL_H
#define SPINCTRLCTRL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
/*
    --------------------------------------------------------------------------------------------------------------------
    Класс компонента выбора масштаба отображения изображений
*/
class SpinCtrl : public wxControl{
 public:
    SpinCtrl    (   wxWindow *  parent
                ,   wxWindowID  id
                ) ;

    void OnScroll(wxCommandEvent& event);

    wxSpinCtrl *    m_spin;

    int             m_chois;
};
//  ------------------------------------------------------------------------------------------------------
//#include "SpinCtrl.cpp"
#endif // SPINCTRL_H
