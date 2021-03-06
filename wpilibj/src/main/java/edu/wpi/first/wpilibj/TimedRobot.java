/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj;

import edu.wpi.first.wpilibj.hal.FRCNetComm.tInstances;
import edu.wpi.first.wpilibj.hal.FRCNetComm.tResourceType;
import edu.wpi.first.wpilibj.hal.HAL;
import edu.wpi.first.wpilibj.hal.NotifierJNI;

/**
 * TimedRobot implements the IterativeRobotBase robot program framework.
 *
 * <p>The TimedRobot class is intended to be subclassed by a user creating a robot program.
 *
 * <p>periodic() functions from the base class are called on an interval by a Notifier instance.
 */
public class TimedRobot extends IterativeRobotBase {
  public static final double DEFAULT_PERIOD = 0.02;

  // Prevents loop from starting if user calls setPeriod() in robotInit()
  private boolean m_startLoop = false;

  // The C pointer to the notifier object. We don't use it directly, it is
  // just passed to the JNI bindings.
  private final int m_notifier = NotifierJNI.initializeNotifier();

  // The absolute expiration time
  private double m_expirationTime = 0;

  private double m_period = DEFAULT_PERIOD;

  public TimedRobot() {
    // HAL.report(tResourceType.kResourceType_Framework, tInstances.kFramework_Periodic);
    HAL.report(tResourceType.kResourceType_Framework, tInstances.kFramework_Iterative);
  }

  @Override
  @SuppressWarnings("NoFinalizer")
  protected void finalize() {
    NotifierJNI.stopNotifier(m_notifier);
    NotifierJNI.cleanNotifier(m_notifier);
  }

  /**
   * Provide an alternate "main loop" via startCompetition().
   */
  public void startCompetition() {
    robotInit();

    // Tell the DS that the robot is ready to be enabled
    HAL.observeUserProgramStarting();

    m_startLoop = true;

    m_expirationTime = RobotController.getFPGATime() * 1e-6 + m_period;
    updateAlarm();

    // Loop forever, calling the appropriate mode-dependent function
    while (true) {
      long curTime = NotifierJNI.waitForNotifierAlarm(m_notifier);
      if (curTime == 0) {
        break;
      }

      m_expirationTime += m_period;
      updateAlarm();

      loopFunc();
    }
  }

  /**
   * Set time period between calls to Periodic() functions.
   *
   * @param period Period in seconds.
   */
  public void setPeriod(double period) {
    m_period = period;

    if (m_startLoop) {
      m_expirationTime = RobotController.getFPGATime() * 1e-6 + period;
      updateAlarm();
    }
  }

  /**
   * Get time period between calls to Periodic() functions.
   */
  public double getPeriod() {
    return m_period;
  }

  /**
   * Update the alarm hardware to reflect the next alarm.
   */
  private void updateAlarm() {
    NotifierJNI.updateNotifierAlarm(m_notifier, (long) (m_expirationTime * 1e6));
  }
}
