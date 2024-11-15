#ifndef COORD_SYS_UI_RIBBON
#define COORD_SYS_UI_RIBBON

#pragma once

#include <afxribbonedit.h>

struct CoordSys2DRibbonEditControls {
    CMFCRibbonEdit* editXmin_;
    CMFCRibbonEdit* editYmin_;
    CMFCRibbonEdit* editXmax_;
    CMFCRibbonEdit* editYmax_;
};


class IRibbonEditCtrlOnCommand {
public:
    virtual void onCommand(CMFCRibbonBar& ribbon, CMFCRibbonEdit& edit) noexcept = 0;
};

#endif // !COORD_SYS_UI_RIBBON
