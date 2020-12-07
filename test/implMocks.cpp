/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "test/tests/api/implMocks.hpp"
#include "okapi/api/control/util/settledUtil.hpp"
#include "okapi/api/device/motor/abstractMotor.hpp"
#include "okapi/api/util/abstractTimer.hpp"
#include <chrono>
#include <memory>

namespace okapi {
double MockContinuousRotarySensor::controllerGet() {
  return value;
}

int32_t MockContinuousRotarySensor::reset() {
  value = 0;
  return 0;
}

double MockContinuousRotarySensor::get() const {
  return value;
}

MockMotor::MockMotor() : encoder(std::make_shared<MockContinuousRotarySensor>()) {
}

void MockMotor::controllerSet(const double ivalue) {
  moveVelocity((int16_t)ivalue);
}

int32_t MockMotor::moveAbsolute(const double iposition, const std::int32_t ivelocity) {
  lastPosition = (int16_t)iposition;
  lastProfiledMaxVelocity = ivelocity;
  return 0;
}

int32_t MockMotor::moveRelative(const double iposition, const std::int32_t ivelocity) {
  lastPosition += static_cast<std::int16_t>(iposition);
  lastProfiledMaxVelocity = ivelocity;
  return 0;
}

double MockMotor::getTargetPosition() {
  return 0;
}

double MockMotor::getPosition() {
  return encoder->get();
}

int32_t MockMotor::getTargetVelocity() {
  return 0;
}

double MockMotor::getActualVelocity() {
  return 0;
}

int32_t MockMotor::tarePosition() {
  return 0;
}

int32_t MockMotor::setBrakeMode(const AbstractMotor::brakeMode imode) {
  brakeMode = imode;
  return 0;
}

int32_t MockMotor::setCurrentLimit(const std::int32_t) {
  return 0;
}

int32_t MockMotor::setEncoderUnits(const AbstractMotor::encoderUnits iunits) {
  encoderUnits = iunits;
  return 0;
}

int32_t MockMotor::setGearing(const AbstractMotor::gearset igearset) {
  gearset = igearset;
  return 0;
}

int32_t MockMotor::setReversed(const bool) {
  return 0;
}

int32_t MockMotor::setVoltageLimit(const std::int32_t) {
  return 0;
}

std::shared_ptr<ContinuousRotarySensor> MockMotor::getEncoder() {
  return encoder;
}

std::shared_ptr<MockContinuousRotarySensor> MockMotor::getMockEncoder() {
  return encoder;
}

std::int32_t MockMotor::moveVelocity(const std::int16_t ivelocity) {
  lastVelocity = ivelocity;
  if (ivelocity > maxVelocity) {
    maxVelocity = ivelocity;
  }
  return 1;
}

std::int32_t MockMotor::moveVoltage(const std::int16_t ivoltage) {
  lastVoltage = ivoltage;
  return 1;
}

int32_t MockMotor::getCurrentDraw() {
  return 0;
}

int32_t MockMotor::getDirection() {
  return 0;
}

double MockMotor::getEfficiency() {
  return 0;
}

int32_t MockMotor::isOverCurrent() {
  return 0;
}

int32_t MockMotor::isOverTemp() {
  return 0;
}

int32_t MockMotor::isStopped() {
  return 0;
}

int32_t MockMotor::getZeroPositionFlag() {
  return 0;
}

uint32_t MockMotor::getFaults() {
  return 0;
}

uint32_t MockMotor::getFlags() {
  return 0;
}

int32_t MockMotor::getRawPosition(std::uint32_t *) {
  return static_cast<int32_t>(encoder->get());
}

double MockMotor::getPower() {
  return 0;
}

double MockMotor::getTemperature() {
  return 0;
}

double MockMotor::getTorque() {
  return 0;
}

int32_t MockMotor::getVoltage() {
  return 0;
}

int32_t MockMotor::modifyProfiledVelocity(std::int32_t) {
  return 0;
}

AbstractMotor::brakeMode MockMotor::getBrakeMode() {
  return brakeMode;
}

int32_t MockMotor::getCurrentLimit() {
  return 2500;
}

AbstractMotor::encoderUnits MockMotor::getEncoderUnits() {
  return encoderUnits;
}

AbstractMotor::gearset MockMotor::getGearing() {
  return gearset;
}

MockTimer::MockTimer() : AbstractTimer(millis()) {
}

QTime MockTimer::millis() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
           std::chrono::high_resolution_clock::now() - epoch)
           .count() *
         millisecond;
}

ConstantMockTimer::ConstantMockTimer(const QTime idt) : AbstractTimer(0_ms), dtToReturn(idt) {
}

