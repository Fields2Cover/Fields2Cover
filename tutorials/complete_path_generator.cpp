#include "fields2cover.h"
#include <iostream>

int main() {
    // Define field and robot
    f2c::Random rand(42);
    F2CRobot robot (1.0, 4.0);
    F2CCells cells(F2CCell(F2CLinearRing({F2CPoint(0,0), F2CPoint(0,100), F2CPoint(40,100),
                                            F2CPoint(40,0), F2CPoint(0,0)})));
    // TODO test on a random field and also with headlands

    // Swath generation
    std::cout << "####### Tutorial 5.1 Boustrophedon ######" << std::endl;
    f2c::sg::BruteForce bf;
    F2CSwaths swaths = bf.generateSwaths(M_PI, robot.op_width, cells.getGeometry(0));
    f2c::rp::BoustrophedonOrder boustrophedon_sorter;
    auto boustrophedon_swaths = boustrophedon_sorter.genSortedSwaths(swaths);

    // Path planner
    f2c::pp::PathPlanning path_planner;
    robot.setMinRadius(2.0);  // m
    robot.linear_curv_change = 0.5;  // 1/m^2 https://fields2cover.github.io/api/program_listing_file_fields2cover_types_Robot.h.html

    // Create swath connection curves
    std::cout << "####### Tutorial 6.2 Dubins curves with Continuous curvature ######" << std::endl;
    f2c::pp::DubinsCurvesCC dubins_cc;
    F2CPath path_dubins_cc = path_planner.searchBestPath(robot, boustrophedon_swaths, dubins_cc);

    std::cout << "Path length \n" << path_dubins_cc.length() << std::endl;
    std::cout << "Accessing elements in path: \n"
              << "First point = "
              << path_dubins_cc.states.at(0).point << "\n"
              << "First angle = "
              << path_dubins_cc.states.at(0).angle << "\n"
              << "First velocity = "
              << path_dubins_cc.states.at(0).velocity << "\n"
              << "First duration = "
              << path_dubins_cc.states.at(0).duration << "\n"
              << "First direction = "
              << static_cast<int>(path_dubins_cc.states.at(0).dir) << "\n"
              << "First path type = "
              << static_cast<int>(path_dubins_cc.states.at(0).type) << "\n"
              << std::endl;

    std::cout << "Robot parameters \n"
              << "Robot cruise speed = " << robot.cruise_speed << " [m/s]\n"
              << std::endl;

        // Discretize the path if it is a swath
        // Loop through all the points in the path
        F2CPath new_path;
        double discretize_step_size = 0.1; // Step size for discretization in [m]

        for (size_t i = 0; i < path_dubins_cc.size(); i++)
        {
            // Check if the current point is a SWATH point
            if (path_dubins_cc.states.at(i).type == f2c::types::PathSectionType::SWATH)
            {
                // We know that swaths are represented by only two point so the next point will
                // be the end of the swath
                std::cout << "Swath starts at " << i << " " << path_dubins_cc.states.at(i).point << "\n";
                std::cout << "Swath ends at " << i+1 << " " << path_dubins_cc.states.at(i+1).point << std::endl;

                // Save start and end swath points to variables
                F2CPoint start_point = path_dubins_cc.states.at(i).point;
                F2CPoint end_point = path_dubins_cc.states.at(i+1).point;

                i++; // Increment i to not discretize twice

                // We can now discretize the swath
                // Calculate the distance between the start and end point
                double distance = start_point.Distance(end_point);
                std::cout << "Distance between points: " << distance << std::endl;

                // Calculate the number of points needed to discretize the swath
                int num_points = floor(distance / discretize_step_size);
                std::cout << "Number of points: " << num_points << std::endl;

                // Create vector with x values from start to end point
                std::vector<double> x_values;
                for (int j = 0; j < num_points; j++)
                {
                    x_values.push_back(start_point.getX() + (j * (end_point.getX() - start_point.getX()) / num_points));
                }
                // Add first point to list
                x_values.push_back(end_point.getX());
                // Now we need to calculate y values based on the Dubin's curve equation
                // https://en.wikipedia.org/wiki/Dubins_path
                // Create vector with y values from start to end point
                std::vector<double> y_values;
                for (int j = 0; j < num_points; j++)
                {
                    y_values.push_back(start_point.getY() + (j * (end_point.getY() - start_point.getY()) / num_points));
                }
                // Add first point to list
                y_values.push_back(end_point.getY());

                // TODO add z-values as well

                // Iterate over each pair of coordinates and add them as states into our new Path object
                for (int j = 0; j < num_points; j++)
                {
                    // Create a new PathState object
                    f2c::types::PathState state;
                    // Set the point to the current x and y values
                    state.point = F2CPoint(x_values.at(j), y_values.at(j), 0.0);
                    // Set the angle to the angle between the current point and the next point
                    // TODO Angle becomes -PI/2 some places!!!
                    state.angle = atan2(y_values.at(j+1) - y_values.at(j), x_values.at(j+1) - x_values.at(j));
                    // Set the velocity to the max speed of the robot
                    state.velocity = robot.cruise_speed;
                    // Set the duration to the distance between the current point and the next point
                    state.duration = sqrt(pow(x_values.at(j+1) - x_values.at(j), 2) + pow(y_values.at(j+1) - y_values.at(j), 2));
                    // Set the direction to forward
                    state.dir = f2c::types::PathDirection::FORWARD;
                    // Set the type to SWATH
                    state.type = f2c::types::PathSectionType::SWATH;
                    // Add the state to the new path
                    new_path.states.push_back(state);
                }

                // Add the last point to the new path
                f2c::types::PathState state;
                state.point = F2CPoint(x_values.at(num_points), y_values.at(num_points));
                state.angle = atan2(y_values.at(num_points) - y_values.at(num_points-1), x_values.at(num_points) - x_values.at(num_points-1));
                state.velocity = robot.cruise_speed;
                state.duration = sqrt(pow(x_values.at(num_points) - x_values.at(num_points-1), 2) + pow(y_values.at(num_points) - y_values.at(num_points-1), 2));
                state.dir = f2c::types::PathDirection::FORWARD;
                state.type = f2c::types::PathSectionType::SWATH;
                new_path.states.push_back(state);

                std::cout << "\n\n\n" << std::endl;

            
            }
            else // Add TURN's to new_path
            {
                // If the current point is not a swath point, we can just add it to the new path
                new_path.states.push_back(path_dubins_cc.states.at(i));
            }
        }

    // Save to file
    new_path.serializePath(0.1);
    new_path.saveToFile("path_1.csv", 5); // Specify precision to the significant number

    f2c::Visualizer::figure();
    f2c::Visualizer::plot(cells);
    f2c::Visualizer::plot(new_path);
    f2c::Visualizer::plot(boustrophedon_swaths);
    f2c::Visualizer::show();
    f2c::Visualizer::save("complete_path");

    return 0;
}


