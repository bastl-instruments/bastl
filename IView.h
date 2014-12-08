/*
 * IView.h
 *
 *  Created on: Jul 25, 2014
 *      Author: bastl
 */

#ifndef IVIEW_H_
#define IVIEW_H_

class IView {
public:
	virtual ~IView() = 0;
	virtual void update() = 0;
};

inline IView::~IView() { }

#endif /* IVIEW_H_ */

