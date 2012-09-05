/**
 ******************************************************************************
 * @file        CGUINumberSwitch.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        04.09.2012
 * @project ThunderCryer
 ******************************************************************************
 */
#ifndef CGUINUMBERSWITCH_H_
#define CGUINUMBERSWITCH_H_

#include "CGUIActor.h"

namespace ThunderCryerGUI {

  class CGUINumberSwitch: public ThunderCryerGUI::CGUIActor {
  private:
    /*
     * pointer to select event callback-function
     */
    void (*_actionCallback)(unsigned int value);

    /*
     * Button coordinates (upper right corner)
     */
    int _x, _y;

    /*
     * set able value
     */
    unsigned int _value;

    /*
     * lower limit for value
     */
    unsigned int _limitLow;

    /*
     * high limit for value
     */
    unsigned int _limitHigh;

    /*
     * digits of the set able value
     */
    unsigned int _digits;

    /*
     * zero fill for text
     */
    bool _zeroFill;

    /*
     * true if actor is in editing mode
     */
    bool _editing;

  public:

    /**
     * main constructor
     */
    CGUINumberSwitch(CGUIActor* prevActor, CGUIActor* nextActor,
               void (*actionCallback)(unsigned int value), int x, int y,
               unsigned int startValue, unsigned int digits, bool zeroFill,
               unsigned int limitLow, unsigned int limitHigh,
               CGraphicLCD *display);

    virtual ~CGUINumberSwitch();


    /**
     * Set position
     *
     * @param x1/y1 upper left corner
     */
    void SetPosition(int x1, int y1);

    /**
     * getter for Value
     */
    unsigned int Value() const;

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
#endif /* CGUINUMBERSWITCH_H_ */
