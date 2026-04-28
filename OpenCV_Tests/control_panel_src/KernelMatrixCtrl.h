//  KernelMatrixCtrl.h
/*
 **************************************************************
 * Name:      OpenCV_Tester.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-21
 * Copyright: Sergej E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 **************************************************************
*/
#ifndef KERNELMATRIXCTRL_H
#define KERNELMATRIXCTRL_H
/*
    --------------------------------------------------------------------------------------------------------------------
    Класс компонента выбора размера ядер фильтров
*/
wxDECLARE_EVENT(EVT_KERNEL_CHANGED, wxCommandEvent);

class KernelMatrixCtrl : public wxControl
{
  public:
	KernelMatrixCtrl    (   wxWindow *      parent
                        ,   wxWindowID      id
                        ,   const wxPoint&  pos     =   wxDefaultPosition
                        ,   const wxSize&   size    =   wxDefaultSize
                        );

    int m_selectedVal;                              //  Значение из ряда нечетных чисел - размеров ядра

    void 	OnSize(wxSizeEvent& event);
	void 	OnPaint(wxPaintEvent& event);
	void    OnMouseClick(wxMouseEvent& event);
};
//  -----------------------------------------------------------------------------------------------------------
//#include "KernelMatrixCtrl.cpp"

#endif // KERNELMATRIXCTRL_H
