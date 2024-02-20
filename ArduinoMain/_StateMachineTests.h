#include <ArduinoUnit.h>
#include "_helper_tests.h"

#include "StateMachine.h"
test(standby_saves_home_coordinates) {
    StateMachine sm(1, 0, 0, 0,
                    1, 0, 0, 0,
                    5);
    assertEqual(sm.GetState(), state::STANDBY); //(1) On init, state machine is in standby


    sm.DecideState();
    sm.ExecuteState();
    assertEqual(sm.GetState(), state::STANDBY);
    assertLocationsEqual(sm.GetHomeCoords(), Msg::gnss_INVALID); //(2) On init, the hoome coords are invalid so long as the gps is unfixed.

    sm.Input(Msg::StateMachineInput{Msg::StateMachineInput::START}); // (3) even if the state machine receives a start command, it doesn't switch states until gps is fixed
    assertEqual(sm.GetState(), state::STANDBY);



    sm.test_SetCurrentLocation(Msg::GNSS{10.0, 10.0, 0.0}); 
    sm.DecideState();
    sm.ExecuteState();
    assertEqual(sm.GetState(), state::STANDBY); // (4) even when gps is fixed, before start command is received, state does not change
    assertLocationsEqual(sm.GetHomeCoords(), Msg::GNSS{10.0, 10.0, 0.0}); // (5) home coords still update


    sm.Input(Msg::StateMachineInput{Msg::StateMachineInput::START});
    assertLocationsEqual(sm.GetHomeCoords(), Msg::GNSS{10.0, 10.0, 0.0}); //(6) home coords should still be saved
    assertEqual(sm.GetState(), state::WAYPOINT); //(7) sm.Input() shoukd change the state

    sm.DecideState();
    sm.ExecuteState();
    assertEqual(sm.GetState(), state::RETURN); //(8) there are 0 waypoints in the itinerary; so the next state should be return.


}