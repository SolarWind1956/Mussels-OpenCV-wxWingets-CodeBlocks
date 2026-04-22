//  KernelMatrixCtrl.cpp
/*
 ***************************************************************
 * Name:      KernelMatrixCtrl.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergi E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-21
 * Copyright: Sergi E. Heckel (www.Solar_Wind1956.com)
 * License:
 *************************************************************
*/
#include "KernelMatrixCtrl.h"
//  ----------------------------------------------------------------------------------------------------------------------------
/*
    Класс компонента выбора размера ядер фильтров
*/
wxDEFINE_EVENT(EVT_KERNEL_CHANGED, wxCommandEvent);

KernelMatrixCtrl::KernelMatrixCtrl      (   wxWindow *      parent
                                        ,   wxWindowID      id
                                        ,   const wxPoint&  pos
                                        ,   const wxSize&   size
                                        )
                                        :   wxControl   (   parent
                                                        ,   id
                                                        ,   pos
                                                        ,   size
                                                        ,   wxBORDER_SIMPLE
                                                        )
{
	//	Обозначим минимальные размеры для фреймового окна с wxGridSizer
	//SetMinSize(wxSize(350, 350));

    Bind(wxEVT_PAINT,       &KernelMatrixCtrl::OnPaint,         this);
    Bind(wxEVT_LEFT_DOWN,   &KernelMatrixCtrl::OnMouseClick,    this);
    Bind(wxEVT_SIZE,        &KernelMatrixCtrl::OnSize,          this);

    Centre();
}

void KernelMatrixCtrl::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);     //  Создание DC автоматически очищает область и убирает "серое пятно"
    PrepareDC(dc);

    wxSize size = GetClientSize();
    int rows = 4;
    int cols = 4;
    int cellW = size.x / cols;      //  Ширина ячейки
    int cellH = size.y / rows;      //  Высота ячейки

    dc.SetPen(wxPen(*wxBLACK, 1));
    dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Рисуем рамку ячейки
            wxRect rect(j * cellW, i * cellH, cellW, cellH);
            dc.DrawRectangle(rect);

            // Считаем число (ваша формула: 2 * n + 1)
            int val = 2 * (i * cols + j) + 1;

            if (val == m_selectedVal) {
                dc.SetBrush(wxBrush(wxColour(200, 220, 255))); // Светло-голубой для выбранной ячейки
            } else {
                dc.SetBrush(*wxWHITE_BRUSH);
            }
            dc.DrawRectangle(rect);

            wxString text = wxString::Format("%d", val);

            // Рисуем текст по центру ячейки
            wxSize textSize = dc.GetTextExtent(text);
            dc.DrawText(text,
                        rect.x + (cellW - textSize.x) / 2,
                        rect.y + (cellH - textSize.y) / 2);
        }
    }
}

//	Каждый раз, когда окно меняет свои размеры, виджет перерисовывается:
void 	KernelMatrixCtrl::OnSize(wxSizeEvent& 	event)
{
	Refresh();
}

void KernelMatrixCtrl::OnMouseClick(wxMouseEvent& event)
{
    int rows = 4;
    int cols = 4;

    wxPoint pos  = event.GetPosition();
    wxSize  size = GetClientSize();

    // 1. Вычисляем физический размер одной ячейки (в пикселях)
    // Важно: делим на double или используем float, чтобы избежать раннего округления,
    // но для индекса сетки достаточно и целых чисел:
    int cellWidth  = size.x / cols;
    int cellHeight = size.y / rows;

    // Предотвращаем деление на ноль (на всякий случай)
    if (cellWidth <= 0 || cellHeight <= 0) return;

    // 2. Находим индекс ячейки (просто делим координату на ширину ячейки)
    int col = pos.x / cellWidth;
    int row = pos.y / cellHeight;

    // 3. "Защита от дурака" (чтобы клик по самой границе не выдал индекс 4)
    if (col >= cols) col = cols - 1;
    if (row >= rows) row = rows - 1;
    if (col < 0) col = 0;
    if (row < 0) row = 0;

    // 4. Считаем ваше нечетное значение
    m_selectedVal = 2 * (row * cols + col) + 1;

    // 2. Создаем "командное" событие
    // Мы используем ваш EVT_KERNEL_CHANGED
    wxCommandEvent  customEvent(EVT_KERNEL_CHANGED, GetId());

    // 3. Кладем данные внутрь события
    customEvent.SetInt(m_selectedVal);
    customEvent.SetEventObject(this);

    // 4. ГЛАВНОЕ: Отправляем событие РОДИТЕЛЮ
    // ProcessWindowEvent заставляет событие "всплывать" к TestFrame
    ProcessWindowEvent(customEvent);

    // Не забываем обновить саму матрицу визуально
    Refresh();
}
