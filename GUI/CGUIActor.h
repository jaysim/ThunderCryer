/**
 ******************************************************************************
 * @file        CGUIActor.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        30.08.2012
 * @project ThunderCryer
 ******************************************************************************
 */

#ifndef CGUIACTOR_H_
#define CGUIACTOR_H_

#include "CGraphicLCD.h"

namespace ThunderCryerGUI {

  class CGUIActor {
  protected:
    /*
     * pointers for linked list elements
     */
    CGUIActor* _next;
    CGUIActor* _prev;

    /**
     * pointer to display interface
     */
    CGraphicLCD* _display;

    /*
     * true when button is in focus
     */
    bool _focus;

  public:

    /**
     * simple constructor
     *
     * @param prevActor is the previous Actor in the list
     * @param nextActor is the next Actor in the list
     */
    CGUIActor(CGUIActor* prevActor, CGUIActor* nextActor, CGraphicLCD *display);
    virtual ~CGUIActor();

    /**
     * simple getters für next an prev object
     */
    CGUIActor* GetNext();
    CGUIActor* GetPrev();

    /**
     * handle changes in display data to keep screen up to date
     */
    virtual void ChangeHandler();

    /**
     * draw actor
     */
    virtual void Draw();

    /**
     * Input handler function for back command
     *
     * @return false if not possible
     */
    virtual bool Back();

    /**
     * Input handler function for select command
     *
     * @return false if actor is not able to execute command
     */
    virtual bool Select();

    /**
     * Input handler function for next command
     *
     * @return false if actor is not able to execute command
     */
    virtual bool Next();

    /**
     * Input handler function for prev command
     *
     * @return false if actor is not able to execute command
     */
    virtual bool Prev();

    /**
     * setter for focus
     */
    void SetFocus(bool newFocus);

  };

} /* namespace ThunderCryerGUI */
#endif /* CGUIACTOR_H_ */
