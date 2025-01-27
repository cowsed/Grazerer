#include "competition/autonomous.h"
#include "robot-config.h"

/**
 * Main entrypoint for the autonomous period
 */

void skills();

int goal_countera = 0;

void autonomous()
{
	vexDelay(700);

	auto_();
}

AutoCommand *intake_command(double amt = 12.0) {
	return new FunctionCommand([=]() {
        
		intake(amt);
		return true;
	});
}

AutoCommand *outtake_command(double amt = 12.0) {
	return new FunctionCommand([=]() {
		outtake(amt);
		return true;
	});
}

AutoCommand *stop_intake() {
	return new FunctionCommand([=]() {
		intake(0);
		return true;
	});
}

AutoCommand *conveyor_intake_command(double amt = 12.0) {
	return new FunctionCommand([=]() {
		conveyor_intake(amt);
		return true;
	});
}

AutoCommand *conveyor_stop_command() {
	return new FunctionCommand([=]() {
		conveyor_intake(0);
		return true;
	});
}

AutoCommand *goal_grabber_command(bool value) {
	return new FunctionCommand([=]() {
		goal_grabber_sol.set(value);
        goal_countera = 10;
		return true;
	});
}

class DebugCommand : public AutoCommand {
public:
	bool run() override {
		drive_sys.stop();
		pose_t pos = odom.get_position();
		printf("ODO X: %.2f, Y: %.2f, R:%.2f\n", pos.x, pos.y, pos.rot);
		while (true) {
			double f = con.Axis3.position() / 200.0;
			double s = con.Axis1.position() / 200.0;
			// double left_enc_start_pos = left_enc.position(vex::rotationUnits::rev);
			drive_sys.drive_arcade(f, s, 1, TankDrive::BrakeType::None);
			pose_t pos = odom.get_position();
			printf("ODO X: %.2f, Y: %.2f, R:%.2f\n", pos.x, pos.y, pos.rot);
			// printf("ENC LEFT REV: %.2f, ENC RIGHT POS: %.2f, ENC BACK POS: %.2f\n", left_enc.position(vex::rotationUnits::deg), right_enc.position(vex::rotationUnits::deg), front_enc.position(vex::rotationUnits::deg));
			// if (left_enc.position(vex::rotationUnits::rev) >= 1.0) {
			//     break;
			// }
			vexDelay(100);
		}
		return false;
	}
};

void auto_() {
	CommandController cc {
		// odom.SetPositionCmd({.x = 9.5, .y = 72, .rot = 0}),

		new Async(new FunctionCommand([]() {
			while(true) {
				OdometryBase *odombase = &odom;
                pose_t pos = odombase->get_position();
            	printf("ODO X: %.2f, Y: %.2f, R:%.2f, Concurr: %f\n", pos.x, pos.y, pos.rot, conveyor.current());
				vexDelay(100);

				if((conveyor.current() > 2) && conveyor.velocity(rpm) < 0.5){
					printf("Conveyor Stalling");
					conveyor_intake(-12);
					vexDelay(500);
					conveyor_intake(12);
				}

                if (goal_sensor.objectDistance(vex::mm) < 25 && goal_countera == 0) {
                goal_grabber_sol.set(true);
                }

                if (goal_countera > 0) {
                    goal_countera--;
                }
			}
			return true;
		})),

		// First Ring

        // drive_sys.DriveForwardCmd(24, fwd, 0.6)->withTimeout(2),
        // drive_sys.TurnToHeadingCmd(90, 0.6),
        // drive_sys.DriveForwardCmd(24, fwd, 0.6)->withTimeout(2),
        // drive_sys.TurnToHeadingCmd(180, 0.6),
        // drive_sys.DriveForwardCmd(24, fwd, 0.6)->withTimeout(2),
        // drive_sys.TurnToHeadingCmd(270, 0.6),
        // drive_sys.DriveForwardCmd(24, fwd, 0.6)->withTimeout(2),
        // drive_sys.TurnToHeadingCmd(360, 0.6),

        // intake_command(),
		drive_sys.DriveToPointCmd({90, 31}, vex::reverse, 1, 0.5) -> withTimeout(4),
		drive_sys.DriveToPointCmd({82.7, 26.4}, vex::reverse, 0.5) -> withTimeout(4),

        };
	cc.run();
}

