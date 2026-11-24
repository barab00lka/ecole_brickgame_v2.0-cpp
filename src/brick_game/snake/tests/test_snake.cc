#include <gtest/gtest.h>

#include <cstdio>

#include <save.h>
#include "../include/snake.h"

using namespace ec;

namespace {
const char *kTestFile = "/tmp/ec_test_hiscore.dat";
}

TEST(SnakeCtxTest, NewSnakeStartsCorrtly) {
  SnakeCtx game;
  EXPECT_EQ(game.get_score(), 0);  // 4 cells - 4 = 0
  EXPECT_EQ(game.get_current_state(), startState);
  EXPECT_EQ(game.get_curdir(), SnakeCtx::dirL);  // or whatever default
}

TEST(SnakeCtxTest, MoveUpdatesHead) {
  SnakeCtx game;
  game.DispatchSignal(Start);  // to start the game
  auto oldHead = game.get_head_pos();
  game.DispatchSignal(::Action);  // simulate "move" action
  auto newHead = game.get_head_pos();
  auto expted = SnakeCtx::dirL + oldHead;
  EXPECT_EQ(newHead, expted);
}

TEST(SnakeCtxTest, CannotReverseDirection) {
  SnakeCtx game;
  game.DispatchSignal(Start);  // to start the game
  game.DispatchSignal(Right);  // try to go right from left (default)
  game.DispatchSignal(Action);
  EXPECT_EQ(game.get_curdir(), SnakeCtx::dirL);  // still left
}

TEST(SnakeCtxTest, WallCollisionTriggersGameOver) {
  SnakeCtx game;
  game.DispatchSignal(Start);  // to start the game
  // Move snake to the right edge (FIELD_COLS - 1) then try to move again
  for (int i = 0; i < FIELD_COLS - 1; ++i) {
    game.DispatchSignal(Action);
  }
  // Next move should collide with wall
  game.DispatchSignal(Action);
  EXPECT_EQ(game.get_current_state(), gameOverState);
}

TEST(SnakeCtxTest, PauseTogglesState) {
  SnakeCtx game;
  game.DispatchSignal(Start);  // to start the game
  game.DispatchSignal(Pause);
  EXPECT_EQ(game.get_current_state(), pausedState);
  game.DispatchSignal(Pause);
  EXPECT_EQ(game.get_current_state(), movingState);
}

TEST(SnakeCtxTest, TickerIncrements) {
  SnakeCtx game;
  game.DispatchSignal(Start);  // to start the game
  int old = game.get_ticker();
  game.tick();
  EXPECT_EQ(game.get_ticker(), old + 1);
}
// --- fsm.cc / snake.cc gaps ---

TEST(SnakeCtxTest, ChangeDirUpValid) {
  SnakeCtx game;
  game.DispatchSignal(Start);
  game.DispatchSignal(Up);
  game.DispatchSignal(Action);
  EXPECT_EQ(game.get_curdir(), SnakeCtx::dirU);
}

TEST(SnakeCtxTest, SecondDirChangeSameTickIgnored) {
  SnakeCtx game;
  game.DispatchSignal(Start);
  game.DispatchSignal(Up);    // first change this tick, accepted
  game.DispatchSignal(Down);  // second change same tick, must be ignored
  game.DispatchSignal(Action);
  EXPECT_EQ(game.get_curdir(), SnakeCtx::dirU);
}

TEST(SnakeCtxTest, TickerResetsOnMove) {
  SnakeCtx game;
  game.DispatchSignal(Start);
  game.tick();
  game.tick();
  ASSERT_GT(game.get_ticker(), 0);
  game.DispatchSignal(Action);
  EXPECT_EQ(game.get_ticker(), 0);
}

TEST(SnakeCtxTest, PauseBlocksMovementSignals) {
  SnakeCtx game;
  game.DispatchSignal(Start);
  auto head = game.get_head_pos();
  game.DispatchSignal(Pause);
  game.DispatchSignal(Action);
  game.DispatchSignal(Left);
  EXPECT_EQ(game.get_current_state(), pausedState);
  EXPECT_EQ(game.get_head_pos(), head);
}

TEST(SnakeCtxTest, GameOverIgnoresNonStartSignals) {
  SnakeCtx game;
  game.DispatchSignal(Start);
  for (int i = 0; i <= FIELD_COLS; ++i) game.DispatchSignal(Action);
  ASSERT_EQ(game.get_current_state(), gameOverState);
  game.DispatchSignal(Pause);
  game.DispatchSignal(Action);
  EXPECT_EQ(game.get_current_state(), gameOverState);
}

TEST(SnakeCtxTest, GameOverStartRestartsFresh) {
  SnakeCtx game;
  game.DispatchSignal(Start);
  for (int i = 0; i <= FIELD_COLS; ++i) game.DispatchSignal(Action);
  ASSERT_EQ(game.get_current_state(), gameOverState);
  game.DispatchSignal(Start);
  EXPECT_EQ(game.get_current_state(), startState);
  game.DispatchSignal(Start);
  EXPECT_EQ(game.get_current_state(), movingState);
  EXPECT_EQ(game.get_score(), 0);
}

TEST(SnakeCtxTest, TerminateBeforeStartDoesNotCrash) {
  SnakeCtx game;
  EXPECT_NO_THROW(game.DispatchSignal(Terminate));
  EXPECT_EQ(game.get_current_state(), startState);
}

// --- scoring.cc gaps ---

TEST(ScoringTest, FetchMissingFileReturnsZero) {
  std::remove(kTestFile);
  EXPECT_EQ(fetch_high_score(kTestFile), 0);
}

TEST(ScoringTest, SaveZeroSkipsWrite) {
  std::remove(kTestFile);
  save_new_high_score(0, kTestFile);
  EXPECT_EQ(fetch_high_score(kTestFile), 0);
}

TEST(ScoringTest, SaveAndFetchRoundTrip) {
  std::remove(kTestFile);
  save_new_high_score(42, kTestFile);
  EXPECT_EQ(fetch_high_score(kTestFile), 42);
}

TEST(ScoringTest, FetchReturnsLastOfMultipleSaves) {
  std::remove(kTestFile);
  save_new_high_score(10, kTestFile);
  save_new_high_score(99, kTestFile);
  EXPECT_EQ(fetch_high_score(kTestFile), 99);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
