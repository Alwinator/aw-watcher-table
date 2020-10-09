import argparse

from aw_core.log import setup_logging

from aw_watcher_table.watcher import TableWatcher


def main() -> None:
    # Set up argparse
    parser = argparse.ArgumentParser("Monitors whether you have set your height-adjustable table to sitting or "
                                     "standing.")
    parser.add_argument("-v", "--verbose", dest='verbose', action="store_true",
                        help='run with verbose logging')
    parser.add_argument("--testing", action="store_true",
                        help='run in testing mode')
    args = parser.parse_args()

    # Set up logging
    setup_logging("aw-watcher-table",
                  testing=args.testing, verbose=args.verbose,
                  log_stderr=True, log_file=True)

    # Start watcher
    watcher = TableWatcher(testing=args.testing)
    watcher.run()


if __name__ == "__main__":
    main()