void skills() {
	CommandController cc {
		// odom.SetPositionCmd({.x = 9.5, .y = 72, .rot = 0}),

		new Async(new FunctionCommand([]() {
			while(true) {
				OdometryBase *odombase = &odom;
                pose_t pos = odombase->get_position();
            	printf("ODO X: %.2f, Y: %.2f, R:%.2f, Concurr: %f\n", pos.x, pos.y, pos.rot, conveyor.current());
				vexDelay(100);

				if((conveyor.current() > 2) && conveyor.velocity(rpm) < 0.5){
					printf("Conveyor Stalling");
					conveyor_intake(-12);
					vexDelay(500);
					conveyor_intake(12);
				}

                if (goal_sensor.objectDistance(vex::mm) < 25 && goal_countera == 0) {
                goal_grabber_sol.set(true);
                }

                if (goal_countera > 0) {
                    goal_countera--;
                }
			}
			return true;
		})),

		// First Ring

        // drive_sys.DriveForwardCmd(24, fwd, 0.6)->withTimeout(2),
        // drive_sys.TurnToHeadingCmd(90, 0.6),
        // drive_sys.DriveForwardCmd(24, fwd, 0.6)->withTimeout(2),
        // drive_sys.TurnToHeadingCmd(180, 0.6),
        // drive_sys.DriveForwardCmd(24, fwd, 0.6)->withTimeout(2),
        // drive_sys.TurnToHeadingCmd(270, 0.6),
        // drive_sys.DriveForwardCmd(24, fwd, 0.6)->withTimeout(2),
        // drive_sys.TurnToHeadingCmd(360, 0.6),

        intake_command(),
		drive_sys.DriveToPointCmd({50, 96}, vex::forward, .6) -> withTimeout(4),

        conveyor_stop_command(),

        // First Stake
        drive_sys.TurnToHeadingCmd(-90, .6) -> withTimeout(4),
        drive_sys.DriveToPointCmd({48, 120}, vex::reverse, .3) -> withTimeout(10),
        // goal_grabber_command(true),
        conveyor_intake_command(),
        

        // Second Ring
        drive_sys.TurnToPointCmd(72, 120, vex::directionType::fwd, .6) -> withTimeout(4),
        drive_sys.DriveToPointCmd({72, 120}, vex::forward, .6) -> withTimeout(4),

        // Third Ring
        // drive_sys.TurnToHeadingCmd(90, .6) -> withTimeout(2),
        drive_sys.TurnToPointCmd(72, 128, vex::directionType::fwd, .6) -> withTimeout(4),
        drive_sys.DriveToPointCmd({72, 128}, vex::forward, .6) -> withTimeout(4),

        // Fourth Ring
        drive_sys.TurnToPointCmd(24, 120, vex::directionType::fwd, .6) -> withTimeout(1),
        drive_sys.DriveToPointCmd({24, 120}, vex::forward, .6) -> withTimeout(1.5),

        // Fifth Ring
        drive_sys.TurnToPointCmd(4, 144, vex::directionType::fwd, .6) -> withTimeout(1),
        drive_sys.DriveToPointCmd({6, 137}, vex::forward, .6) -> withTimeout(1.05),
        //new DebugCommand(),
            
        // Deposit First Stake
        drive_sys.DriveForwardCmd(18, vex::directionType::rev, .6) -> withTimeout(.7),
        new DelayCommand(500),
        // drive_sys.TurnToPointCmd(96, 120, vex::directionType::fwd, .6) -> withTimeout(3),
        drive_sys.TurnToHeadingCmd(315, .5) -> withTimeout(1),
        //drive_sys.DriveForwardCmd(15, vex::directionType::rev, .8) -> withTimeout(.3),
        new DelayCommand(500),
        drive_sys.DriveTankCmd(-.5,-.5) -> withTimeout(1),
        //drive_sys.DriveForwardCmd(15, vex::directionType::rev, .6) -> withTimeout(.7),
        conveyor_stop_command(),
        
        

        //drop goal/move away
        goal_grabber_command(false),


        };
	cc.run();
}