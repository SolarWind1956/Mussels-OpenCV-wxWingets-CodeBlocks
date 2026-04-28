//  SpinCtrl.cpp
/*
 ***************************************************************
 * Name:      SpinCtrl.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-23
 * Copyright: Sergi E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 *************************************************************
*/
#include "SpinCtrl.h"

/*
    Класс компонента выбора некоторого целого числа - количества, например, итераций
*/
SpinCtrl::SpinCtrl  (   wxWindow *      parent
                    ,   wxWindowID      id
                    )
                    :   wxControl   (   parent
                                    ,   id
                                    )
{
    // Создаем: (родитель, id, текст, позиция, размер, стиль, мин, макс, начальное)
    m_spin     =   new     wxSpinCtrl  (   this
                                        ,   wxID_ANY
                                        ,   wxEmptyString
                                        ,   wxDefaultPosition
                                        ,   wxDefaultSize
                                        ,   wxSP_ARROW_KEYS
                                        ,   1
                                        ,   7
                                        ,   1
                                        );
}
