# FSM (Finite State Machine)

The FSM( Fintite State Machine) is a simple mathematical model of computation. It is an abstract machine that can be in one of the finite number of state at given time.
For the distillation project a system with many phases should be implemented. It has been shown that the FSM is a clever soluton. 
The state and the condition for the transition to the next state is given in the following graph.

![FiniteStateMachine](/FSM_distillation_state.png)

If the distillation programm starts it begins with the initial state. The first state calls ControllerInit. In this state controller do nothing for 60 seconds. 
This time was necessary because while the system identification the initial state of the system should be made known.

Afterwards follows the heating phase with PI Controller. 
To measure the real boiling temperature of wine the initial reference temperature is set as 92°C. A little bit lower than the value in the literature. 
The controller increases the reference value by 0.5°C if the initial reference is reached. Then the controller waits 16 seconds and measures the head temperature. 
If the current head temperature doesn't change with the initial head temperature controller increases the reference temperature by 0.5°C again.  
Else controller changes the phase from heating to boiling. PI Controller will switched with a physical model based controller (PMC). More information about the physical model based controller read (LINK).
Distillation process is finished if the head temperature is over 92°C. Heater will turned off and the system gets the end state.

