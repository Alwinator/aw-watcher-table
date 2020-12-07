from configparser import ConfigParser
from aw_core.config import load_config

default_settings = {
    "poll_time": "5",  # seconds
    "height_levels": "ball:0-70;sitting:70-90;standing:90-200",  # cm
    "esp_ip": "192.168.2.4"
}
default_testing_settings = {
    "poll_time": "5",  # seconds
    "height_levels": "ball:0-70;sitting:70-90;standing:90-200",  # cm
    "esp_ip": "192.168.2.4"
}

default_config = ConfigParser()
default_config['aw-watcher-table'] = default_settings
default_config['aw-watcher-table-testing'] = default_testing_settings
watcher_config = load_config("aw-watcher-table", default_config)
