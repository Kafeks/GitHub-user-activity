# Project Activity

Project URL: https://github.com/Kafeks/GitHub-user-activity

A small command-line tool written in C++ that fetches a GitHub user's recent public activity (events) using the GitHub REST API and prints it to the console. For `PushEvent` entries, it also fetches the commit comparison between the `before` and `head` commits and prints the total number of commits in that push.

## Features

- Prompts for a GitHub username and an optional personal access token
- Fetches `/users/{username}/events` from the GitHub API
- Prints each event's type and the related repository
- For push events, calls the `compare` endpoint and prints the total commit count

## Requirements

- A C++ compiler with C++11 support or newer
- [libcurl](https://curl.se/libcurl/)
- [nlohmann/json](https://github.com/nlohmann/json) (header-only library)

## Installation

### macOS

Install dependencies with [Homebrew](https://brew.sh/):

```bash
brew install curl nlohmann-json
```

If you already have Xcode Command Line Tools installed, you have a working C++ compiler (`clang++`). If not, install them first:

```bash
xcode-select --install
```

### Linux (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install build-essential libcurl4-openssl-dev nlohmann-json3-dev
```

### Linux (Fedora)

```bash
sudo dnf install gcc-c++ libcurl-devel json-devel
```

### Linux (Arch)

```bash
sudo pacman -S base-devel curl nlohmann-json
```

### Windows

The easiest way is to use [MSYS2](https://www.msys2.org/):

1. Install MSYS2 from the official website and open the "MSYS2 MinGW 64-bit" terminal.
2. Update packages and install the toolchain and dependencies:

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-curl mingw-w64-x86_64-nlohmann-json
```

Alternatively, on Windows you can use [vcpkg](https://github.com/microsoft/vcpkg):

```powershell
vcpkg install curl nlohmann-json
```

Then compile with Visual Studio's `cl.exe`, pointing the include and library paths to your vcpkg installation, or use MinGW as described above.

## Compilation

### macOS (Homebrew, Apple Silicon)

```bash
g++ project-activity.cpp \
  -I/opt/homebrew/include \
  -I/opt/homebrew/opt/curl/include \
  -L/opt/homebrew/opt/curl/lib \
  -lcurl \
  -o project_activity
```

On Intel Macs, Homebrew installs to `/usr/local` instead of `/opt/homebrew`, so adjust the paths accordingly:

```bash
g++ project-activity.cpp \
  -I/usr/local/include \
  -I/usr/local/opt/curl/include \
  -L/usr/local/opt/curl/lib \
  -lcurl \
  -o project_activity
```

### Linux

On most distributions, the headers and libraries are installed in standard system paths, so a simple command is enough:

```bash
g++ project-activity.cpp -lcurl -o project_activity
```

If your package manager placed `nlohmann/json.hpp` in a nonstandard location, add an `-I` flag pointing to it.

### Windows (MSYS2 MinGW 64-bit shell)

```bash
g++ project-activity.cpp -lcurl -o project_activity.exe
```

### Windows (vcpkg + MSVC, from a Developer Command Prompt)

```powershell
cl project-activity.cpp /I <path-to-vcpkg>\installed\x64-windows\include ^
  /link /LIBPATH:<path-to-vcpkg>\installed\x64-windows\lib libcurl.lib
```

Replace `<path-to-vcpkg>` with the actual path to your vcpkg installation.

## Usage

Run the compiled binary:

```bash
./project_activity
```

You will be prompted for a GitHub username, and then optionally for a GitHub personal access token (press Enter to skip and make an unauthenticated request):

```
Write username: octocat
Enter your GitHub Token (or press Enter to skip):
```

Example output:

```
PushEvent->octocat/Hello-World
Commits: 3
WatchEvent->octocat/Spoon-Knife
IssuesEvent->octocat/Hello-World
PushEvent->octocat/Hello-World
Commits: 1
```

If the request fails (for example, due to an invalid username or rate limiting), the tool prints the error message returned by the GitHub API:

```
Error: Not Found
```

## Notes

- Unauthenticated requests to the GitHub API are subject to a low rate limit (60 requests per hour per IP). Providing a personal access token raises this limit and is recommended for repeated use.
- A GitHub token only needs read access to public data for this tool; no special scopes are required for public event data.