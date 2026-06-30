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

# Installation

## Install the `main` branch

Clone the repository:

```bash
git clone https://github.com/codeXxero/lockie---cpp-password-manager.git
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
```

Run:

```bash
./main
```

---

## Install the `encryption` branch

Clone the repository:

```bash
git clone -b encryption https://github.com/codeXxero/lockie---cpp-password-manager.git
```

Enter the project:

```bash
cd lockie---cpp-password-manager
```

Install libsodium if it is not already installed.

### Arch Linux

```bash
sudo pacman -S libsodium
```

### Ubuntu/Debian

```bash
sudo apt install libsodium-dev
```

Build:

```bash
mkdir build
cd build
cmake ..
make
```

Run:

```bash
./main
```

---

# Usage

```
./main
```

Available commands:

| Command            | Description               |
| ------------------ | ------------------------- |
| `add`              | Add a new password entry  |
| `search <website>` | Search for a website      |
| `edit`             | Modify an existing entry  |
| `delete <website>` | Delete a specific entry   |
| `delete_all`       | Delete every stored entry |
| `help`             | Show available commands   |
| `exit`             | Save changes and quit     |

Example:

```bash
./main

> add
> search github
> delete github
> delete_all
> exit
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
