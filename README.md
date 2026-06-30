# Lockie – A Simple C++ Password Manager

A lightweight command-line password manager built in C++ for learning file handling, data structures, and encryption.

---

## Branches

This repository contains two versions of Lockie:

### `main`

The stable version of Lockie.

**Features**

- Add password entries
- Search by website
- Edit saved entries
- Delete individual entries
- Delete all entries
- Plain text storage
- Easy to understand source code

Recommended if you want to learn how the password manager works internally.

---

### `encryption`

An experimental branch that adds encryption using **libsodium**.

**Additional Features**

- Encrypted password database
- Master key authentication
- Improved security over the `main` branch

Recommended if you want a more secure version or wish to study encryption in C++.

---

# Dependencies

### Arch Linux

```bash
sudo pacman -S libsodium
```

### Ubuntu/Debian

```bash
sudo apt install libsodium-dev
```

### Fedora

```bash
sudo dnf install libsodium-devel
```

# Installation

## one command installation

### main

```bash
git clone https://github.com/codeXxero/lockie---cpp-password-manager.git
cd lockie---cpp-password-manager
mkdir build
cd build
cmake ..
make
sudo make install

```

### encryption

```bash
git clone -b encryption https://github.com/codeXxero/lockie---cpp-password-manager.git
cd lockie---cpp-password-manager
mkdir build
cd build
cmake ..
make
sudo make install

```

## Manual installation

### Install the `main` branch

Clone the repo:

```bash
git clone https://github.com/codeXxero/lockie---cpp-password-manager.git
```

enter the project:

```bash
cd lockie---cpp-password-manager
```

Build:

```bash
mkdir build
cd build
cmake ..
make
sudo make install
```

Run:

```bash
lockie
```

### Install the `encryption` branch

Clone the repo:

```bash
git clone -b encryption https://github.com/codeXxero/lockie---cpp-password-manager.git
```

Enter the project:

```bash
cd lockie---cpp-password-manager
```

Build:

```bash
mkdir build
cd build
cmake ..
make
sudo make install
```

Run:

```bash
lockie
```

---

# Usage

There is a menu driven approach or direct command approach which will do the task directly without entering menu.
Users can use it whatever way they are comfortable

## open menu

just enter

```
lockie
```

and you will enter the menu mode

## Command mode

| Commands                                                     | Description                |
| ------------------------------------------------------------ | -------------------------- |
| `lockie add <website> <username> <password>` or `lockie add` | Add a new password entry   |
| `lockie search <website>`                                    | Search for a website       |
| `lockie edit <website>`                                      | Modify an existing entry   |
| `lockie delete <website>`                                    | Delete a specific entry    |
| `lockie clearall`                                            | Delete every stored entry  |
| `lockie listall`                                             | Display every stored entry |

## add website

```bash
lockie add <website> <username> <password>
```

or

```bash
lockie add
```

### search website

```bash
lockie search <webiste name>
```

### delete website

```bash
lockie delete <webiste name>
```

### displaying all entries

```bash
lockie listall
```

### clear all entries

```bash
lockie clearall
```

---

# Reporting Bugs

Found a bug or have a suggestion?

Please open a GitHub Issue and include:

- Operating system
- Compiler version
- Branch (`main` or `encryption`)
- Steps to reproduce the issue
- Expected behavior
- Actual behavior
- Error messages (if any)

If possible, include screenshots or terminal output.

---

## License

This project is open source. Feel free to learn from the code, modify it, and contribute improvements.
