# Telecraft

Telecraft is a powerful and flexible C++ library designed to simplify interactions with the Telegram API. It provides a robust framework for creating, managing, and sending HTTP requests, tailored specifically for Telegram's needs.

## Features

- **Modular Design**: Easily extendable components allow for customized request generation and management.
- **Type Safety**: Strong typing ensures safer code and fewer runtime errors.
- **Easy Integration**: Simple to integrate with existing projects.
- **Customizable Builders**: Define and use various builders for different request parameters and bodies.
- **Error Handling**: Comprehensive error checking and handling mechanisms. (in future)

## Getting Started

### Prerequisites

- **C++20 or later**: Telecraft utilizes modern C++ features.
- **CMake**: To build the project.

### Installation

1. **Clone the Repository**:
    ```sh
    git clone https://github.com/yourusername/telecraft.git
    cd telecraft
    ```

2. **Build the Project**:
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

### Basic Usage

Telecraft is designed to simplify the process of interacting with the Telegram API by providing flexible and customizable components. Here’s a brief overview of how you can use Telecraft:

1. **Setup HTTP Management**: Manage HTTP headers and other related fields.
2. **Define Method Builders**: Create method builders for different Telegram API methods. These builders help in constructing the request body and managing its content.
3. **Generate and Send Requests**: Use the `RequestGenerator` and `RequestComponentManager` to build and send HTTP requests to the Telegram API.

### Extending Telecraft

To extend Telecraft for your own use case, you can create custom method builders for various Telegram API methods. The modular design of Telecraft allows you to add new features and functionalities with ease.