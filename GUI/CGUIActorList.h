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
     */
    CGUIActorList(CGUIActor* start);

    virtual ~CGUIActorList();

    /**
     * initialise GUI and draw start page
     */
    void Init();

    /**
     * redraw actual display content
     */
    void Draw();

    /**
     * Input handler function for back command
     */
    void Back();

    /**
     * Input handler function for select command
     */
    void Select();

    /**
     * Input handler function for next command
     */
    void Next();

    /**
     * Input handler function for Prev command
     */
    void Prev();

  };

} /* namespace ThunderCryerGUI */
#endif /* CGUIACTORLIST_H_ */
