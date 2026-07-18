#include "passfunctions.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sodium.h>
#include <string>
#include <vector>
std::string storage_Path() {
  const char *home = std::getenv("HOME");

  std::filesystem::path dir =
      std::filesystem::path(home) / ".local" / "share" / "lockie";

  std::filesystem::create_directories(dir);

  return dir.string();
}
std::string KeyPath() { return storage_Path() + "/key.dat"; }

std::string vaultPath() { return storage_Path() + "/vault.dat"; }
void password_functions::add_CLI(std::string site, std::string username,
                                 std::string password) {
  Details d1(site, username, password);
  vault.push_back(d1);
  save_details();
  cout << "added successfully\n\n";
}
void password_functions::search_CLI(std::string site) {
  load_details();
  int index = site_index(site);
  if (index == -1) {
    cout << "site not found:(\n";
    return;
  }
  cout << "Site: " << vault[index].getsite() << '\n';

  cout << "Username: " << vault[index].getUsername() << '\n';

  cout << "Password: " << vault[index].getPassword() << '\n';
}

void password_functions::delete_CLI(std::string site) {
  load_details();
  int index = site_index(site);
  if (index == -1) {
    cout << "site not found:(\n";
    return;
  }

  vault.erase(vault.begin() + index);
  save_details();
  cout << "deleted successfully\n\n";
  return;
}
void password_functions::edit_CLI(std::string site) {

  std::string new_site;
  std::string new_username;
  std::string new_password;
  int op;

  int index = site_index(site);

  if (index == -1) {
    cout << "site not found:(\n\n";
    return;
  }

selection:
  cout << "what do you wanna update \n 1.site \n 2.username \n"
          "3.password\nselect option 1 2 3: ";
  cin >> op;
  cin.ignore();

  if (op == 1) {
    cout << "enter the updated site: ";
    std::getline(cin, new_site);
    vault[index].setSite(new_site);

  } else if (op == 2) {
    cout << "enter the updated username: ";
    std::getline(cin, new_username);

    vault[index].setUsername(new_username);

  } else if (op == 3) {
    cout << "enter the new password: ";
    std::getline(cin, new_password);
    vault[index].setPassword(new_password);

  } else {
    cout << "enter a valid option :<\n\n";
    goto selection;
  }

  save_details();
  cout << "edited successfully\n\n";
  return;
}

void password_functions::save_details() {

  std::ofstream file{vaultPath()};
  if (!file) {
    std::cerr << "error saving details\n";
    return;
  }
  std::string site;
  std::string username;
  std::string password;
  for (auto &det : vault) {

    file << det.getsite() << '\n';
    file << det.getUsername() << '\n';
    file << det.getPassword() << '\n';
  }
}
void password_functions::add_details() {
  std::string site;
  std::string username;
  std::string password;

  std::cout << "enter site: ";
  std::getline(cin, site);
  std::cout << "enter username: ";
  std::getline(cin, username);
  std::cout << "enter password: ";
  std::getline(cin, password);

  Details d1(site, username, password);
  vault.push_back(d1);
  save_details();
  cout << "added successfully\n\n";
}
void password_functions::load_details() {
  vault.clear();
  std::ifstream file(vaultPath());
  if (!file) {
    std::ofstream create(vaultPath());
    return;
  }
  std::string site;
  std::string username;
  std::string password;

  while (std::getline(file, site) && std::getline(file, username) &&
         std::getline(file, password)) {
    Details d1(site, username, password);
    vault.push_back(d1);
  }
}
void password_functions::search_details() {
  std::string search;
  cout << "enter the site to search: ";
  std::getline(cin, search);
  int index = site_index(search);
  if (index == -1) {
    cout << "site not found:(";
  }
  cout << "Site: " << vault[index].getsite() << '\n';

  cout << "Username: " << vault[index].getUsername() << '\n';

  cout << "Password: " << vault[index].getPassword() << '\n';
}

void password_functions::show_all() {
  if (vault.empty()) {
    cout << "file is empty,nothing to display:(\n";
    return;
  }
  for (auto &d : vault) {
    std::cout << "Site: " << d.getsite() << '\n';
    std::cout << "Username: " << d.getUsername() << '\n';
    std::cout << "Password: " << d.getPassword() << '\n';
    cout << '\n';
  }
}
void password_functions::delete_det() {
  std::string search;
  cout << "enter the site to delete: ";
  std::getline(cin, search);
  int index = site_index(search);
  if (index == -1) {
    cout << "site not found:(\n";
    return;
  }

  vault.erase(vault.begin() + index);
  save_details();
  cout << "deleted successfully\n\n";
  return;
}

