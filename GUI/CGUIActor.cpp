/**
 ******************************************************************************
 * @file        CGUIActor.cpp
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        30.08.2012
 * @project ThunderCryer
 ******************************************************************************
 */

#include "CGUIActor.h"

namespace ThunderCryerGUI {

  /**
   * simple constructor
   *
   * @param prevActor is the previous Actor in the list
   * @param nextActor is the next Actor in the list
   */
  CGUIActor::CGUIActor(CGUIActor* prevActor, CGUIActor* nextActor):
                      _next(nextActor),_prev(prevActor),_focus(false){

  }

  CGUIActor::~CGUIActor() {
  }


  /**
   * simple getters for next an prev object
   */
  CGUIActor* CGUIActor::GetNext(){
    return _next;
  }

  CGUIActor* CGUIActor::GetPrev(){
    return _prev;
  }

  /**
   * setter for focus, subclass has to implement this function
   */
  void CGUIActor::SetFocus(bool newFocus){
    _focus = newFocus;
    Draw();
  }

} /* namespace ThunderCryerGUI */
