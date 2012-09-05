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
#include "SansSerif12.h"
#include <cstdio>

namespace ThunderCryerGUI {

  /**
   * main constructor
   */
  CGUINumberSwitch::CGUINumberSwitch(CGUIActor* prevActor, CGUIActor* nextActor,
                   void (*actionCallback)(unsigned int value), int x, int y,
                   unsigned int startValue, unsigned int digits, bool zeroFill,
                   unsigned int limitLow, unsigned int limitHigh,
                   CGraphicLCD *display)
                  : CGUIActor(prevActor,nextActor, display),
                    _actionCallback(actionCallback), _x(x), _y(y),
                   _value(startValue), _limitLow(limitLow),
                   _limitHigh(limitHigh), _digits(digits), _zeroFill(zeroFill),
                   _editing(false){

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
    bool inverseState = _display->Inverse();
    char bufferFormatString[5];
    char bufferDisplayString[10];

    //build format string for display text
    if(_zeroFill){
      sprintf(bufferFormatString,"%%0%dd",_digits);
    }else{
      sprintf(bufferFormatString,"%%%dd",_digits);
    }
    // build text with calculated format
    sprintf(bufferDisplayString,bufferFormatString,_value);

    // invert in _focus = true, non invert on _focus = false
    _display->Inverse(inverseState ^ _focus);

    // one digit is 9 pix wide, and 18 high
    _display->SectorClear(_x, _y, 9*_digits + 4, 24);
    if(_editing){ // when editing
      //mark rectangle
      _display->Rectangle(_x+1,_y+1,9*_digits + 2, 22,false);

    }

    //place text 3 pixels from left edge
    //and in the middle of the Button
    _display->WriteString(bufferDisplayString, c_FontSansSerif12,_x+3, _y + 4);

    // set back to original setting
    _display->Inverse(inverseState);
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
      _actionCallback(_value);
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
      if((_value > _limitHigh) || (_value < _limitLow)){
        _value = _limitHigh;
      }

      Draw();
    }
    return _editing;
  }

} /* namespace ThunderCryerGUI */
