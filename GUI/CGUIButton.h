/**
 ******************************************************************************
 * @file        CGUIButton.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        03.09.2012
 * @project ThunderCryer
 ******************************************************************************
 */

#ifndef CGUIBUTTON_H_
#define CGUIBUTTON_H_

#include "CGUIActor.h"

namespace ThunderCryerGUI {

  class CGUIButton: public ThunderCryerGUI::CGUIActor {
  private:
    /*
     * pointer to select event callback-function
     */
    void (*_actionCallback)(void);

    /*
     * pointer to static visible Text when pressed
     */
    char* _textPressed;

    /*
     * pointer to static visible Text when released
     */
    char* _textReleased;

    /*
     * Button coordinates (upper right corner)
     */
    int _x, _y, _width, _heigth;

    /*
     * Buttonstate, true means pressed
     */
    bool _state;

    /*
     * toggle enable setting, true means button toggles on select
     */
    bool _toggle;

  public:
    /**
     * main constructor
     */
    CGUIButton(CGUIActor* prevActor, CGUIActor* nextActor,
                 void (*actionCallback)(void),char* textPressed,
                 char *textReleased, int x, int y, int width, int height,
                 bool state, bool toggle, CGraphicLCD *display);



    virtual ~CGUIButton();


    /**
     * Getter an setter for Button state
     */
    void State(bool newState);
    bool State() const;

    /**
     * Set position
     *
     * @param x1/y1 upper left corner
     */
    void SetPosition(int x1, int y1);

    /**
     * handle changes in display data to keep screen up to date
     */
    void ChangeHandler();

    /**
     * draw actor
     */
    void Draw();

    /**
     * Input handler function for back command
     *
     * @return false if not possible
     */
    bool Back();

    /**
     * Input handler function for select command
     *
     * @return false if actor is not able to execute command
     */
    bool Select();

    /**
     * Input handler function for next command
     *
     * @return false if actor is not able to execute command
     */
    bool Next();

    /**
     * Input handler function for prev command
     *
     * @return false if actor is not able to execute command
     */
    bool Prev();

  };

} /* namespace ThunderCryerGUI */
#endif /* CGUIBUTTON_H_ */
