#include <IHWLayer.h>


////////////////// EXAMPLE //////////////////////
/*

enum stateNames {START,SHIFT};

STATEFUNCS

	STATE(START)

		ON_ENTER

		ALWAYS

		ON_EVENT

			EVENTCHECK(BTN_DOWN,ShiftButton)
				CHANGE_STATE(SHIFT);
			END_EVENTCHECK

		ON_EXIT

	END_STATE

	STATE(SHIFT)

		ON_ENTER

		ALWAYS

		ON_EVENT

			EVENTCHECK(BTN_DOWN,ShiftButton)
				CHANGE_STATE(START);
			END_EVENTCHECK

		ON_EXIT

	END_STATE

END_STATEFUNCS

*/



////// Marcos for implicitly defining run() which
////// processes the current state
////////////////////////////////////////////////
#define STATEFUNCS void states::run() {\
					switch (activeState) {

#define STATE(N) case N:
#define ON_ENTER if (newState) {
#define ALWAYS   newState = false;}
#define ON_EVENT if (!eventProcessed) {
#define ON_EXIT  eventProcessed = true;}\
					if (newState) {
#define END_STATE }break;
#define END_STATEFUNCS default: break;	}}\
						states stateObj;

#define CHANGE_STATE(N) activeState = N; newState = true;


////// Macros for analysing the last read event
////////////////////////////////////////////////
#define DONT_CARE_NUMBER lastEvent.number

// check for type and number
#define EVENTCHECK(...) EVENTCHECK_(__VA_ARGS__)
#define EVENTCHECK_(TY,NU) if((lastEvent.type == IHWLayer::TY) && (lastEvent.number == NU) ) {
#define END_EVENTCHECK }

// check for type and if number is inside array
// if match has been found it will be written to VAR
#define EVENTCHECK_ARRAY(TY,AR,VAR) if ((lastEvent.type == IHWLayer::TY)) {\
										for (uint8_t VAR = 0; VAR<sizeof(AR); VAR++) {\
											if (lastEvent.number == AR[VAR]) {
#define END_EVENTCHECK_ARRAY 			}}}



////// HOOKS
////////////////////////////////////////////////

// where to run current state
#define RUN_ACTIVE_STATE stateObj.run();

// add a new event to be processed during next run of state
#define ADD_EVENT(E) stateObj.queueEvent(E);




class states {
public:

	states() :
		activeState(0),
		newState(true),
		eventProcessed(true)
	{}

	void queueEvent(hardwareEvent event) {
		if (eventProcessed) {
			lastEvent = event;
			eventProcessed = false;
		}
	}
	void run();



private:
	uint8_t activeState;
	bool newState;
	bool eventProcessed;
	hardwareEvent lastEvent;

};




