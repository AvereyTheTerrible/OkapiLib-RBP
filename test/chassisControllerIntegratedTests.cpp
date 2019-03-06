/**
 * @author Ryan Benasutti, WPI
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "okapi/api/chassis/controller/chassisControllerIntegrated.hpp"
#include "okapi/api/chassis/model/skidSteerModel.hpp"
#include "test/tests/api/implMocks.hpp"
#include <gtest/gtest.h>

using namespace okapi;

class MockSkidSteerModel : public SkidSteerModel {
  public:
  using SkidSteerModel::maxVelocity;
  using SkidSteerModel::SkidSteerModel;
};

class ChassisControllerIntegratedTest : public ::testing::Test {
  protected:
  void SetUp() override {
    scales = new ChassisScales({6_in, 11_in}, imev5GreenTPR);
    leftMotor = new MockMotor();
    rightMotor = new MockMotor();

    leftController = new MockAsyncPosIntegratedController();
    rightController = new MockAsyncPosIntegratedController();

    model = new MockSkidSteerModel(
      std::unique_ptr<AbstractMotor>(leftMotor), std::unique_ptr<AbstractMotor>(rightMotor), 100);

    controller = new ChassisControllerIntegrated(
      createTimeUtil(),
      std::shared_ptr<ChassisModel>(model),
      std::unique_ptr<AsyncPosIntegratedController>(leftController),
      std::unique_ptr<AsyncPosIntegratedController>(rightController),
      AbstractMotor::gearset::green,
      *scales);
  }

  void TearDown() override {
    delete scales;
    delete controller;
  }

  ChassisScales *scales;
  ChassisController *controller;
  MockMotor *leftMotor;
  MockMotor *rightMotor;
  MockAsyncPosIntegratedController *leftController;
  MockAsyncPosIntegratedController *rightController;
  MockSkidSteerModel *model;
};

TEST_F(ChassisControllerIntegratedTest, GearsetIsCorrect) {
  EXPECT_EQ(AbstractMotor::gearset::green, controller->getGearsetRatioPair().internalGearset);
  EXPECT_EQ(imev5GreenTPR, gearsetToTPR(controller->getGearsetRatioPair().internalGearset));
  EXPECT_EQ(1, controller->getGearsetRatioPair().ratio);
}

TEST_F(ChassisControllerIntegratedTest, MoveDistanceRawUnitsTest) {
  controller->moveDistance(100);

  EXPECT_DOUBLE_EQ(leftController->getTarget(), 100);
  EXPECT_DOUBLE_EQ(rightController->getTarget(), 100);

  EXPECT_TRUE(leftController->isDisabled());
  EXPECT_TRUE(rightController->isDisabled());

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, MoveDistanceUnitsTest) {
  controller->moveDistance(6_in * 1_pi);

  EXPECT_DOUBLE_EQ(leftController->getTarget(),
                   gearsetToTPR(controller->getGearsetRatioPair().internalGearset));
  EXPECT_DOUBLE_EQ(rightController->getTarget(),
                   gearsetToTPR(controller->getGearsetRatioPair().internalGearset));

  EXPECT_TRUE(leftController->isDisabled());
  EXPECT_TRUE(rightController->isDisabled());

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, MoveDistanceAsyncRawUnitsTest) {
  controller->moveDistanceAsync(100);

  EXPECT_DOUBLE_EQ(leftController->getTarget(), 100);
  EXPECT_DOUBLE_EQ(rightController->getTarget(), 100);

  EXPECT_FALSE(leftController->isDisabled());
  EXPECT_FALSE(rightController->isDisabled());

  controller->waitUntilSettled();

  EXPECT_TRUE(leftController->isDisabled());
  EXPECT_TRUE(rightController->isDisabled());

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, MoveDistanceAsyncUnitsTest) {
  controller->moveDistanceAsync(6_in * 1_pi);

  EXPECT_DOUBLE_EQ(leftController->getTarget(),
                   gearsetToTPR(controller->getGearsetRatioPair().internalGearset));
  EXPECT_DOUBLE_EQ(rightController->getTarget(),
                   gearsetToTPR(controller->getGearsetRatioPair().internalGearset));

  EXPECT_FALSE(leftController->isDisabled());
  EXPECT_FALSE(rightController->isDisabled());

  controller->waitUntilSettled();

  EXPECT_DOUBLE_EQ(leftController->getTarget(),
                   gearsetToTPR(controller->getGearsetRatioPair().internalGearset));
  EXPECT_DOUBLE_EQ(rightController->getTarget(),
                   gearsetToTPR(controller->getGearsetRatioPair().internalGearset));

  EXPECT_TRUE(leftController->isDisabled());
  EXPECT_TRUE(rightController->isDisabled());

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, TurnAngleRawUnitsTest) {
  controller->turnAngle(100);

  EXPECT_DOUBLE_EQ(leftController->getTarget(), 100);
  EXPECT_DOUBLE_EQ(rightController->getTarget(), -100);

  EXPECT_TRUE(leftController->isDisabled());
  EXPECT_TRUE(rightController->isDisabled());

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, TurnAngleUnitsTest) {
  controller->turnAngle(gearsetToTPR(controller->getGearsetRatioPair().internalGearset) *
                        (6.0 / 11.0) * degree);

  EXPECT_DOUBLE_EQ(leftController->getTarget(),
                   gearsetToTPR(controller->getGearsetRatioPair().internalGearset));
  EXPECT_DOUBLE_EQ(rightController->getTarget(),
                   -1 * gearsetToTPR(controller->getGearsetRatioPair().internalGearset));

  EXPECT_TRUE(leftController->isDisabled());
  EXPECT_TRUE(rightController->isDisabled());

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, TurnAngleAsyncRawUnitsTest) {
  controller->turnAngleAsync(100);

  EXPECT_DOUBLE_EQ(leftController->getTarget(), 100);
  EXPECT_DOUBLE_EQ(rightController->getTarget(), -100);

  EXPECT_FALSE(leftController->isDisabled());
  EXPECT_FALSE(rightController->isDisabled());

  controller->waitUntilSettled();

  EXPECT_TRUE(leftController->isDisabled());
  EXPECT_TRUE(rightController->isDisabled());

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, TurnAngleAsyncUnitsTest) {
  controller->turnAngleAsync(gearsetToTPR(controller->getGearsetRatioPair().internalGearset) *
                             (6.0 / 11.0) * degree);

  EXPECT_DOUBLE_EQ(leftController->getTarget(),
                   gearsetToTPR(controller->getGearsetRatioPair().internalGearset));
  EXPECT_DOUBLE_EQ(rightController->getTarget(),
                   -1 * gearsetToTPR(controller->getGearsetRatioPair().internalGearset));

  EXPECT_FALSE(leftController->isDisabled());
  EXPECT_FALSE(rightController->isDisabled());

  controller->waitUntilSettled();

  EXPECT_TRUE(leftController->isDisabled());
  EXPECT_TRUE(rightController->isDisabled());

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, MirrorTurnTest) {
  controller->setTurnsMirrored(true);
  controller->turnAngle(gearsetToTPR(controller->getGearsetRatioPair().internalGearset) *
                        (6.0 / 11.0) * degree);

  EXPECT_DOUBLE_EQ(leftController->getTarget(),
                   -1 * gearsetToTPR(controller->getGearsetRatioPair().internalGearset));
  EXPECT_DOUBLE_EQ(rightController->getTarget(),
                   gearsetToTPR(controller->getGearsetRatioPair().internalGearset));
}

TEST_F(ChassisControllerIntegratedTest, MoveDistanceThenTurnAngleAsyncTest) {
  controller->moveDistanceAsync(100);

  EXPECT_DOUBLE_EQ(leftController->getTarget(), 100);
  EXPECT_DOUBLE_EQ(rightController->getTarget(), 100);

  EXPECT_FALSE(leftController->isDisabled());
  EXPECT_FALSE(rightController->isDisabled());

  controller->turnAngleAsync(200);

  EXPECT_DOUBLE_EQ(leftController->getTarget(), 200);
  EXPECT_DOUBLE_EQ(rightController->getTarget(), -200);

  EXPECT_FALSE(leftController->isDisabled());
  EXPECT_FALSE(rightController->isDisabled());

  controller->waitUntilSettled();

  EXPECT_TRUE(leftController->isDisabled());
  EXPECT_TRUE(rightController->isDisabled());

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, TurnAngleThenMoveDistanceAsyncTest) {
  controller->turnAngleAsync(200);

  EXPECT_DOUBLE_EQ(leftController->getTarget(), 200);
  EXPECT_DOUBLE_EQ(rightController->getTarget(), -200);

  EXPECT_FALSE(leftController->isDisabled());
  EXPECT_FALSE(rightController->isDisabled());

  controller->moveDistanceAsync(100);

  EXPECT_DOUBLE_EQ(leftController->getTarget(), 100);
  EXPECT_DOUBLE_EQ(rightController->getTarget(), 100);

  EXPECT_FALSE(leftController->isDisabled());
  EXPECT_FALSE(rightController->isDisabled());

  controller->waitUntilSettled();

  EXPECT_TRUE(leftController->isDisabled());
  EXPECT_TRUE(rightController->isDisabled());

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, MoveDistanceAndWaitTest) {
  controller->moveDistance(100);
  controller->waitUntilSettled();

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, MoveDistanceGetPushedAndWaitTest) {
  controller->moveDistance(100);

  // First bump
  leftMotor->encoder->value = 500;
  rightMotor->encoder->value = 500;
  controller->waitUntilSettled();

  // Second bump
  leftMotor->encoder->value = 1000;
  rightMotor->encoder->value = 1000;
  controller->waitUntilSettled();

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, TurnAngleAndWaitTest) {
  controller->turnAngle(100);
  controller->waitUntilSettled();

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, TurnAngleGetPushedAndWaitTest) {
  controller->turnAngle(100);

  // First bump
  leftMotor->encoder->value = 500;
  rightMotor->encoder->value = 500;
  controller->waitUntilSettled();

  // Second bump
  leftMotor->encoder->value = 1000;
  rightMotor->encoder->value = 1000;
  controller->waitUntilSettled();

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
}

TEST_F(ChassisControllerIntegratedTest, MoveDistanceAndStopTest) {
  controller->moveDistanceAsync(100);
  controller->stop();

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
  EXPECT_TRUE(leftController->isDisabled());
  EXPECT_TRUE(rightController->isDisabled());
}

TEST_F(ChassisControllerIntegratedTest, TurnAngleAndStopTest) {
  controller->turnAngleAsync(100);
  controller->stop();

  assertMotorsHaveBeenStopped(leftMotor, rightMotor);
  EXPECT_TRUE(leftController->isDisabled());
  EXPECT_TRUE(rightController->isDisabled());
}

TEST_F(ChassisControllerIntegratedTest, SetMaxVelocityTest) {
  controller->setMaxVelocity(42);
  EXPECT_EQ(leftController->maxVelocity, 42);
  EXPECT_EQ(rightController->maxVelocity, 42);
  EXPECT_EQ(model->maxVelocity, 42);
}
