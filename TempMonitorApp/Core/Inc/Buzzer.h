/*
 * Buzzer.h
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

typedef enum  _NOTES{
						   NOTE_1 = 381,
						   NOTE_2 = 340,
						   NOTE_3 = 322,
						   NOTE_4 = 286,
						   NOTE_5 = 255,
						   NOTE_6 = 227,
						   NOTE_7 = 202,

}NOTES;
typedef enum _FRAME{
						   FRAME_0 = 1000,
						   FRAME_1 = 500,
						   FRAME_2 = 255
}FRAME;

class BUZZER{

private:
	TIM_HandleTypeDef *		_htim;

public:


	BUZZER(TIM_HandleTypeDef *htim);
	void buzzerStartPlay();
	void buzzerStopPlay();
};



#endif /* INC_BUZZER_H_ */
