from dataclasses import dataclass

@dataclass
class RobotSettings:
    min_turning_radius: float
    width: float
    cov_width: float
    max_curv: float
    max_diff_curv: float
