# Breakout

W Kavanagh & N Merchant. Summer 2024 

## controls

A/D to move the paddle right and left.
P to pause.

## Powerups

big/small paddle (blue)
fast/slow ball (fancy yellow)
fire ball (green)

# Tasklist

## Suggested fixes

* Fix the compiler issues in the code

## Suggested tasks

* Implement mouse input for pad
* Improved VFX (ball trail, paddle hit response, brick particle destruction)
* Better UI (progress bar rather than timer for countdown).
* GameLoop
* Better ball physics (Box2D)
* Leaderboards
* More ball types (e.g., multiball, sticky ball [where you shoot the ball from the paddle every time], tiny ball, big ball, brick-tracking ball)
* Sounds with increasing tone between bounces.
* Implement commentary with calls to an LLM such as LLama

# Time Details and Changelist
- Fixed compiler errors (10 mins)
- Implemented bounce SFX, including rising pitch for each hit (resetting when the ball hits the paddle again) and a fixed array-based queue to prevent each sound effect from overwriting the previous one, while keeping memory consumption low. (~1 hr)
- Refactored this sound effect implementation into a more generic and flexible Sound Manager. (45 mins)
- Used this system to implement death and pausing sound effects, extended it to allow gameplay sounds to be paused during the pause screen (30 mins)



