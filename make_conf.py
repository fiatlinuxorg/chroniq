import signal
import sys
import toml

from rich.console import Console
from rich.text import Text
from rich.prompt import Prompt

class App:
    def __init__(self: object) -> None:
        self.__console = Console()

    def run(self: object) -> None:
        self.__display_menu()
        user_input = self.__get_user_input()

        match user_input:
            case "1":
                self.__create_new_conf()
            case "2":
                self.__edit_conf()
            case "3":
                file_name = self.__ask_file_name()
                self.__display_conf(file_name)
            case "4":
                sys.exit(0)

    def __display_menu(self: object) -> None:

        from rich.panel import Panel
        from rich.table import Column, Table

        option_table = Table(box=None, show_header=False)

        option_table.add_row("1. Create new configuration")
        option_table.add_row("2. Edit existing configuration")
        option_table.add_row("3. View configuration")
        option_table.add_row("4. Exit")
        
        self.__console.print(
            Panel.fit(
                option_table,
                border_style="dim",
                title="Chroniq - Configration Manager",
                title_align="left"
            )
        )

    def __display_conf(self: object, file_name: str) -> None:

        from rich.syntax import Syntax

        try:
            with open(file_name, "r") as file:
                syntax = Syntax(file.read(), "toml", theme="ansi_dark")

            self.__console.print(syntax)
        except FileNotFoundError:
            self.__console.print(f"File {file_name} not found")
        except toml.TomlDecodeError:
            self.__console.print(f"File {file_name} is not a valid TOML file")

    def __get_user_input(self: object) -> str:

        user_input = Prompt.ask(
            "Select an option",
            choices=["1", "2", "3"],
            default="1",
            show_choices=False,
        )

        return user_input

    def __ask_file_name(self: object) -> str:

        file_name = Prompt.ask(
            "Enter the file path",
            default="chroniq.conf",
        )

        if not file_name.endswith(".conf"):
            file_name += ".conf"

        return file_name

    def __ask_conf(self: object, old_conf: dict = None) -> dict:
        
        from rich.prompt import Confirm

        new_conf = {}

        # WiFi settings
        wifi_section = {}
        if old_conf is not None:
            self.__console.print(old_conf)
            wifi_section["ssid"] = Prompt.ask(
                "WiFi SSID", default=old_conf["wifi"]["ssid"]
            )
            wifi_section["pswd"] = Prompt.ask(
                "WiFi Password", default=old_conf["wifi"]["pswd"]
            )
        else:
            wifi_section["ssid"] = Prompt.ask("WiFi SSID")
            wifi_section["pswd"] = Prompt.ask("WiFi Password")
        
        new_conf["wifi"] = wifi_section

        # Bus API settings
        bus_api_section = {}
        if old_conf is not None:
            bus_api_section["url"] = Prompt.ask(
                "Bus API URL", default=old_conf.get("bus", {}).get("api", {}).get("url")
            )
        else:
            bus_api_section["url"] = Prompt.ask("Bus API URL")

        new_conf.setdefault("bus", {})["api"] = bus_api_section

        # Views settings
        views_section = {}
        views_section["clock"] = Confirm.ask("Show clock view", default=True)

        num_bus_views = int(Prompt.ask("Number of bus views", default="1"))
        bus_views = []

        for i in range(num_bus_views):
            stop_id = Prompt.ask(f"Stop ID #{i+1}")
            bus_id = Prompt.ask(f"Bus ID #{i+1}")
            bus_views.append({"stop_id": stop_id, "bus_id": bus_id})

        views_section["bus"] = bus_views
        new_conf["views"] = views_section

        task_schedules = []
        num_tasks = int(Prompt.ask("Number of tasks", default="1"))

        for i in range(num_tasks):
            task = {}
            task["action"] = Prompt.ask(
                f"Task action #{i+1}",
                choices=["backlight", "no_backlight", "set_view"]
            )
            task["time"] = Prompt.ask(f"Task scheduled time #{i+1}")

            if task["action"] == "set_view":
                task["index"] = int(Prompt.ask(f"Task view index #{i+1}"))

            task_schedules.append(task)

        new_conf["task_schedules"] = task_schedules

        return new_conf

    def __load_conf(self: object, file_name: str) -> dict:

        conf = {}

        try:
            with open(file_name, "r") as file:
                conf = toml.load(file)
        except FileNotFoundError:
            self.__console.print(f"File {file_name} not found")
            return None
        except toml.TomlDecodeError:
            self.__console.print(f"File {file_name} is not a valid TOML file")
            return None

        return conf

    def __save_conf(self: object, file_name: str, conf: dict) -> bool:

        try:
            with open(file_name, "w") as file:
                toml.dump(conf, file)
        except FileNotFoundError:
            self.__console.print(f"File {file_name} not found")
            return False

        return True

    def __create_new_conf(self: object) -> None:
        
        file_name = self.__ask_file_name()
        conf = self.__ask_conf()

        if (self.__save_conf(file_name, conf)):
            self.__display_conf(file_name)
        else:
            self.__console.print("Failed to save configuration")

    def __edit_conf(self: object) -> None:

        file_name = self.__ask_file_name()
        conf = self.__load_conf(file_name)

        if conf is None:
            self.__console.print("Empty configuration")
            return

        conf = self.__ask_conf(conf)

        if (self.__save_conf(file_name, conf)):
            self.__display_conf(file_name)
        else:
            self.__console.print("Failed to save configuration")


def singal_handler(signum: int, frame: object) -> None:
    sys.exit(0)

signal.signal(signal.SIGINT, singal_handler)

if __name__ == "__main__":
    app = App()
    app.run()

