/**
 ******************************************************************************
 * @file        CRTCHander.h
 * @author      Tecnologic86
 * @version     V0.0.0
 * @date        12.08.2013 21:26:40
 * @project ThunderCryer
 ******************************************************************************
 */

#ifndef CRTCHANDER_H_
#define CRTCHANDER_H_

namespace chibios_rt {

  class CRTCHander : public BaseStaticThread<128>{
  public:
    CRTCHander();
    virtual ~CRTCHander();
  };

} /* namespace chibios_rt */
#endif /* CRTCHANDER_H_ */
