/**
 * @author Ryan Benasutti, WPI
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "okapi/odometry/odometry.hpp"
#include "api.h"
#include "okapi/util/mathUtil.hpp"
#include <cmath>

namespace okapi {
OdomState::OdomState() = default;

OdomState::OdomState(const double ix, const double iy, const double itheta)
  : x(ix), y(iy), theta(itheta) {
}

OdomState::~OdomState() = default;

OdometryArgs::OdometryArgs(std::shared_ptr<SkidSteerModel> imodel, const double iscale,
                           const double iturnScale)
  : model(imodel), scale(iscale), turnScale(iturnScale) {
}

OdometryArgs::~OdometryArgs() = default;

Odometry::Odometry(std::shared_ptr<SkidSteerModel> imodel, const double iscale,
                   const double iturnScale)
  : model(imodel), scale(iscale), turnScale(iturnScale), lastTicks{0, 0}, mm(0) {
}

Odometry::Odometry(const OdometryArgs &iparams)
  : model(iparams.model), scale(iparams.scale), turnScale(iparams.turnScale) {
}

Odometry::~Odometry() = default;

void Odometry::setScales(const double iscale, const double iturnScale) {
  scale = iscale;
  turnScale = iturnScale;
}

void Odometry::loop() {
  std::uint32_t now = pros::millis();
  std::valarray<std::int32_t> newTicks{0, 0}, tickDiff{0, 0};

  while (true) {
    newTicks = model->getSensorVals();
    tickDiff = newTicks - lastTicks;
    lastTicks = newTicks;

    mm = (static_cast<double>(tickDiff[1] + tickDiff[0]) / 2.0) * scale;

    state.theta += (tickDiff[1] - tickDiff[0]) * turnScale;
    if (state.theta > 180)
      state.theta -= 360;
    else if (state.theta < -180)
      state.theta += 360;

    state.x += mm * std::cos(state.theta * degreeToRadian);
    state.y += mm * std::sin(state.theta * degreeToRadian);

    pros::c::task_delay_until(&now, 10);
  }
}

void Odometry::trampoline(void *context) {
  static_cast<Odometry *>(context)->loop();
}

OdomState Odometry::getState() const {
  return state;
}

void Odometry::setState(const OdomState &istate) {
  state = istate;
  lastTicks[0] = 0;
  lastTicks[1] = 0;
}

} // namespace okapi