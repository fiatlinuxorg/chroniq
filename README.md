<a align="center" href="#fiatlinux-chroniq-page"><img src="https://fiatlinux.it/banner/4"></a><br>


<!--_<p align="center">Your Open-Source knick-knack.</p>_-->

<p align="center">
    <img src="https://img.shields.io/github/license/fiatlinuxorg/chroniq" alt="License">
    <img src="https://img.shields.io/github/watchers/fiatlinuxorg/chroniq">
    <img src="https://img.shields.io/github/stars/fiatlinuxorg/chroniq" alt="Stars">
</p>

## Table of Contents

- [Description](#description)
- [Installation](#installation)
    - [Prerequisites](#prerequisites)
    - [Building and Flashing](#building-and-flashing)
- [License](#license)
- [Contributing](#contributing)
    - [Bug Reports](#bug-reports)
    - [Feature Requests](#feature-requests)
    - [Pull Requests](#pull-requests)
- [Authors](#authors)

## Description
Chroniq is an open-source project that aims to create a simple and easy-to-use digital clock, chat system and bus timetable. Chroniq views are fully customizable and users can create their own views (coding skills required).

## Installation

### Prerequisites
- [PlatformIO CLI](https://platformio.org/install/cli)
- [Arduino IDE](https://www.arduino.cc/en/software) (optional)

### Building and Flashing
1. Clone the repository:
    ```sh
    git clone git@github.com:fiatlinuxorg/chroniq.git
    ```
2. Change directory to the project root:
    ```sh
    cd chroniq
    ```

#### With PlatformIO CLI
1. Build the project:
    ```sh
    pio run
    ```
2. Flash the project to the board:
    ```sh
    pio run --target upload
    ```
3. Done!

#### With Arduino IDE
1. Open the project in the Arduino IDE:
    ```sh
    arduino chroniq.ino
    ```
2. Select the board and port from the `Tools` menu.
3. Click the upload button.
4. Done!

## Configuration
Soon.

## License
This project is licensed under the GPL-3.0 License - see the [LICENSE](LICENSE) file for details.

## Contributing
We welcome contributions from the community to improve this project. If you'd like to contribute, please follow these guidelines:

### Bug Reports
If you find a bug, please open an issue and provide as much information as possible. This includes the version of the project and a detailed description of the bug.

### Feature Requests
If you'd like to request a new feature, please open an issue and provide a detailed description of the feature you'd like to see or contribute a pull request.

### Pull Requests
We welcome pull requests from the community. Before submitting a pull request, please ensure that your code is properly formatted.

If you'd like to contribute, follow these steps:

1. Fork the repository;
2. Create a new branch;
3. Commit your changes to the new branch;
4. Push the new branch to your fork;
5. Open a pull request.

## Authors
- [RiccardoSegala04](https://github.com/RiccardoSegala04)
- [LanaMirko04](https://github.com/LanaMirko04)
- [EtanolotYT](https://github.com/EtanolotYT)
