import random
import math

class Yard(object):
    """Set horizontal and vertical boundaries (meters)"""
    def __init__(self):
        """Create an yard with specific boundaries"""
        self.x_limit = 15
        self.y_limit = 10

    def is_in_yard(self, aDot):
        """Return True if the dot is within the yard boundaries. False otherwise"""
        if  round(aDot.x) in range(0, self.x_limit + 1) and round(aDot.y) in range(0, self.y_limit + 1):
            return True
        return False

class Dot(object):
    """A coordinate"""
    def __init__(self, aYard):
        self.x = round(random.uniform(0, aYard.x_limit), 2)
        self.y = round(random.uniform(0, aYard.y_limit), 2)

    def __str__(self) -> str:
       return f"({round(self.x, 2)}, {round(self.y, 2)})"

class Bot(Dot):
    """A coordinate that moves from the origin to a static target"""
    def __init__(self, aYard):
        super().__init__(aYard)
        self.x = 0
        self.y = 0
        self.step = 0.01    #0.01 meter per step

    def find_distance_to_target(self, target):
        dist_x = target.x - self.x
        dist_y = target.y - self.y
        return round(math.sqrt(dist_x**2 + dist_y**2), 2)

    def find_angle_to_target(self, target):
        """Angle found in degrees"""
        delta_x = target.x - self.x
        delta_y = target.y - self.y
        return round(math.degrees(math.atan(delta_y/delta_x)), 2)

    def move_one_step_toward(self, target):
        angle = math.radians(self.find_angle_to_target(target))
        step_x = round(self.step * math.cos(angle), 4)
        step_y = round(self.step * math.sin(angle), 4)
        self.x += step_x
        self.y += step_y

yard = Yard()
target = Dot(yard)
bot = Bot(yard)
print(target)
count = 0
while target.x - bot.x > 0.1 and target.y - bot.y > 0.1:
    bot.move_one_step_toward(target)
    count += 1

print(f"Move complete. Total steps of {count}. Target is at {target} and Bot is at {bot}")
