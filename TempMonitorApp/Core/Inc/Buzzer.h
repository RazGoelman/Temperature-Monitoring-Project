/*
 * Buzzer.h
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_



class BUZZER{

private:
	TIM_HandleTypeDef *		_htim;

public:


	BUZZER(TIM_HandleTypeDef *htim);
	void buzzerStartPlay();
	void buzzerStopPlay();
};

#endif /* INC_BUZZER_H_ */
