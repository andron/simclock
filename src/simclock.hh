// -*- mode:c++; indent-tabs-mode:nil; -*-

/*
  Copyright (c) 2013 Anders Ro, anders.ronnbrant@gmail.com

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __simclock_hh__
#define __simclock_hh__

#include <atomic>
#include <chrono>
#include <iostream>

class SimulationClock
{
  // Definition of time - Type for holding amount of time to represent
  // and the resolution of the clock.
  typedef int64_t clock_storage;
  typedef std::chrono::duration<clock_storage, std::ratio<1,64>> simulation_duration;

 public:

  typedef simulation_duration                            duration;
  typedef duration::period                               period;
  typedef duration::rep                                  rep;
  typedef std::chrono::time_point<SimulationClock>       time_point;
  typedef std::chrono::duration<double, std::ratio<1,1>> dseconds;

  static constexpr bool const is_steady = false;
  static constexpr bool const is_system = false;

  SimulationClock();
  SimulationClock(SimulationClock const&);
  SimulationClock& operator=(SimulationClock const&);

  time_point now() const   noexcept;
  void       next()        noexcept;
  void       step(double)  noexcept;
  void       step(int32_t) noexcept;
  void       step(int64_t) noexcept;

  duration   seconds(int32_t)      const noexcept;
  duration   milliseconds(int32_t) const noexcept;
  duration   microseconds(int32_t) const noexcept;

  template<typename TimeType>
  void step(TimeType ts) noexcept
  {
    current_time += std::chrono::duration_cast<duration>(ts).count();
  }

  template<typename TimeType>
  static TimeType as(time_point tp) noexcept
  {
    return as(tp, Specialize<TimeType>());
  }

  template<typename TimeType>
  static TimeType as(SimulationClock const& clock) noexcept
  {
    return as(clock.now(), Specialize<TimeType>());
  }

  friend time_point operator+(time_point, double);
  friend std::ostream& operator<<(std::ostream&, time_point);

 private:

  std::atomic<int64_t> current_time;
  time_point const epoch = time_point();

  // Struct that carries the datatype we want to use for specialization. It is
  // required in the generic case as well, as can be seen in the two methods
  // below.
  template<typename T> struct Specialize { typedef T type; };

  template<typename TimeType>
  static TimeType as(time_point t, Specialize<TimeType>)
  {
    return std::chrono::duration_cast<TimeType>(t.time_since_epoch());
  }

  static double as(time_point t, Specialize<double>)
  {
    return std::chrono::duration_cast<dseconds>(t.time_since_epoch()).count();
  }
};

#endif // __simclock_hh__
