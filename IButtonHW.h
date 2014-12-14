/*
 * IButtonHW.h
 *
 *  Created on: Dec 12, 2014
 *      Author: martin
 */

#ifndef IBUTTONHW_H_
#define IBUTTONHW_H_

#include <inttypes.h>

class IButtonHW {
public:

	enum ButtonState{UP, DOWN};

    virtual ButtonState getButtonState(unsigned char index) = 0;

};

#endif /* IBUTTONHW_H_ */
