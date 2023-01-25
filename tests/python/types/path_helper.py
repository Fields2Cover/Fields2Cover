import math
import fields2cover as f2c

class PathHelper:
    
    def __init__(self) -> None:
        rand = f2c.Random(42)
        robot = f2c.Robot(2.0, 6.0)
        const_hl = f2c.HG_Const_gen()
        field = rand.generateRandField(5, 1e5)
        cells = field.field
        no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.robot_width)
        bf = f2c.SG_BruteForce()
        swaths = bf.generateSwaths(math.pi, robot.op_width, no_hl.getGeometry(0))
        snake_sorter = f2c.RP_Snake()
        swaths = snake_sorter.genSortedSwaths(swaths)

        robot.setMinRadius(2)  # m
        robot.linear_curv_change = 0.1  # 1/m^2
        path_planner = f2c.PP_PathPlanning()
        dubins = f2c.PP_DubinsCurvesCC()
        self.path = path_planner.searchBestPath(robot, swaths, dubins);
        self.points = []
    
    def init_points(self):
        
        l = list(self.path.states)
        d = [item.point for item in l]
        _points = [] + d
        self.points = _points
        