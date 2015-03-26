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

			EVENTCHECK(BTN_UP,ShiftButton)
				CHANGE_STATE(START);
			END_EVENTCHECK

		ON_EXIT

	END_STATE

END_STATEFUNCS

*/



////// Marcos for implicitly defining run() which
////// processes the current state
////////////////////////////////////////////////
#define STATEFUNCS 	uint8_t states::activeState = 0;\
					bool states::newState = true;\
					bool states::eventProcessed = true;\
					hardwareEvent states::lastEvent;\
					void states::run() {

#define ANYSTATE
#define ANYSTATE_ENTER if(newState) {
#define ANYSTATE_EVENT } if (!eventProcessed) {
#define END_ANYSTATE } switch (activeState) {

#define STATE(N) case N:
#define ON_ENTER if (newState) {							// code that is executed everytime this state is run for the first time
#define ALWAYS   newState = false;}							// code that is executed everytime this state is active
#define ON_EVENT if (!eventProcessed) {						// code that is executed when there is a new event
#define ON_EXIT  eventProcessed = true;}if (newState) { 	// code that is executed when this state is run for the last time

#define END_STATE }break;
#define END_STATEFUNCS default: break;	}}

#define CHANGE_STATE(N) activeState = N; newState = true; 					// switch to different state in next run
#define RERUN_STATE newState = true;	eventProcessed = true; return;		// re-renter the current state in next run.
																			// In ANYSTATE, this will NOT enter the real state!



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
#define RUN_ACTIVE_STATE states::run();

// add a new event to be processed during next run of state
#define ADD_EVENT(E) states::queueEvent(E);

// for use as event receiver of hardware layer
#define STATEHANDLER states::queueEvent



class states {
public:

	static void queueEvent(hardwareEvent event) {
		if (eventProcessed) {
			lastEvent = event;
			eventProcessed = false;
		}
	}
	static void run();



private:
	static uint8_t activeState;
	static bool newState;
	static bool eventProcessed;
	static hardwareEvent lastEvent;

};





