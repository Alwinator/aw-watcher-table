class Settings:
    def __init__(self, config_section):
        self.poll_time = config_section.getfloat("poll_time")
        self.ip = config_section.get("esp_ip")
        self.standing_min_height = config_section.getfloat("standing_min_height")
        self.min_change_height = config_section.getfloat("min_change_height")

        assert self.poll_time > 0
        assert self.min_change_height > 0
        assert self.ip is not None, "IP address cannot be None! Please check the aw-watcher-table config!"