void password_functions::delete_all() {
  if (vault.empty()) {
    cout << "file is already empty:O \n";
    return;
  }
  char op;
start:
  cout << "are you sure? this will delete the entire data inside the file  "
          "y/n: ";
  cin >> op;
  if (op == 'y' || op == 'Y') {
    vault.clear();
    save_details();
    cout << "everythings cleared now :>\n\n";
    return;
  } else if (op == 'n' || op == 'Y') {
    cout << "alright then exiting...\n\n";
  } else {
    cout << "invalid command \n";
    goto start;
  }
  save_details();
}
void password_functions::edit_details() {
  std::string sitesearch;
  std::string new_site;
  std::string new_username;
  std::string new_password;
  int op;
  char option;
  cout << "enter the site you wanna edit: ";
  std::getline(cin, sitesearch);

  int index = site_index(sitesearch);

  if (index == -1) {
    cout << "site not found:(\n\n";
    return;
  }

selection:
  cout << "what do you wanna update \n 1.site \n 2.username \n"
          "3.password\nselect option 1 2 3: ";
  cin >> option;
  cin.ignore();
  op = option - '0';

  if (op == 1) {
    cout << "enter the updated site: ";
    std::getline(cin, new_site);
    vault[index].setSite(new_site);

  } else if (op == 2) {
    cout << "enter the updated username: ";
    std::getline(cin, new_username);

    vault[index].setUsername(new_username);

  } else if (op == 3) {
    cout << "enter the new password: ";
    std::getline(cin, new_password);
    vault[index].setPassword(new_password);

  } else {
    cout << "enter a valid option duhh\n\n";
    goto selection;
  }

  save_details();
  cout << "edited successfully\n\n";
  return;
}

int password_functions::site_index(const std::string site) {
  int count = 0;
  std::vector<int> match;
  for (int i = 0; i < vault.size(); ++i) {
    if (vault[i].getsite() == site) {
      match.push_back(i);
    }
  }
  if (match.empty()) {
    return -1;
  }
  if (match.size() == 1) {
    return match[0];
  }
  for (int i = 0; i < match.size(); ++i) {
    cout << i + 1 << ". " << vault[match[i]].getUsername() << '\n';
    count += i + 1;
  }
selection:
  int choice;
  char option;
  cout << "Select user account: ";

  cin >> option;
  cin.ignore();
  choice = option - '0';
  if (choice >= count) {
    cout << "invalid option \n\n";
    goto selection;
  }

  return match[choice - 1];
}
void set_masterkey() {
  std::ofstream file(KeyPath());

  if (!file) {
    std::cerr << "Error creating masterkey file\n";
    return;
  }

  std::string masterkey;

  std::cout << "Master key not set yet\n";
  std::cout << "Set a master key: ";

  std::getline(std::cin, masterkey);
  char hashed[crypto_pwhash_STRBYTES];
  if (crypto_pwhash_str(hashed, masterkey.c_str(), masterkey.size(),
                        crypto_pwhash_OPSLIMIT_INTERACTIVE,
                        crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
    std::cerr << "Hashing failed (out of memory)\n";
    return;
  }

  file << hashed;
}

std::string readKey_file() {
  std::ifstream file(KeyPath());

  if (!file) {
    return "file empty";
  }

  std::string stored_hashKey;
  std::getline(file, stored_hashKey);

  return stored_hashKey;
}

int verify_masterkey(const std::string &storedHash,
                     const std::string &userKey) {
  return crypto_pwhash_str_verify(storedHash.c_str(), userKey.c_str(),
                                  userKey.size()) == 0
             ? 0
             : 1;
}

int verify_entry() {
  std::ifstream file(KeyPath());

  if (!file || file.peek() == std::ifstream::traits_type::eof()) {
    set_masterkey();
    std::cout << "Master key created.Restart the program\n";
    return 2;
  }

  std::string userKey;

  std::cout << "Enter master key: ";
  std::getline(std::cin, userKey);
  cout << '\n';
  std::string stored_hashKey = readKey_file();

  return verify_masterkey(stored_hashKey, userKey);
}

void change_masterkey() {

  std::string oldKey;
  std::cout << "Enter current master key: ";
  std::getline(std::cin, oldKey);

  if (verify_masterkey(readKey_file(), oldKey) != 0) {
    std::cout << "Wrong master key:3, Cannot change.\n";
    return;
  }

  std::string newKey;
  std::cout << "Enter new master key: ";
  std::getline(std::cin, newKey);

  char hashed[crypto_pwhash_STRBYTES];
  if (crypto_pwhash_str(hashed, newKey.c_str(), newKey.size(),
                        crypto_pwhash_OPSLIMIT_INTERACTIVE,
                        crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
    std::cerr << "Hashing failed (out of memory)\n";
    return;
  }

  std::ofstream file(KeyPath(), std::ios::trunc);
  if (!file) {
    std::cerr << "Error opening masterkey file\n";
    return;
  }
  file << hashed;
  std::cout << "Master key changed successfully.\n";
}
void password_functions::menu_driven() {

  while (1) {
    char option;
    cout << "-------LOCKIE - PASSWORD MANAGER-------\n";
    cout << "1.Add\n2.Edit\n3.Search\n4.Display all\n5.Delete\n6.Clear "
            "ALL\nEnter Q to QUIT\n";
    cout << "Enter: ";
    cin >> option;
    cin.ignore();
    if (option == 'q' || option == 'Q') {
      return;
    }
    int op = option - '0';
    switch (op) {
    case 1:
      add_details();
      break;
    case 2:
      edit_details();
      break;
    case 3:
      search_details();
      break;
    case 4:
      show_all();
      break;
    case 5:
      delete_det();
      break;
    case 6:
      delete_all();

      break;
    default:
      cout << "invalid option duh\n";
    }
  }
}
