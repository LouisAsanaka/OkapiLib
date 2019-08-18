/**
 * @author Ryan Benasutti, WPI
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "okapi/api/odometry/odometry.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/api/util/mathUtil.hpp"
#include <cmath>

namespace okapi {
Odometry::Odometry(const TimeUtil &itimeUtil,
                   const std::shared_ptr<ReadOnlyChassisModel> &imodel,
                   const ChassisScales &ichassisScales,
                   const QSpeed &iwheelVelDelta,
                   const std::shared_ptr<Logger> &ilogger)
  : logger(ilogger),
    rate(itimeUtil.getRate()),
    timer(itimeUtil.getTimer()),
    model(imodel),
    chassisScales(ichassisScales),
    wheelVelDelta(iwheelVelDelta) {
}

void Odometry::setScales(const ChassisScales &ichassisScales) {
  chassisScales = ichassisScales;
}

void Odometry::step() {
  const auto deltaT = timer->getDt();

  if (deltaT.getValue() != 0) {
    newTicks = model->getSensorVals();
    tickDiff = newTicks - lastTicks;
    lastTicks = newTicks;

    const auto newState = odomMathStep(tickDiff, deltaT);

    state.x += newState.x;
    state.y += newState.y;
    state.theta += newState.theta;
  }
}

OdomState Odometry::odomMathStep(const std::valarray<std::int32_t> &itickDiff, const QTime &) {
  const double deltaL = itickDiff[0] / chassisScales.straight;
  const double deltaR = itickDiff[1] / chassisScales.straight;

  double deltaTheta = (deltaL - deltaR) / chassisScales.wheelTrack.convert(meter);
  double localOffX, localOffY;

  if (deltaTheta != 0) {
    localOffX = 2 * sin(deltaTheta / 2) * chassisScales.middleWheelDistance.convert(meter);
    localOffY =
      2 * sin(deltaTheta / 2) * (deltaR / deltaTheta + chassisScales.wheelTrack.convert(meter) / 2);
  } else {
    localOffX = 0;
    localOffY = deltaR;
  }

  double avgA = state.theta.convert(radian) + (deltaTheta / 2);

  double polarR = sqrt(localOffX * localOffX + localOffY * localOffY);
  double polarA = atan2(localOffY, localOffX) - avgA;

  double dX = sin(polarA) * polarR;
  double dY = cos(polarA) * polarR;

  if (isnan(dX)) {
    dX = 0;
  }

  if (isnan(dY)) {
    dY = 0;
  }

  if (isnan(deltaTheta)) {
    deltaTheta = 0;
  }

  return OdomState{dX * meter, dY * meter, deltaTheta * radian};
}

OdomState Odometry::getState(const StateMode &imode) const {
  if (imode == StateMode::FRAME_TRANSFORMATION) {
    return state;
  } else {
    return OdomState{state.y, state.x, state.theta};
  }
}

void Odometry::setState(const OdomState &istate, const StateMode &imode) {
  LOG_DEBUG("State set to: " + istate.str());
  if (imode == StateMode::FRAME_TRANSFORMATION) {
    state = istate;
  } else {
    state = OdomState{istate.y, istate.x, istate.theta};
  }
}

bool OdomState::operator==(const OdomState &rhs) const {
  return x == rhs.x && y == rhs.y && theta == rhs.theta;
}

bool OdomState::operator!=(const OdomState &rhs) const {
  return !(rhs == *this);
}

std::string OdomState::str() const {
  std::ostringstream os;
  os << "OdomState(x=" << std::to_string(x.convert(meter))
     << "m, y=" << std::to_string(y.convert(meter))
     << "m, theta=" << std::to_string(theta.convert(degree)) << "deg)";
  return os.str();
}
} // namespace okapi
