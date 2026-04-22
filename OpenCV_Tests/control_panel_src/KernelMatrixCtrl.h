//  KernelMatrixCtrl.h
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
