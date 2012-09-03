/*
 * CGUIPage.cpp
 *
 *  Created on: 30.08.2012
 *      Author: brand
 */

#include "CGUIPage.h"

namespace ThunderCryerGUI {

  /**
   * basic constructor
   *
   * @param actorList  list of actors on page
   */
  CGUIPage::CGUIPage(CGUIActorList* actorList): _action(actorList),
                                                _focus(false) {
  }

  /**
   * standard destructor
   */
  CGUIPage::~CGUIPage() {
  }

  /**
   * redraw actual display content
   */
  void CGUIPage::Draw(){
    _action->Draw();
  }

  /**
   * Input handler function for back command
   * @return false if back not possible
   */
  bool CGUIPage::Back(){
    return _action->Back();
  }

  /**
   * Input handler function for select command
   */
  void CGUIPage::Select(){
    return _action->Select();
  }

  /**
   * Input handler function for next command
   */
  void CGUIPage::Next(){
    return _action->Next();
  }

  /**
   * Input handler function for Prev command
   */
  void CGUIPage::Prev(){
    return _action->Prev();
  }

  /**
   * handle changes in display data to keep screen up to date
   */
  void CGUIPage::ChangeHandler(){
    return _action->ChangeHandler();
  }

} /* namespace ThunderCryerGUI */
