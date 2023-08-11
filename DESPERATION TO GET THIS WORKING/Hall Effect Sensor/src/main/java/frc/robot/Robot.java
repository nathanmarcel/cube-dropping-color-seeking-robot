/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.DigitalInput;

public class Robot extends TimedRobot {
  DigitalInput heffect;


  @Override
  public void robotInit() {
    heffect = new DigitalInput(0);
  }


  @Override
  public void teleopPeriodic() {
    boolean state = heffect.get();
    SmartDashboard.putBoolean("DB/LED 0", state);
  }

}
