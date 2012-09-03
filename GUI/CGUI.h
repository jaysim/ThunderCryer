/**
 ******************************************************************************
 * @file        CGUI.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        30.08.2012
 * @project ThunderCryer
 ******************************************************************************
 */

#ifndef CGUI_H_
#define CGUI_H_

#include "CGUIPage.h"

namespace ThunderCryerGUI {

  class CGUI {
  private:
    CGUIPage* _lastPage;
    CGUIPage* _currentPage;
    CGUIPage* _homePage;

  public:
    /**
     * basic constructor sets home screen
     *
     * @param homePage  pointer to home screen page
     */
    CGUI(CGUIPage* homePage);

    virtual ~CGUI();

    /**
     * redraw actual display content
     */
    void Draw();

    /**
     * Input handler function for back command
     */
    void Back();

    /**
     * Input handler function for select command
     */
    void Select();

    /**
     * Input handler function for next command
     */
    void Next();

    /**
     * Input handler function for Prev command
     */
    void Prev();

    /**
     * Goto homescreen
     */
    void GoHome();

    /**
     * handle changes in display data to keep screen up to date
     */
    void ChangeHandler();

  };

} /* namespace ThunderCryerGUI */
#endif /* CGUI_H_ */
