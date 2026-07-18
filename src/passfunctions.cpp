#include "passfunctions.h"
#include "crypto.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
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

std::string VaultPath() { return storage_Path() + "/vault.dat"; }
std::string saltpath() { return storage_Path() + "/salt.dat"; }

void password_functions::saveSalt() {
  std::ofstream file(saltpath(), std::ios::binary | std::ios::trunc);
  if (!file)
    throw std::runtime_error("Failed to save salt");
  file.write(reinterpret_cast<const char *>(salt), sizeof(salt));
}

void password_functions::loadSalt() {
  std::ifstream file(saltpath(), std::ios::binary);
  if (!file) {
    randombytes_buf(salt, sizeof(salt));
    saveSalt();
  } else {
    file.read(reinterpret_cast<char *>(salt), sizeof(salt));
  }
}

void password_functions::deriveKey(const std::string &masterpassword) {
  loadSalt();
  derived_key.resize(crypto_aead_xchacha20poly1305_ietf_KEYBYTES);
  if (crypto_pwhash(
          reinterpret_cast<unsigned char *>(derived_key.data()),
          derived_key.size(), masterpassword.c_str(), masterpassword.size(),
          salt, crypto_pwhash_OPSLIMIT_INTERACTIVE,
          crypto_pwhash_MEMLIMIT_INTERACTIVE, crypto_pwhash_ALG_DEFAULT) != 0)
    throw std::runtime_error("Key derivation failed (out of memory)");
}

void password_functions::save_details() {
  if (vault.empty()) {
    std::ofstream file(VaultPath(), std::ios::binary | std::ios::trunc);
    return;
  }

  std::string plaintext;
  for (const auto &det : vault) {
    plaintext += det.getsite() + '\n';
    plaintext += det.getUsername() + '\n';
    plaintext += det.getPassword() + '\n';
  }

  auto encrypted = Crypto::encrypt(plaintext, derived_key);

  std::ofstream file(VaultPath(), std::ios::binary | std::ios::trunc);
  if (!file.is_open())
    throw std::runtime_error("Failed to open file for writing");

  file.write(reinterpret_cast<const char *>(encrypted.data()),
             encrypted.size());

  if (!file.good())
    throw std::runtime_error("Failed to write encrypted data to file");
}

void password_functions::load_details() {
  vault.clear();
  std::ifstream file(VaultPath(), std::ios::binary);
  if (!file) {
    std::ofstream create(VaultPath(), std::ios::binary);
    return;
  }

  std::vector<unsigned char> encrypted((std::istreambuf_iterator<char>(file)),
                                       std::istreambuf_iterator<char>());
  if (encrypted.empty())
    return;

  std::string plaintext = Crypto::decrypt(encrypted, derived_key);

  std::istringstream ss(plaintext);
  std::string site, username, password;
  while (std::getline(ss, site) && std::getline(ss, username) &&
         std::getline(ss, password))
    vault.push_back(Details(site, username, password));
}

void password_functions::add_details() {
  std::string site, username, password;
  cout << "enter site: ";
  std::getline(cin, site);
  cout << "enter username: ";
  std::getline(cin, username);
  cout << "enter password: ";
  std::getline(cin, password);
  vault.push_back(Details(site, username, password));
  save_details();
  cout << "added successfully\n\n";
}

void password_functions::search_details() {
  std::string search;
  cout << "enter the site to search: ";
  std::getline(cin, search);
  int index = site_index(search);
  if (index == -1) {
    cout << "site not found:(\n";
    return;
  }
  cout << "Site: " << vault[index].getsite() << '\n';
  cout << "Username: " << vault[index].getUsername() << '\n';
  cout << "Password: " << vault[index].getPassword() << '\n';
}

