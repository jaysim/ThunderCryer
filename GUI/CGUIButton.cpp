/**
 ******************************************************************************
 * @file        CGUIButton.cpp
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        03.09.2012
 * @project ThunderCryer
 ******************************************************************************
 */

#include "CGUIButton.h"
#include "SansSerif12.h"

namespace ThunderCryerGUI {

  /**
   * main constructor
   */
  CGUIButton::CGUIButton(CGUIActor* prevActor, CGUIActor* nextActor,
             void (*actionCallback)(void),char* textPressed,
             char *textReleased, int x, int y, int width, int height,
             bool state, bool toggle, CGraphicLCD *display)

          : CGUIActor(prevActor, nextActor, display), _actionCallback(actionCallback),
            _textPressed(textPressed), _textReleased(textReleased), _x(x), _y(y),
            _width(width), _heigth(height), _state(state), _toggle(toggle) {

  }


  /**
   * Getter an setter for Button state
   */
  void CGUIButton::State(bool newState){
    _state = newState;
  }

  bool CGUIButton::State() const{
    return _state;
  }

  /**
   * Set position
   *
   * @param x1/y1 upper left corner, x2/y2 lower right corner
   */
  void CGUIButton::SetPosition(int x, int y){
    _x = x;
    _y = y;

  }

  /**
   * handle changes in display data to keep screen up to date
   */
  void CGUIButton::ChangeHandler(){
    Draw();
  }

  /**
   * draw actor
   */
  void CGUIButton::Draw(){
    bool inverseState = _display->Inverse();

    // invert in _focus = true, non invert on _focus = false
    _display->Inverse(inverseState ^ _focus);

    _display->SectorClear(_x, _y, _width, _heigth);

    //unfilled Rect with shadow
    _display->Rectangle(_x,_y ,_width-1, _heigth-1,false);
    _display->Rectangle(_x+1,_y+1 ,_width-1, _heigth-1,false);

    if(_state){ //Pressed
       _display->Rectangle(_x+3,_y+3,_width-3, _heigth-3,true);

       // text needs to be inverted on filled rect
       // in whatever inverted state the driver is
       bool displayState = _display->Inverse();
       _display->Inverse(!displayState);
       //place text 5 pixels from left edge
       //and in the middle of the Button
       _display->WriteString(_textPressed, c_FontSansSerif12,_x+5,
                             _y +_width/2 - 9);
       // set back to normal
       _display->Inverse(displayState);

    } else {    //Released

      //place text 5 pixels from left edge
      //and in the middle of the Button
      _display->WriteString(_textReleased, c_FontSansSerif12,_x+4,
                            _y +_heigth/2 - 9);
    }

    // set back to original setting
    _display->Inverse(inverseState);
  }

  /**
   * Input handler function for back command
   *
   * @return false if not possible
   */
  bool CGUIButton::Back(){
    return false; // no action possible
  }

  /**
   * Input handler function for select command
   *
   * @return false if actor is not able to execute command
   */
  bool CGUIButton::Select(){
    if(_toggle){ //toggle state if needed
      _state = !_state;
    }
    _actionCallback();
    return true;
  }

  /**
   * Input handler function for next command
   *
   * @return false if actor is not able to execute command
   */
  bool CGUIButton::Next(){
    return false;
  }

  /**
   * Input handler function for prev command
   *
   * @return false if actor is not able to execute command
   */
  bool CGUIButton::Prev(){
    return false;
  }

} /* namespace ThunderCryerGUI */
