/**
 ******************************************************************************
 * @file        CGUIActorList.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        30.08.2012
 * @project ThunderCryer
 ******************************************************************************
 */


#ifndef CGUIACTORLIST_H_
#define CGUIACTORLIST_H_

#include "CGUIActor.h"

namespace ThunderCryerGUI {

  class CGUIActorList {
  private:
    /**
     * current element in actor list
     */
    CGUIActor* _current;
  public:
    /**
     * simple constuctor with start element of the list
     *
     * @param start  pointer to start element of the list
     */
    CGUIActorList(CGUIActor* start);

    virtual ~CGUIActorList();

    /**
     * redraw actual display content
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
     */
    bool Select();

    /**
     * Input handler function for next command
     *
     * @return false if not possible
     */
    bool Next();

    /**
     * Input handler function for Prev command
     *
     * @return false if not possible
     */
    bool Prev();

    /**
     * handle changes in display data to keep screen up to date
     */
    void ChangeHandler();

  };

} /* namespace ThunderCryerGUI */
#endif /* CGUIACTORLIST_H_ */
