import logging
import requests
import datetime
from time import sleep
from typing import Optional

from aw_core.models import Event
from aw_client import ActivityWatchClient

from aw_watcher_table.config import watcher_config
from aw_watcher_table.settings import Settings

logger = logging.getLogger(__name__)


class TableWatcher:
    def __init__(self, testing=False):
        config_section = "aw-watcher-table" if not testing else "aw-watcher-table-testing"
        self.settings = Settings(watcher_config[config_section])

        self.client = ActivityWatchClient("aw-watcher-table", testing=testing)
        self.bucket_id = "{}_{}".format(self.client.client_name, self.client.client_hostname)

    def run(self):
        logger.info("aw-watcher-table started")

        # Initialization
        sleep(1)

        self.client.create_bucket(self.bucket_id, event_type='table_state', queued=True)

        # Start table checking loop
        with self.client:
            self.heartbeat_loop()

    def ping(self, table_height: Optional[int]):
        event = Event(
            timestamp=datetime.datetime.now(datetime.timezone.utc),
            data={
                "status": self.get_table_status(table_height)
            }
        )
        # 10 seconds request timeout
        self.client.heartbeat(self.bucket_id, event, pulsetime=self.settings.poll_time+1+10, queued=True)

    def get_table_height(self) -> Optional[int]:
        try:
            r = requests.get(f'http://{self.settings.ip}/measure')
            return r.json()['table_height']

        except Exception as ex:
            logger.warning(f'aw-watcher-table: Measurement failed! Please make sure http://{self.settings.ip}/measure '
                           f'delivers a JSON object which includes the field "table_height"')
            return None

    def get_table_status(self, table_height):
        return self.settings.get_height_level(table_height)

    def heartbeat_loop(self):
        while True:
            try:
                table_height = self.get_table_height()
                if table_height is None:
                    logger.warning(
                        f'aw-watcher-table: table_height corrected from None to -1!')
                    table_height = -1
                self.ping(table_height)
                sleep(self.settings.poll_time)
            except KeyboardInterrupt:
                logger.info("aw-watcher-table stopped by keyboard interrupt")
                break
