#include <iostream>
#include <memory>
#include <string>

#include <Joystick.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <RobotDrive.h>
#include <Timer.h>
#include <ADXRS450_Gyro.h>
#include <Solenoid.h>
#include <Victor.h>
#include <CameraServer.h>

class Robot: public SampleRobot
{

	frc::RobotDrive myRobot {2, 1, 0, 3};
	frc::Joystick stick {0};
	frc::Solenoid solenoid {3};
	frc::Victor WindowMotor{4};
	frc::ADXRS450_Gyro Jyro;
	frc::SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";

public:
	Robot(){
		myRobot.SetExpiration(0.1);
	}

	void RobotInit(){
		chooser.AddObject(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		SmartDashboard::PutData("Auto Modes", &chooser);
		Jyro.Reset();
		frc::CameraServer::GetInstance()->StartAutomaticCapture():
	}

	void Autonomous()
	{
		auto autoSelected = chooser.GetSelected();
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
			std::cout << "Running custom Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.MecanumDrive_Polar(0.1, 90.0, 0.0);
			frc::Wait(1.0);
			myRobot.MecanumDrive_Polar(0.0, 0.0, 0.0);
		} else {
			//Default Auto goes here
			std::cout << "Running default Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
			frc::Wait(2.0); 				//    for 2 seconds
			myRobot.Drive(0.0, 0.0); 	// stop robot
		}

	}

	/**
	 * Runs the motors with arcade steering.
	 */
	void OperatorControl()
	{
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl() && IsEnabled())
		{
			float stickY = (float)stick.GetY();//converting to float
			float stickX = (float)stick.GetX();
			float stickZ = (float)stick.GetZ();

			myRobot.MecanumDrive_Cartesian(stickX, -stickZ, -stickY, Jyro.GetAngle());

			std::cout << "jyro Angle "<< Jyro.GetAngle() << std::endl;

			if (stick.GetRawButton(1)){
			solenoid.Set(true);
			frc::Wait(0.005);
			solenoid.Set(false);
			}
			if (stick.GetRawButton(6)){
				WindowMotor.Set(0.5);
				Wait(0.005); // CHANGE
			}else if (stick.GetRawButton(4)){
				WindowMotor.Set(-0.5);
				Wait(0.005);
				WindowMotor.Set(0);
			}else
			{
				WindowMotor.Set(0);
			}

			Wait(0.005);				// wait for a motor update time
		}
	}

	/**s
	 * Runs during test mode
	 */
	void Test()
	{
	}
};

START_ROBOT_CLASS(Robot)
