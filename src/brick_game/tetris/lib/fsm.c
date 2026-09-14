#include "../include/fsm.h"

#include "../include/field.h"
#include <scoring.h>
#include <save.h>

/* The order of initialization must be unchanged!!! */
StateHandler disp_table_fsm[StateCount] = {
    on_start_state,    on_spawn_state,  on_moving_state,
    on_clearing_state, on_paused_state, on_gameover_state,
};

void on_start_state(tetrisCtx_t *game, UserAction_t sig, bool hold) {
  (void)hold;

  if (game->current_piece.rotation > 0) {
    // cleans grid with animation
    game->field[FIELD_ROWS - game->current_piece.rotation--] = 0U;
    return;
  }

  switch (sig) {
    case Terminate:
      if (game->stats.high_score > fetch_high_score(TETRIS_SAVE_FN))
        save_new_high_score(game->stats.high_score, TETRIS_SAVE_FN); 
	  break;
	case Start:
	  // TODO:
      // this is only needed on initial game start, redundant on other occasion.
      game->stats.high_score = fetch_high_score(TETRIS_SAVE_FN);

      clear_field(game->field);
  	  game->stats.level = 1;
  	  game->stats.score = 0;
  	  game->ticker = 0;
  	  game->current_piece.position.x = 0;
  	  game->current_piece.position.y = 0;
	  game->state = spawnState;
	  break;
    default:
      break;
  }
}

void on_spawn_state(tetrisCtx_t *game, UserAction_t sig, bool hold) {
  // (void)hold;
  switch (sig) {
    case Terminate:
      if (game->stats.high_score > fetch_high_score(TETRIS_SAVE_FN))
        save_new_high_score(game->stats.high_score, TETRIS_SAVE_FN);
      break;
    default:
      // начальная позиция фигурки при спавне (наверху по центру)
	  if (game->next_piece.position.y != -1)
      	game->next_piece.position.x = (FIELD_COLS / 2) - 2;

      game->next_piece.position.y = -1;

	  // player can steer the piece 
	  // before it's placed on the field
      for (int n = hold ? 2 : 1; n > 0; n--)
        steer_piece(sig, &game->next_piece, game->field);

      if (!can_spawn_piece(&game->next_piece, game->field))
        game->state = gameOverState;
      else {
        if (game->timer != 0)  // Appearance Delay
          --game->timer;
        else {
          game->current_piece = game->next_piece;
          game->next_piece = get_random_piece();
          game->state = movingState;
        }
      }
      break;
  }
}

void on_moving_state(tetrisCtx_t *game, UserAction_t sig, bool hold) {
  switch (sig) {
    case Terminate:
      if (game->stats.high_score > fetch_high_score(TETRIS_SAVE_FN))
        save_new_high_score(game->stats.high_score, TETRIS_SAVE_FN);
      break;
    case Pause:
      game->state = pausedState;
      break;
    case Up:
      rotate_piece_cw(&game->current_piece, game->field);
      break;
    case Left: case Right:
      for (int n = hold ? 2 : 1; n > 0; n--)
        steer_piece(sig, &game->current_piece, game->field);
      break;
	case Down: case Action: case TimerTick:
	  // To prevent automatic drop from overlapping with the manual drop.
	  if (sig != TimerTick) game->ticker = 0; // reset the ticker

	  int ndrops = 1;
	  if (hold) {
		if (sig == Down) ndrops = 3;
		else if (sig == Action) ndrops = 0;
	  }
	  else if (sig == Action) ndrops = FIELD_ROWS;

	  for (bool can_drop = true; can_drop && ndrops > 0; ndrops--) {
	  can_drop = try_drop_down(&game->current_piece, game->field);
	    if (can_drop == false) {
	      attach_piece(&game->current_piece, game->field);
	      game->timer = 10;  // Apperance Delay timer
	      int complete_lines = complete_linecount(game->field);
	      if (complete_lines > 0) {
	        score_update(&game->stats, complete_lines);
	        game->state = clearingState;
	      } else
	        game->state = spawnState;
		  // update high_score 
	      if (game->stats.high_score < game->stats.score)
	        game->stats.high_score = game->stats.score;
	    }
	  }
    default:
      break;
  }
}

void on_paused_state(tetrisCtx_t *game, UserAction_t sig, bool hold) {
  (void)hold;
  switch (sig) {
    case Pause:
      game->state = movingState;
      break;
    case Terminate:
      if (game->stats.high_score > fetch_high_score(TETRIS_SAVE_FN))
        save_new_high_score(game->stats.high_score, TETRIS_SAVE_FN);
      break;
    default:
      break;
  }
}

void on_gameover_state(tetrisCtx_t *game, UserAction_t sig, bool hold) {
  (void)hold;
  switch (sig) {
    case Terminate:
      if (game->stats.high_score > fetch_high_score(TETRIS_SAVE_FN))
        save_new_high_score(game->stats.high_score, TETRIS_SAVE_FN);
      break;
    default:
      if (game->current_piece.rotation ==
          20) {  // Анимация в clearingState довeдёт rotation до 20, только
                 // после этого запускаем заново
        game->state = startState;
        if (game->stats.high_score > fetch_high_score(TETRIS_SAVE_FN))
          save_new_high_score(game->stats.high_score, TETRIS_SAVE_FN);
      } else
        // fills the field with animation
        game->field[FIELD_ROWS - 1 - game->current_piece.rotation++] = ~0U;
      break;
  }
}

void on_clearing_state(tetrisCtx_t *game, UserAction_t sig, bool hold) {
  (void)hold;
  switch (sig) {
    case Terminate:
      if (game->stats.high_score > fetch_high_score(TETRIS_SAVE_FN))
        save_new_high_score(game->stats.high_score, TETRIS_SAVE_FN);
      break;
    default:
      if (clearing_animation(game->field) == 0) { 
        shift_remove_lines(game->field);
        game->state = spawnState;
      }
      break;
  }
}
