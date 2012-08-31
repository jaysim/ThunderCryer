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
    bool focus;

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
     * function operator reimplementated by a subclass as actors action
     * @return this object
     */
    virtual CGUIActor operator()();

    /**
     * simple getters für next an prev object
     */
    const CGUIActor Next();
    const CGUIActor Prev();

    /**
     * virtual setter for focus, subclass has to implement this function
     */
    void SetFocus(bool newFocus);

    /**
     * handle changes in display data to keep screen up to date
     */
    virtual void ChangeHandler();
  };

} /* namespace ThunderCryerGUI */
#endif /* CGUIACTOR_H_ */
