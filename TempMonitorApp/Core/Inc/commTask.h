/*
 * commTask.h
 */

#ifndef INC_COMMTASK_H_
#define INC_COMMTASK_H_

#include "main.h"

#define MAX_BUFFER_LENGTH 			100

class CommTASK {
private:
	int _cmdcount;
	int _cmdprint;
public:
	CommTASK(){
		_cmdcount 						= 0;
		_cmdprint 						= 0;
	}
	int commTask();
	void handleCommand();


};


#endif /* INC_COMMTASK_H_ */
