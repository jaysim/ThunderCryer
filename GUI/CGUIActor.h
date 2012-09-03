/**
 ******************************************************************************
 * @file        CGUIActor.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        30.08.2012
 * @project ThunderCryer
 ******************************************************************************
 */

#ifndef CGUIACTOR_H_
#define CGUIACTOR_H_


namespace ThunderCryerGUI {

  class CGUIActor {
  private:
    /*
     * pointers for linked list elements
     */
    CGUIActor* _next;
    CGUIActor* _prev;

    /**
     * focus indicator, true when actor is selected
     */
    bool _focus;

  public:

    /**
     * simple constructor
     *
     * @param prevActor is the previous Actor in the list
     * @param nextActor is the next Actor in the list
     */
    CGUIActor(CGUIActor* prevActor, CGUIActor* nextActor);
    virtual ~CGUIActor();

    /**
     * simple getters für next an prev object
     */
    CGUIActor* GetNext();
    CGUIActor* GetPrev();

    /**
     * setter for focus, subclass has to implement this function
     */
    void SetFocus(bool newFocus);

    /**
     * handle changes in display data to keep screen up to date
     */
    virtual void ChangeHandler();

    /**
     * draw actor
     */
    virtual void Draw();

    /**
     * Input handler function for back command
     *
     * @return false if not possible
     */
    virtual bool Back();

    /**
     * Input handler function for select command
     */
    virtual bool Select();

  };

} /* namespace ThunderCryerGUI */
#endif /* CGUIACTOR_H_ */