void password_functions::show_all() {
  if (vault.empty()) {
    cout << "file is empty, nothing to display:(\n";
    return;
  }
  for (auto &d : vault) {
    cout << "Site: " << d.getsite() << '\n';
    cout << "Username: " << d.getUsername() << '\n';
    cout << "Password: " << d.getPassword() << '\n';
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
}

void password_functions::delete_all() {
  if (vault.empty()) {
    cout << "file is already empty :O\n";
    return;
  }
  char op;
start:
  cout
      << "are you sure? this will delete the entire data inside the file y/n: ";
  cin >> op;
  cin.ignore();
  if (op == 'y' || op == 'Y') {
    vault.clear();
    save_details();
    cout << "everything's cleared now :>\n\n";
  } else if (op == 'n' || op == 'N') {
    cout << "alright then exiting...\n\n";
  } else {
    cout << "invalid command\n";
    goto start;
  }
}

void password_functions::edit_details() {
  std::string sitesearch;
  cout << "enter the site you wanna edit: ";
  std::getline(cin, sitesearch);
  int index = site_index(sitesearch);
  if (index == -1) {
    cout << "site not found:(\n\n";
    return;
  }

  std::string new_val;
  int op;
  char option;
selection:
  cout << "what do you wanna update\n 1.site\n 2.username\n "
          "3.password\nselect: ";
  cin >> option;
  cin.ignore();
  op = option - '0';
  if (op == 1) {
    cout << "enter updated site: ";
    std::getline(cin, new_val);
    vault[index].setSite(new_val);
  } else if (op == 2) {
    cout << "enter updated username: ";
    std::getline(cin, new_val);
    vault[index].setUsername(new_val);
  } else if (op == 3) {
    cout << "enter new password: ";
    std::getline(cin, new_val);
    vault[index].setPassword(new_val);
  } else {
    cout << "enter a valid option duhh\n\n";
    goto selection;
  }
  save_details();
  cout << "edited successfully\n\n";
}

void password_functions::add_CLI(std::string site, std::string username,
                                 std::string password) {
  vault.push_back(Details(site, username, password));
  save_details();
  cout << "added successfully\n\n";
}

void password_functions::search_CLI(std::string site) {
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
  int index = site_index(site);
  if (index == -1) {
    cout << "site not found:(\n";
    return;
  }
  vault.erase(vault.begin() + index);
  save_details();
  cout << "deleted successfully\n\n";
}

void password_functions::edit_CLI(std::string site) {
  int index = site_index(site);
  if (index == -1) {
    cout << "site not found:(\n\n";
    return;
  }

  std::string new_val;
  int op;
  char option;
selection:
  cout << "what do you wanna update\n 1.site\n 2.username\n "
          "3.password\nselect: ";
  cin >> option;
  cin.ignore();
  op = option - '0';
  if (op == 1) {
    cout << "enter updated site: ";
    std::getline(cin, new_val);
    vault[index].setSite(new_val);
  } else if (op == 2) {
    cout << "enter updated username: ";
    std::getline(cin, new_val);
    vault[index].setUsername(new_val);
  } else if (op == 3) {
    cout << "enter new password: ";
    std::getline(cin, new_val);
    vault[index].setPassword(new_val);
  } else {
    cout << "enter a valid option duhh\n\n";
    goto selection;
  }
  save_details();
  cout << "edited successfully\n\n";
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
    if (option == 'q' || option == 'Q')
      return;
    switch (option - '0') {
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

int password_functions::site_index(const std::string site) {
  std::vector<int> match;
  for (int i = 0; i < (int)vault.size(); ++i)
    if (vault[i].getsite() == site)
      match.push_back(i);

  if (match.empty())
    return -1;
  if (match.size() == 1)
    return match[0];

  for (int i = 0; i < (int)match.size(); ++i)
    cout << i + 1 << ". " << vault[match[i]].getUsername() << '\n';

selection:
  int choice;
  char option;
  cout << "Select user account: ";
  cin >> option;
  cin.ignore();
  choice = option - '0';
  if (choice < 1 || choice > (int)match.size()) {
    cout << "invalid option\n\n";
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
  std::cout << "Master key not set yet\nSet a master key: ";
  std::getline(std::cin, masterkey);

  char hashed[crypto_pwhash_STRBYTES];
  if (crypto_pwhash_str(hashed, masterkey.c_str(), masterkey.size(),
                        crypto_pwhash_OPSLIMIT_INTERACTIVE,
                        crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0)
    throw std::runtime_error("Hashing failed (out of memory)");

  file << hashed;
}

std::string readKey_file() {
  std::ifstream file(KeyPath());
  if (!file)
    return "";
  std::string storedHash;
  std::getline(file, storedHash);
  return storedHash;
}

int verify_masterkey(const std::string &storedHash,
                     const std::string &userKey) {
  return crypto_pwhash_str_verify(storedHash.c_str(), userKey.c_str(),
                                  userKey.size()) == 0
             ? 0
             : 1;
}

// Returns 0 = success, 1 = wrong key, 2 = first run
// userKeyOut is set to the raw key on success (used for deriveKey)
int verify_entry(std::string &userKeyOut) {
  std::ifstream file(KeyPath());
  if (!file || file.peek() == std::ifstream::traits_type::eof()) {
    set_masterkey();
    std::cout << "Master key created. Restart the program\n";
    return 2;
  }

  std::cout << "Enter master key: ";
  std::getline(std::cin, userKeyOut);
  std::cout << '\n';

  std::string storedHash = readKey_file();
  return verify_masterkey(storedHash, userKeyOut);
}

void change_masterkey() {
  std::string oldKey;
  std::cout << "Enter current master key: ";
  std::getline(std::cin, oldKey);

  std::string storedHash = readKey_file();
  if (verify_masterkey(storedHash, oldKey) != 0) {
    std::cout << "Wrong master key. Cannot change.\n";
    return;
  }

  std::string newKey;
  std::cout << "Enter new master key: ";
  std::getline(std::cin, newKey);

  char hashed[crypto_pwhash_STRBYTES];
  if (crypto_pwhash_str(hashed, newKey.c_str(), newKey.size(),
                        crypto_pwhash_OPSLIMIT_INTERACTIVE,
                        crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0)
    throw std::runtime_error("Hashing failed (out of memory)");

  std::ofstream file(KeyPath(), std::ios::trunc);
  if (!file) {
    std::cerr << "Error opening masterkey file\n";
    return;
  }
  file << hashed;
  std::cout << "Master key changed successfully.\n";
}