QTime ConstantMockTimer::millis() const {
  return 0_ms;
}

QTime ConstantMockTimer::getDt() {
  return dtToReturn;
}

QTime ConstantMockTimer::readDt() const {
  return dtToReturn;
}

QTime ConstantMockTimer::getStartingTime() const {
  return 0_ms;
}

QTime ConstantMockTimer::getDtFromStart() const {
  return dtToReturn;
}

void ConstantMockTimer::placeMark() {
}

void ConstantMockTimer::placeHardMark() {
}

QTime ConstantMockTimer::clearHardMark() {
  return 0_ms;
}

QTime ConstantMockTimer::getDtFromMark() const {
  return dtToReturn;
}

QTime ConstantMockTimer::getDtFromHardMark() const {
  return dtToReturn;
}

bool ConstantMockTimer::repeat(QTime) {
  return false;
}

bool ConstantMockTimer::repeat(QFrequency) {
  return false;
}

QTime ConstantMockTimer::clearMark() {
  return 0_ms;
}

MockRate::MockRate() = default;

void MockRate::delay(QFrequency ihz) {
  std::this_thread::sleep_for(std::chrono::milliseconds(1000 / static_cast<int>(ihz.convert(Hz))));
}

void MockRate::delayUntil(QTime itime) {
  delayUntil(static_cast<uint32_t>(itime.convert(millisecond)));
}

void MockRate::delayUntil(uint32_t ims) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ims));
}

std::unique_ptr<SettledUtil> createSettledUtilPtr(const double iatTargetError,
                                                  const double iatTargetDerivative,
                                                  const QTime iatTargetTime) {
  return std::make_unique<SettledUtil>(
    std::make_unique<MockTimer>(), iatTargetError, iatTargetDerivative, iatTargetTime);
}

TimeUtil createTimeUtil() {
  return TimeUtil(
    Supplier<std::unique_ptr<AbstractTimer>>([]() { return std::make_unique<MockTimer>(); }),
    Supplier<std::unique_ptr<AbstractRate>>([]() { return std::make_unique<MockRate>(); }),
    Supplier<std::unique_ptr<SettledUtil>>([]() { return createSettledUtilPtr(); }));
}

TimeUtil createConstantTimeUtil(const QTime idt) {
  return TimeUtil(
    Supplier<std::unique_ptr<AbstractTimer>>(
      [=]() { return std::make_unique<ConstantMockTimer>(idt); }),
    Supplier<std::unique_ptr<AbstractRate>>([]() { return std::make_unique<MockRate>(); }),
    Supplier<std::unique_ptr<SettledUtil>>([]() { return createSettledUtilPtr(); }));
}

TimeUtil createTimeUtil(const Supplier<std::unique_ptr<AbstractTimer>> &itimerSupplier) {
  return TimeUtil(
    itimerSupplier,
    Supplier<std::unique_ptr<AbstractRate>>([]() { return std::make_unique<MockRate>(); }),
    Supplier<std::unique_ptr<SettledUtil>>([]() { return createSettledUtilPtr(); }));
}

TimeUtil createTimeUtil(const Supplier<std::unique_ptr<SettledUtil>> &isettledUtilSupplier) {
  return TimeUtil(
    Supplier<std::unique_ptr<AbstractTimer>>([]() { return std::make_unique<MockTimer>(); }),
    Supplier<std::unique_ptr<AbstractRate>>([]() { return std::make_unique<MockRate>(); }),
    isettledUtilSupplier);
}

SimulatedSystem::SimulatedSystem(FlywheelSimulator &isimulator) : simulator(isimulator) {
}

SimulatedSystem::~SimulatedSystem() {
  dtorCalled.store(true, std::memory_order_release);
}

double SimulatedSystem::controllerGet() {
  return simulator.getAngle();
}

void SimulatedSystem::controllerSet(double ivalue) {
  simulator.setTorque(ivalue);
}

void SimulatedSystem::step() {
  while (!dtorCalled.load(std::memory_order_acquire)) {
    simulator.step();
    rate.delayUntil(10_ms);
  }
}

void SimulatedSystem::trampoline(void *system) {
  if (system) {
    static_cast<SimulatedSystem *>(system)->step();
  }
}

void SimulatedSystem::startThread() {
  thread = std::thread(trampoline, this);
}

void SimulatedSystem::join() {
  dtorCalled.store(true, std::memory_order_release);
  thread.join();
}

MockAsyncPosIntegratedController::MockAsyncPosIntegratedController()
  : AsyncPosIntegratedController(std::make_shared<MockMotor>(),
                                 AbstractMotor::gearset::green,
                                 200,
                                 createTimeUtil()) {
}

