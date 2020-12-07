class Settings:
    def __init__(self, config_section):
        self.poll_time = config_section.getfloat("poll_time")
        self.ip = config_section.get("esp_ip")
        self.height_levels_raw = config_section.get("height_levels")
        self.height_levels = []

        assert self.poll_time > 0
        assert self.ip is not None, "IP address cannot be None! Please check the aw-watcher-table config!"
        assert self.height_levels_raw is not None, "height_levels cannot be None! Please check the aw-watcher-table " \
                                                   "config! "

        try:
            self.height_levels = list(map(HeightLevel, self.height_levels_raw.split(";")))
        except Exception as ex:
            raise AssertionError("height_levels Syntax error! Please check the aw-watcher-table config!", ex)

        def height_level(height_in_cm):
            for level in self.height_levels:
                if height_in_cm in level:
                    return level.name
            return "unknown"

        self.get_height_level = height_level


class HeightLevel:
    def __init__(self, s):
        x = s.split(":")
        self.name = x[0]
        y = x[1].split("-")
        self.min = int(y[0])
        self.max = int(y[1])

    def __contains__(self, value):
        return self.min <= value <= self.max
