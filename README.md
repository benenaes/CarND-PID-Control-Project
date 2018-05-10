# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Reflection

### PID parameters

The steering angle $s_a$ is determined by the cross-track error ($cte$). 
$$
s_a = - cte * τ_p - Δcte * τ_d - Σ cte * τ_i
$$

- The proportional parameter $τ_p$ will make sure the steering angle is adapted immediately when the car is leaving the middle of the track (the absolute cross-track error or absolute $cte$ is getting larger). If $τ_p$ is small, then the steering angle will be adapted too slowly and thus will leave the track in larger turns. If $τ_p$ is large, then the car will react fast to large $cte$ values (thus resulting in larger steering values), but it will start to oscillate around the middle of the track, which can also result in the car leaving the track.
- The differential parameter $τ_d$ works on changes (delta's) in the steering angle. If the car is moving quickly towards the outside of the track, it will make the steering angle counteract even more. If the car is moving towards the middle of the track, it will dampen the steering angle oscillations and thus make the driving experience smoother. If the parameter is too large, it will counteract the effect of $τ_p$ too much. If the parameter is too small, then the oscillations will not be dampened enough.
- The integral parameter $τ_i$ works on the sum of all *cte* values and thus on persistent errors in the steering angle. If the parameter is too small, then persistent errors like the steering wheel bias, will not be corrected. If the parameter is too large, then the car will tend to drift towards one side of the track     

### Hyperparameter search

I used the Twiddle hyperparameter search algorithm to find good values for the $τ_p$, $τ_i$, $τ_d$ parameters. 

Instead of starting at (0, 0, 0), I started at (0.5, 0.005, 0.5), because:

- (0,0,0) would mean no steering would take place
- the initial $cte$ is 0.7598, while the car is not that far from the middle of the track and steering angle can only range from [-1, 1]: this means that $τ_p$ needs to be positive (otherwise the car would be pushed towards the outside of the track) and also not too large (otherwise the steering angle would immediately reach absolute values larger than 1).  $τ_d$ usually needs to be larger because it works on derivatives (which are quite smaller than the proportional value). Since the error derivatives are divided by time delta's (the simulator provides updates at around 40 Hz, so usually the time delta's are around 0.025 seconds), I assumed that a good initial value for $τ_d$ would be more or less the same as $τ_p$
- having a large $τ_i$ doesn't make sense as there is likely not such a big steering wheel bias 

The program can run in "optimization mode", which means a hyperparameter search is performed. The PID controller is initialized with the initial $τ_p$, $τ_i$, $τ_d$ parameters from the *TwiddleOptimizer* class. Each time a certain amount of $cte$ updates $n_u$ was reached, the program resets the simulator, updates the *TwiddleOptimizer* and takes the next step in the hyperparameter search.  

*twiddle.cpp* takes care of the hyperparameter search. 

Initially, I left the throttle value at 0.3 and did a hyperparameter search by gradually increasing the amount of *cte* updates $n_u$ to update the *TwiddleOptimizer*. The gradual increase in $n_u$ has the advantage that we can find good guesses for the $τ_p$, $τ_i$, $τ_d$ parameters much faster and that they can be improved by making the part of the track they are trained on, larger (larger $n_u$).

### Result

I added a video where the steering angle of the car is automatically determined by the PID controller. The maximum speed of the car is around 70 mph and doesn't leave the track. 

[Video here](./pid_controlled_track.mp4)

I let the PID controller control the steering angle of the car for a couple of laps. The car stays on track until there is a large time delta in between track error updates (> 0.1 seconds) while it is taking one of the larger turns. I added some debugging data in the code to see when the time delta exceeds 0.08 seconds. I saw these three events (leaving track, large turn and large time delta) occur on multiple trials.

### Improvements

- It would make sense to tune the $τ_i$ parameter separately on straight parts of the track. If the car would have a steering bias, then this could be covered up by the turns in the track.
- The calculation of integral error could be limited in time (e.g. only the last *n* points) and could also take the time delta's into account.
- Also, it would make a lot of sense to brake or at least control the speed when the track error is becoming large. A PID controller for the throttle would be a good idea. 
- The Twiddle optimizer could refuse the sum of squared track errors and re-run on the same parameter set if the time delta in between certain *cte* updates exceeded a certain value (e.g. 0.1 seconds). There is a chance that the car left the track, because of this large time delta and not because of the current parameter set.