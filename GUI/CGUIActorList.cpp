/**
 ******************************************************************************
 * @file        CGUIActorList.cpp
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        30.08.2012
 * @project ThunderCryer
 ******************************************************************************
 */

#include "CGUIActorList.h"

namespace ThunderCryerGUI {

  CGUIActorList::CGUIActorList(CGUIActor* start): _current(start) {
    //select first element on page
    _current->SetFocus(true);
  }

  CGUIActorList::~CGUIActorList() {
    // TODO Auto-generated destructor stub
  }
  /**
   * redraw actual display content
   */
  void CGUIActorList::Draw(){
    // draw selected actor element with focus on
    _current->SetFocus(true);
    _current->Draw();
    // draw all the other actors in the list
    for(CGUIActor* pTmpActor = _current->GetNext();
        pTmpActor != _current;
        pTmpActor = pTmpActor->GetNext()){

      pTmpActor->SetFocus(false);
      pTmpActor->Draw();
    }

  }

  /**
   * Input handler function for back command
   *
   * @return false if not possible
   */
  bool CGUIActorList::Back(){
     return _current->Back();
  }

  /**
   * Input handler function for select command
   */
  bool CGUIActorList::Select(){
    return _current->Select();
  }

  /**
   * Input handler function for next command
   *
   * @return false if not possible
   */
  bool CGUIActorList::Next(){
    //try next on Actor
    if(!(_current->Next())){
      //no action possible
      _current = _current->GetNext();
    }
    return true;
  }

  /**
   * Input handler function for Prev command
   *
   * @return false if not possible
   */
  bool CGUIActorList::Prev(){
    //try prev on actor first
    if(!(_current->Prev())){
      //no action possible
      _current = _current->GetPrev();
    }
    return true;
  }

  /**
   * handle changes in display data to keep screen up to date
   */
  void CGUIActorList::ChangeHandler(){
    // handle changes for selected actor element
    _current->ChangeHandler();
    // handle all the other actors in the list
    for(CGUIActor* pTmpActor = _current->GetNext();
        pTmpActor != _current;
        pTmpActor = pTmpActor->GetNext()){

      pTmpActor->ChangeHandler();
    }
  }

} /* namespace ThunderCryerGUI */
