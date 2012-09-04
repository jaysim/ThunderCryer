/**
 ******************************************************************************
 * @file        CGUINumberSwitch.cpp
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        04.09.2012
 * @project ThunderCryer
 ******************************************************************************
 */

#include "CGUINumberSwitch.h"

namespace ThunderCryerGUI {

  /**
   * main constructor
   */
  CGUINumberSwitch::CGUINumberSwitch(CGUIActor* prevActor, CGUIActor* nextActor,
                   void (*actionCallback)(void), int x, int y,
                   unsigned int startValue, unsigned int digits,
                   unsigned int limitLow, unsigned int limitHigh,
                   CGraphicLCD *display)
                  :_prev(prevActor), _next(nextActor),
                   _actionCallback(actionCallback), _x(x), _y(y),
                   _value(startValue), _limitLow(limitLow),
                   _limitHigh(limitHigh), _digits(digits), _display(display),
                   _editing(false) {

  }

  CGUINumberSwitch::~CGUINumberSwitch(){

  }


  /**
   * Set position
   *
   * @param x/y upper left corner
   */
  void CGUINumberSwitch::SetPosition(int x, int y){
    _x = x;
    _y = y;
  }

  /*
   * getter for actor value
   */
  unsigned int CGUINumberSwitch::Value() const{
    return _value;
  }

  /**
   * handle changes in display data to keep screen up to date
   */
  void CGUINumberSwitch::ChangeHandler(){

  }

  /**
   * draw actor
   */
  void CGUINumberSwitch::Draw(){

  }

  /**
   * Input handler function for back command
   *
   * @return false if not possible
   */
  bool CGUINumberSwitch::Back(){
    if(_editing){
      _editing = false;
      Draw(); //redraw in non editing mode
      return true;
    }
    return false;
  }

  /**
   * Input handler function for select command
   *
   * @return false if actor is not able to execute command
   */
  bool CGUINumberSwitch::Select(){
    if(!_editing){
      _editing = true;
      Draw();
    }
    _editing = true;

    return true;
  }

  /**
   * Input handler function for next command
   *
   * @return false if actor is not able to execute command
   */
  bool CGUINumberSwitch::Next(){
    if(_editing){

      _value++;
      //wrap around
      if(_value > _limitHigh){
        _value = _limitLow;
      }

      Draw();
    }
    return _editing;
  }

  /**
   * Input handler function for prev command
   *
   * @return false if actor is not able to execute command
   */
  bool CGUINumberSwitch::Prev(){
    if(_editing){

      _value--;
      //wrap around, even on overflow
      if((value > _limitHigh) || (value < _limitLow)){
        _value = _limitHigh;
      }

      Draw();
    }
    return _editing;
  }

} /* namespace ThunderCryerGUI */
