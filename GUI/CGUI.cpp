/**
 ******************************************************************************
 * @file        CGUI.cpp
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        30.08.2012
 * @project ThunderCryer
 ******************************************************************************
 */

#include "CGUI.h"

namespace ThunderCryerGUI {

   /**
    * basic constructor sets home screen
    *
    * @param homePage  pointer to home screen page
    */
  CGUI::CGUI(CGUIPage* homePage): _lastPage(homePage), _currentPage(homePage),
                                  _homePage(homePage){

  }

  /**
   * standard destructor
   */
  CGUI::~CGUI() {
  }

  /**
   * redraw actual display content
   */
  void CGUI::Draw(){
    _currentPage->Draw();
  }

  /**
   * Input handler function for back command
   */
  void CGUI::Back(){
    //is the page able to go back
    if(!(_currentPage->Back())){
      //if not go to lastPage
      _currentPage = _lastPage;
      //redraw screen
      Draw();
    }
  }

  /**
   * Input handler function for select command
   */
  void CGUI::Select(){
    _currentPage->Select();
  }

  /**
   * Input handler function for next command
   */
  void CGUI::Next(){
    _currentPage->Next();
  }

  /**
   * Input handler function for Prev command
   */
  void CGUI::Prev(){
    _currentPage->Prev();
  }

  /**
   * Goto homescreen
   */
  void CGUI::GoHome(){
    _currentPage = _homePage;
    //redraw screen
    Draw();
  }

  /**
   * handle changes in display data to keep screen up to date
   */
  void CGUI::ChangeHandler(){
    _currentPage->ChangeHandler();
  }



} /* namespace ThunderCryerGUI */
