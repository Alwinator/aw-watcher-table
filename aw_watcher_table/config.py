from configparser import ConfigParser
from aw_core.config import load_config

default_settings = {
    "poll_time": "5",  # seconds
    "standing_min_height": "90",  # cm
    "min_change_height": "3",  # cm
    "esp_ip": "192.168.2.4"
}
default_testing_settings = {
    "poll_time": "5",  # seconds
    "standing_min_height": "90",  # cm
    "min_change_height": "3",  # cm
    "esp_ip": "192.168.2.4"
}

default_config = ConfigParser()
default_config['aw-watcher-table'] = default_settings
default_config['aw-watcher-table-testing'] = default_testing_settings
watcher_config = load_config("aw-watcher-table", default_config)