MockAsyncPosIntegratedController::MockAsyncPosIntegratedController(const TimeUtil &itimeUtil)
  : AsyncPosIntegratedController(std::make_shared<MockMotor>(),
                                 AbstractMotor::gearset::green,
                                 200,
                                 itimeUtil) {
}

bool MockAsyncPosIntegratedController::isSettled() {
  switch (isSettledOverride) {
  case IsSettledOverride::none:
    return AsyncPosIntegratedController::isSettled();
  case IsSettledOverride::alwaysSettled:
    return true;
  case IsSettledOverride::neverSettled:
    return false;
  }
}

MockAsyncVelIntegratedController::MockAsyncVelIntegratedController()
  : AsyncVelIntegratedController(std::make_shared<MockMotor>(),
                                 AbstractMotor::gearset::green,
                                 200,
                                 createTimeUtil()) {
}

bool MockAsyncVelIntegratedController::isSettled() {
  switch (isSettledOverride) {
  case IsSettledOverride::none:
    return AsyncVelIntegratedController::isSettled();
  case IsSettledOverride::alwaysSettled:
    return true;
  case IsSettledOverride::neverSettled:
    return false;
  }
}

void MockAsyncVelIntegratedController::setTarget(const double itarget) {
  lastTarget = itarget;

  if (itarget > maxTarget) {
    maxTarget = itarget;
  }

  AsyncVelIntegratedController::setTarget(itarget);
}

void MockAsyncVelIntegratedController::controllerSet(const double ivalue) {
  lastControllerOutputSet = ivalue;

  if (ivalue > maxControllerOutputSet) {
    maxControllerOutputSet = ivalue;
  }

  AsyncVelIntegratedController::controllerSet(ivalue);
}

MockIterativeController::MockIterativeController()
  : IterativePosPIDController(0, 0, 0, 0, createTimeUtil()) {
}

MockIterativeController::MockIterativeController(const double ikP)
  : IterativePosPIDController(ikP, 0, 0, 0, createTimeUtil()) {
}

bool MockIterativeController::isSettled() {
  switch (isSettledOverride) {
  case IsSettledOverride::none:
    return IterativePosPIDController::isSettled();
  case IsSettledOverride::alwaysSettled:
    return true;
  case IsSettledOverride::neverSettled:
    return false;
  }
}

void assertMotorsHaveBeenStopped(MockMotor *leftMotor, MockMotor *rightMotor) {
  
}

void assertMotorsGearsetEquals(const AbstractMotor::gearset expected,
                               const std::initializer_list<MockMotor> &motors) {
  
}

void assertMotorsBrakeModeEquals(const AbstractMotor::brakeMode expected,
                                 const std::initializer_list<MockMotor> &motors) {
  
}

void assertMotorsEncoderUnitsEquals(const AbstractMotor::encoderUnits expected,
                                    const std::initializer_list<MockMotor> &motors) {
  
}

void assertAsyncControllerFollowsDisableLifecycle(AsyncController<double, double> &controller,
                                                  std::int16_t &domainValue,
                                                  std::int16_t &voltageValue,
                                                  int expectedOutput) {
  
}

void assertIterativeControllerFollowsDisableLifecycle(
  IterativeController<double, double> &controller) {
  
}

void assertControllerFollowsTargetLifecycle(ClosedLoopController<double, double> &controller) {
  
}

void assertIterativeControllerScalesControllerSetTargets(
  IterativeController<double, double> &controller) {
  
}

void assertAsyncWrapperScalesControllerSetTargets(AsyncWrapper<double, double> &controller) {
  
}

ThreadedMockMotor::ThreadedMockMotor() : encoder(std::make_shared<MockContinuousRotarySensor>()) {
}

void ThreadedMockMotor::controllerSet(double ivalue) {
  moveVelocity(static_cast<int16_t>(ivalue * toUnderlyingType(gearset)));
}

int32_t ThreadedMockMotor::moveAbsolute(double iposition, std::int32_t ivelocity) {
  targetPosition = iposition;
  targetProfiledVelocity = ivelocity;
  mode = position;
  return 1;
}

int32_t ThreadedMockMotor::moveRelative(double iposition, std::int32_t ivelocity) {
  targetPosition += iposition;
  targetProfiledVelocity = ivelocity;
  mode = position;
  return 1;
}

int32_t ThreadedMockMotor::moveVelocity(std::int16_t ivelocity) {
  targetVelocity = ivelocity;
  mode = velocity;
  return 1;
}

int32_t ThreadedMockMotor::moveVoltage(std::int16_t ivoltage) {
  setVoltage = ivoltage;
  mode = voltage;
  return 1;
}

