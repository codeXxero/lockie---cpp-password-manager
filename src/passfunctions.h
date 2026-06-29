#ifndef PASSFUNCTIONS_H
#define PASSFUNCTIONS_H
#include "details.h"
#include <iostream>
#include <sodium.h>
#include <string>
#include <vector>

using namespace std;

class password_functions {
private:
  std::vector<Details> vault;
  std::string derived_key;
  unsigned char salt[crypto_pwhash_SALTBYTES];

public:
  void deriveKey(const std::string &masterpassword);
  void loadSalt();
  void saveSalt();

  template <typename Func> void choice_direction(int direction, Func action) {
    if (direction == 0)
      action();
    else if (direction == 1)
      std::cout << "Wrong master key. Access denied.\n";
  }

  void add_details();
  void load_details();
  void search_details();
  void show_all();
  void delete_det();
  void delete_all();
  void edit_details();
  void menu_driven();
  void save_details();

  void add_CLI(std::string site, std::string username, std::string password);
  void search_CLI(std::string site);
  void delete_CLI(std::string site);
  void edit_CLI(std::string site);

  int site_index(const std::string site);
};

// ─── Master key functions
// ─────────────────────────────────────────────────────
void set_masterkey();
std::string readKey_file();
int verify_masterkey(const std::string &storedHash, const std::string &userKey);
int verify_entry(std::string &userKeyOut); // returns direction, outputs raw key
void change_masterkey();

#endif