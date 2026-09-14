#ifndef FSM_H_
#define FSM_H_

#include <brickgame_api.h>

// Order of initializtion musnt be changed!
enum SnakeState {
  startState,
  movingState,
  pausedState,
  gameOverState,
  stateCount,
};
#endif
