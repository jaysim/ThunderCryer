/**
 ******************************************************************************
 * @file        CGUIPage.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        30.08.2012
 * @project ThunderCryer
 ******************************************************************************
 */

#ifndef CGUIPAGE_H_
#define CGUIPAGE_H_

#include "CGUIActorList.h"
#include "CGraphicLCD.h"

namespace ThunderCryerGUI {

  class CGUIPage {
  private:
    /**
     * Actors on Page
     */
    CGUIActorList* _action;

    /**
     * pointer to display interface
     */
    CGraphicLCD* _display;

  public:

    CGUIPage(CGUIActorList* actorList);
    virtual ~CGUIPage();

    /**
     * redraw actual display content
     */
    void Draw();

    /**
     * Input handler function for back command
     * @return false if back not possible
     */
    bool Back();

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
     * handle changes in display data to keep screen up to date
     */
    void ChangeHandler();

  };

} /* namespace ThunderCryerGUI */
#endif /* CGUIPAGE_H_ */
