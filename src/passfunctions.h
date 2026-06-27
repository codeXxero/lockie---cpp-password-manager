#ifndef PASSFUNCTIONS_H
#define PASSFUNCTIONS_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using std::cin;
using std::cout;

class Details {
private:
  std::string m_site;
  std::string m_username;
  std::string m_password;

public:
  Details(const std::string &site, const std::string &username,
          const std::string &password)
      : m_site(site), m_username(username), m_password(password) {}
  const std::string &getsite() const { return m_site; }
  const std::string &getUsername() const { return m_username; }
  const std::string &getPassword() const { return m_password; }
  void setSite(const std::string &site) { m_site = site; }
  void setUsername(const std::string &username) { m_username = username; }
  void setPassword(const std::string &password) { m_password = password; }
};

class password_functions {
private:
  std::vector<Details> vault;

public:
  void add_CLI(const std::string site, const std::string username,
               const std::string password);
  void search_CLI(const std::string site);
  void delete_CLI(const std::string site);
  template <typename T> void choice_direction(int direction, T func) {
    if (direction == 0) {

      func();
      return;
    }
    if (direction == 2) {
      return;
    } else {
      cout << "wrong masterkey :3\n";
    }
  }
  void menu_driven();
  void edit_CLI(std::string site);
  int site_index(const std::string site);
  void save_details();
  void add_details();
  void load_details();
  void search_details();
  void show_all();
  void delete_det();
  void delete_all();
  void edit_details();
};
void set_masterkey();
std::string readKey_file();
int verify_masterkey(const std::string &storedKey, const std::string &userKey);

int verify_entry();
void change_masterkey();
#endif