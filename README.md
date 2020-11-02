<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/dburnsii/MessageBox">
    <!-- Work in progress  <img src="images/logo.png" alt="Logo" width="80" height="80"> -->
  </a>

  <h3 align="center">MessageBox</h3>

  <p align="center">
    Client-Side application for sharing heartfelt messages
    <br />
    <a href="https://github.com/dburnsii/MessageBox"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://messagebox.unitfi.com">View Demo</a>
    ·
    <a href="https://github.com/dburnsii/MessageBox/issues">Report Bug</a>
    ·
    <a href="https://github.com/dburnsii/MessageBox/issues">Request Feature</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)
* [Roadmap](#roadmap)
* [Contributing](#contributing)
* [License](#license)
* [Contact](#contact)
* [Acknowledgements](#acknowledgements)



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://messagebox.unitfi.com)

This project serves as the client application for the MessageBox project. MessageBox allows you to easily
send messages to friends or loved ones with a simple and easy to use interface. The messages are
sent to the MessageBox hardware, and the user on the other end is notified of the new message. After
opening the box and reading the message, the box clears it's screen and listens for more messages.
This provides a clean interface to send personal messages to someone at work or home, without distracting
the recipient or requiring them to use any complex technology.

### Built With

* [PlatformIO](https://platformio.org/)


<!-- GETTING STARTED -->
## Getting Started

To get a MessageBox up and running follow these simple steps.

### Prerequisites

#### Hardware

You'll need the following:
* 3D Printed Parts in the CAD_Files/FreeCAD folder
* PCB in the CAD_Files/KiCad folder
* Off-the-shelf parts listed in BOM.md
* Soldering Iron and Solder
* *Optional* Heatshrink Tubing
* *Optional* Hot Glue Gun

#### Software

You'll need the following:
* PlatformIO

### Installation

1. To run the project, build and upload with PlatformIO:
```sh
pio run
pio run -t uploadfs
pio run -t upload
```

The uploadfs command will ensure all necessary SSL certificates are installed on the system, for use with HTTPS.


<!-- USAGE EXAMPLES -->
## Usage

Setup instructions will appear on the TFT LCD attached to the box.


<!-- ROADMAP -->
## Roadmap

See the [open issues](https://github.com/dburnsii/MessageBox/issues) for a list of proposed features (and known issues).



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

<!-- CONTACT -->
## Contact

Desone Burns II - dburnsii@live.com

Project Link: [https://github.com/dburnsii/MessageBox](https://github.com/dburnsii/MessageBox)


<!-- ACKNOWLEDGEMENTS
## Acknowledgements

* []()
* []()
* []()

-->



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/dburnsii/MessageBox.svg?style=flat-square
[contributors-url]: https://github.com/dburnsii/MessageBox/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/dburnsii/MessageBox.svg?style=flat-square
[forks-url]: https://github.com/dburnsii/MessageBox/network/members
[stars-shield]: https://img.shields.io/github/stars/dburnsii/MessageBox.svg?style=flat-square
[stars-url]: https://github.com/dburnsii/MessageBox/stargazers
[issues-shield]: https://img.shields.io/github/issues/dburnsii/MessageBox.svg?style=flat-square
[issues-url]: https://github.com/dburnsii/MessageBox/issues
[license-shield]: https://img.shields.io/github/license/dburnsii/MessageBox.svg?style=flat-square
[license-url]: https://github.com/dburnsii/MessageBox/blob/master/LICENSE
[product-screenshot]: public/images/screenshot.png