int32_t ThreadedMockMotor::modifyProfiledVelocity(std::int32_t ivelocity) {
  targetProfiledVelocity = ivelocity;
  return 1;
}

double ThreadedMockMotor::getTargetPosition() {
  return targetPosition;
}

double ThreadedMockMotor::getPosition() {
  return encoder->get();
}

int32_t ThreadedMockMotor::tarePosition() {
  encoder->reset();
  return 1;
}

int32_t ThreadedMockMotor::getTargetVelocity() {
  return targetVelocity;
}

double ThreadedMockMotor::getActualVelocity() {
  return actualVelocity;
}

int32_t ThreadedMockMotor::getCurrentDraw() {
  return 0;
}

int32_t ThreadedMockMotor::getDirection() {
  return 0;
}

double ThreadedMockMotor::getEfficiency() {
  return 0;
}

int32_t ThreadedMockMotor::isOverCurrent() {
  return 0;
}

int32_t ThreadedMockMotor::isOverTemp() {
  return 0;
}

int32_t ThreadedMockMotor::isStopped() {
  return 0;
}

int32_t ThreadedMockMotor::getZeroPositionFlag() {
  return 0;
}

uint32_t ThreadedMockMotor::getFaults() {
  return 0;
}

uint32_t ThreadedMockMotor::getFlags() {
  return 0;
}

int32_t ThreadedMockMotor::getRawPosition(std::uint32_t *) {
  return 0;
}

double ThreadedMockMotor::getPower() {
  return 0;
}

double ThreadedMockMotor::getTemperature() {
  return 0;
}

double ThreadedMockMotor::getTorque() {
  return 0;
}

int32_t ThreadedMockMotor::getVoltage() {
  return 0;
}

int32_t ThreadedMockMotor::setBrakeMode(AbstractMotor::brakeMode imode) {
  brakeMode = imode;
  return 1;
}

AbstractMotor::brakeMode ThreadedMockMotor::getBrakeMode() {
  return brakeMode;
}

int32_t ThreadedMockMotor::setCurrentLimit(std::int32_t ilimit) {
  currentLimit = ilimit;
  return 1;
}

int32_t ThreadedMockMotor::getCurrentLimit() {
  return currentLimit;
}

int32_t ThreadedMockMotor::setEncoderUnits(AbstractMotor::encoderUnits iunits) {
  encoderUnits = iunits;
  return 1;
}

AbstractMotor::encoderUnits ThreadedMockMotor::getEncoderUnits() {
  return encoderUnits;
}

int32_t ThreadedMockMotor::setGearing(AbstractMotor::gearset igearset) {
  gearset = igearset;
  return 1;
}

AbstractMotor::gearset ThreadedMockMotor::getGearing() {
  return gearset;
}

int32_t ThreadedMockMotor::setReversed(bool ireverse) {
  if (ireverse) {
    reverse = -1;
  } else {
    reverse = 1;
  }

  return 1;
}

int32_t ThreadedMockMotor::setVoltageLimit(std::int32_t ilimit) {
  voltageLimit = ilimit;
  return 1;
}

std::shared_ptr<ContinuousRotarySensor> ThreadedMockMotor::getEncoder() {
  return encoder;
}

void ThreadedMockMotor::startThread() {
  thread = std::thread(&ThreadedMockMotor::threadFunc, this);
}

void ThreadedMockMotor::stopThread() {
  threadShouldStop = true;
}

void ThreadedMockMotor::threadFunc() {
  while (!threadShouldStop) {
    switch (mode) {
    case position: {
      // If the encoder value was written to from outside this thread
      if (static_cast<std::int32_t>(actualPosition) != encoder->value) {
        actualPosition = encoder->value;
      }

      actualVelocity = targetProfiledVelocity;
      actualPosition += actualVelocity * (dt / 1000.0);

      if (actualPosition > targetPosition) {
        actualPosition = targetPosition;
      }

      encoder->value = static_cast<std::int32_t>(actualPosition);
      break;
    }

    case velocity: {
      // If the encoder value was written to from outside this thread
      if (static_cast<std::int32_t>(actualPosition) != encoder->value) {
        actualPosition = encoder->value;
      }

      actualVelocity = targetVelocity;
      actualPosition += actualVelocity * (dt / 1000.0);

      encoder->value = static_cast<std::int32_t>(actualPosition);
      break;
    }

    case voltage: {
      break;
    }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(dt));
  }
}

void assertOdomStateEquals(double x, double y, double theta, const OdomState &actual) {
  
}

void assertOdomStateEquals(Odometry *odom, QLength x, QLength y, QAngle theta) {
  
}
} // namespace okapi
